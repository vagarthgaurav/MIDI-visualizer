#include <MIDI.h>
#include <SoftwareSerial.h>
#include "screen.h"
#include "shape.h"

// Setting up MIDI suart connection
SoftwareSerial midiSerial(2, 3);
MIDI_CREATE_INSTANCE(SoftwareSerial, midiSerial, MIDI);

// Setting up LED screen
// TO DO: screen.h eats up a lot of space and prevents sketch from working, it should be refactored
Screen screen(3, 100, 2);

// Preallocating shapes. Arduino memory is tiny, 20 shapes already put us at an overall 65% memory consumption.
Shape shapes[30];

unsigned long lastTimestamp = 0;

// Utility functions //////////////////
float noteToFraction(byte note)
{
    int min = 36;
    int max = 96;
    int diff = max - min;
    float frac = (note - min) / diff;
    if (frac < 0)
        frac = 0.0;
    if (frac > 1.0)
        frac = 1.0;
    return frac;
}
////////////////////////////////////////

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);
    Serial.println("Arduino says Hi!");

    // Listening to MIDI events
    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    Serial.println("Listening to note events...");

    // Setting up shape animations config, so far these settings do absolutely nothing :)
    // We can change animation behaviour by altering these settings.
    // TO DO: Store all of these in a single config struct so config presets can be easily swapped.
    Shape::sType = ShapeType::COLUMN;
    Shape::sFadeInDuration = 0.3;
    Shape::sFadeInStartScaleMult = 1.5;
    Shape::sFadeOutDuration = 0.3;
    Shape::sFadeOutEndScaleMult = 1.5;
    Shape::sPulsePeriod = 1.0;
    Shape::sPulseOpacityLowMult = 0.75;
    Shape::sPulseHueRotationLowDeg = 25;
}

void handleNoteOn(byte channel, byte note, byte velocity)
{
    // Velocity range is 1 - 127
    // Note range in neutral octave setup is 36 - 96
    Serial.print("Note ON: ");
    Serial.println(note);
    Serial.print("Velocity ON: ");
    Serial.println(velocity);

    // WARNING: looping through a C array like that or via using sizeof work only if it was statically declared or declared in the same scope,
    // otherwise the length of an array will be unknown and the loop will only iterate over the first element. So dont pass arrays as arguments.
    for (auto shape : shapes)
    {
        if (!shape.isActive())
        {
            // Spawn a Shape in a noteToFraction(note) place on the screen.
            // TO DO: Figure what we're gonna do with colors
            shape.init({.x = noteToFraction(note), .y = 0.0}, 0.2, {1, 1, 1, 1}, note);
            return;
        }
    }

    digitalWrite(LED_BUILTIN, HIGH);
}

void handleNoteOff(byte channel, byte note, byte velocity)
{
    // Velocity range is 1 - 127
    // Note range in neutral octave setup is 36 - 96
    Serial.print("Note OFF: ");
    Serial.println(note);
    Serial.print("Velocity OFF: ");
    Serial.println(velocity);

    // Here we remove a shape that was created when the note was first pressed.
    // WARNING: looping through the C array like that or via using sizeof work only if it was statically declared or declared in the same scope,
    // otherwise the length of an array will be unknown and the loop will only iterate over the first element. So dont pass arrays as arguments.
    for (auto shape : shapes)
    {
        if (shape.getNote() == note && shape.isActive())
        {
            shape.fadeOut();
            return;
        }
    }

    digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
    // Need to keep this read() in a loop, otherwise events wont trigger
    MIDI.read();

    // Track time, calculate delta time (dt), loop through all shapes and update them.
    // TO DO: Lock to a reasonable framerate (120hz?)
    auto now = millis();
    if (lastTimestamp == 0)
        lastTimestamp = now;

    auto dt = now - lastTimestamp;

    // Update all the shapes
    for (auto shape : shapes)
    {
        if (shape.isActive())
            shape.updateTick(dt);
    }

    // Loop through all of the screen pixels and sample all of the objects from each pixel. Mix colors (if pixel belongs to multiple objects), set pixel color.
    screen.forEach(pixelBrain);
}

Color pixelBrain(const AbsPosition &absPos, const RelPosition &relPos)
{
    Serial.print(relPos.x);
    Serial.print(" ");
    Serial.println(relPos.y);

    // TO DO: Sample colors from every object (Hope arduino will be able to handle that), mix colors.

    // Return pixel color
    return {0, 0, 0, 0};
}
