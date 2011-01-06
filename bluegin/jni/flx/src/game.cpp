#include "flx/flx.h"
#include "flx/flxG.h"
#include "flx/game.h"
#include "bluegin/app.h"
#include "cinder/Color.h"

using namespace flx;
using namespace cinder;

extern FlxGlobal FlxG;

static const float MAX_ELAPSED = 0.0333f;

/**
 * Game object constructor - sets up the basic properties of your game.
 * 
 * @param	GameSizeX		The width of your game in pixels (e.g. 320).
 * @param	GameSizeY		The height of your game in pixels (e.g. 240).
 * @param	InitialState	The class name of the state you want to create and switch to first (e.g. MenuState).
 * @param	Zoom			The level of zoom (e.g. 2 means all pixels are now rendered twice as big).
 */
Game::Game(bluegin::BlueginApp& app, int gameSizeX, int gameSizeY, StatePtr initialState, Vec2f zoom)
    : mApp(app)
{
    _zoom = zoom;
    State::bgColor = Color(0, 0, 0);
    FlxG.setGameData(this, gameSizeX, gameSizeY, zoom);
    _elapsed = 0;
    _total = 0;
    // pause = new FlxPause();
    _state = initialState;

    // useDefaultHotKeys = true;

    // _frame = null;
    _gameXOffset = 0;
    _gameYOffset = 0;

    _paused = false;
    _created = false;
}

/**
 * Switch from one <code>FlxState</code> to another.
 * Usually called from <code>FlxG</code>.
 * 
 * @param	State		The class name of the state you want (e.g. PlayState)
 */
void Game::switchState(StatePtr state)
{ 
    //Basic reset stuff
    // FlxG.panel.hide();
    FlxG.unfollow();
    // FlxG.destroySounds();
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

void Game::update()
{
    //Frame timing
    _elapsed = mApp.getDT();
    _total   = mApp.getElapsedSeconds();

    FlxG.elapsed = _elapsed;
    if(FlxG.elapsed > MAX_ELAPSED)
        FlxG.elapsed = MAX_ELAPSED;
    FlxG.elapsed *= FlxG.timeScale;

    if (_created) {
        //Animate flixel HUD elements
        // FlxG.panel.update();
        // _console.update();

        //State updating
        if(_paused)
        {
            // pause.update();
            // if(_flipped)
            //     FlxG.buffer.copyPixels(_bmpFront.bitmapData,_r,new Point(0,0));
            // else
            //     FlxG.buffer.copyPixels(_bmpBack.bitmapData,_r,new Point(0,0));
            // pause.render();
        }
        else
        {
            //Update the camera and game state
            FlxG.doFollow();
            _state->update();

            //Update the various special effects
            if (FlxG.flash.exists)
                FlxG.flash.update(FlxG.elapsed);
            if (FlxG.fade.exists)
                FlxG.fade.update(FlxG.elapsed);
            FlxG.quake.update();

            // if(FlxG.panel.visible)
            //     FlxG.panel.render();
        }
    }
    else {
        //  switch to initial state
        _created = true;
        switchState(_state);
    }
}

void Game::draw()
{
    //Render game content, special fx, and overlays
    const ci::Vec2f& zoom = FlxG.zoom;

    if (_created) 
    {
        // gl::setMatricesWindow(FlxG.width, FlxG.height, true);

        gl::color(Color(1.0f, 1.0f, 1.0f));
        gl::pushMatrices();
        gl::scale(Vec3f(zoom.x, zoom.y, 1.0f));

        gl::pushMatrices();
        // render quake effect
        gl::translate(Vec2f(FlxG.quake.x, FlxG.quake.y));

        _state->preProcess();
        _state->render();
        //  complete batch render
        FlxG.renderer->render();
        _state->postProcess();

        gl::popMatrices();

        glDisable(GL_TEXTURE_2D);

        if(FlxG.flash.exists) {
            gl::color(ColorA(FlxG.flash.color, FlxG.flash.alpha));
            gl::drawSolidRect(Rectf(0, 0, FlxG.width, FlxG.height));
        }

        if (FlxG.fade.exists) {
            gl::color(FlxG.fade.fadeColor);
            gl::drawSolidRect(Rectf(0, 0, FlxG.width, FlxG.height));
        }

        gl::popMatrices();
    }
}

Vec2f Game::toViewCoords(Vec2f pos)
{
    return Vec2f(pos.x / FlxG.zoom.x, pos.y / FlxG.zoom.y);
}
