#pragma once

#include "cinder/gl/Texture.h"
#include "cinder/Matrix.h"

namespace bluegin {

//  High level asset interface 
ci::gl::Texture getTextureAsset(const char* name);
void setModelView( const ci::Matrix44f &mtx );

}

