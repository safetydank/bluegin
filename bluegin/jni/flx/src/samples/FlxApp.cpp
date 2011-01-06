#include "bluegin/app.h"

#include "flx/game.h"
#include "flx/state.h"
#include "flx/samples/Play.h"

#include "cinder/gl/gl.h"
#include "cinder/Color.h"

#include "bluegin/config.h"
#include "bluegin/resourcemanager.h"

using namespace flx;
using namespace ci;
using namespace ci::app;
using namespace bluegin;

extern FlxGlobal FlxG;

class FlxApp : public bluegin::App
{
public:
	void setup();
	void update();
	void draw();
#ifndef ANDROID
    void prepareSettings( Settings *settings );
#endif

protected:
    shared_ptr<Game> mGame;
};

#ifndef ANDROID
void FlxApp::prepareSettings( Settings *settings )
{
    settings->setWindowSize( 800, 480 );
    settings->setFrameRate( 60.0f );
    settings->setFullScreen( false );
}
#endif

void FlxApp::setup()
{    
    glDisable(GL_DITHER);
    gl::enableAlphaBlending();
    gl::setMatricesWindow(getWindowWidth(), getWindowHeight(), true);
    StatePtr initialState = StatePtr(new Play());
    mGame = shared_ptr<Game>(new Game(*this, getWindowWidth(), getWindowHeight(), initialState, 2));
    FlxG.resources->loadResourceConfig("resources.config");
    gl::clear();
}

void FlxApp::update()
{
    mGame->update();
}

void FlxApp::draw()
{	
    mGame->draw();
}

CINDER_APP_BASIC( FlxApp, RendererGl )

