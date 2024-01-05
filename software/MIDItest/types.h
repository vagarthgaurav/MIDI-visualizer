#ifndef types_h
#define types_h

struct Color
{
    byte r;
    byte g;
    byte b;
    byte a;
};

struct RelPosition
{
    float x;
    float y;
};

struct AbsPosition
{
    int x;
    int y;
};

enum ShapeType
{
    COLUMN
};

struct ShapeConfig
{
    ShapeType type;
    float fadeInDuration;
    float fadeInStartScaleMult;
    float fadeOutDuration;
    float fadeOutEndScaleMult;
    float pulsePeriod;
    float pulseOpacityLowMult;
    float pulseHueRotationLowDeg;
};

#endif