#pragma once

namespace flx {

/**
 * This is a special effects utility class to help FlxGame do the 'quake' or screenshake effect.
 */
class Quake
{
protected:
    /**
     * The intensity of the quake effect: a percentage of the screen's size.
     */
    float _intensity;
    /**
     * Set to countdown the quake time.
     */
    float _timer;

public:
    /**
     * The amount of X distortion to apply to the screen.
     */
    int x;
    /**
     * The amount of Y distortion to apply to the screen.
     */
    int y;

    /**
     * Constructor.
     */
    Quake();

    /**
     * Reset and trigger this special effect.
     * 
     * @param	Intensity	Percentage of screen size representing the maximum distance that the screen can move during the 'quake'.
     * @param	Duration	The length in seconds that the "quake" should last.
     */
    void start(float Intensity=0.05,float Duration=0.5);

    /**
     * Stops this screen effect.
     */
    void stop();

    /**
     * Updates and/or animates this special effect.
     */
    void update();
};

} // namespace
