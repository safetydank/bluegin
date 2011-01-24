#pragma once

#include "flx.h"
#include "flx/group.h"
#include "flx/object.h"
#include "cinder/Color.h"

namespace flx {

class State;
typedef shared_ptr<State> StatePtr;

/**
 * This is the basic game "state" object - e.g. in a simple game
 * you might have menu state and a play state.
 * It acts as a kind of container for all your game objects.
 */
class State
{
public:
    State();
    virtual ~State();

    /**
     * Adds a new Object subclass (Sprite, Tileblock, etc) to the game loop.
     * State is adding this object to its built-in Group to automate updating and rendering.
     * 
     * @param	core	The object you want to add to the game loop.
     */
    void add(ObjectPtr core);

    /**
     * Override this function to set up your game state.
     * This is where you create your groups and game objects and all that good stuff.
     */
    virtual void create();

    /**
     * Automatically goes through and calls update on everything you added to the game loop,
     * override this function to handle custom input and perform collisions/
     */
    virtual void update();

    /**
     * This function collides <code>defaultGroup</code> against <code>defaultGroup</code>
     * (basically everything you added to this state).
     */
    virtual void collide();

    /**
     * Override this function to do special pre-processing FX like motion blur.
     * You can use scaling or blending modes or whatever you want against
     * <code>State.screen</code> to achieve all sorts of cool FX.
     */
    virtual void preProcess();

    /**
     * Automatically goes through and calls render on everything you added to the game loop,
     * override this loop to manually control the rendering process.
     */
    virtual void render();

    /**
     * Override this function to do special pre-processing FX like light bloom.
     * You can use scaling or blending modes or whatever you want against
     * <code>State.screen</code> to achieve all sorts of cool FX.
     */
    virtual void postProcess();

    /**
     * Override this function to handle any deleting or "shutdown" type operations you
     * might need (such as removing traditional Flash children like Sprite objects).
     */
    virtual void destroy();

    /**
     * This static variable indicates the "clear color"
     * or default background color of the game.
     * Change it at ANY time using <code>State::bgColor</code>.
     */
    static ci::Color bgColor;

    /**
     * Internal group used to organize and display objects you add to this state.
     */
    Group defaultGroup;
};

}

