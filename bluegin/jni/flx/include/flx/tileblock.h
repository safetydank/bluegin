#pragma once

#include "bluegin/graphic.h"
#include "flx/object.h"
#include "cinder/Rect.h"

namespace flx {

/**
 * This is the basic "environment object" class, used to create simple walls and floors.
 * It can be filled with a random selection of tiles to quickly add detail.
 */
class Tileblock : public Object
{
protected:
    /**
     * Array of rectangles used to quickly blit the tiles to the screen.
     */
    vector<ci::Rectf> _rects;
    /**
     * The size of the tiles (e.g. 8 means 8x8).
     */
    float _tileSize;
    /**
     * Rendering helper.
     */

public:
    bluegin::Graphic graphic;
    ci::Rectf frameRect;

    /**
     * Creates a new <code>Tileblock</code> object with the specified position and size.
     * 
     * @param	X			The X position of the block.
     * @param	Y			The Y position of the block.
     * @param	Width		The width of the block.
     * @param	Height		The height of the block.
     */
    Tileblock(int X, int Y, int Width, int Height);

    /**
     * Fills the block with a randomly arranged selection of graphics from the image provided.
     * 
     * @param	TileGraphic The graphic class that contains the tiles that should fill this block.
     * @param	Empties		The number of "empty" tiles to add to the auto-fill algorithm (e.g. 8 tiles + 4 empties = 1/3 of block will be open holes).
     */
    void loadGraphic(bluegin::Graphic TileGraphic, int Empties=0);

    /**
     * Draws this block.
     */
    virtual void render();

    /**
     * Internal function to draw this block
     */
    void renderBlock();
};

typedef shared_ptr<Tileblock> TileblockPtr;

}


