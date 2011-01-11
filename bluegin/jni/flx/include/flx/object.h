#pragma once

#include "flx/flx.h"
#include "flx/data/rect.h"
#include "cinder/Vector.h"

namespace flx {

// Object kind
const int OBJECT = -1;

//  Forward declarations of friends
class Group;
class QuadTree;

class Object;
typedef shared_ptr<Object> ObjectPtr;

/**
 * This is the base class for most of the display objects (<code>Sprite</code>, <code>Text</code>, etc).
 * It includes some basic attributes about game objects, including retro-style flickering,
 * basic state information, sizes, scrolling, and basic physics & motion.
 */
class Object : public Rect
{
public:
    static ObjectPtr create(float x=0, float y=0, float w=0, float h=0);

    Object(float x=0, float y=0, float w=0, float h=0);
    virtual ~Object();

    /**
     * Called by <code>Object.updateMotion()</code> and some constructors to
     * rebuild the basic collision data for this object.
     */
    virtual void refreshHulls();

    /**
     * Called by <code>Object.updateMotion()</code> and some constructors to
     * rebuild the basic collision data for this object.
     */
    void updateMotion();

    /**
     * Just updates the retro-style flickering.
     * Considered update logic rather than rendering because it toggles visibility.
     */
    void updateFlickering();

    /**
     * Checks to see if some <code>Object</code> object overlaps this <code>Object</code> object.
     * 
     * @param	Object	The object being tested.
     * 
     * @return	Whether or not the two objects overlap.
     */
    virtual bool overlaps(Object& object);

    /**
     * If you don't want to call <code>FlxU.collide()</code> you can use this instead.
     * Just calls <code>FlxU.collide(this,Object);</code>.  Will collide against itself
     * if Object==null.
     * 
     * @param	Object		The <Object> you want to collide with.
     */
    virtual bool collide(Object& object);

    /**
     * Called by the main game loop, handles motion/physics and game logic
     */
    virtual void update();

    /**
     * Override this function to draw graphics (see <code>Sprite</code>).
     */
    virtual void render() { }

    /**
     * Call this function to "kill" a sprite so that it no longer 'exists'.
     */
    virtual void kill();

    /**
     * Handy function for reviving game objects.
     * Resets their existence flags and position, including LAST position.
     * 
     * @param	X	The new X position of this object.
     * @param	Y	The new Y position of this object.
     */
    virtual void reset(float x, float y);

    /**
     * Called by <code>Group</code>, commonly when game states are changed.
     */
    virtual void destroy() { }

    /**
     * Tells this object to flicker, retro-style.
     * 
     * @param	Duration	How many seconds to flicker for.
     */
    void flicker(float Duration);

    /**
     * Check to see if the object is still flickering.
     * 
     * @return	Whether the object is flickering or not.
     */
    bool flickering();

    /**
     * <code>FlxU::collide()</code> (and thus <code>Object.collide()</code>) call
     * this function each time two objects are compared to see if they collide.
     * It doesn't necessarily mean these objects WILL collide, however.
     * 
     * @param	Object	The <code>Object</code> you're about to run into.
     */
    virtual void preCollide(Object& object) { }

    /**
     * Called when this object's left side collides with another <code>Object</code>'s right.
     * 
     * @param	Contact		The <code>Object</code> you just ran into.
     * @param	Velocity	The suggested new velocity for this object.
     */
    virtual void hitLeft(Object& contact, float velocity);

    /**
     * Called when this object's right side collides with another <code>Object</code>'s left.
     * 
     * @param	Contact		The <code>Object</code> you just ran into.
     * @param	Velocity	The suggested new velocity for this object.
     */
    virtual void hitRight(Object& contact, float velocity);

    /**
     * Called when this object's top collides with the bottom of another <code>Object</code>.
     * 
     * @param	Contact		The <code>Object</code> you just ran into.
     * @param	Velocity	The suggested new velocity for this object.
     */
    virtual void hitTop(Object& contact, float velocity);

    /**
     * Called when this object's bottom edge collides with the top of another <code>Object</code>.
     * 
     * @param	Contact		The <code>Object</code> you just ran into.
     * @param	Velocity	The suggested new velocity for this object.
     */
    virtual void hitBottom(Object& contact, float velocity);

    /**
     * Checks to see if a point in 2D space overlaps this <code>Object</code> object.
     * 
     * @param	pt			The coordinate of the point.
     * 
     * @return	Whether or not the point overlaps this object.
     */
    virtual bool overlapsPoint(ci::Vec2f pt);

    /**
     * Call this function to "damage" (or give health bonus) to this sprite.
     * 
     * @param	Damage		How much health to take away (use a negative number to give a health bonus).
     */
    void hurt(int damage);

    /**
     * Call this function to figure out the on-screen position of the object.
     * 
     * @param	P	Takes a <code>Point</code> object and assigns the post-scrolled X and Y values of this object to it.
     * 
     * @return	The <code>Point</code> you passed in, or a new <code>Point</code> if you didn't pass one, containing the screen X and Y position of this object.
     */
    virtual ci::Vec2d& getScreenXY(ci::Vec2d& pt);

    /**
     * Check and see if this object is currently on screen.
     * 
     * @return	Whether the object is on screen or not.
     */
    bool  onScreen();

    /**
     * Use this to set an object identifier on this instance that can be used to
     * compare objects of different classes.  A simple substitute for class
     * type reflection.
     *
     *  Returns the object identifier.  Passing the optional argument
     *  setKind will also set the the object identifier.
     */
    int kind(int setKind=-1);

public:
    /**
     * Kind of a global on/off switch for any objects descended from <code>Object</code>.
     */
    bool exists;

    /**
     * If an object is not alive, the game loop will not automatically call <code>update()</code> on it.
     */
    bool active;

    /**
     * If an object is not visible, the game loop will not automatically call <code>render()</code> on it.
     */
    bool visible;

    /**
     * Internal tracker for whether or not the object collides (see <code>solid</code>).
     */
    bool solid;

    /**
     * Set <code>fixed</code> to true if you want the object to stay in place during collisions.
     * Useful for levels and other environmental objects.
     */
    bool fixed;
    bool moves;

    /**
     * Flag for direction collision resolution.
     */
    bool collideLeft;
    /**
     * Flag for direction collision resolution.
     */
    bool collideRight;
    /**
     * Flag for direction collision resolution.
     */
    bool collideTop;
    /**
     * Flag for direction collision resolution.
     */
    bool collideBottom;

    /**
     * WARNING: The origin of the sprite will default to its center.
     * If you change this, the visuals and the collisions will likely be
     * pretty out-of-sync if you do any rotation.
     */
    ci::Vec2f origin;

    /**
     * The basic speed of this object.
     */
    ci::Vec2f velocity;
    /**
     * How fast the speed of this object is changing.
     * Useful for smooth movement and gravity.
     */
    ci::Vec2f acceleration;
    /**
     * This isn't drag exactly, more like deceleration that is only applied
     * when acceleration is not affecting the sprite.
     */
    ci::Vec2f drag;
    /**
     * If you are using <code>acceleration</code>, you can use <code>maxVelocity</code> with it
     * to cap the speed automatically (very useful!).
     */
    ci::Vec2f maxVelocity;

    /**
     * Set the angle of a sprite to rotate it.
     */
    int angle;

    /**
     * This is how fast you want this sprite to spin.
     */
    float angularVelocity;

    /**
     * How fast the spin speed should change.
     */
    float angularAcceleration;

    /**
     * Like <code>drag</code> but for spinning.
     */
    float angularDrag;

    /**
     * Use in conjunction with <code>angularAcceleration</code> for fluid spin speed control.
     */
    float maxAngular;
    
    /**
     * If you want to do Asteroids style stuff, check out thrust,
     * instead of directly accessing the object's velocity or acceleration.
     */
    float thrust;

    /**
     * Used to cap <code>thrust</code>, helpful and easy!
     */
    float maxThrust;
    
    /**
     * A point that can store numbers from 0 to 1 (for X and Y independently)
     * that governs how much this object is affected by the camera subsystem.
     * 0 means it never moves, like a HUD element or far background graphic.
     * 1 means it scrolls along a the same speed as the foreground layer.
     * scrollFactor is initialized as (1,1) by default.
     */
    ci::Vec2d scrollFactor;

    /**
     * Handy for storing health percentage or armor points or whatever.
     */
    int   health;

    /**
     * Handy for tracking gameplay or animations.
     */
    bool  dead;

    /**
     * Flag that indicates whether or not you just hit the floor.
     * Primarily useful for platformers, this flag is reset during the <code>updateMotion()</code>.
     */
    bool  onFloor;

    /**
     * These store a couple of useful numbers for speeding up collision resolution.
     */
    Rect colHullX;
    Rect colHullY;
    ci::Vec2f colVector;

    /**
     * An array of <code>Point</code> objects.  By default contains a single offset (0,0).
     */
    vector<ci::Vec2f> colOffsets;

    /**
	 * Dedicated internal flag for whether or not this class is a Group.
	 */
    bool _group;


protected:
    bool  _flicker;
    float _flickerTimer;

    ci::Vec2d _point;
    Rect _rect;
    Rect _flashPoint;

    friend class Group;
    friend class QuadTree;

    int _kind;

public:
    virtual bool hasFrame() { return false; }
    virtual int getFrameWidth() { return 0; }
    virtual int getFrameHeight() { return 0; }
};

}
