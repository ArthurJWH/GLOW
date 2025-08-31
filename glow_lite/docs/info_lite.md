<!-- Ctrl + Shift + V to read -->
# G.L.O.W. Lite Documentation

## G-Code Generator

### The CSV file
Each row of the CSV file must contain its printing information, including:

 - `idx` : is the index of the row;
 - `laser_power` : is the laser power used as a % of 1 kW total power (e.g. 50);
 - `scanning_speed` : is the scanning speed of the print, in mm/min;
 - `rpm_1` : is the rpm for hopper 1;
 - `rpm_2` : is the rpm for hopper 2;
 - `width` : is the width of the printed tracks, in mm (optional);
 - `height` : is the height of the printed tracks, in mm (optional);
 - `hatch_spacing` : is the ideal spacing between adjacent tracks as a fraction (e.g. 0.5) of `width` (optional if not printing Cube);
 - `layer_height` : is the ideal spacing between successive layers as a fraction (e.g. 0.5) of `height` (optional);
 - `layers` : is the number of layers that each row will be used in, the code will automatically change the row between objects (optional);

Obs.: the heading must contain the column name as specified above.

Obs.: at least one rpm column must be on the CSV file.

### Printing shape
There are three types of supported shapes: Single Track, Thin Wall and Cube. Each type has its own parameters:

 - `Length` : is the X-Y dimension of the object, in mm;
 - `Height` : is the Z dimension of the object, in mm;
 - `Number` : is the amount of objects to be printed;
 - `Horizontal spacing` : is the spacing between objects in X direction, in mm;
 - `Vertical spacing` : is the spacing between objects in Y direction, in mm;
 - `Print horizontally` : start the print of each object in the X direction;

### Substract shape
There are two types of supported substract: Rectangle and Circle. Each type has its own parameters:

 - `width` : is the width of the substrate in the X direction, in mm (for Rectangle);
 - `height` : is the height of the substrate in the Y direction, in mm (for Rectangle);
 - `radius` : is the radius of the substrate, in mm (for Circle);
 - `margin` : is the distance from the substrate ends from which it is allowed to print in, in mm;

### Calculate
This button will calculate the position of each object (if all objects fit in the substrate), and plot the outcome in `Printing Configuration`.

Obs.: the code creates a rectangle within which are the objects and analyze the fitting of it in the substrate.

Obs.: the code prioritizes a filled grid formation.

### Add position
When clicking in the plot from `Printing Configuration` a point (x, y) will be selected.

This button allows the selected point to be used as the bottom-left corner to create a new object.

### Generate G-Code
This button will take all inputs and generate save in a `.gcode` file.

The G-Code will follow some rules:

 - The objects will be printed in rows, from left to right and bottom to top;
 - In each object, the print will start in the bottom-left corner, moving up if printing vertically or moving right if printing horizontally;
 - For thin walls, each other layer will have the printing direction inverted;
 - For cubes, each layer will be printed in zigzag pattern and between each layer the print will rotate its direction by 90&deg;, starting on the same point it ended the previous layer;

### Display
The interface has both graphical and text displays, that allow user to view current configuration and also receive feedback from the code;

The plot is interactive, allowing : position selection, pan, zoom in/out and reset (`R` key).

<br/>

## Machine Settings

### Basic Parameters
 - `Save directory` : is the main folder in which the generated G-Codes will be saved;
 - `Safe height` : is the height by which the nozzle will be lifted when moving between objects, to avoid damages;
 - `Not printing speed` : is the speed used to move the nozzle when not printing, in mm/min;
 - `Gas flow rate` : is the flow rate of the gas used to feed the powder, in L/min;
 - `Waiting time after feed rate change` : is the time required to steady the feed rate, in s;
 - `Cooldown intertracks` : is the cooldown between printing tracks in a Cube's layer, in s;
 - `Cooldown interlayers` : is the cooldown between printing layers in Thin Wall and Cube, in s;
 - `Cooldown interobjects` : is the cooldown between printing the objects, in s;
 - `Use camera` : enables image capturing during printing;
    - `Camera exposure time` : is the time following the camera start, after which it will stop;
    - `Camera start delay` : is the delay itt takes the camera to record after it starts;
    - `Camera sampling interval`  : is the interval between image capturing;
    - machine specific codes to turn on/off and start the camera;

Obs.: `Cooldown intertracks`, `Cooldown interlayers` and `Cooldown interobjects` can be a value or a sequence of values separated by "," (e.g. 1,2,3). If a sequence is given the code will loop through it, restarting on every layer if intertracks or object if interobjects.

### Machine Specific Code
 - indices: are the number by which the machine refers to each component;
 - codes: are the specific code used to call each function; by clicking in each code, it shows additional description and allows the code to be changed;

### Reset & Save
 - `Reset` : resets the configuration to the last saved one;
 - `Save` : the changed configuration can be used while the GUI is opened, but for convenience the save button allows the configuration to be saved for future applications;

<br/>

## NC viewer
It connects with NC viewer website (https://ncviewer.com/) allowing for easy visualization of `.gcode` files.
