#ifndef Shape_h
#define Shape_h

#include "Arduino.h"
#include "types.h"
#include <Deneyap_RenkDonusturme.h>

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

    void init(RelPosition position, float width, Color color, byte note, unsigned long *now)
    {
        // Revitalise and display the object
        mNow = now;
        mAnchorTime = *now;
        mActive = true;
        mFadingIn = true;
        mFadingOut = false;

        mNote = note;
        mPosition = position;
        mWidth = width;
        mColor = color;
        originalAlpha = color.a;
    }

    void fadeOut()
    {
        // Start fade out animation
        // Serial.print("Fading out at timepoint ");
        // Serial.println(*mNow);
        mAnchorTime = *mNow;
        mFadingIn = false;
        mFadingOut = true;
    }

    void update(unsigned long dt)
    {
        // Runs every update, responsible for animating object properties
        if (!mActive)
            return;

        // Checking if its time to fade out
        float dif = (*mNow - mAnchorTime) / 1000.0f;
        if (!mFadingOut && dif >= sConfig.lifeTime)
        {
            fadeOut();
            return;
        }

        // Are we done fading out?
        if (mFadingOut)
        {
            auto progress = dif / sConfig.fadeOutDuration;

            if (progress >= 1.0f)
                progress = 1.0f;

            mColor.a = (1 - progress) * originalAlpha;

            if (dif > sConfig.fadeOutDuration)
            {
                mActive = false;
                mFadingOut = false;
            }
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
            {
                return mColor;
            }

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
    unsigned long *mNow;

    // This is used to save start timestamp for both fadeIn and fadeOut animation.
    unsigned long mAnchorTime;

    Color mColor;
    byte originalAlpha;

    RelPosition mPosition;
    float mWidth;

    bool mFadingIn;
    bool mFadingOut;
    bool mActive;
};

// Apparently we need to define those statics here as well, or everything breaks.
ShapeConfig Shape::sConfig;

#endif