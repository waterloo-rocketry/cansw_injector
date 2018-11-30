### Injector Board Control Software

Runs on the injector PCB (thank mr goose). To load code onto the board:
* Open MPLAB X
* Connect a PICKIT 3 to your computer via USB
* Line up the PICKIT cable such that the red wire lines up with A) the arrow on the PICKIT, and B) the MCLR marking on the board.
* Power the board through the 5V bus input. The power from the PICKIT is not sufficient to power the entire board.
* Connect a separate 12V power source to the large screw terminal at the bottom of the board. This powers the injector valve and its control circuitry.
* Load the code onto the board using MPLAB.

The injector valve connects to the board through the large barrier block at the bottom of the board. The pins for the limit switches and valve connections are labeled on the bottom side of the board. Double check the valve connections - improper connections can damage the valve and the board.
