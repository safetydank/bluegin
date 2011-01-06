#pragma once

#include <string>
#include <vector>

namespace flx {

/**
 * Just a helper structure for the FlxSprite animation system
 */
class Anim 
{
public:
    /**
     * String name of the animation (e.g. "walk")
     */
    std::string name;
    /**
     * Seconds between frames (basically the framerate)
     */
    float delay;
    /**
     * A list of frames stored as <code>int</code> objects
     */
    std::vector<int> frames;
    /**
     * Whether or not the animation is looped
     */
    bool looped;

    /**
     * Constructor
     * 
     * @param	Name		What this animation should be called (e.g. "run")
     * @param	Frames		An array of numbers indicating what frames to play in what order (e.g. 1, 2, 3)
     * @param	FrameRate	The speed in frames per second that the animation should play at (e.g. 40)
     * @param	Looped		Whether or not the animation is looped or just plays once
     */
    Anim(std::string name, std::vector<int>& frames, float frameRate, bool looped=true);
};

}
