#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri May 17 19:01:47 2024

@author: xiao

Contains helper functions and result visualization functions
"""

import sys
import cv2
import joblib
import warnings
import numpy as np
from keras.models import load_model
from pathlib import Path


# Suppress all UserWarning warnings
warnings.filterwarnings("ignore", category=UserWarning)
warnings.filterwarnings("ignore", category=DeprecationWarning)

## ---------------------- Load the trained ML models ------------------------ #
def resource_path(relative_path: str) -> str:
    """Get the absolute path to the resource, works for development and PyInstaller."""
    try:
        # PyInstaller creates a temp folder and stores path in _MEIPASS
        base_path = sys._MEIPASS
    except AttributeError:
        base_path = Path(__file__).resolve().parent.parent
    return str((Path(base_path) / relative_path).resolve())

para2geom = load_model(resource_path("assets\\ML_models\\para2geom.h5"), compile=False)
para2geom_pca = joblib.load(resource_path("assets\\ML_models\\pca_transformer.pkl"))
sc = joblib.load(resource_path("assets\\ML_models\\sc.bin"))
hs2angle = joblib.load(resource_path("assets\\ML_models\\hs2angle.pkl"))

# para2geom = load_model("..\\assets\\ML_models\\para2geom.h5", compile=False)
# para2geom_pca = joblib.load("..\\assets\\ML_models\\pca_transformer.pkl")
# sc = joblib.load("..\\assets\\ML_models\\sc.bin")
# hs2angle = joblib.load("..\\assets\\ML_models\\hs2angle.pkl")


# calculate meltpool geometries from prediction. Can also visualize/save the meltpool. The input mp is the
# prediction directly from para2geom model.
# Input units must be e.g. power 40 speed 480 rpm 0.4
def meltpool_geom_cal(
    power,
    speed,
    rpm,
    hatch_spacing,
    para2geom=para2geom,
    para2geom_pca=para2geom_pca,
    sc=sc,
):
    scale_measured = 0.0038  # length of mm for 1 pix for a 1280x960 image. Measured on 20240607 using /home/xiao/projects/DED/BO_processing/images/20240418_singletrack_data_retake/scale_bar_67um_mp10&11.jpg
    resize_dim = (96, 96)  # original size (550,550), cropped to (96,96)
    scale = (
        scale_measured / resize_dim[1] * 550
    )  # this is the scale to transfer predicted pix value to mm
    # maybe change

    input_sc = sc.transform([[power, rpm, speed]])
    mp = para2geom.predict(input_sc, verbose=0)
    mp = para2geom_pca.inverse_transform(mp)
    ret, mp_true = cv2.threshold(mp.reshape(resize_dim), 127, 255, cv2.THRESH_BINARY)

    mask_x = np.sum(mp_true, axis=0)
    # Find the left-most x extreme value
    x_ext_l = (mask_x != 0).argmax(axis=0)
    # Find the right-most x extreme value
    x_ext_r = x_ext_l + np.count_nonzero(mask_x) - 1
    # Select the point with the lowest y for the left-most x extreme
    y_ext_l = (np.where(mp_true[:, x_ext_l] != 0)[0]).max()
    # Select the point with the lowest y for the righht-most x extreme
    y_ext_r = (np.where(mp_true[:, x_ext_r] != 0)[0]).max()
    # select the lower of the two for width measurement
    y_ext = max(y_ext_l, y_ext_r)

    # Calculate the centres
    # y_centre = (y_ext_l+y_ext_r)/2
    y_centre = y_ext

    mask_y = np.sum(mp_true, axis=1)
    y_ext_high = (mask_y != 0).argmax(axis=0)

    # extract meltpool width and tilt angle from print bed
    # width = ((x_ext_l-x_ext_r)**2+(y_ext_l-y_ext_r)**2)**0.5*scale
    width = x_ext_r - x_ext_l
    height = abs(y_ext_high - y_centre)

    angle = hs2angle.predict([[width, speed, hatch_spacing, height]])[0]

    coords = np.argwhere(mp_true)

    y_min, x_min = coords.min(axis=0)
    y_max, x_max = coords.max(axis=0)

    aoi = mp_true[max(y_min - 10, 0) : y_max + 10, max(x_min - 10, 0) : x_max + 10]
    aoi_mask = (aoi > 0).astype(np.uint8)

    center = (aoi.shape[1] // 2, aoi.shape[0] // 2)
    rot_matrix = cv2.getRotationMatrix2D(center, 360 - angle, 1.0)
    rotated_image = cv2.warpAffine(
        aoi, rot_matrix, (aoi.shape[1] + 50, aoi.shape[0] + 50)
    )
    rotated_mask = cv2.warpAffine(
        aoi_mask, rot_matrix, (aoi_mask.shape[1] + 50, aoi_mask.shape[0] + 50)
    )

    patterned_image = np.zeros((1000, 1000))

    coords_mp = np.argwhere(aoi)
    _, x_idx_max_mp = np.argmax(coords_mp, axis=0)

    right_most_homogeneous = np.array(
        [coords_mp[x_idx_max_mp][1], coords_mp[x_idx_max_mp][0], 1]
    )
    right_most_new = np.dot(rot_matrix, right_most_homogeneous)

    coords_mp_rotated = np.argwhere(rotated_image)
    y_min, x_min = coords_mp_rotated.min(axis=0)
    y_max, x_max = coords_mp_rotated.max(axis=0)

    rotated_mp_height = right_most_new[1] - y_min
    # rotated_mp_depth = y_max - right_most_new[1]

    coords_patterned = np.argwhere(patterned_image)
    y_min_patterned, x_min_patterned = coords_patterned.min(axis=0)
    y_max_patterned, x_max_patterned = coords_patterned.max(axis=0)
    surface_padding = 0  # extrat pattern to prevent unnecessary crop
    patterned_image = patterned_image[
        y_min_patterned - surface_padding : y_max_patterned + surface_padding,
        x_min_patterned : x_max_patterned + surface_padding,
    ]

    surface_mask = (patterned_image > 0).astype(np.uint8)
    # ratio of overlap in height
    t_ratio = 1
    counter = 0
    # Place the surface repeatedly in the new image until there is no zero values in the centre 50%
    while True:
        # Establish an array of all zero
        cube = np.zeros((1000, patterned_image.shape[1]))
        cube_to_check = np.zeros((1000, patterned_image.shape[1]))
        # Calculate the current layer thickness
        layer_t = int(rotated_mp_height * t_ratio)

        # Pattern the layers
        for i in range(2):
            x_offset = 0
            total_height = 2 * patterned_image.shape[0]

            # Define the region of interest in the destination image
            roi = cube[
                total_height - patterned_image.shape[0] - layer_t * i : total_height
                - layer_t * i,
                x_offset : x_offset + patterned_image.shape[1],
            ]
            roi_to_check = cube_to_check[
                total_height - patterned_image.shape[0] - layer_t * i : total_height
                - patterned_image.shape[0]
                - layer_t * i
                + surface_mask[0 : rotated_mp_height + surface_padding, :].shape[0],
                x_offset : x_offset + patterned_image.shape[1],
            ]

            # Update only the AOI regions using the mask
            np.copyto(roi, patterned_image, where=surface_mask.astype(bool))
            np.copyto(
                roi_to_check,
                patterned_image[0 : rotated_mp_height + surface_padding, :],
                where=surface_mask[0 : rotated_mp_height + surface_padding, :].astype(
                    bool
                ),
            )

            surface = np.fliplr(patterned_image)
            surface_mask = np.fliplr(surface_mask)
        # Crop the image to the width of the patterned AOIs
        coords_cube = np.argwhere(cube)
        y_min_cube, x_min_cube = coords_cube.min(axis=0)
        y_max_cube, x_max_cube = coords_cube.max(axis=0)
        cube = cube[(y_min_cube - 1) : (y_max_cube + 1), x_min_cube : (x_max_cube + 1)]
        cube_to_check = cube_to_check[
            (y_min_cube - 1) : (y_max_cube + 1), x_min_cube : (x_max_cube + 1)
        ]

        cube_to_check_centre = cube_to_check[
            int(cube.shape[0] * 0.25) : int(cube.shape[0] * 0.75),
            int(cube.shape[1] * 0.25) : int(cube.shape[1] * 0.75),
        ]

        # if all cube images should be plotted
        if counter == 0:
            porosity_init = np.count_nonzero(cube_to_check_centre == 0)

        porosity = np.count_nonzero(cube_to_check_centre == 0)
        counter = counter + 1
        # Check pores
        if porosity <= porosity_init * 0.1:
            break
        else:
            t_ratio = t_ratio - 0.01

    return width * scale * np.cos(angle), rotated_mp_height * scale, t_ratio
