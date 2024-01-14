#ifndef Screen_h
#define Screen_h

#include "types.h"
#include <Adafruit_NeoPixel.h>

class Screen
{
public:
    Screen(int rows, int columns, int skip, Adafruit_NeoPixel *pixels)
    {
        mRows = rows;
        mColumns = columns;
        mSkip = skip;
        mPixels = pixels;
    }

    void perPixelUpdate(Color (*fn)(const AbsPosition &absPos, const RelPosition &relPos))
    {
        // Reset all pixels

        // Loop through pixels and set their colors
        int ledInd = 0;
        for (int r = 0; r < mRows; r++)
        {
            for (int c = 0; c < mColumns; c++)
            {
                // Note that every odd row - led order should be flipped, taking care of that here.
                auto adjustedC = c;
                if (r % 2 != 0)
                    adjustedC = (mColumns - 1) - c;

                AbsPosition absPos = {.x = adjustedC, .y = r};
                RelPosition relPos = absToRelPosition(absPos);

                auto pixelColor = fn(absPos, relPos);
                // setPixel(ledInd, pixelColor);
                float alpha = (float)pixelColor.a / 255.0f;
                mPixels->setPixelColor(ledInd, mPixels->Color(pixelColor.r * alpha, pixelColor.g * alpha, pixelColor.b * alpha));

                ledInd++;
            }
            // Skip necessary amount of leds at the end of the row
            ledInd += mSkip;
        }
    }

private:
    int mColumns;
    int mSkip;
    int mRows;
    Adafruit_NeoPixel *mPixels;

    void setPixel(int ledInd, Color col)
    {
    }

    RelPosition absToRelPosition(const AbsPosition &absPos)
    {
        return {
            .x = (float)absPos.x / (mColumns - 1),
            .y = (float)absPos.y / (mRows - 1)};
    }

    AbsPosition relToAbsPosition(const RelPosition &relPos)
    {
        return {
            .x = relPos.x * (mColumns - 1),
            .y = relPos.y * (mRows - 1)};
    }
};
#endif