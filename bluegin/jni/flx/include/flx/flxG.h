// FlxG dependencies, should be moved to FlxG.h
#include "cinder/Camera.h"
#include "bluegin/resourcemanager.h"
#include "bluegin/sound.h"
#include "bluegin/hgefont.h"
#include "flx/game.h"
#include "flx/state.h"
#include "flx/render.h"
#include "flx/data/fade.h"
#include "flx/data/quake.h"
#include "flx/data/flash.h"
#include "flx/data/accelerometer.h"
#include "flx/data/touch.h"

#include "Box2D/Common/b2BlockAllocator.h"

namespace flx {

class Object;
class IGame;

struct FlxGlobal
{
    RendererPtr renderer;
    bluegin::ResourceManagerPtr resources;
    b2BlockAllocator* blockAllocator;

    double elapsed;

    //  Game viewport width, height
    float width;
    float height;

    //  Window width, height (in pixels)
    ci::Vec2f windowSize;

    /**
     * Tells the camera to follow this <code>Object</code> around.
     */
    ObjectPtr followTarget;
    ci::Vec2f* followLead;
    /**
     * Used to force the camera to look ahead of the <code>followTarget</code>.
     */
    ci::Vec2f _followLead;
    /**
     * Used to smoothly track the camera as it follows.
     */
    double followLerp;
    /**
     * Stores the top and left edges of the camera area.
     */
    ci::Vec2f* followMin;
    ci::Vec2f _followMin;
    /**
     * Stores the bottom and right edges of the camera area.
     */
    ci::Vec2f* followMax;
    ci::Vec2f _followMax;
    /**
     * Internal, used to assist camera and scrolling.
     */
    ci::Vec2d _scrollTarget;
    
    /**
     * Stores the basic parallax scrolling values.
     */
    ci::Vec2d scroll;

    /**
     * Camera used for OpenGL rendering
     */
    ci::CameraOrtho camera;

    /**
     * Accelerometer input
     */
    Accelerometer accel;

    /*
     * (Single) touch input
     */
    Touch touch;

    /*
     * Display zoom level
     */
    ci::Vec2f zoom;

    /*
     * Pointer to the game object
     */
    IGame* game;

    /**
     * How fast or slow time should pass in the game; default is 1.0.
     */
    float timeScale;

    /**
     * FlxG.level and FlxG.score are generic
     * global variables that can be used for various cross-state stuff.
     */
    int score;
    int level;

    /**
     * A special effect that fades a color onto the screen.  Usage: FlxG.fade.start();
     */
    Fade  fade;

    /**
     * A special effect that shakes the screen.  Usage: FlxG.quake.start();
     */
    Quake quake;

    /**
     * A special effect that flashes a color on the screen.  Usage: FlxG.flash.start();
     */
    Flash flash;

    /*
     * Sets the default font used by Text instances
     */
    bluegin::hgeFont* defaultFont;

    /*
     * Pointer to the currently active state
     */
    State* getState();

    void   setState(StatePtr state);

    /**
     * Called by <code>Game</code> to set up <code>FlxG</code> during <code>Game</code>'s constructor.
     */
    void setGameData(IGame* game, float gameSizeX, float gameSizeY, ci::Vec2f zoom);

    /**
     * Tells the camera subsystem what <code>Object</code> to follow.
     * 
     * @param	Target		The object to follow.
     * @param	Lerp		How much lag the camera should have (can help smooth out the camera movement).
     */
    void follow(ObjectPtr Target, double Lerp=1.0);

    /**
     * Specify an additional camera component - the velocity-based "lead",
     * or amount the camera should track in front of a sprite.
     * 
     * @param	LeadX		Percentage of X velocity to add to the camera's motion.
     * @param	LeadY		Percentage of Y velocity to add to the camera's motion.
     */
    void followAdjust(float LeadX=0, float LeadY=0);

    /**
     * Specify the boundaries of the level or where the camera is allowed to move.
     * 
     * @param	MinX				The smallest X value of your level (usually 0).
     * @param	MinY				The smallest Y value of your level (usually 0).
     * @param	MaxX				The largest X value of your level (usually the level width).
     * @param	MaxY				The largest Y value of your level (usually the level height).
     * @param	UpdateWorldBounds	Whether the quad tree's dimensions should be updated to match.
     */
    void followBounds(int MinX=0, int MinY=0, int MaxX=0, int MaxY=0, bool UpdateWorldBounds=true);

    /**
     * Internal function that updates the camera and parallax scrolling.
     */
    void doFollow();

    /**
     * Stops and resets the camera.
     */
    void unfollow();

    //  Sound support
    bluegin::AudioPtr music;

    /** 
     * Play a sound effect from an audio resource
     *
     * @param source Sound effect audio resource
     * @param volume Volume level in the range [0, 1.0]
     * @param looped Whether to loop the effect
     *
     * @return an Audio object representing the played effect
     */
    bluegin::AudioPtr play(bluegin::AudioSourcePtr source, float volume=1.0f, bool looped=false);

    /**
     * Play a music track from an audio resource
     *
     * @param source Music audio resource
     * @param volume Volume level in the range [0, 1.0]
     * @param looped Whether to loop the music
     *
     * @return an Audio object representing the played music track
     */
    bluegin::AudioPtr playMusic(bluegin::AudioSourcePtr source, float volume=1.0f);

    /**
     * Stop any playing music
     */
    void stopMusic();

    /**
     * Not implemented
     */
    bool getMute();

    /**
     * Not implemented
     */
    void setMute(bool Mute);

    /**
     * Get the volume level
     */
    float getVolume();

    /**
     * Set volume to a number between 0 and 1 to change the global volume.
     */
    void  setVolume(float Volume);

    /**
     * Get a number that represents the mute state that we can multiply into a sound transform.
     * 
     * @return		An unsigned integer - 0 if muted, 1 if not muted.
     */
    float getMuteValue();

    /**
     * An internal function that adjust the volume levels and the music channel after a change.
     */
    void changeSounds();

    /**
     * Called by the game loop to make sure the sounds get updated each frame.
     */
    void updateSounds();

    //  Device identification (hooks, for compatibility with flixel-iOS)
    bool iPad;
    bool iPhone3G;
    bool iPhone1G;
    bool iPodTouch1G;

protected:
    bool  _mute;
    float _volume;
};

}
