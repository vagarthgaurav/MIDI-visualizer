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
- x2 ???OpAmp (For the midi through)
- x1 Buck convertor 12V-3.3V LLM2574-3.3YN
- x1 330uH inductor
- x1 Schottky Diode 11DQ05
- x1 22uF capacitor 
- x1 220uF capacitor

- Atmega328PU (3.3V)
- ???Components required by the MCU, if any.
- MCU programming board

- 12V LED strip WS2811 

- 12V 5A Power block (already have it)

## Further research

Specified above are rought outlines, to narrow them down we need to decide/research the following:
- Input voltage of the LED strips available in the lab
- Operational voltage of the MCU
- Specifications of our power source

## Links
Convertor - https://www.mouser.de/ProductDetail/Microchip-Technology/LM2574-3.3YN?qs=kh6iOki%2FeLFm%2F7QjFwBwAg%3D%3D
Schottky - https://www.mouser.de/ProductDetail/Vishay-General-Semiconductor/SB150-E3-73?qs=u91jd7aysvNh88d3MDclzw%3D%3D



## Block diagram

![Block diagram](block_diagram.jpeg)
