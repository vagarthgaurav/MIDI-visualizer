#include <MIDI.h>
#include <SoftwareSerial.h>
#include "screen.h"
#include "shape.h"

// Setting up MIDI suart connection
SoftwareSerial midiSerial(2, 3);
MIDI_CREATE_INSTANCE(SoftwareSerial, midiSerial, MIDI);

// Setting up LED screen
Screen screen(3, 100, 2);

// Preallocating shapes. Arduino memory is tiny, 20 shapes already put us at an overall 65% memory consumption.
Shape shapes[30];

int refreshRate = 120;

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
    // We can change shapes and behaviour by swapping ShapeConfigs.
    ShapeConfig config = {
        .type = ShapeType::COLUMN,
        .fadeInDuration = 0.3,
        .fadeInStartScaleMult = 1.5,
        .fadeOutDuration = 0.3,
        .fadeOutEndScaleMult = 1.5,
        .pulsePeriod = 1.0,
        .pulseOpacityLowMult = 0.75,
        .pulseHueRotationLowDeg = 25};

    Shape::sConfig = config;
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

unsigned long lastLoopTime = 0;
unsigned long framePeriodMs = 1000 / refreshRate;

void loop()
{
    // Locking to a reasonable refresh rate
    auto now = millis();
    auto loopStartTime = now;
    if (lastLoopTime == 0)
        lastLoopTime = now;
    auto dt = now - lastLoopTime;
    if (dt < framePeriodMs)
        return;

    // Need to keep this read() in a loop, otherwise events wont trigger
    MIDI.read();

    // Update all the shapes
    for (auto shape : shapes)
    {
        if (shape.isActive())
            shape.update(dt);
    }

    // Loop through all of the screen pixels and sample all of the objects from each pixel. Mix colors (if pixel belongs to multiple objects), set pixel color.
    screen.perPixelUpdate(pixelBrain);

    auto loopEndTime = millis();
    lastLoopTime = loopStartTime;

    Serial.print("Performance: frame duration: ");
    Serial.print(loopEndTime - loopStartTime);
    Serial.print("ms, frame period: ");
    Serial.print(dt);
    Serial.println("ms.");
}

Color pixelBrain(const AbsPosition &absPos, const RelPosition &relPos)
{
    // Serial.print(relPos.x);
    // Serial.print(" ");
    // Serial.println(relPos.y);

    Color color = {0, 0, 0, 0};

    // TO DO: Sample colors from every object (Hope arduino will be able to handle that), mix colors.
    for (auto shape : shapes)
    {
        if (shape.isActive())
        {
            // For now this is simple a color of a last active shape to which this pixel belong
            auto c = shape.sampleColor(relPos);
            if (c.a != 0)
                color = c;
        }
    }

    // Return pixel color
    return color;
}
