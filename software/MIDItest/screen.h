#ifndef Screen_h
#define Screen_h

#include "types.h"

class Screen
{
public:
    Screen(int rows, int columns, int skip)
    {
        mRows = rows;
        mColumns = columns;
        mSkip = skip;
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
                setPixel(ledInd, pixelColor);

                ledInd++;
            }
            // Skip necessary amount of leds at the end of the row
            ledInd += mSkip;
        }
    }

private:
    int mRows;
    int mColumns;
    int mSkip;

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