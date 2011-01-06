#pragma once

namespace flx {

/*
 * An interface to implement for classes requiring touch event notifications
 */
class TouchListener
{
public:
    virtual void handleTouchDown(float X, float Y, float XScroll, float YScroll)  { }
    virtual void handleTouchMoved(float X, float Y, float XScroll, float YScroll) { }
    virtual void handleTouchUp(float X, float Y, float XScroll, float YScroll)    { }
};

/**
 * This class helps contain and track the mouse pointer in your game.
 * Automatically accounts for parallax scrolling, etc.
 */
class Touch
{
public:
    /**
     * Current X position of the touch in the game world.
     */
    float x;
    /**
     * Current Y position of the touch in the game world.
     */
    float y;
    /**
     * Current X position of the touch on the screen.
     */
    float screenX;
    /**
     * Current Y position of the touch on the screen.
     */
    float screenY;

protected:
    int _current;
    int _last;
    vector<TouchListener*> _listeners;

public:
    Touch();
    void update();

    /**
     * Resets the just pressed/just released flags and sets mouse to not pressed.
     */
    void reset();

    /**
     * Check to see if the touch is pressed.
     * 
     * @return	Whether the touch is pressed.
     */
    bool pressed();

    /**
     * Check to see if the touch was just pressed.
     * 
     * @return Whether the touch was just pressed.
     */
    bool justPressed();

    /**
     * Check to see if the touch was just released.
     * 
     * @return Whether the touch was just released.
     */
    bool justReleased();

    void handleTouchDown(float X, float Y, float XScroll, float YScroll);
    void handleTouchMoved(float X, float Y, float XScroll, float YScroll);
    void handleTouchUp(float X, float Y, float XScroll, float YScroll);

    void addTouchListener(TouchListener* listener);
    void removeTouchListener(TouchListener* listener);
};

}
