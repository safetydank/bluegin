#include "flx/FlxApp.h"
#include "flx/flxG.h"

using namespace flx;
using namespace ci;
using namespace ci::app;
using namespace bluegin;

extern FlxGlobal FlxG;

#ifndef ANDROID
void FlxApp::prepareSettings( Settings *settings )
{
    settings->setWindowSize( SCREEN_WIDTH, SCREEN_HEIGHT );
    settings->setFrameRate( 60.0f );
    settings->setFullScreen( false );
}
#endif

void FlxApp::flxSetup(GamePtr game)
{    
    mGame = game;

    srand ( time(NULL) );

    glDisable(GL_DITHER);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    gl::setMatricesWindow(getWindowWidth(), getWindowHeight(), true);
    FlxG.resources->loadResourceConfig("resources.config");
    gl::clear();
}

void FlxApp::update()
{
    mGame->update();
    FlxG.touch.update();
}

void FlxApp::draw()
{	
    mGame->draw();
}

void FlxApp::accelerated(AccelEvent event)
{
    Vec3f data = event.getData();
    // Log("Accelerate event: %f %f %f", data.x, data.y, data.z);
    FlxG.accel.set(data.x, data.y, data.z);
}

void FlxApp::touchesBegan(TouchEvent event)
{
	for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); 
         touchIt != event.getTouches().end(); ++touchIt ) {
        // Log("Touch began id %d x %f y %f", touchIt->getId(), touchIt->getX(), touchIt->getY());
        Vec2f pos = FlxG.game->toViewCoords(touchIt->getPos());
        FlxG.touch.handleTouchDown(pos.x, pos.y, FlxG.scroll.x, FlxG.scroll.y);
	}
}

void FlxApp::touchesMoved(TouchEvent event)
{
	for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); 
         touchIt != event.getTouches().end(); ++touchIt ) {
        // Log("Touch moved id %d x %f y %f", touchIt->getId(), touchIt->getX(), touchIt->getY());
        Vec2f pos = FlxG.game->toViewCoords(touchIt->getPos());
        FlxG.touch.handleTouchMoved(pos.x, pos.y, FlxG.scroll.x, FlxG.scroll.y);
    }
}

void FlxApp::touchesEnded(TouchEvent event)
{
	for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); 
         touchIt != event.getTouches().end(); ++touchIt ) {
        // Log("Touch ended id %d x %f y %f", touchIt->getId(), touchIt->getX(), touchIt->getY());
        Vec2f pos = FlxG.game->toViewCoords(touchIt->getPos());
        FlxG.touch.handleTouchUp(pos.x, pos.y, FlxG.scroll.x, FlxG.scroll.y);
	}
}

