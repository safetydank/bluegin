#pragma once

#include "cinder/gl/Texture.h"
#include "cinder/Rect.h"

namespace bluegin {

class Graphic
{
public:
    Graphic();
    Graphic(string texName, ci::Rectf sourceRect);
    // Graphic(ci::gl::Texture texture, ci::Rectf sourceRect);
    ~Graphic();

    int getWidth();
    int getHeight();

    void             setTexture(ci::gl::Texture& tex);
    ci::gl::Texture& getTexture();
    ci::Rectf&       getSourceRect();
    ci::Rectf&       getTextureRect();

    ci::Rectf        mapFrameRect(ci::Rectf& frameRect, bool flip=false);

    //  source texture identifier
    string texName;

protected:
    //  source texture
    ci::gl::Texture mTexture;
    //  graphic location in source rectangle
    ci::Rectf mSourceRect;
    //  sourceRect as texture coordinates (texcoord mapping target)
    ci::Rectf mTexRect;
};

}
