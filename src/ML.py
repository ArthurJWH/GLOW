#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri May 17 19:01:47 2024

@author: xiao

Contains helper functions and result visualization functions
"""

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
model_path = str(Path(__file__).resolve().parent) + "\\..\\assets\\ML_models\\"

para2geom = load_model(model_path + "para2geom.h5", compile=False)
para2geom_pca = joblib.load(model_path + "pca_transformer.pkl")
sc = joblib.load(model_path + "sc.bin")
# hs2angle = joblib.load(model_path + "hs2angle.pkl")


# calculate meltpool geometries from prediction. Can also visualize/save the meltpool. The input mp is the
# prediction directly from para2geom model.
# Input units must be e.g. power 40 speed 480 rpm 0.4
def meltpool_geom_cal(
    power,
    speed,
    rpm,
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

    # print('The width is {0}'.format(width))
    return width * scale, height * scale
