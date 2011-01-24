#pragma once

#include "bluegin/graphic.h"
#include "cinder/gl/Texture.h"
#include "cinder/Color.h"
#include "cinder/Vector.h"
#include "cinder/Matrix.h"
#include "cinder/Rect.h"

#include "flx/data/anim.h"
#include "flx/object.h"

namespace flx {
    
/**
 * Useful for controlling flipped animations and checking player orientation.
 */
enum Direction_t 
{
    LEFT = 0,
    RIGHT,
    UP,
    DOWN
};

class Sprite;
typedef shared_ptr<Sprite> SpritePtr;

/**
 * The main "game object" class, handles basic physics and animation.
 *
 * @example sprite_basic.cpp
 */
class Sprite : public Object
{
public:
    static SpritePtr create(float x=0, float y=0, bluegin::Graphic graphic=bluegin::Graphic());

    Sprite(float x=0, float y=0, bluegin::Graphic graphic=bluegin::Graphic());
    virtual ~Sprite() { }

    /**
    * If you changed the size of your sprite object to shrink the bounding box,
    * you might need to offset the new bounding box from the top-left corner of the sprite.
    */
    ci::Vec2f offset;

    /**
     * Change the size of your sprite's graphic.
     * NOTE: Scale doesn't currently affect collisions automatically,
     * you will need to adjust the width, height and offset manually.
     */
    ci::Vec2f scale;

    /**
     * Controls whether the object is smoothed when rotated, affects performance.
     * @default false
     */
    bool antialiasing;
    /**
     * Whether the current animation has finished its first (or only) loop.
     */
    bool finished;

    /**
     * This function creates a flat colored square image dynamically.
     * 
     * @param	Width		The width of the sprite you want to generate.
     * @param	Height		The height of the sprite you want to generate.
     * @param	Color		Specifies the color of the generated block.
     * @param	Unique		Whether the graphic should be a unique instance in the graphics cache.
     */
    void createGraphic(float w, float h, ci::ColorA color=ci::Color::white());

    /**
     * Load an image from a graphic resource
     * 
     * @param	Graphic		The image you want to use.
     * @param	Animated	Whether the Graphic parameter is a single sprite or a row of sprites.
     * @param	Reverse		Whether you need this class to generate horizontally flipped versions of the animation frames.
     * @param	Width		OPTIONAL - Specify the width of your sprite (helps Sprite figure out what to do with non-square sprites or sprite sheets).
     * @param	Height		OPTIONAL - Specify the height of your sprite (helps Sprite figure out what to do with non-square sprites or sprite sheets).
     * @param	Unique		Whether the graphic should be a unique instance in the graphics cache.
     * 
     * @return	This Sprite instance (nice for chaining stuff together, if you're into that).
     */
    void loadGraphic(bluegin::Graphic graphic, bool animated=false, 
            bool reverse=false, int width=0, int height=0, bool unique=false);

    void resetHelpers();
    virtual void calcOrigin();

    Direction_t getFacing();
    /**
     * Set facing direction using LEFT, RIGHT, UP and DOWN to take advantage 
     * of flipped sprites and/or just track player orientation more easily.
     */
    Direction_t setFacing(Direction_t dir);

    float getAlpha();
    /**
     * Set alpha to a number between 0 and 1 to change the opacity of the sprite.
     */
    void setAlpha(float alpha);

    ci::ColorA getColor();
    /**
     * Set sprite's vertex color.  Tints the whole sprite to be this color.
     */
    void setColor(ci::ColorA color);

    //  Set to true if you want colored vertices applied at render time
    bool colored;

    /**
     * Not implemented
     */
    void draw(Sprite& brush, ci::Vec2f pos);

    /**
     * Internal function for updating the sprite's animation.
     * Useful for cases when you need to update this but are buried down in too many supers.
     * This function is called automatically by <code>Sprite.update()</code>.
     */
    void updateAnimation();

    /**
     * Main game loop update function.  Override this to create your own sprite logic!
     * Just don't forget to call super.update() or any of the helper functions.
     */
    void update();

    /**
     * Internal function that performs the actual sprite rendering, called by render().
     */
    void renderSprite();

    /**
     * Called by game loop, updates then blits or renders current frame of animation to the screen
     */
    virtual void render();

    /**
     * Checks to see if a point in 2D space overlaps this Object.
     * 
     * @param	X			The X coordinate of the point.
     * @param	Y			The Y coordinate of the point.
     * @param	PerPixel	Whether or not to use per pixel collision checking.
     * 
     * @return	Whether or not the point overlaps this object.
     */
    virtual bool overlapsPoint(ci::Vec2f pt);

    /**
     * Triggered whenever this sprite is launched by a <code>Emitter</code>.
     */
    virtual void onEmit() { }

    /**
     * Adds a new animation to the sprite.
     * 
     * @param	Name		What this animation should be called (e.g. "run").
     * @param   FrameCount  The number of frames in this animation
     * @param	Frames		A pointer to an array of numbers indicating what frames to play in what order (e.g. 1, 2, 3).
     * @param	FrameRate	The speed in frames per second that the animation should play at (e.g. 40 fps).
     * @param	Looped		Whether or not the animation is looped or just plays once.
     */
    void addAnimation(const char* name, int frameCount, int frames[], float framerate=0, bool looped=true);
    // void addAnimationCallback();

    /**
     * Plays an existing animation (e.g. "run").
     * If you call an animation that is already playing it will be ignored.
     * 
     * @param	AnimName	The string name of the animation you want to play.
     * @param	Force		Whether to force the animation to restart.
     */
    void play(const char* name, bool force=false);

    void randomFrame();
    int getFrame();

    /**
     * Tell the sprite to change to a specific frame of animation.
     * 
     * @param	Frame	The frame you want to display.
     */
    void setFrame(int frame);

    /**
     * Call this function to figure out the on-screen position of the object.
     * 
     * @param	P	Takes a <code>Point</code> object and assigns the post-scrolled X and Y values of this object to it.
     * 
     * @return	The <code>Point</code> you passed in, or a new <code>Point</code> if you didn't pass one, containing the screen X and Y position of this object.
     */
    virtual ci::Vec2d& getScreenXY(ci::Vec2d& pt);

    /**
     * Internal function to update the current animation frame.
     */
    virtual void calcFrame();

    bluegin::Graphic graphic;
    //  maps to _flashRect in Flixel
    ci::Rectf frameRect;

protected:
    ci::ColorA mColor;

    Direction_t _facing;
    vector<Anim> _animations;
    int _flipped;

    Anim* _curAnim;
    int   _curFrame;
    int   _caf;
    float _frameTimer;

    ci::Matrix44f _mtx;
    // _callback = null;
    int frameWidth;
    int frameHeight;

public:
    virtual bool hasFrame() { return true; }
    virtual int getFrameWidth() { return frameWidth; }
    virtual int getFrameHeight() { return frameHeight; }
};

}

