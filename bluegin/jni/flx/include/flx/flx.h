#pragma once

// Flixel port
//
#include <vector>
#include "cinder/Vector.h"
#include "cinder/Color.h"

//  globals (static in flixel)
#ifndef FLXU_STATIC
#define FLXU_STATIC extern
#endif

#ifdef ANDROID
#include "yasper.h"
#else
#include "boost/shared_ptr.hpp"
#endif

using std::vector;
using boost::shared_ptr;

/// 2D game engine 
namespace flx {

class Object;

typedef bool (*OverlapCallback)(Object&, Object&);

/// Global utility methods for flx
namespace FlxU {
    // inline float floor(float x) { return ::floor(x); }

    ///  Round down a number
    inline float floor(float N) { 
        float n = float(int(N));
        return (N>0)?(n):((n!=N)?(n-1):(n)); 
    }
    ///  Round up a number
    inline float ceil(float N) {
        return ::ceilf(N);
    }

    /**
     * A tween-like function that takes a starting velocity
     * and some other factors and returns an altered velocity.
     * 
     * @param	vel     Any component of velocity (e.g. 20).
     * @param	acc     Rate at which the velocity is changing.
     * @param	drag    Really kind of a deceleration, this is how much the velocity changes if Acceleration is not set.
     * @param	max     An absolute value cap for the velocity.
     * 
     * @return	The altered Velocity value.
     */
    float computeVelocity(float vel, float acc, float drag, float max=10000.0f);

    /**
     * Rotates a point in 2D space around another point by the given angle.
     * 
     * @param	x		The X coordinate of the point you want to rotate.
     * @param	y		The Y coordinate of the point you want to rotate.
     * @param	pivotX	The X coordinate of the point you want to rotate around.
     * @param	pivotY	The Y coordinate of the point you want to rotate around.
     * @param	angle	Rotate the point by this many degrees.
     * @param	Dst		Optional <code>Point</code> to store the results in.
     * 
     * @return	A <code>Point</code> containing the coordinates of the rotated point.
     */
    ci::Vec2f rotatePoint(float x, float y, float pivotX, float pivotY, float angle, ci::Vec2f* dst=0);

    /**
     * Calculates the angle between a point and the origin (0,0).
     * 
     * @param	X		The X coordinate of the point.
     * @param	Y		The Y coordinate of the point.
     * 
     * @return	The angle in degrees.
     */
    float getAngle(float X, float Y);

    /**
     * Call this function to see if one <code>Object</code> overlaps another.
     * Can be called with one object and one group, or two groups, or two objects,
     * whatever floats your boat!  It will put everything into a quad tree and then
     * check for overlaps.  For maximum performance try bundling a lot of objects
     * together using a <code>Group</code> (even bundling groups together!)
     * NOTE: does NOT take objects' scrollfactor into account.
     * 
     * @param	Object1		The first object or group you want to check.
     * @param	Object2		The second object or group you want to check.  If it is the same as the first, flixel knows to just do a comparison within that group.
     * @param	Callback	A function with two <code>Object</code> parameters - e.g. <code>myOverlapFunction(Object1:Object,Object2:Object);</code>  If no function is provided, <code>QuadTree</code> will call <code>kill()</code> on both objects.
     */
    bool overlap(Object& Object1, Object& Object2, OverlapCallback Callback);

    /**
     * Call this function to see if one <code>Object</code> collides with another.
     * Can be called with one object and one group, or two groups, or two objects,
     * whatever floats your boat!  It will put everything into a quad tree and then
     * check for collisions.  For maximum performance try bundling a lot of objects
     * together using a <code>Group</code> (even bundling groups together!)
     * NOTE: does NOT take objects' scrollfactor into account.
     * 
     * @param	Object1		The first object or group you want to check.
     * @param	Object2		The second object or group you want to check.  If it is the same as the first, flixel knows to just do a comparison within that group.
     */
    bool collide(Object& object1, Object& object2);

    /**
     * This quad tree callback function can be used externally as well.
     * Takes two objects and separates them along their X axis (if possible/reasonable).
     * 
     * @param	Object1		The first object or group you want to check.
     * @param	Object2		The second object or group you want to check.
     */
    bool solveXCollision(Object& Object1, Object& Object2);

    /**
     * This quad tree callback function can be used externally as well.
     * Takes two objects and separates them along their Y axis (if possible/reasonable).
     * 
     * @param	Object1		The first object or group you want to check.
     * @param	Object2		The second object or group you want to check.
     */
    bool solveYCollision(Object& Object1, Object& Object2);

    /**
     * Call this function to specify a more efficient boundary for your game world.
     * This boundary is used by <code>overlap()</code> and <code>collide()</code>, so it
     * can't hurt to have it be the right size!  Flixel will invent a size for you, but
     * it's pretty huge - 256x the size of the screen, whatever that may be.
     * Leave width and height empty if you want to just update the game world's position.
     * 
     * @param	X			The X-coordinate of the left side of the game world.
     * @param	Y			The Y-coordinate of the top of the game world.
     * @param	Width		Desired width of the game world.
     * @param	Height		Desired height of the game world.
     * @param	Divisions	Pass a non-zero value to set <code>quadTreeDivisions</code>.  Default value is 3.
     */
    void setWorldBounds(float X=0, float Y=0, float Width=0, float Height=0, int Divisions=3);


    /**
     * Generate a pseudo-random number.
     * 
     * @return	A pseudo-random Number object.
     */
    float random();

    /**
     * Helps to eliminate false collisions and/or rendering glitches caused by rounding errors
     * Lower values result in spurious collisions on Android
     */
    const float roundingError = 1e-3f;

    /**
     * Helper function to convert from a Flixel-style 32-bit hex color value to a <code>Color</code>
     */
    ci::ColorA color(unsigned int c);

    /**
     * Controls the granularity of the quad tree.  Default is 3 (decent performance on large and small worlds).
     */
    FLXU_STATIC int quadTreeDivisions;
}

}
