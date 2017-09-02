# RGB LED Display & Tetris Game

__________________________________________________________________________________________________________________________________________

This project was an RGB LED Display. It consited of an 200 pixel RGB LED panel that is driven by hardware, an RFID reader and a sound system. The game Tetris was played using a wireless controller to control the movement of pieces being displayed on the panel. This repository contains all the code that went into this project. I may upload the PCB files in the future.

This repository also includes the Atmel Studio project file that can be opened with Atmel Studio.

The led_driver.h is used to drive the Atmel AVR microprocessor. It uses a full port to drive shift registers which drive transistors to 
multiplex the LED display. This is the most important part of the project.

The other code files is the game TETRIS that is played on the board. The game files utilize the driver file in order to display the game 
onto the panel.
