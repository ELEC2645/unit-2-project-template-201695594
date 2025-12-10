[![Open in Codespaces](https://classroom.github.com/assets/launch-codespace-2972f46106e565e64193e422d61a12cf1da4916b45550586e14ef0a7c637dd04.svg)](https://classroom.github.com/open-in-codespaces?assignment_repo_id=21758124)
# ELEC2645 DC-DC Converter CLI App

This project implements a simple CLI app for designing basic DC-DC converters. Based on the content covered in ELEC2501 - Power Electronics, the app can be used to generate component values for Buck, Boost, Buck-Boost and Ćuk converters. The user must provide input specifications/parameters that are then used to compute the required inductor/capacitor values.


### 1 Run app

To build the app, run `make -B` or `make main.out` to build the project.

The app can be run with `./main.out`


### 2 Project File Structure

The project is based on the provided template from the assignment brief but has been expanded to include several more files and libraries. These are:

- `main.c` - Very similar to the template version, this handles the running of the main menu interface and has simply been adapted to handle new versions of the `menu_item_x` functions in `menu_funcs.c`
 - `menu_funcs.c` - This files contains the functions for each of the 5 main menu options.
 - `funcs.c` - This is the main library of functions for the project. It contains functions for configuring converter design, calculating design values and handling user inputs
 - `files.c` - This holds functions for handling saving/loading converter designs to and from .json files stored in the `/saves` directory.
- `conv_math.c` - This has functions that implement all the base equations needed for calculating the converter designs. These were split into discrete functions to improve readability of the `compute_converter` function in `funcs.c`

All library source files are stored in the `/src` directory, with `.h` files in `/src/include` and external library source files in `/src/lib`.

The external libraries used in the project are the `cJSON` ([cJSON repo](https://github.com/DaveGamble/cJSON)) and `dirent` ([Dirent Repo](https://github.com/tronkko/dirent)) libraries. `cJSON` is used in `files.c` for writing and reading from .json files that store converter designs. `dirent` is also used in `files.c` for printing the contents of the `/saves` directory for the user to load designs from. Both of these libraries are available under the MIT license.

### 3 Using the app

Running the app will present the user with the main menu interface as shown here (text colours are not shown):
```
----------------- Main Menu -----------------

        1. Set converter type and name
        2. Input converter parameters
        3. View/edit converter parameters
        4. Compute current design
        5. Save/load design
        6. Exit

---------------------------------------------

Select item:
```
#### Options
 **1. Set converter type and name** - 
 Here the save name and converter type of the current design can be set. The name is a string and the type takes an integer selection from a sub menu.
 ```
>> Setup New Converter

Please enter a filename for your new converter design: ANewConvertName
Design saved under: ANewConvertName

Please select converter type:
1. Buck
2. Boost
3. Buck-Boost
4. Cuk
Select type (1-4): 2
Selected type: Boost

Enter 'b' or 'B' to go back to main menu:
 ```
**2. Input converter Parameters** - This prompts the user to enter values for the following converter parameters. Inputs should be positive float values and if a value is unknown it can be left blank and the program will set it to -1.
The parameters are:


| Parameter | Units | Description |
|-------|-------|-------|
| V_o | V | Nominal output voltage |
| V_i | V | Nominal input voltage |
| I_o | A | Nominal output current|
|I_i| A | Nominal input current 
|R_l| Ohm | The resistance of the connected load |
|i_rip| A | The maximum current ripple through the primary inductor|
|v_rip| V |The maximum voltage ripple at the output|
|i_rip2|A| The maximum current ripple through the secondary inductor (Ćuk converters only)|
|v_rip2|V| The maximum voltage ripple through the transfer capacitor, Cn (Ćuk converters only)|

Once all the parameters have been entered, a summary is printed and the user is prompted to return to the main menu.
```
 Entered converter specs:

                Name: ANewConvertName
---------------------------------------------------------
Type: Boost
---------------------------------------------------------
|1.      V_o: 45.0000            2.     V_i: 23.0000    |
|3.      I_o: -1.0000            4.     I_i: -1.0000    |
|5.      R_l: 300.0000           6.     F_s: 10000.0000 |
|7.  delta_i: 0.5000             8. delta_v: 4.0000     |
---------------------------------------------------------

---------------------------------------------------------
```
**3. View/edit converter parameters** - This option lets the user view the current configuration of the converter and edit individual parameters as desired. The field to change can be selected and a new value entered (or set to blank if unknown). If no edits are required the user can enter 'C' to cancel and return to the main menu.

```
>> Edit design parameters

Enter the field you wish to edit

                Name: ANewConvertName
---------------------------------------------------------
Type: Boost
---------------------------------------------------------
|1.      V_o: 45.0000            2.     V_i: 23.0000    |
|3.      I_o: -1.0000            4.     I_i: -1.0000    |
|5.      R_l: 300.0000           6.     F_s: 10000.0000 |
|7.  delta_i: 0.5000             8. delta_v: 4.0000     |
---------------------------------------------------------

---------------------------------------------------------
Enter option (enter 'C' or 'c' to cancel): 2

Enter a new V_i value (V): 30

V_i set to: 30.0000(V)
Enter 'b' or 'B' to go back to main menu:
```

**4. Compute Current Design** - This option takes the currently selected design and runs it through a solving algorithm. This first fills in any missing input parameters and then calculates the required values for the inductor(s) and capacitor(s) needed to build the converter. If too many parameters are unknown the user is redirected to the edit parameters menu and prompted to enter additional values.

```
>> Design computation

                Name: ANewConvertName
---------------------------------------------------------
Type: Boost
---------------------------------------------------------
|1.      V_o: 45.0000            2.     V_i: 30.0000    |
|3.      I_o: -1.0000            4.     I_i: -1.0000    |
|5.      R_l: 300.0000           6.     F_s: 10000.0000 |
|7.  delta_i: 0.5000             8. delta_v: 4.0000     |
---------------------------------------------------------
                Converter Components 
         Primary Inductor (L): -1.0000e+00 H
        Output Capacitor (C_o): -1.0000e+00 F
---------------------------------------------------------

---------------------------------------------------------
Duty cycle computed as: 0.3333
I_o computed as: 0.1500
I_i computed as: 0.2250
Converter computation successful, moving to component calculations...
L computed as: 2.0000e-03H

                Name: ANewConvertName
---------------------------------------------------------
Type: Boost
---------------------------------------------------------
|1.      V_o: 45.0000            2.     V_i: 30.0000    |
|3.      I_o: 0.1500             4.     I_i: 0.1500     |
|5.      R_l: 300.0000           6.     F_s: 10000.0000 |
|7.  delta_i: 0.5000             8. delta_v: 4.0000     |
---------------------------------------------------------
                Converter Components
         Primary Inductor (L): 2.0000e-03 H
        Output Capacitor (C_o): 1.2500e-06 F
---------------------------------------------------------

---------------------------------------------------------
```

**5. Save/Load Design** - This option allows designs to be saved and opened from the `/saves` directory. From the submenu of this option either loading or saving can be selected.
 - *Loading* prints a list of files currently in `/saves` and prompts the user to enter the name of the design they wish to load. That file is then read and loaded into app.
 - *Saving* writes the current converter design to  `/saves/NAME.json` where NAME is the name field set under menu option 1.



