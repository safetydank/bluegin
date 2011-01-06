#pragma once

#include "bluegin/bluegin.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Vbo.h"
#include "cinder/Color.h"
#include "cinder/Matrix.h"

//  Rendering implementations
namespace flx {

class Sprite;
class Tileblock;
class Text;

struct BatchState
{
    ci::gl::Texture texture;
    bool            preblend;
    BatchState();
    BatchState(ci::gl::Texture& t, bool pb);
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

    ci::gl::Texture  mActiveTexture;

    ci::gl::VboMesh  mVboMesh;

    vector<ci::Vec3f>  mPositions;
    vector<index_t>    mIndices;
    vector<ci::Vec2f>  mTexCoords;
    vector<ci::ColorA> mColors;

    int mIndexCount;

    bool mTextBatch;
};

typedef shared_ptr<Renderer> RendererPtr;

};
