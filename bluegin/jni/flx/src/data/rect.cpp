#include "flx/data/rect.h"

using namespace flx;

Rect::Rect(double rx, double ry, double w, double h)
{
    set(rx, ry, w, h);
}

Rect::~Rect() { }

double Rect::left()   { return x; }
double Rect::right()  { return x + width; }
double Rect::top()    { return y; }
double Rect::bottom() { return y + height; }

void Rect::set(double rx, double ry, double w, double h)
{
    x = rx;
    y = ry;
    width = w;
    height = h;
}
