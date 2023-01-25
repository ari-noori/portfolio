# ECE453 Bucky-Humbuckers Final Project - Hardware

The hardware for this project was designed using Altium Designer to interface with the PSoC6 MCU. At a high level, the board interacts with the user by processing user input in the form of an electrical signal from a musical instrument and delivering feedback to the user for further action. This is accomplished by three main components of the hardware:

## Analog Signal Processing
The incoming electrical signal (coming from an electric guitar or piezo pickup) is amplified and filtered before being passed into the SAR ADC of the MCU. The signal is biased to 1.65V (the center of our 3.3V MCU), amplified by 5x, and filtered through a 2nd-order Sallen Key low-pass filter. Importantly, the impedance of our first op-amp stage is matched with the high impedance of the guitar.

## Power Delivery
Power delivery was designed with ease-of-use, flexibility, and signal integrity in mind. An LDO is used to mitigate any interference with our analog signal from the instrument. The entire device is powered by a single 5V micro-USB which can be plugged into a wall wart, PC, power bank, etc.

## LCD and MCU Integration
A large amount of debugability and flexibility was built into this board. Test points and jumpers are featured on all crucial I2C and SPI lines shared between the LCD and MCU. This allows for a more seamless integration of the LCD to provide an optimal user experience.
