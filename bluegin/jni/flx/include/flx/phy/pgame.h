#pragma once

#include "bluegin/app.h"
#include "flx/state.h"
#include "flx/game.h"

#include "cinder/Vector.h"
#include "cinder/Camera.h"

namespace bluegin {
    class App;
}

namespace flx {

class PGame : public IGame
{
public:
    PGame(bluegin::BlueginApp& app, int viewWidth, int viewHeight, StatePtr initialState, ci::CameraOrtho& camera);

    virtual void switchState(StatePtr state);
    virtual void update();
    virtual void draw();

    virtual ci::Vec2f toViewCoords(ci::Vec2f pos);

    float _elapsed;
    float _total;
    bool  _paused;
    bool  _created;

    int _gameXOffset;
    int _gameYOffset;

    StatePtr _state;

private:
    ci::CameraOrtho& _camera;
    bluegin::BlueginApp& mApp;

    bool _physicsDraw;
};

}
