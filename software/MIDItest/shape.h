#ifndef Shape_h
#define Shape_h

#include "Arduino.h"
#include "types.h"

// Using inheritance sounds like a bad idea since we have to preallocate everything and cant use "new".
// So instead this class will handle every type of a displayed shape we might want to implement
class Shape
{
public:
    Shape(){};

    // These are global configs affecting all the shapes, not only this specific instance
    // keeping them global saves memory.
    static ShapeConfig sConfig;
    ////////////////////////////////////////////////////////////////////////////////////

    void init(RelPosition position, float width, Color color, byte note)
    {
        // Revitalise and display the object
        mAnchorTime = millis();
        mActive = true;
        mFadingIn = true;
        mFadingOut = false;

        mNote = note;
        mPosition = position;
        mWidth = width;
        mColor = color;
    }

    void fadeOut()
    {
        // Start fade out animation
        mAnchorTime = millis();
        mFadingIn = false;
        mFadingOut = true;
    }

    void update(unsigned long dt)
    {
        // Note: dt is in milliseconds here
        // Runs every update, responsible for animating object properties
        if (mFadingOut)
        {
            // Not animating anything for now, merely short-circuiting
            mColor.r = 0;
            mColor.g = 0;
            mColor.b = 0;
            mColor.a = 0;

            mActive = false;
            mFadingOut = false;
        }
    }

    Color sampleColor(const RelPosition &relPos)
    {
        // Different shape types can be implemented here
        // Given pixel screen position (in 0 - 1) what is a color of this pixel if it's within this Shape?
        if (sConfig.type == ShapeType::COLUMN)
        {
            // Can probably fade out the edges instead of abruptly cutting them
            if (relPos.x >= mPosition.x - mWidth / 2 && relPos.x <= mPosition.x + mWidth / 2)
                return mColor;
            else
                return {0, 0, 0, 0};
        }
    };

    bool isActive()
    {
        // Inactive objects can be reused
        return mActive;
    };

    byte getNote()
    {
        return mNote;
    };

private:
    byte mNote;

    // This is used to save start timestamp for both fadeIn and fadeOut animation.
    unsigned long mAnchorTime;

    Color mColor;

    RelPosition mPosition;
    float mWidth;

    bool mFadingIn;
    bool mFadingOut;
    bool mActive;
};

// Apparently we need to define those statics here as well, or everything breaks.
ShapeConfig Shape::sConfig;

#endif