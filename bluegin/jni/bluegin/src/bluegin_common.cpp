#include "bluegin/app.h"
#include "cinder/gl/Texture.h"

#ifndef ANDROID
#include "cinder/DataSource.h"
#include "cinder/ImageSourcePng.h"
#endif

using namespace cinder;
using namespace cinder::gl;

namespace bluegin
{

void setModelView( const Matrix44f &mtx )
{
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf( mtx.m );
}

//  High level functions
cinder::gl::Texture getTextureAsset(const char* name)
{
    int width, height;
    width = height = 0;

#ifdef ANDROID
    int texID = bluegin_load_texture(name, &width, &height);
    if (texID == 0 || width == 0 || height == 0) {
        return Texture();
    }
    return Texture(GL_TEXTURE_2D, texID, width, height, true);
#else
    //  load from assets folder
    DataSourcePathRef path  = DataSourcePath::createRef(string("assets\\")+string(name));
    ImageSourceRef    image = ImageSourcePng::createSourceRef(path);
    return Texture(image);
#endif
}

}
