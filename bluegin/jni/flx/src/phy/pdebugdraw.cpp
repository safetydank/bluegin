#include "flx/phy/pdebugdraw.h"
#include "cinder/gl/gl.h"

#include <cstdio>
#include <cstdarg>

#include <cstring>

using namespace ci;
using namespace flx;

PDebugDraw::PDebugDraw(Camera& cam) : camera(cam)
{
}

void PDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    gl::color(Color(color.r, color.g, color.b));
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void PDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    gl::color(ColorA(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f));

    glEnableClientState(GL_VERTEX_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);

	glDisable(GL_BLEND);

    gl::color(ColorA(color.r, color.g, color.b, 1.0f));
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_LINE_LOOP, 0, vertexCount);

    glDisableClientState(GL_VERTEX_ARRAY);
}

void PDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
    const int segments = 16;
    gl::color(Color(color.r, color.g, color.b));
    gl::drawStrokedCircle(Vec2f(center.x, center.y), radius, segments);
}

void PDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
    const int segments = 16;

	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    gl::color(ColorA(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f));
    gl::drawSolidCircle(Vec2f(center.x, center.y), radius, segments);
	glDisable(GL_BLEND);

    DrawCircle(center, radius, color);

    // XXX draw axis
	// b2Vec2 p = center + radius * axis;
	// glBegin(GL_LINES);
	// glVertex2f(center.x, center.y);
	// glVertex2f(p.x, p.y);
	// glEnd();
}

void PDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    gl::color(Color(color.r, color.g, color.b));
    gl::drawLine(Vec2f(p1.x, p1.y), Vec2f(p2.x, p2.y));
}

void PDebugDraw::DrawTransform(const b2Transform& xf)
{
	const float32 k_axisScale = 0.4f;

	b2Vec2 p1 = xf.position, p2;
   
    gl::color(Color(1.0f, 0.0f, 0.0f));
    p2 = p1 + (k_axisScale * xf.R.col1);
    gl::drawLine(Vec2f(p1.x, p1.y), Vec2f(p2.x, p2.y));

    gl::color(Color(0.0f, 1.0f, 0.0f));
	p2 = p1 + (k_axisScale * xf.R.col2);
    gl::drawLine(Vec2f(p1.x, p1.y), Vec2f(p2.x, p2.y));
}

void PDebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	// glPointSize(size);
	// glBegin(GL_POINTS);
	// glColor3f(color.r, color.g, color.b);
	// glVertex2f(p.x, p.y);
	// glEnd();
	// glPointSize(1.0f);
}

void PDebugDraw::DrawString(int x, int y, const char *string, ...)
{
	// char buffer[128];

	// va_list arg;
	// va_start(arg, string);
	// vsprintf(buffer, string, arg);
	// va_end(arg);

	// glMatrixMode(GL_PROJECTION);
	// glPushMatrix();
	// glLoadIdentity();
	// int w = glutGet(GLUT_WINDOW_WIDTH);
	// int h = glutGet(GLUT_WINDOW_HEIGHT);
	// gluOrtho2D(0, w, h, 0);
	// glMatrixMode(GL_MODELVIEW);
	// glPushMatrix();
	// glLoadIdentity();

	// glColor3f(0.9f, 0.6f, 0.6f);
	// glRasterPos2i(x, y);
	// int32 length = (int32)strlen(buffer);
	// for (int32 i = 0; i < length; ++i)
	// {
	// 	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, buffer[i]);
	// }

	// glPopMatrix();
	// glMatrixMode(GL_PROJECTION);
	// glPopMatrix();
	// glMatrixMode(GL_MODELVIEW);
}

void PDebugDraw::DrawAABB(b2AABB* aabb, const b2Color& c)
{
    Vec2f vertices[4] = {
	    Vec2f(aabb->lowerBound.x, aabb->lowerBound.y),
	    Vec2f(aabb->upperBound.x, aabb->lowerBound.y),
	    Vec2f(aabb->upperBound.x, aabb->upperBound.y),
	    Vec2f(aabb->lowerBound.x, aabb->upperBound.y),
    };

    gl::color(Color(c.r, c.g, c.b));
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
}

