#include "bluegin/app.h"
#include "cinder/app/AccelEvent.h"
#include "cinder/app/TouchEvent.h"
#include "flx/game.h"

namespace flx {

const int SCREEN_WIDTH  = 800;
const int SCREEN_HEIGHT = 480;

/**
 * A base class for Flx games, implemented as a Bluegin app.
 *
 * Performs common setup and writes Cinder input events to Flx.  See FlxMode
 * for an example of use.
 *
 */
class FlxApp : public bluegin::BlueginApp
{
public:
    void update();
    void draw();

    void flxSetup(flx::GamePtr game);

    void touchesBegan(ci::app::TouchEvent event);
    void touchesMoved(ci::app::TouchEvent event);
    void touchesEnded(ci::app::TouchEvent event);
    void accelerated(ci::app::AccelEvent event);

#ifndef ANDROID
    void prepareSettings( Settings *settings );
#endif

protected:
    GamePtr mGame;
};

}

