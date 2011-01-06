#pragma once

namespace flx {

struct Accelerometer
{
    float x;
    float y;
    float z;

    void set(float ax, float ay, float az) { x = ax; y = ay; z = az; }
};

}
