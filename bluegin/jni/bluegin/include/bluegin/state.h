#pragma once

#include "bluegin/bluegin.h"

#include <map>
#include <string>

namespace bluegin
{

class State
{
public:
    void enter();
    void leave();
    void update(float dt);
    void draw();

    std::string name()
    {
        return mName;
    }

private:
    virtual void onEnter()  = 0;
    virtual void onLeave()  = 0;
    virtual void onUpdate(float dt) = 0;
    virtual void onDraw()   = 0;

    std::string mName;
};

class StateManager
{
private:
    vector<State*> mStates;
    State*         mCurrent;
    std::map<std::string, State*> mStateMap;

public:
    StateManager() : mCurrent(0) { }

    void addState(const char* name, State* state);
    void setInitialState(const char* name);
};

}
