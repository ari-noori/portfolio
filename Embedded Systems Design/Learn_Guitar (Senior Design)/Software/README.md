# ECE453 Bucky-Humbuckers Final Project - Software

This is the final design project for the Bucky-Humbuckers. This uses the FreeRTOS configuration from one of the Modus Toolbox examples layered on a new project to utilize the updated HAL. It uses FatFs as a file storage system to load in images to free SRAM for usage.

## Requirements

- [ModusToolbox&trade; software](https://www.cypress.com/products/modustoolbox-software-environment) v2.4.1 or later (used on v2.4.1). Modus Toolbox will display errors on v3.x.x however the code compiles.

## Supported toolchains (make variable 'TOOLCHAIN')

- GNU Arm® embedded compiler v9.3.1 (`GCC_ARM`) - Default value of `TOOLCHAIN`
- Arm&reg; compiler v6.13 (`ARM`)
- IAR C/C++ compiler v8.42.2 (`IAR`)

## Supported kits (make variable 'TARGET')

- [PSoC&trade; 6 Bluetooth&reg; LE prototyping kit](https://www.cypress.com/CY8CPROTO-063-BLE) (`CY8CPROTO-063-BLE`)
- Our custom PCB

## Related resources

Resources  | Links
-----------|----------------------------------
FatFs | http://elm-chan.org/fsw/ff/00index_e.html <br>
FreeRTOS | https://www.freertos.org/ <br>
Cypress HAL | https://infineon.github.io/psoc6hal/html/index.html <br>
