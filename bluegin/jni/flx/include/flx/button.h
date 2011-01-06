#pragma once

#include "cinder/Vector.h"
#include "flx/group.h"
#include "flx/sprite.h"
#include "flx/text.h"
#include "flx/data/touch.h"

namespace flx {

typedef void (*ButtonCallback)(void*);

/**
 * A simple button class that calls a function when clicked by the mouse.
 * Supports labels, highlight states, and parallax scrolling.
 */
class Button : public Group, public TouchListener
{
protected:
    /**
     * Used for checkbox-style behavior.
     */
    bool _onToggle;
    /**
     * Stores the 'off' or normal button state graphic.
     */
    SpritePtr _off;
    /**
     * Stores the 'on' or highlighted button state graphic.
     */
    SpritePtr _on;
    /**
     * Stores the 'off' or normal button state label.
     */
    TextPtr _offT;
    /**
     * Stores the 'on' or highlighted button state label.
     */
    TextPtr _onT;
    /**
     * This function is called when the button is clicked.
     */
    ButtonCallback _callback;
    /**
     * Tracks whether or not the button is currently pressed.
     */
    bool _pressed;
    /**
     * Whether or not the button has initialized itself yet.
     */
    bool _initialized;
    /**
     * Helper variable for correcting its members' <code>scrollFactor</code> objects.
     */
    ci::Vec2f _sf;
    
public:
    /**
     * Creates a new <code>Button</code> object with a gray background
     * and a callback function
     * 
     * @param	X			The X position of the button.
     * @param	Y			The Y position of the button.
     * @param	Callback	The function to call whenever the button is clicked.
     */
    Button(int X,int Y,ButtonCallback Callback);
    virtual ~Button();
    
    /**
     * Set your own image as the button background.
     * 
     * @param	Image				A Sprite object to use for the button background.
     * @param	ImageHighlight		A Sprite object to use for the button background when highlighted (optional).
     */
    void loadGraphic(SpritePtr Image,SpritePtr ImageHighlight=SpritePtr());

    /**
     * Add a text label to the button.
     * 
     * @param	Text				A Text object to use to display text on this button (optional).
     * @param	TextHighlight		A Text object that is used when the button is highlighted (optional).
     */
    void loadText(TextPtr Text,TextPtr TextHighlight=TextPtr());
    
    /**
     * Called by the game loop automatically, handles mouseover and click detection.
     */
    virtual void update();
    
    bool getOn();
    
    /**
     * Use this to toggle checkbox-style behavior.
     */
    void setOn(bool On);
    
    /**
     * Internal function for handling the visibility of the off and on graphics.
     * 
     * @param	On		Whether the button should be on or off.
     */
    void visibility(bool On);
    
    //  Respond to touch up events
    virtual void handleTouchUp(float X, float Y, float XScroll, float YScroll);

};

typedef shared_ptr<Button> ButtonPtr;

}

