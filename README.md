# SeniorDesign
This project was an RGB LED Display. It consited of an 200 pixel RGB LED panel that is driven by hardware, an RFID reader and a sound system.

---------------------------------------------------------

The led_driver.h is used to drive the Atmel AVR microprocessor. It uses a full port to drive shift registers which drive transistors to 
multiplex the LED display. This is the most important part of the project.

The other code files is the game TETRIS that is played on the board. The game files utilize the driver file in order to display the game 
onto the panel.

There is a PCB for every piece of hardware and all the files are uploaded here. Will update with an explanation of each file.
