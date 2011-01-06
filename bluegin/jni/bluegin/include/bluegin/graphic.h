#pragma once

#include "cinder/gl/Texture.h"
#include "cinder/Rect.h"

namespace bluegin {

class Graphic
{
public:
    Graphic();
    Graphic(ci::gl::Texture texture, ci::Rectf sourceRect);
    ~Graphic();

    int getWidth();
    int getHeight();

    ci::gl::Texture& getTexture();
    ci::Rectf&       getSourceRect();
    ci::Rectf&       getTextureRect();

    ci::Rectf        mapFrameRect(ci::Rectf& frameRect, bool flip=false);

protected:
    //  source texture
    ci::gl::Texture mTexture;
    //  graphic location in source rectangle
    ci::Rectf mSourceRect;
    //  sourceRect as texture coordinates (texcoord mapping target)
    ci::Rectf mTexRect;
};

}
