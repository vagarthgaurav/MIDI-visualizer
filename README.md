# MIDI Visualiser

by: Vagarth Gaurav and Maksim Eremenko


**Faculty of Technology and Bionics**

***Rhine-Waal University of Applied Sciences***

Date: 23 January 2024

----

## Abstract

The MIDI Visualiser implements an overarching idea of showing visual patterns on a rudimentary screen made of smart light-emitting diode (LED) strips based on the Musical Instrument Digital Interface (MIDI) keyboard output. The board design includes a buck-converter that steps down the power input voltage from 12v to 5v for the ATmega micro controller unit (MCU), while also feeding the 12v to LED strips. MCU receives a MIDI input through a software serial, processes it and sends an output signal controlling an LED strip to display different patterns based on the pressed key. Out of curious ambition - prevalently SMD components were used, which ultimately led to a failed attempt to print at Hochschule Rhein-Waal (HSRW), instead the PCB was ordered from a third party. After arduous soldering, resoldering, coding and debugging the concept prooved to be feasible. Although the prototype is working - further code optimisation is necessary and additional ergonomic improvements should be considered.

## Table of Contents

[[_TOC_]]

## 1 Introduction

From the very dawn of Humankind our senses were the main gateway of percieving and accepting the world around us. Throughout the social evolution of Homo sapiens we've developed peculiar ways of stimulating our senses, ways that "make sense" only from a perspective of a concious being with ears and sight. A marvelous play of stimulations to our hearing facilities that became known as music, and an intriguing play of visual symbols that found itself in different aspects of visual art and media. Conciously or not - our project stands among the attempts to unite and enhance an auditory sensation with a visual one.

At the end of 20th centiry music entered electronic era, synthesizers rose in popularity and MIDI was developed. Digital MIDI signals can be sent by any MIDI-compatibly musical instrument, such as keyboards, guitars and other more exotic contraptions. These signals contain information about the note being pressed or released, which can be processed by a 3rd party computing unit usually responsible for generating a corresponding sound, but in our case - responsible for generating visual patterns on an LED screen made of smart LED strips (further simply "LED screen").

LED screen therefore can dynamically respond to the notes being played, different animated patterns can be shown in different parts of the screen. Patterns can be colored and animated based on the press and release velocities creating an immersive supplementary experience to the music being played.

The following Chapter 2 presents our aproach at implementing this idea, hardware we designed and software we wrote, as well as inevitable issues we've encountered.
Chapter 3 describes and illustrates the results, our success and our shortcomings. 
And finally Chapter 4 will discuss a viability of the aproach and potential improvements.

## 2	Methodology

### 2.1	Concept development
The initial concept was outlined in the following diagram:

![alt text](resources/block_diagram.jpeg "Block Diagram")
A block diagram of an initial idea.

And the following MIDI schematic depicting a MIDI input and throughput isolated from the process control block (PCB) circuit via an opto-coupler.

![alt text](resources/midi_schematic_ref2.gif "Block Diagram")
Isolated MIDI IN and THRU.
(C) 1985 MIDI Manufacturers Association via https://diyaudiocircuits.com/midi-buffer/

Additionally we already had a 12V 5A power source in our possession, so it was decided to design the solution around that power input.

From these the an idea of core circuit elements crystalised:
- Voltage regulator that can efficiently step down from 12V to 5V.
- MIDI input and throughput isolated from the circuit by the means of optocoupler.
- 5V ATmega328p MCU to read and interpret incoming MIDI signal.
- LED screen made of 3 rows of smart LED strips powered by 12V line.
- Performant software to bind MIDI input with visual LED screen output.
- Programmer pins
- Pins for power and auxilarry switches

### 2.2	Circuit design

The circuit was designed in KiCad

SMD components vere selected wherever possible (ADD INFO ON PACKAGE SIZE?) for their compact size and as a personal opportunity to discover SMD PCB manufacturing process.
As a voltage regulator - buck convertor was used due to the high efficiency in comparison to archaic linear voltage regulators.

FURTHER CIRCUIT DETAILS

### 2.3	Prototyping

A BIT ABOUT THOSE FEW PROTOTYPING ATTEMPTS WE MADE

### 2.4	Printing

The first attempt to pring the KiCad design was made using HSRW electronics lab chemical PCB etching facilities. 
Printing process included following steps:
PCB ETCHING STEPS IN SHORT? OR JUST DROP THE MENTION OF STEPS?

Unfortunately, but not unexpectedly, due to the small width of SMD pads and traces (under 0.5mm) too many of the traces got destroyed during the etching process. As a result it was decided to order an etched PCB from a 3rd party. Fortunately this time we received a perfect PCB that didn't require any manual alteration, so we were able to start component soldering process.

### 2.5 Soldering

Soldering pipeline used involved a carefull application of a limited amount of soldering paste to component pads using soldering paste pump, followed by a precise placement of SMD components using magnetic pick-and-place machine. Afterwards, PCB was placed in a reflow oven which melted the paste effectively soldering the components in place. Initial attempt was not ideal, MCU came out of the oven with multiple pins short-circuited with melted solder. A cleanup attempt employing soldering iron and a solder braid succeeded only partially. It was decided to desolder the MCU using the heatgun in the process of which the first PCB was destroyed.
PCB soldering was started from scratch using a fresh PCB and a new set of components, same process was repeated, yet second bake resulted in a resounding success with no short-circuits.

Following that thru-hole components (THC), such as MIDI ports, power jack and connector pins - were soldered. Subsequent connectivity tests showed that hardware functions as expected and opened a road ahead to software development and testing.

### 2.6 Software

Software was written using Arduino IDE as well as Visual Studio Code with an Arduino plugin.
The core idea behind the software was inspired by a style of image rendering employed by graphics processing unit (GPU) shaders. The software implements multiple crucial concepts:
- A screen, where every pixel is an LED adressable by an x and y coordinate.
- A shape that can have a specific size, position and color.
- A screen pixel that can sample every existing shape to determine it's own color, potentially as a blend of colors of intersecting shapes.

Following libraries were used:

Every main loop iteration - each screen pixel attempts to sample every existing shape, mix the colors of all the shapes it belong to (if any) and based on that conclude it's own color:

At the same time, MIDI input is listened to and in an event of a key press or release - new shape is created or removed based on the note value in a way that leftmost keys generate shapes in the leftmost part of the screen, while rightmost - in the rightmost. The shape was meant to quickly fade in and gradually fade out when the key is released, although this aproach was later altered.

The downside of this aproach: slow code

Additionally - the research suggests that due to limitations of ATmega memory - dynamic memory management is highly undesirable due to possible rapid memory fragmentation leading to an inevitable silent crash. In response to that - everything, including shape instance was preallocated, but due to limited amount of memory (2kb) the hard limit of 30 simultaneous shapes on the screen was introduced.

Throughout the implementation testing it became obvious that some of the MIDI events are simply missing. Through the following process of research and debugging it was discovered that the culprit migh've been an initial ignorant decision to use a software serial pin instead of the hardware serial one, it appears that in a former case some serial signals can be lost when MCU is busy with other work. 
If a shape dissapearance depends of a MIDI note-off signal - then if such signal went missing - shape will linger, which is visually noticeably and undoubtly undesirable. Since we had no ability to reroute the MIDI trace to hardware serial pin - the issue was worked around on the software side by listening only to note-on events that spawn the shape and fading out the shape after a set amount of time, this way it was possible to trade noticeably false positives for less notiseable false negatives.

This is an example how to include code snippet:
```python
def function():
    #indenting works just fine in the fenced code block
    s = "Python code"
    print s
```

Example how to draw a table:

| Left Aligned | Centered | Right Aligned |
| :---         | :---:    | ---:          |
| Cell 1       | Cell 2   | Cell 3        |
| Cell 4       | Cell 5   | Cell 6        |
| OpAmp 741    | 2        | 1.00          |


## 5	Results
Here you should present your results.

This is an example how to include image:
![alt text](resources/Open_Source_Hardware_(OSHW)_Logo_on_blank_PCB.jpg "Example Image")
(C) Altzone, CC BY-SA 3.0 <https://creativecommons.org/licenses/by-sa/3.0>, via Wikimedia Commons


## 6	Discussion
SLOW CODE CAN BE OPTIMISED, but otherwise everything is pretty much as expected?
Software serial should be changed to hardware serial.
Maybe mention the overall cost here?


## 7	Concluding Comments
You might want to discuss possible future work here

ERGONOMICS? A printed case for LED screen with a despersing screen? Clamping the whole contraption to the MIDI keyboard?

## 8	References

* [1]
* [2] 

## 9	Appendices

