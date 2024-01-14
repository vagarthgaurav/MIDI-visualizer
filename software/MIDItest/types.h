#ifndef types_h
#define types_h

struct HSLColor
{
    double h;
    double s;
    double l;
};

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
    float lifeTime;
    float fadeOutDuration;
    float hueRotationVelocity;
};

#endif