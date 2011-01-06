#pragma once

#include "cinder/Color.h"

namespace flx {

/**
 * This is a special effects utility class to help FlxGame do the 'flash' effect
 */
class Flash
{
protected:
	/**
	 * How long the effect should last.
	 */
	float _delay;
	/**
	 * Callback for when the effect is finished.
	 */
	float _complete;

public:
    bool exists;
    float alpha;
    ci::Color color;
	
	/**
	 * Constructor for this special effect
	 */
	Flash();
	
	/**
	 * Reset and trigger this special effect
	 * 
	 * @param	Color			The color you want to use
	 * @param	Duration		How long it takes for the flash to fade
	 * @param	FlashComplete	A function you want to run when the flash finishes
	 * @param	Force			Force the effect to reset
	 */
    void start(ci::Color color, float Duration=1.0f, void* FlashComplete=NULL, 
        bool Force=false);
	
	/**
	 * Stops and hides this screen effect.
	 */
	void stop();
	
	/**
	 * Updates and/or animates this special effect
	 */
	void update(float elapsed);
};

}

