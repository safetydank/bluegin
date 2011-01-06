// #include "Resources.h"

#include "bluegin/app.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/Texture.h"
#include "cinder/Camera.h"

using namespace ci;
using namespace ci::app;
using namespace bluegin;

// dynamic VBO support missing
class VboSampleApp : public App 
{
 public:
	void setup();
	void update();
	void draw();
    void cleanup();

	gl::VboMesh		mVboMesh;
	CameraPersp		mCamera;
    gl::Texture     mTexture;
};

void VboSampleApp::setup()
{
    mCamera.setAspectRatio(getWindowAspectRatio());

    mTexture = bluegin::getTextureAsset("cinder_logo.png");
    mTexture.setMinFilter(GL_LINEAR);
    mTexture.setMagFilter(GL_LINEAR);

	gl::VboMesh::Layout layout;
	layout.setStaticIndices();
	layout.setStaticPositions();
    layout.setStaticTexCoords2d();

    const int vertexCount = 4;
    const int indexCount  = 6;

	mVboMesh = gl::VboMesh( vertexCount, indexCount, layout, GL_TRIANGLES );

    vector<Vec3f> positions;
    positions.push_back(Vec3f(-1.0f, 1.0f, 0));
    positions.push_back(Vec3f(-1.0f, -1.0f, 0));
    positions.push_back(Vec3f(1.0f, -1.0f, 0));
    positions.push_back(Vec3f(1.0f, 1.0f, 0));
    mVboMesh.bufferPositions(positions);

    vector<Vec2f> texcoords;
    texcoords.push_back(Vec2f(0, 0));
    texcoords.push_back(Vec2f(0, 1.0f));
    texcoords.push_back(Vec2f(1.0f, 1.0f));
    texcoords.push_back(Vec2f(1.0f, 0));
    mVboMesh.bufferTexCoords2d(0, texcoords);

    vector<index_t> indices;
    indices.push_back(index_t(0));
    indices.push_back(index_t(1));
    indices.push_back(index_t(2));
    indices.push_back(index_t(2));
    indices.push_back(index_t(3));
    indices.push_back(index_t(0));
    mVboMesh.bufferIndices(indices);
}

void VboSampleApp::update()
{
    // mCamera.lookAt(Vec3f(0, 0, 5.0f), Vec3f(0, 0, 0));
	gl::setMatrices( mCamera );

    vector<Vec3f> positions;
    positions.push_back(Vec3f(-1.0f+0.2f*sin(getElapsedSeconds()), 1.0f, 0));
    positions.push_back(Vec3f(-1.0f, -1.0f, 0));
    positions.push_back(Vec3f(1.0f, -1.0f, 0));
    positions.push_back(Vec3f(1.0f, 1.0f, 0));
    mVboMesh.bufferPositions(positions);
}

void VboSampleApp::draw()
{
    // this pair of lines is the standard way to clear the screen in OpenGL
    gl::clear( Color( 0.15f, 0.15f, 0.55f ) );
    gl::scale(Vec3f(10.0f+8.0f*sin(getElapsedSeconds()*2.0f), 10.0f+8.0f*sin(getElapsedSeconds()*2.0f), 10.0f+8.0f*sin(getElapsedSeconds()*2.0f)));
    mTexture.enableAndBind();
    gl::draw( mVboMesh );
    // Log("drawn texture %d", mTexture.getTextureId());
}

void VboSampleApp::cleanup()
{
    //  XXX If GL resources are not cleaned up they are unavailable the next time
    //  the program is run.
    mTexture.reset();
    mVboMesh.reset();
}

CINDER_APP_BASIC( VboSampleApp, RendererGl )

