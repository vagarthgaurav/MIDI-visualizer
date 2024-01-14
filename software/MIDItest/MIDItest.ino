

#include <MIDI.h>
#include <SoftwareSerial.h>
#include "screen.h"
#include "shape.h"

using Transport = MIDI_NAMESPACE::SerialMIDI<SoftwareSerial>;

// Setting up MIDI suart connection
SoftwareSerial midiSerial(5, 3);
// MIDI_CREATE_INSTANCE(SoftwareSerial, midiSerial, MIDI);
Transport serialMIDI(midiSerial);
MIDI_NAMESPACE::MidiInterface<Transport> MIDI((Transport &)serialMIDI);
// Setting up LED screen
#define PIN 8
#define NUMPIXELS 60
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Screen screen(3, 20, 0, &pixels);

// Preallocating shapes. Arduino memory is tiny, 20 shapes already put us at an overall 65% memory consumption.
Shape shapes[30];
HSLColor shapeColor;

int refreshRate = 120;
unsigned long lastLoopTime = 0;
unsigned long framePeriodMs = 1000 / refreshRate;

// Utility functions //////////////////
float noteToFraction(byte note)
{
    int min = 36;
    int max = 96;
    float diff = max - min;
    float frac = (float)(note - min) / diff;
    if (frac < 0)
        frac = 0.0;
    if (frac > 1.0)
        frac = 1.0;
    return frac;
}

float byteToFloat(byte b)
{
    return (float)b / 255.0f;
}

byte lerp(byte a, byte b, float f)
{
    return a + f * (b - a);
}
////////////////////////////////////////

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    // Serial.begin(115200);
    // //Serial.println("Arduino says Hi!");

    // Listening to MIDI events
    midiSerial.begin(250000);
    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.setHandleNoteOn(handleNoteOn);
    // MIDI.setHandleNoteOff(handleNoteOff);
    //  //Serial.println("Listening to note events...");

    // Setting up shape animations config, so far these settings do absolutely nothing :)
    // We can change shapes and behaviour by swapping ShapeConfigs.
    pixels.begin();
    // Init shape color
    double h;
    double s;
    double l;
    ColorConverter::RgbToHsl(255, 50, 0, h, s, l);
    shapeColor = {h, s, l};
    // Init shape config
    ShapeConfig config = {
        .type = ShapeType::COLUMN,
        .fadeInDuration = 0,
        .lifeTime = 0,
        .fadeOutDuration = 0.5,
        .hueRotationVelocity = 0};

    Shape::sConfig = config;
}

void handleNoteOn(byte channel, byte note, byte velocity)
{
    // Velocity range is 1 - 127
    // Note range in neutral octave setup is 36 - 96
    // Serial.print("Note ON: ");
    // Serial.println(note);
    // Serial.print("Velocity ON: ");
    // Serial.println(velocity);

    // pixels.setPixelColor(1, pixels.Color(255, 255, 255));
    // pixels.show();

    // WARNING: looping through a C array like that or via using sizeof work only if it was statically declared or declared in the same scope,
    // otherwise the length of an array will be unknown and the loop will only iterate over the first element. So dont pass arrays as arguments.
    for (auto &shape : shapes)
    {
        if (!shape.isActive())
        {
            // //Serial.println("MidiON Found inactive shape");
            //  Spawn a Shape in a noteToFraction(note) place on the screen.
            //  TO DO: Figure what we're gonna do with colors
            auto pos = noteToFraction(note);
            // Serial.print("Shape position ");
            // Serial.println(pos);

            Color col;
            ColorConverter::HslToRgb(shapeColor.h, shapeColor.s, shapeColor.l, col.r, col.g, col.b);
            // velocity - based alpha
            col.a = 55 + 200 * ((float)velocity / 127.0f);
            shape.init({.x = pos, .y = 0.0}, 0.1, col, note, &lastLoopTime);
            return;
        }
    }

    // digitalWrite(LED_BUILTIN, HIGH);
}

void handleNoteOff(byte channel, byte note, byte velocity)
{
    // Velocity range is 1 - 127
    // Note range in neutral octave setup is 36 - 96
    // //Serial.print("Note OFF: ");
    // //Serial.println(note);
    // //Serial.print("Velocity OFF: ");
    // //Serial.println(velocity);

    // Here we remove a shape that was created when the note was first pressed.
    // WARNING: looping through the C array like that or via using sizeof work only if it was statically declared or declared in the same scope,
    // otherwise the length of an array will be unknown and the loop will only iterate over the first element. So dont pass arrays as arguments.
    for (auto &shape : shapes)
    {
        if (shape.getNote() == note && shape.isActive())
        {
            // //Serial.println("MidiOFF Found note shape");
            shape.fadeOut();
            return;
        }
    }

    // digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
    // pixels.clear();
    // pixels.show();
    /* for (float i = 0.1; i < 1; i += 0.1)
    { // For each pixel...

        shapes[10].init({.x = (float)1 / i, .y = 0.4}, 0.2, {255, 0, 0, 1}, 35);
        // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
        // Here we're using a moderately bright green color:
        // pixels.setPixelColor(i, pixels.Color(0, 150, 0));

        // Loop through all of the screen pixels and sample all of the objects from each pixel. Mix colors (if pixel belongs to multiple objects), set pixel color.
        screen.perPixelUpdate(pixelBrain);
        pixels.show(); // Send the updated pixel colors to the hardware.

        delay(1000); // Pause before next pass through loop
    } */

    // Locking to a reasonable refresh rate
    auto now = lastLoopTime + framePeriodMs;

    // Need to keep this read() in a loop, otherwise events wont trigger
    MIDI.read();

    // Rotating shape color (will only affect new shapes)
    if (Shape::sConfig.hueRotationVelocity > 0)
    {
        shapeColor.h = shapeColor.h + Shape::sConfig.hueRotationVelocity * ((float)framePeriodMs / 1000.0f);
        if (shapeColor.h >= 1)
            shapeColor.h = 0;
    }
    // Update all the shapes
    for (auto &shape : shapes)
    {
        if (shape.isActive())
            shape.update(framePeriodMs);
    }

    // Loop through all of the screen pixels and sample all of the objects from each pixel. Mix colors (if pixel belongs to multiple objects), set pixel color.
    screen.perPixelUpdate(pixelBrain);
    pixels.show();

    lastLoopTime = now;

    ///* //Serial.print("Performance: frame duration: ");
    // //Serial.print(loopEndTime - loopStartTime);
    // //Serial.print("ms, frame period: ");
    // //Serial.print(dt);
    // //Serial.println("ms."); */
    delay(framePeriodMs);
}

Color pixelBrain(const AbsPosition &absPos, const RelPosition &relPos)
{
    //// //Serial.print(relPos.x);
    //// //Serial.print(" ");
    //// //Serial.println(relPos.y);

    Color color = {0, 0, 0, 0};
    bool nocolor = true;

    // TO DO: Sample colors from every object (Hope arduino will be able to handle that), mix colors.
    for (auto &shape : shapes)
    {
        if (shape.isActive())
        {
            // For now this is simple a color of a last active shape to which this pixel belong
            auto c = shape.sampleColor(relPos);
            if (c.a != 0)
            {
                if (nocolor)
                {
                    color = c;
                    nocolor = false;
                }
                else
                {
                    color.a = lerp(color.a, 255, byteToFloat(c.a));
                }
            }
        }
    }

    // Return pixel color
    return color;
}
