#pragma once

#include "cinder/Vector.h"

namespace flx {

/**
 * Not used
 *
 * Will eventually replace Cinder's 4x4 matrices with 3x3 matrices,
 * for 2D transforms.
 */
class Matrix
{
	// m[idx]
	// | m[0] m[3] m[6]|   c  -s
	// | m[1] m[4] m[7]|   s   c
	// | m[2] m[5] m[8]|

private:
    float m[9];

public:
    Matrix();
    void setToIdentity();
    void translate(ci::Vec2f& pos);
    void rotate(float angle);
    void scale(ci::Vec2f& scaling);

	const ci::Vec2f operator*( const ci::Vec2f &rhs ) const;
};

}
