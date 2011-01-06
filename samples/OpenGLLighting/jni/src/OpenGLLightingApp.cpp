#include "bluegin/app.h"

#include "cinder/gl/gl.h"
#include "cinder/Camera.h"

using namespace cinder;
using namespace cinder::app;
using namespace bluegin;

GLfloat no_mat[]			= { 0.0, 0.0, 0.0, 1.0 };
GLfloat mat_ambient[]		= { 0.6, 0.3, 0.4, 1.0 };
GLfloat mat_diffuse[]		= { 0.3, 0.5, 0.8, 1.0 };
GLfloat mat_specular[]		= { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_emission[]		= { 0.0, 0.1, 0.3, 0.0 };

GLfloat mat_shininess[]		= { 128.0 };
GLfloat no_shininess[]		= { 0.0 };

class OpenGLLightingApp : public bluegin::BlueginApp
{
public:
    void setup();
    void update();
    void touchesBegan(TouchEvent event);
    void touchesMoved(TouchEvent event);
    void touchesEnded(TouchEvent event);
    void draw();
    
    float			mCounter;
    CameraPersp		mCam;
    Vec2f			mMousePos;
    bool mIsMouseDown;
    
    bool DIFFUSE;
    bool AMBIENT;
    bool SPECULAR;
    bool EMISSIVE;
    float mDirectional;
    
    Vec3f mLightPos;
};

void OpenGLLightingApp::setup()
{
	mIsMouseDown = false;
	DIFFUSE		= true;
	AMBIENT		= false;
	SPECULAR	= true;
	EMISSIVE	= true;

	mDirectional = 1.0f;
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	
	gl::enableDepthWrite();
	gl::enableDepthRead();
	gl::enableAlphaBlending();
	mCounter = 0.0f;
	glDisable( GL_TEXTURE_2D );

	mCam.lookAt( Vec3f( 0.0f, 0.0f, 750.0f ), Vec3f::zero() );
	mCam.setPerspective( 60, getWindowAspectRatio(), 1, 1000 );
	gl::setMatrices( mCam );
}

void OpenGLLightingApp::touchesMoved( TouchEvent event )
{
	for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); 
         touchIt != event.getTouches().end(); ++touchIt ) {
        mMousePos.x = touchIt->getX() - getWindowWidth() * 0.5f;
        mMousePos.y = getWindowHeight() * 0.5f - touchIt->getY();
    }
}

void OpenGLLightingApp::touchesBegan( TouchEvent event )
{
    if (!event.getTouches().empty())
        mIsMouseDown = true;
}

void OpenGLLightingApp::touchesEnded( TouchEvent event )
{
    if (!event.getTouches().empty())
        mIsMouseDown = false;
}

// void OpenGLLightingApp::keyDown( KeyEvent event )
// {
// 	if( event.getChar() == 'd' || event.getChar() == 'D' ){
// 		DIFFUSE = ! DIFFUSE;
// 	}
// 	else if( event.getChar() == 'a' || event.getChar() == 'A' ){
// 		AMBIENT = ! AMBIENT;
// 	}
// 	else if( event.getChar() == 's' || event.getChar() == 'S' ){
// 		SPECULAR = ! SPECULAR;
// 	}
// 	else if( event.getChar() == 'e' || event.getChar() == 'E' ){
// 		EMISSIVE = ! EMISSIVE;
// 	}
// 	else if( event.getChar() == 'f' || event.getChar() == 'F' ){
// 		setFullScreen( ! isFullScreen() );
// 	}
// 	else if( event.getChar() == '/' || event.getChar() == '?' ){
// 		mInfoPanel.toggleState();
// 	}
// 	else if( event.getChar() == ',' || event.getChar() == '<' ){
// 		mat_shininess[0] *= 0.5f;
// 		if( mat_shininess[0] < 8.0f )
// 			mat_shininess[0] = 8.0f;
// 	}
// 	else if( event.getChar() == '.' || event.getChar() == '>' ){
// 		mat_shininess[0] *= 2.0f;
// 		if( mat_shininess[0] > 128.0f )
// 			mat_shininess[0] = 128.0f;
// 	}
// }

void OpenGLLightingApp::update()
{
    if( mIsMouseDown ) // using small number instead of 0.0 because lights go black after a few seconds when going to 0.0f
        mDirectional -= ( mDirectional - 0.00001f ) * 0.1f;  
    else 
        mDirectional -= ( mDirectional - 1.0f ) * 0.1f;
}

void OpenGLLightingApp::draw()
{	
	float sphereSpacing		= 300.0f;
	float sphereRadius		= 140.0f;
	int sphereDetail		= 64;
	int spheresPerRow		= 4;
	int spheresPerColumn	= 3;
	
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	GLfloat light_position[] = { mMousePos.x, mMousePos.y, 75.0f, mDirectional };
	// GLfloat light_position[] = { mLightPos.x, mLightPos.y, mLightPos.z, mDirectional };
	glLightfv( GL_LIGHT0, GL_POSITION, light_position );

	for( int x=0; x<spheresPerRow; x++ ){
		float xPer = (float)x/(float)(spheresPerRow - 1 );
		
		for( int y=0; y<spheresPerColumn; y++ ){
			float yPer = (float)y/(float)(spheresPerColumn - 1 );
			
			float xp = sphereSpacing * ( x - ( spheresPerRow - 1 ) * 0.5f );
			float yp = sphereSpacing * ( y - ( spheresPerColumn - 1 ) * 0.5f );
			glPushMatrix();
				glTranslatef( xp, yp, 0.0);
				
				if( DIFFUSE ){
					ci::ColorA color( CM_HSV, xPer, yPer, 1.0f, 1.0f );
					glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE,	color );
				} else {
					glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE,	no_mat );
				}
					
				if( AMBIENT )
					glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT,	mat_ambient );
				else
					glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT,	no_mat );
					
				if( SPECULAR ){
					glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular );
					glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess );
				} else {
					glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, no_mat );
					glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, no_shininess );
				}
				
				if( EMISSIVE )
					glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, mat_emission );
				else
					glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, no_mat );			

				gl::drawSphere( Vec3f::zero(), sphereRadius, sphereDetail );
			glPopMatrix();
		}
	}
	
	mCounter++;
}

CINDER_APP_BASIC( OpenGLLightingApp, RendererGl )

