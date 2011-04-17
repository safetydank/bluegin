#include "bluegin/app.h"

//  Force linking of app native entry points into shared library
#ifdef ANDROID
#include "bluegin/android.h"
#define FORCE_STATIC_LINK(X) void* bluegin_link_##X = (void*) &(X)
extern "C" {
FORCE_STATIC_LINK(Java_com_expb_bluegin_Native_setup);
FORCE_STATIC_LINK(Java_com_expb_bluegin_Native_update);
FORCE_STATIC_LINK(Java_com_expb_bluegin_Native_draw);
FORCE_STATIC_LINK(Java_com_expb_bluegin_Native_addTouchEvent);
FORCE_STATIC_LINK(Java_com_expb_bluegin_Native_setTouches);
FORCE_STATIC_LINK(Java_com_expb_bluegin_Native_setAccelerometer);
FORCE_STATIC_LINK(Java_com_expb_bluegin_Native_setScreenSize);
FORCE_STATIC_LINK(Java_com_expb_bluegin_Native_cleanup);
};
#endif

using namespace bluegin;
using namespace ci;
using namespace ci::app;

#ifdef ANDROID
//  Cinder App emulation
BlueginApp::BlueginApp() : mElapsed(0), mPrevAccelData(0, 0, 0)
{
}

void BlueginApp::doResize(int width, int height)
{
    mWindowSize.x = width;
    mWindowSize.y = height;
    Log("doResize set window size to (%d, %d) from (%d, %d)", 
            mWindowSize.x, mWindowSize.y, width, height);
    resize(ResizeEvent(mWindowSize));
}

void BlueginApp::doSetup()
{
    this->setup();
}

void BlueginApp::doUpdate(int dt)
{
    mDT = dt;
    mElapsed += dt;
    update();
}

void BlueginApp::doDraw()
{
    this->draw();
}

float BlueginApp::getWindowAspectRatio()
{
    return (mWindowSize.y != 0) ? (float) mWindowSize.x / mWindowSize.y : 1.0f;
}

int BlueginApp::getWindowWidth()
{
    return mWindowSize.x;
}

int BlueginApp::getWindowHeight()
{
    return mWindowSize.y;
}

#else

//  Cinder emulation of BlueGin input
void BlueginApp::mouseDown(MouseEvent event) 
{
    Vec2f pos = event.getPos();
    if (event.isLeft())
        addTouchEvent(0, pos.x, pos.y, pos.x, pos.y, 0);
    doTouch();
}

void BlueginApp::mouseMove(MouseEvent event)
{
    Vec2f pos = event.getPos();
    addTouchEvent(1, pos.x, pos.y, pos.x, pos.y, 0);
    doTouch();
}

void BlueginApp::mouseDrag(MouseEvent event)
{
    mouseMove(event);
}

void BlueginApp::mouseUp(MouseEvent event)
{
    Vec2f pos = event.getPos();
    if (event.isLeft())
        addTouchEvent(2, pos.x, pos.y, pos.x, pos.y, 0);
    doTouch();
}

#endif

//  Common app code for BlueGin / Cinder
void BlueginApp::doAccelerated(float x, float y, float z)
{
    // Log("doAccelerated %f %f %f", x, y, z);
    // XXX no filtering yet
    mAccelData = Vec3f(x, y, z);
    ci::app::AccelEvent event(mAccelData, mAccelData, mPrevAccelData, mPrevAccelData);
    accelerated(event);
    mPrevAccelData = Vec3f(x, y, z);
}

void BlueginApp::addTouchEvent(int eventType, float x, float y, float px, float py, int id)
{
    TouchEvent::Touch touch(Vec2f(x, y), Vec2f(px, py), id, getElapsedSeconds(), NULL);
    switch (eventType) {
        case 0:
            mTouchesBegan.push_back(touch);
            break;
        case 1:
            mTouchesMoved.push_back(touch);
            break;
        case 2:
            mTouchesEnded.push_back(touch);
            break;
        default:
            break;
    }
}

void BlueginApp::addKeyEvent(bool keyDown, char aChar, unsigned int aModifiers, unsigned int aNativeKeyCode)
{
    // XXX translate keycode, modifiers too?
    //
    KeyEvent keyEvent(aNativeKeyCode, aChar, aModifiers, aNativeKeyCode);
    if (keyDown)
        mKeyDownEvents.push_back(keyEvent);
    else
        mKeyUpEvents.push_back(keyEvent);
}

void BlueginApp::doTouch()
{
    if (!mTouchesBegan.empty()) {
        touchesBegan(TouchEvent(mTouchesBegan));
        mTouchesBegan.clear();
    }
    if (!mTouchesMoved.empty()) {
        touchesMoved(TouchEvent(mTouchesMoved));
        mTouchesMoved.clear();
    }
    if (!mTouchesEnded.empty()) {
        touchesEnded(TouchEvent(mTouchesEnded));
        mTouchesEnded.clear();
    }
}

void BlueginApp::doKeys()
{
    if (!mKeyDownEvents.empty()) {
        for (vector<KeyEvent>::iterator it = mKeyDownEvents.begin(); it != mKeyDownEvents.end(); ++it) {
            keyDown(*it);
        }
        mKeyDownEvents.clear();
    }

    if (!mKeyUpEvents.empty()) {
        for (vector<KeyEvent>::iterator it = mKeyUpEvents.begin(); it != mKeyUpEvents.end(); ++it) {
            keyUp(*it);
        }
        mKeyUpEvents.clear();
    }
}

void BlueginApp::doPause()
{
    this->paused();
}

void BlueginApp::doResume()
{
    Log("XXX doResume()");
    this->resumed();
}

