#ifndef Screen_h
#define Screen_h

#include "Arduino.h"
#include "types.h"

class Screen
{
public:
    Screen(int rows, int columns, int skip)
    {
        mRows = rows;
        mColumns = columns;

        // Apparently using new on Arduino is frowned upon heavily due to memory fragmentation,
        // but since we are doing this one only once per entire runtime - it's ok
        mLedIndices = new int[rows * columns];

        int ledInd = 0;
        for (int r = 0; r < rows; r++)
        {
            if (r != 0)
                ledInd += 2;
            for (int c = 0; c < columns; c++)
            {
                mLedIndices[r * columns + c] = ledInd;
            }
        }
    }

    void setPixel(AbsPosition absPos, Color col)
    {
    }

    void setPixel(float xFrac, float yFrac, byte r, byte g, byte b, byte a)
    {
        int x = xFrac * (mColumns - 1);
        int y = yFrac * (mRows - 1);
        setPixel(x, y, r, g, b, a);
    }

    AbsPosition indexToAbsPosition(int i)
    {
        return {
            .x = i % mRows,
            .y = floor(i / mRows)};
    }

    RelPosition absToRelPosition(const AbsPosition &absPos)
    {
        return {
            .x = absPos.x / (mColumns - 1),
            .y = absPos.y / (mRows - 1)};
    }

    AbsPosition retToAbsPosition(const RelPosition &relPos)
    {
        return {
            .x = relPos.x * (mColumns - 1),
            .y = relPos.y * (mRows - 1)};
    }

    int absPositionToIndex(const AbsPosition &position)
    {
        auto i = mLedIndices[position.y * mColumns + position.x];
    }

    void forEach(void (*fn)(AbsPosition absPos, RelPosition relPos))
    {
        int total = mRows * mColumns;
        for (int i = 0; i < total; i++)
        {
            mLedIndices[i];
            auto aPos = indexToAbsPosition(i);
            auto rPos = absToRelPosition(aPos);
            // TO DO: We dont really need ledInd here, but rather and x and y coordinates in 0 - 1.0 range. I.e same as shape Position struct.
            fn(aPos, rPos);
        }
    }

private:
    int mRows;
    int mColumns;
    int *mLedIndices;
};
#endif