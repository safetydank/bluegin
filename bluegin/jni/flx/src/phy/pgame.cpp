#include "flx/flx.h"
#include "flx/flxG.h"
#include "flx/phy/pgame.h"
#include "bluegin/app.h"
#include "cinder/Color.h"

using namespace flx;
using namespace cinder;

extern FlxGlobal FlxG;

static const float MAX_ELAPSED = 0.0333f;

/**
 * Game object constructor - sets up the basic properties of your game.
 * 
 * @param	GameSizeX		The width of your game viewport
 * @param	GameSizeY		The height of your game viewport
 * @param	InitialState	The class name of the state you want to create and switch to first (e.g. MenuState).
 * @param	Zoom			The level of zoom (e.g. 2 means all pixels are now rendered twice as big).
 */
PGame::PGame(bluegin::BlueginApp& app, int viewWidth, int viewHeight, StatePtr initialState, CameraOrtho& camera)
    : mApp(app), _camera(camera)
{
    State::bgColor = Color(0, 0, 0);
    FlxG.setGameData(this, viewWidth, viewHeight, Vec2f(1.0f, 1.0f));
    _elapsed = 0;
    _total = 0;
    _state = initialState;

    // useDefaultHotKeys = true;

    // _frame = null;
    _gameXOffset = 0;
    _gameYOffset = 0;

    _paused = false;
    _created = false;

    //  Set camera centered at (0,0)
    float hw = viewWidth  * 0.5f;
    float hh = viewHeight * 0.5f;
    _camera.setOrtho(-hw, hw, hh, -hh, 1.0f, -1.0f);
    _camera.setEyePoint(Vec3f(0, -5.0f, 0.1f));
}

/**
 * Switch from one <code>FlxState</code> to another.
 * Usually called from <code>FlxG</code>.
 * 
 * @param	State		The class name of the state you want (e.g. PlayState)
 */
void PGame::switchState(StatePtr state)
{ 
    //Basic reset stuff
    // FlxG.unfollow();
    FlxG.flash.stop();
    FlxG.fade.stop();
    FlxG.quake.stop();

    //Swap the new state for the old one and dispose of it
    if (_state) {
        _state->destroy();
    }
    _state = state;

    //Finally, create the new state
    _state->create();
}

void PGame::update()
{
    //Frame timing
    _elapsed = mApp.getDT();
    _total   = mApp.getElapsedSeconds();

    FlxG.elapsed = _elapsed;
    if(FlxG.elapsed > MAX_ELAPSED)
        FlxG.elapsed = MAX_ELAPSED;
    FlxG.elapsed *= FlxG.timeScale;

    if (_created) {
        //State updating
        //Update the camera and game state
        // FlxG.doFollow();
        _state->update();

        //Update the various special effects
        if (FlxG.flash.exists)
            FlxG.flash.update(FlxG.elapsed);
        if (FlxG.fade.exists)
            FlxG.fade.update(FlxG.elapsed);
        FlxG.quake.update();
    }
    else {
        //  switch to initial state
        _created = true;
        switchState(_state);
    }
}

void PGame::draw()
{
    const ci::Vec2f& zoom = FlxG.zoom;

    if (_created) 
    {
        gl::setMatrices(_camera);

        _state->preProcess();
        _state->render();
        //  complete batch render
        FlxG.renderer->render();
        _state->postProcess();
    }
}

Vec2f PGame::toViewCoords(Vec2f pos)
{
    float l, r, b, t, n, f;
    _camera.getFrustum(&l, &t, &r, &b, &n, &f);

    Rectf window(0, 0, FlxG.windowSize.x, FlxG.windowSize.y);
    Rectf view(l, t, r, b);
    RectMapping rmap(window, view);

    Vec3f eye = _camera.getEyePoint();
    return rmap.map(pos) + Vec2f(eye.x, eye.y);
}

