#pragma once

#include "bluegin/app.h"
#include "flx/state.h"
#include "cinder/Vector.h"

namespace bluegin {
    class App;
}

namespace flx {

class IGame
{
public:
    /**
     * Switch from one <code>State</code> to another.
     * Usually called from <code>FlxG</code>.
     * 
     * @param	State		The class name of the state you want (e.g. PlayState)
     */
    virtual void switchState(StatePtr state) = 0;

    /**
     * This is the main game loop.  It controls all the updating and rendering.
     */
    virtual void update() = 0;

    /**
     * This is the main game loop.  It controls all the updating and rendering.
     */
    virtual void draw() = 0;
    
    //  translate a point from screen coords to viewport coords
    virtual ci::Vec2f toViewCoords(ci::Vec2f pos) = 0;
    
    StatePtr _state;    
};

class Game : public IGame
{
public:
    Game(bluegin::BlueginApp& app, int gameSizeX, int gameSizeY, StatePtr initialState, ci::Vec2f zoom=ci::Vec2f(1.0f, 1.0f));

    void switchState(StatePtr state);
    void update();
    void draw();

    double _elapsed;
    double _total;
    bool  _paused;
    bool  _created;

    int _gameXOffset;
    int _gameYOffset;
    ci::Vec2f _zoom;

    bluegin::BlueginApp& mApp;

    virtual ci::Vec2f toViewCoords(ci::Vec2f pos);
};

typedef shared_ptr<Game> GamePtr;

}
