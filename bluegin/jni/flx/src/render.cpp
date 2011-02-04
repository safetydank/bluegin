#include "flx/flxG.h"
#include "flx/render.h"

#include "cinder/Rect.h"

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

using namespace flx;
using namespace ci;
using namespace ci::gl;
using namespace bluegin;

//  Max primitives (quads) to render in a batch.
const int VBO_QUADS = 128;

// XXX DEBUG
int SPRITES = 0;
int TILEBLOCKS = 0;
int TEXTS = 0;
int CULLED = 0;

extern FlxGlobal FlxG;

const int VertexCount = VBO_QUADS * 6;
const int IndexCount  = VBO_QUADS * 6;
const int TexCount    = VBO_QUADS * 6;

Renderer::Renderer()
{
    Texture noTex;
    mBatchState = BatchState(noTex, false, true);

    mPositions.resize(IndexCount);
    mTexCoords.resize(TexCount);
    mColors.resize(IndexCount);

    mFirstPosition = &(mPositions[0]);
    mFirstTexCoord = &(mTexCoords[0]);
    mFirstColor    = &(mColors[0]);

    mIndexCount = 0;
}

Renderer::~Renderer()
{
}

void Renderer::reset()
{
    mIndexCount = 0;
}

void Renderer::render()
{
    drawBatch();
}

//  Transform a Vec2f by a 4x4 matrix
void transform(Vec3f& pos, Matrix44f& matrix) {
    Vec4f newPos(pos.x, pos.y, 0, 1.0f);
    newPos = matrix * newPos;
    pos.x = newPos.x; pos.y = newPos.y; pos.z = newPos.z;
}

void Renderer::renderQuad(ColorA color, float width, float height, Rectf& texRect, 
                          Matrix44f* xform, Vec2f* offset)
{
    if (mIndexCount + 6 > IndexCount) {
        drawBatch();
    }

    int startVertex, iVertex, iTexCoord;
    iTexCoord = iVertex = startVertex = mIndexCount;
    int iIndex  = mIndexCount;

    Vec2f off = (offset == NULL) ? Vec2f(0,0) : *offset;
    Vec3f* itVertex = mFirstPosition + startVertex;

    itVertex->x = 0 + off.x;      itVertex->y = 0 + off.y;       ++itVertex;
    itVertex->x = width + off.x;  itVertex->y = 0 + off.y;       ++itVertex;
    itVertex->x = 0 + off.x;      itVertex->y = height + off.y;  ++itVertex;
    itVertex->x = width + off.x;  itVertex->y = 0 + off.y;       ++itVertex;
    itVertex->x = width + off.x;  itVertex->y = height + off.y;  ++itVertex;
    itVertex->x = 0 + off.x;      itVertex->y = height + off.y;  ++itVertex;

    //  transform quad using a supplied matrix
    if (xform != NULL) {
        // for (vector<Vec3f>::iterator it=mPositions.begin() + startVertex; 
        Vec3f* startPos = mFirstPosition + startVertex;
        Vec3f* stopPos = startPos + 6;
        for (Vec3f* it=startPos; it != stopPos; ++it) {
            transform(*it, *xform);
        }
    }

    Vec2f* itTexCoord = mFirstTexCoord + startVertex;

    itTexCoord->x = texRect.x1;   itTexCoord->y = texRect.y1;  ++itTexCoord;
    itTexCoord->x = texRect.x2;   itTexCoord->y = texRect.y1;  ++itTexCoord;
    itTexCoord->x = texRect.x1;   itTexCoord->y = texRect.y2;  ++itTexCoord;
    itTexCoord->x = texRect.x2;   itTexCoord->y = texRect.y1;  ++itTexCoord;
    itTexCoord->x = texRect.x2;   itTexCoord->y = texRect.y2;  ++itTexCoord;
    itTexCoord->x = texRect.x1;   itTexCoord->y = texRect.y2;  ++itTexCoord;

    if (mBatchState.colored) {
        ColorA* itColor = mFirstColor+startVertex;
        for (int i=0; i<6; ++i, ++itColor) {
            *itColor = color;
        }
    }

    mIndexCount += 6;
}

void Renderer::drawBatch()
{
    gl::enableAlphaBlending(mBatchState.alphaBlend);

    //  skip empty batches
    if (mIndexCount == 0)
        return;

    if (mBatchState.texture) {
        mBatchState.texture.enableAndBind();
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, &mTexCoords[0]);
    }
    else {
        glDisable(GL_TEXTURE_2D);
    }

    //  Using vertex arrays
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &mPositions[0]);

    if (mBatchState.colored) {
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_FLOAT, 0, &mColors[0]);
    }

    glDrawArrays(GL_TRIANGLES, 0, mIndexCount);

    glDisableClientState(GL_VERTEX_ARRAY);

    if (mBatchState.colored)
        glDisableClientState(GL_COLOR_ARRAY);

    if (mBatchState.texture) {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    // Log("Drew %d indexes, %d sprites, %d tiles, %d texts, culled %d quads", mIndexCount, SPRITES, TILEBLOCKS, TEXTS, CULLED);
    TILEBLOCKS = SPRITES = TEXTS = CULLED = 0;    
    reset();
}

BatchState::BatchState(Texture& t, bool ab, bool colored) 
    : texture(t), alphaBlend(ab), colored(colored)
{
}

BatchState::BatchState() : texture(gl::Texture()), alphaBlend(false)
{
}

void Renderer::setBatchState(BatchState& state)
{
    bool textureChanged  = (bool(state.texture) && bool(mBatchState.texture) 
            && mBatchState.texture.getId() != state.texture.getId());
    textureChanged |= (bool(state.texture) != bool(mBatchState.texture));
    bool alphaBlendChanged = mBatchState.alphaBlend != state.alphaBlend;
    bool coloredChanged = mBatchState.colored != state.colored;

    if (textureChanged || alphaBlendChanged || coloredChanged) {
        //  finish batch, prepare to start new
        if (mIndexCount) drawBatch();
        mBatchState = state;
    }
}

