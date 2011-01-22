#pragma once

#include "bluegin/bluegin.h"
#include "cinder/gl/Texture.h"
#include "cinder/Color.h"
#include "cinder/Matrix.h"

//  Rendering implementations
namespace flx {

class Sprite;
class Tileblock;
class Text;

struct BatchState
{
    /** The texture to bind */
    ci::gl::Texture texture;
    /** Enables alpha blending */
    bool            alphaBlend;
    /** Enable vertex coloring */
    bool            colored;

    BatchState();
    BatchState(ci::gl::Texture& t, bool pb, bool colored=false);
};

/**
 * The default renderer interface used to draw Flx objects
 *
 *  This implementation uses software transforms but tries to batch writes
 *  to a vertex buffer
 */
class Renderer
{
public:
    Renderer();
    ~Renderer();

    void reset();
    void render();
    void drawBatch();
    void setBatchState(BatchState& state);
    void renderQuad(ci::ColorA color, 
                    float width, float height, 
                    ci::Rectf& texRect, 
                    ci::Matrix44f* xform, ci::Vec2f* offset);

protected:
    BatchState       mBatchState;

    vector<ci::Vec3f>  mPositions;
    vector<ci::Vec2f>  mTexCoords;
    vector<ci::ColorA> mColors;

    ci::ColorA* mFirstColor;
    ci::Vec2f* mFirstTexCoord;
    ci::Vec3f* mFirstPosition;

    int mIndexCount;
};

typedef shared_ptr<Renderer> RendererPtr;

};
