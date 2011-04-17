#pragma once

#include "bluegin/bluegin.h"

#include "cinder/app/AccelEvent.h"
#include "cinder/app/TouchEvent.h"
#include "cinder/app/KeyEvent.h"
#include "cinder/app/ResizeEvent.h"

#ifndef ANDROID
#include "cinder/app/MouseEvent.h"
#include "cinder/app/AppBasic.h"

///  Platform abstraction layer
namespace bluegin {

///  Emulates a BlueGin app on Windows Cinder
class BlueginApp : public ci::app::AppBasic
{
public:
    virtual void pause() { }
    virtual void resume() { }

    inline double getDT() { return 1.0/60.0; }

    //  Input handlers
    virtual void accelerated(ci::app::AccelEvent event) { }
    virtual void touchesBegan(ci::app::TouchEvent event) { }
    virtual void touchesMoved(ci::app::TouchEvent event) { }
    virtual void touchesEnded(ci::app::TouchEvent event) { }

    virtual void keyDown(ci::app::KeyEvent event) { }
    virtual void keyUp(ci::app::KeyEvent event) { }

    void doTouch();             // emulate touch from mouse input
    void doPause();
    void doResume();
    void doAccelerated(float x, float y, float z);

    void mouseDown(ci::app::MouseEvent event);
    void mouseMove(ci::app::MouseEvent event);
    void mouseDrag(ci::app::MouseEvent event);
    void mouseUp(ci::app::MouseEvent event);

    void addTouchEvent(int eventType, float x, float y, float px, float py, int id);
    void addKeyEvent(bool keyDown, char aChar, unsigned int aModifiers, unsigned int aNativeKeyCode);

private:
    ci::Vec3f mAccelData;
    ci::Vec3f mPrevAccelData;

    //  Touches
    std::vector<ci::app::TouchEvent::Touch> mTouchesBegan;
    std::vector<ci::app::TouchEvent::Touch> mTouchesMoved;
    std::vector<ci::app::TouchEvent::Touch> mTouchesEnded;

    //  Key events
    std::vector<ci::app::KeyEvent> mKeyEvents;
};

} // namespace

#else

#include "cinder/app/ResizeEvent.h"
#include "cinder/gl/Texture.h"

namespace bluegin {

class BlueginApp
{
public:
    BlueginApp();

    //  overridden by implementation
    virtual void setup()   = 0;
	virtual void update()  = 0;
	virtual void draw()    = 0;

    //  Input handlers
    virtual void accelerated(ci::app::AccelEvent event) { }
    virtual void touchesBegan(ci::app::TouchEvent event) { }
    virtual void touchesMoved(ci::app::TouchEvent event) { }
    virtual void touchesEnded(ci::app::TouchEvent event) { }

    virtual void keyDown(ci::app::KeyEvent event) { }
    virtual void keyUp(ci::app::KeyEvent event) { }

    float getWindowAspectRatio();
    int   getWindowWidth();
    int   getWindowHeight();

    //  Optional overrides
    virtual void pause() { }
    virtual void resume() { }

    virtual void resize(cinder::app::ResizeEvent event) { }

    inline float getElapsedSeconds() { return mElapsed / 1000.0f; }

    //  Bluegin specific (not from Cinder)
    void doSetup();
    void doUpdate(int dt);
    void doDraw();
    void doResize(int width, int height);
    void doAccelerated(float x, float y, float z);
    void doPause();
    void doResume();
    void doTouch();
    void doKeys();

    inline double getDT() { return mDT / 1000.0; }

    void addTouchEvent(int eventType, float x, float y, float px, float py, int id);
    void addKeyEvent(bool keyDown, char aChar, unsigned int aModifiers, unsigned int aNativeKeyCode);

private:
    long mElapsed;
    int mDT;

    ci::Vec2i mWindowSize;
    ci::Vec3f mAccelData;
    ci::Vec3f mPrevAccelData;

    //  Touches
    std::vector<ci::app::TouchEvent::Touch> mTouchesBegan;
    std::vector<ci::app::TouchEvent::Touch> mTouchesMoved;
    std::vector<ci::app::TouchEvent::Touch> mTouchesEnded;

    //  Key events
    std::vector<ci::app::KeyEvent> mKeyDownEvents;
    std::vector<ci::app::KeyEvent> mKeyUpEvents;
};

} // namespace bluegin

//  App macro
#define BLUEGIN_APP( APP )         \
bluegin::BlueginApp* theApp__ = 0; \
void nativeCreate() {              \
    theApp__ = new APP;            \
}

//  CINDER_APP_BASIC compatibility shim 
namespace cinder { namespace app {
typedef bool RendererGl;
} }
#define CINDER_APP_BASIC( APP, X ) BLUEGIN_APP( APP )

#endif

