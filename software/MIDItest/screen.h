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

        // TO DO: Storing all these indices takes too much space, we will have to do calculate proper index
        // each time we are trying to adress led instead
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

    void setPixel(const AbsPosition &absPos, Color col)
    {
        int ledInd = absPositionToIndex(absPos);
        // Set led color
    }

    void setPixel(const RelPosition &relPos, Color col)
    {
        auto absPos = relToAbsPosition(relPos);
        setPixel(absPos, col);
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

    AbsPosition relToAbsPosition(const RelPosition &relPos)
    {
        return {
            .x = relPos.x * (mColumns - 1),
            .y = relPos.y * (mRows - 1)};
    }

    int absPositionToIndex(const AbsPosition &position)
    {
        auto i = mLedIndices[position.y * mColumns + position.x];
    }

    void forEach(void (*fn)(const AbsPosition &absPos, const RelPosition &relPos))
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
    int mLedIndices[1000];
};
#endif