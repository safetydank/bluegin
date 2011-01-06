#pragma once

#include "cinder/Color.h"

namespace flx {

typedef void (*FadeCallback)(void*);

class Fade
{
private:
    /**
     * How long the effect should last.
     */
    float _delay;
    ci::Color _color;

    /**
     * Callback for when the effect is finished.
     */
    // protected var _complete:Function;
    FadeCallback _complete;
    void* _completeData;

public:
    float alpha;
    bool  exists;

    ci::ColorA fadeColor;

    /**
     * Constructor initializes the fade object
     */
    Fade();

    /**
     * Reset and trigger this special effect
     * 
     * @param	Color			The color you want to use
     * @param	Duration		How long it should take to fade the screen out
     * @param	FadeComplete	A function you want to run when the fade finishes
     * @param	Force			Force the effect to reset
     */
    void start(ci::Color color, float Duration=1, FadeCallback FadeComplete=NULL, bool Force=false);

    /**
     * Stops and hides this screen effect.
     */
    void stop();

    /**
     * Updates and/or animates this special effect
     */
    void update(float elapsed);

    void setCallbackData(void* data);
};

}

