#include "flx/data/matrix.h"

using namespace flx;
using namespace ci;

// 2d transformation matrix
Matrix::Matrix()
{
    for (int i=0; i < 9; ++i)
        m[i] = 0;
}

void Matrix::setToIdentity()
{
    for (int i=0; i < 9; ++i)
        m[i] = 0;
    m[0] = 1.0f; m[4] = 1.0f; m[8] = 1.0f;
}

void Matrix::translate(ci::Vec2f& pos)
{
    m[6] += pos.x * m[0] + pos.y * m[3];
    m[7] += pos.y * m[1] + pos.y * m[4];
}

void Matrix::rotate(float angle)
{
    m[0] = m[0] *  cos(angle) + m[3] * sin(angle);
    m[3] = m[0] * -sin(angle) + m[3] * cos(angle);
    m[1] = m[1] *  cos(angle) + m[4] * sin(angle);
    m[4] = m[1] * -sin(angle) + m[4] * cos(angle);
}

void Matrix::scale(Vec2f& s)
{
    m[0] *= s.x;
    m[1] *= s.x;
    m[2] *= s.x;
    m[3] *= s.y;
    m[4] *= s.y;
    m[5] *= s.y;
}

const Vec2f Matrix::operator*( const Vec2f &rhs ) const
{
    return Vec2f(m[0] * rhs.x + m[3] * rhs.y + m[6],
                 m[1] * rhs.x + m[4] * rhs.y + m[7]);
}

