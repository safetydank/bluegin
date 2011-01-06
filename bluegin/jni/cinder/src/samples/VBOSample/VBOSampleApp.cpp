#include "bluegin/app.h"
#include "bluegin/common.h"

#include "cinder/gl/gl.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/Texture.h"
#include "cinder/Camera.h"

using namespace ci;
using namespace ci::app;
using namespace bluegin;

using std::vector;

/*** This sample demonstrates the Vbo class by creating a simple grid mesh with a texture mapped onto it.
  */

class VboSampleApp : public App {
 public:
	void setup();
	void update();
	void draw();

	static const int VERTICES_X = 20, VERTICES_Z = 20;

	gl::VboMesh		mVboMesh;
	CameraPersp		mCamera;
	gl::Texture		mTexture;

    // dynamic vertices
    vector<Vec3f>   mPositions;
};

void VboSampleApp::setup()
{
    mCamera.setAspectRatio(getWindowAspectRatio());

    mTexture = getTextureAsset("cinder_logo.png");
    mTexture.setMinFilter(GL_LINEAR);
    mTexture.setMagFilter(GL_LINEAR);

	// setup the parameters of the Vbo
	int totalVertices = VERTICES_X * VERTICES_Z;
	int totalQuads    = ( VERTICES_X - 1 ) * ( VERTICES_Z - 1 );

	gl::VboMesh::Layout layout;
	layout.setStaticIndices();
	layout.setStaticPositions();
	layout.setStaticTexCoords2d();
	mVboMesh = gl::VboMesh( totalVertices, totalQuads * 6, layout, GL_TRIANGLES );
	
	// buffer our static data - the texcoords and the indices
	vector<index_t> indices;
	vector<Vec2f> texCoords;
	for( int x = 0; x < VERTICES_X; ++x ) {
		for( int z = 0; z < VERTICES_Z; ++z ) {
			// create quads for each vertex, except for along the bottom and right edges
			if( ( x + 1 < VERTICES_X ) && ( z + 1 < VERTICES_Z ) ) {
				indices.push_back( (x+0) * VERTICES_Z + (z+0) );
				indices.push_back( (x+1) * VERTICES_Z + (z+0) );
				indices.push_back( (x+0) * VERTICES_Z + (z+1) );
                
				indices.push_back( (x+1) * VERTICES_Z + (z+0) );
				indices.push_back( (x+1) * VERTICES_Z + (z+1) );
				indices.push_back( (x+0) * VERTICES_Z + (z+1) );
			}
			// the texture coordinates are mapped to [0,1.0]
			texCoords.push_back( Vec2f( x / ((float)VERTICES_X-1), z / ((float)VERTICES_Z-1) ) );
            Vec3f pos( x / (float)VERTICES_X, 0, z / (float)VERTICES_Z );
            mPositions.push_back(pos);
		}
	}
	
	mVboMesh.bufferIndices( indices );
    mVboMesh.bufferPositions( mPositions );
	mVboMesh.bufferTexCoords2d( 0, texCoords );
}

void VboSampleApp::update()
{
	gl::setMatrices( mCamera );

	const float timeFreq = 5.0f;
	const float zFreq = 3.0f;
	const float xFreq = 7.0f;
	float offset = getElapsedSeconds() * timeFreq;

    vector<Vec3f>::iterator it = mPositions.begin();
	for( int x = 0; x < VERTICES_X; ++x ) {
		for( int z = 0; z < VERTICES_Z; ++z ) {
			float height = sin( z / (float)VERTICES_Z * zFreq + x / (float)VERTICES_X * xFreq + offset ) / 5.0f;
            it->y = height;
            ++it;
		}
	}
    mVboMesh.bufferPositions(mPositions);
}

void VboSampleApp::draw()
{
    // this pair of lines is the standard way to clear the screen in OpenGL
    gl::clear( Color( 0.15f, 0.15f, 0.15f ) );
    gl::scale( Vec3f( 10.0f, 10.0f, 10.0f ) );
    mTexture.enableAndBind();
    gl::draw( mVboMesh );
}

CINDER_APP_BASIC( VboSampleApp, RendererGl )
