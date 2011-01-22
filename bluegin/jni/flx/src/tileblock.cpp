#include "flx/flx.h"
#include "flx/flxG.h"
#include "flx/tileblock.h"

using namespace flx;
using namespace cinder;
using namespace bluegin;

extern FlxGlobal FlxG;

Tileblock::Tileblock(int X, int Y, int Width, int Height) 
    : Object(X, Y, Width, Height)
{
    fixed = true;
    refreshHulls();
}

void Tileblock::loadGraphic(Graphic TileGraphic, int Empties)
{
    if (!TileGraphic.getTexture())
        return;
    graphic = TileGraphic;

    _rects.clear();

    _tileSize = graphic.getHeight();

    int widthInTiles  = ceil(width / _tileSize);
    int heightInTiles = ceil(height / _tileSize);
    width  = widthInTiles  * _tileSize;
    height = heightInTiles * _tileSize;
    int numTiles    = widthInTiles * heightInTiles;
    int numGraphics = graphic.getWidth() / _tileSize;
    for (int i = 0; i < numTiles; i++)
    {
        if (FlxU::random() * (numGraphics+Empties) > Empties) {
            float xpos = _tileSize*FlxU::floor(FlxU::random()*numGraphics);
            _rects.push_back(Rectf(xpos, 0, xpos + _tileSize, _tileSize));
        }
        else {
            //  empty block
            _rects.push_back(Rectf(0,0,0,0));
        }
    }
}

void Tileblock::render()
{
    renderBlock();
}

void Tileblock::renderBlock()
{
    if (!onScreen())
        return;

    // getScreenXY(_point);
    float opx = _point.x;
    Vec2f pos(_point.x, _point.y);

    for(vector<Rectf>::iterator it = _rects.begin(); it != _rects.end(); ++it)
    {
        frameRect = *it;
        if (frameRect.x1 != frameRect.x2 && frameRect.y1 != frameRect.y2)  {
            //  Render block at pos
            BatchState batch(graphic.getTexture(), false, false);
            FlxG.renderer->setBatchState(batch);
            Rectf texRect = graphic.mapFrameRect(frameRect);
            FlxG.renderer->renderQuad(Color::white(), 
                    frameRect.getWidth(), frameRect.getHeight(), 
                    texRect, NULL, &pos);
        }

        pos.x += _tileSize;
        if (pos.x + FlxU::roundingError >= opx + width)
        {
            pos.x = opx;
            pos.y += _tileSize;
        }
    }
}

