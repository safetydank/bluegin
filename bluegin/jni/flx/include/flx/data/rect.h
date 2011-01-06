#pragma once

namespace flx {

/**
 * Stores a rectangle.
 */
class Rect
{
public:
    Rect(double rx=0, double ry=0, double w=0, double h=0);
    virtual ~Rect();

    void set(double rx, double ry, double rw, double rh);

    double x, y, width, height;

    double left();
    double right();
    double top();
    double bottom();
};

}

