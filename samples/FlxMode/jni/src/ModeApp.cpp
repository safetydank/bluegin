#include "flx/FlxApp.h"

#include "flx/flxG.h"
#include "flx/game.h"
#include "flx/state.h"
#include "mode/PlayState.h"
#include "mode/MenuState.h"

#include "cinder/gl/gl.h"

#include "bluegin/resourcemanager.h"

using namespace flx;
using namespace ci;
using namespace ci::app;
using namespace bluegin;

extern FlxGlobal FlxG;

class ModeApp : public FlxApp
{
public:
	void setup()
    {
        Vec2f zoom(2.0f, 2.0f);
        StatePtr initialState = StatePtr(new MenuState());
        mGame = GamePtr(new Game(*this, 
                                 getWindowWidth() / zoom.x, getWindowHeight() / zoom.y, 
                                 initialState, zoom));
        flxSetup(mGame);

        //  Load all configured textures and sounds
        FlxG.resources->acquireAllTextures();
        FlxG.resources->acquireAllSounds();

        //  Set default font
        FlxG.defaultFont = FlxG.resources->font("nokia").get();

        gl::clear();
    }
};

CINDER_APP_BASIC( ModeApp, RendererGl )

