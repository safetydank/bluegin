#include "bluegin/bluegin.h"
#include "bluegin/graphic.h"

using namespace ci;
using namespace ci::gl;
using namespace bluegin;

Graphic::Graphic() : mSourceRect(0,0,0,0), mTexRect(0,0,0,0)
{
}

Graphic::Graphic(string texName, Rectf sourceRect) :
    texName(texName), mSourceRect(sourceRect), mTexRect(0,0,0,0)
{
}

void Graphic::setTexture(ci::gl::Texture& tex)
{
    Rectf& src = mSourceRect;

    if (!tex) {
        Log("Error: call to Graphic::setTexture with empty texture");
    }
    else {
        mTexture = tex;
    }

    if (src.x1 == 0 && src.x2 == 0 && src.y1 == 0 && src.y2 == 0) {
        //  No source rect specified and first time texture is read,
        //  so use entire texture
        src.set(0, 0, tex.getWidth(), tex.getHeight());
        mTexRect.set(0, 0, 1.0f, 1.0f);
    } 
    else {
        RectMapping sourceMap(Rectf(0, 0, tex.getWidth(), tex.getHeight()), 
                              Rectf(0, 0, 1.0f, 1.0f));
        mTexRect = sourceMap.map(src);
    }
    Log("Graphic::setTexture texRect (%f, %f), (%f, %f)", mTexRect.x1, mTexRect.y1,
            mTexRect.x2, mTexRect.y2);
}

// Graphic::Graphic(Texture texture, Rectf sourceRect) :
//     mSourceRect(0,0,0,0), mTexRect(0,0,0,0), mTexture(texture)
// {
//     if (!texture) {
//         Log("Error: empty texture passed to Graphic, not initialized");
//         return;
//     }
// 
//     Rectf& src = sourceRect;
//     if (src.x1 == 0 && src.x2 == 0 && src.y1 == 0 && src.y2 == 0) {
//         //  Use entire texture
//         mSourceRect.set(0, 0, texture.getWidth(), texture.getHeight());
//         mTexRect.set(0, 0, 1.0f, 1.0f);
//     } 
//     else {
//         mSourceRect = sourceRect;
//         RectMapping sourceMap(Rectf(0, 0, texture.getWidth(), texture.getHeight()), 
//                               Rectf(0, 0, 1.0f, 1.0f));
//         mTexRect = sourceMap.map(sourceRect);
//     }
// }

Graphic::~Graphic()
{
}

int Graphic::getWidth()
{
    return mSourceRect.getWidth();
}

int Graphic::getHeight()
{
    return mSourceRect.getHeight();
}

Texture& Graphic::getTexture()
{
    return mTexture;
}

Rectf& Graphic::getSourceRect()
{
    return mSourceRect;
}

Rectf& Graphic::getTextureRect()
{
    return mTexRect;
}

Rectf Graphic::mapFrameRect(Rectf& frameRect, bool flip)
{
    if (!mTexture) {
        return mTexRect;
    }

    RectMapping frameMap(Rectf(0, 0, getWidth(), getHeight()), mTexRect);
    Rectf texRect = frameMap.map(frameRect);
    if (flip) {
        float s = texRect.x1;
        texRect.x1 = texRect.x2;
        texRect.x2 = s;
    }
    return texRect;
}

