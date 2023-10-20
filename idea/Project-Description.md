# Midi keyboard visualizer.

## Concept
A dynamic midi-driven visualizer that translates an incoming MIDI signal into visual patterns on a rudimantary LED screen made of multiple rows of neopixel LED strip. 

## Overview of design 

The device would take in a MIDI input (coming from a MIDI-controller keyboard or any other source of MIDI signal) and activate the LEDS to show some interesting graphics when music is played. 

The brains of the PCB would be a MCU like an STM32 or ATmel chip. The input would be decoded and processed to create an animation. 

Optionally there could be a passthrough for daisy chaining other MIDI devices. 

## Components
See basic circuit reference images in the "/idea" folder.

Resistor values assume 5v in the main circuit.

- x4 220 Ohm resistor
- x1 4.7k Ohm resistor

- x2 Midi connectors, female
- x1 1N914 Diode
- x1 6N138 Optoisolator
- x2 OpAmp (For the midi through)
- x1 Buck convertor 12V-3.3V LM2675N-3.3/NOPB
- (!cant find this) x1 100uH inductor (see convertor datasheet table 2. e.g Pulse engineering PE-53811, renco RL-5470-5)
- x1 Convertor output capacitor (see table 3), eg Nichicon pl 120/35 or panasonic hfq 120/35
- x2 0.1 - 1 uF Capacitors. *For the linear regulator. Check specific regulator datasheet.*

- Atmega328PU (3.3V)
- Components required by the MCU, if any.
- MCU programming board

- 1600-2400mm LED Strip - Neopixel or similar. 
*Might need to simplify or look for alternatives, those dimensions of the Neopixel strip are out of our budget.*

- ??v power-source. 
*Probably can use a laptop brick or similar.*

## Further research

Specified above are rought outlines, to narrow them down we need to decide/research the following:
- Input voltage of the LED strips available in the lab
- Operational voltage of the MCU
- Specifications of our power source

## Links
Convertor - https://www.mouser.de/ProductDetail/Texas-Instruments/LM2675N-3.3-NOPB?qs=X1J7HmVL2ZGRiUKR0DT9lg%3D%3D
Convertor output capacitor https://www.mouser.de/ProductDetail/Panasonic/EEU-FC1V121BJ?qs=MtOUKumLmnbfkV83WLYzhA%3D%3D


## Block diagram

![Block diagram](block_diagram.jpeg)
