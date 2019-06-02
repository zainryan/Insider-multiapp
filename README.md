# Insider-multiapp
![Status](https://img.shields.io/badge/Version-Experimental-green.svg)
![License](https://img.shields.io/cran/l/devtools.svg)

- [Insider-multiapp](#insider-multiapp)
  * [Restrictions](#restrictions)
  * [Build from source](#build-from-source)
  * [Usage](#usage)
    + [Code Synthesis](#code-synthesis)
    + [C Simulation](#c-simulation)
    + [C-RTL Co-Simulation](#c-rtl-co-simulation)

## Restrictions
We have not integrated the support of simultaneous multiple applications into Insider compiler. Instead, we provide a template that supports three concurrent applications; the user can replace the placeholder kernel with their application logic.

This version is simply a **proof of concept**. Since it is not integrated with Insider compiler, the user needs to manually add the reset logic into their kernels which is somehow cumbersome. For the normal use case, please adopt the [single-application version](https://github.com/zainryan/INSIDER-System) which has the full end-to-end compilation support.

## BUILD from Source
The same as the [one](https://github.com/zainryan/INSIDER-System#build-from-source) in the single-application version.

## Usage

We provide the template in the `template` folder which is structured as the following.
```
template
 |------- cosim      # The template for C-RTL co-simulation.
 |------- host       # Host code folder which contains the data generator and the host program.
 |------- synthesis  # The template for code synthesis.
```
### Code Synthesis

`synthesis` folder contains the code for the end-to-end execution. It contains three very simple pass-through kernels (`kernels/app_pt_{0, 1, 2}`) that have different executing rates. The user can replace that with their application logic, and `interconnects.cpp` should be modified accordingly.

After that, execute `staccel_syn` to generate the `project` folder for the code synthesis. Steps go exactly the same as the [ones](https://github.com/zainryan/INSIDER-System#compiling-device-code) in the single-application version.

### C Simulation

Execute `staccel_csim` in the folder `Insider-multiapp/template/synthesis/` will generate the C-Sim folder `csim`. After that, edit `csim/src/interconnects.cpp` to add user's CSIM logic. Steps go exactly the same as the [ones](https://github.com/zainryan/INSIDER-System#c-simulation) in the single-application version.

We have already provided an example at `Insider-multiapp/template/synthesis/csim`. Please refer to it to see how to write the CSIM logic. 

### C-RTL Co-Simulation

We provide a template at `Insider-multiapp/template/cosim/`. User first needs to replace `kernels/app_pt*` into their application kernels, and then updates `interconnects.cpp` accordingly to instantiate them. After that, execute `staccel_syn` to generate the `project` folder. User needs to modify `project/software/verif_rtl/src/test_main.c` to add their COSIM logic. The original`test_main.c` contains our example code. 

Finally, execute `make C_TEST=test_main` in `project/verif/scripts`. Steps go exactly the same as the [ones](https://github.com/zainryan/INSIDER-System#c-rtl-co-simulation) in the single-application version.
