#include "flx/flx.h"
#include "flx/flxG.h"
#include "flx/sprite.h"

#include "cinder/CinderMath.h"
#include "cinder/gl/Texture.h"

#include <algorithm>

using namespace ci;
using namespace ci::gl;
using namespace flx;

extern FlxGlobal FlxG;

Sprite::Sprite(float x, float y, bluegin::Graphic graphic)
    : Object(x, y, 0, 0)
{
    offset.set(0, 0);
    scale.set(1.0f, 1.0f);
    frameRect.set(0, 0, 0, 0);
    mColor = ColorA(1.0f, 1.0f, 1.0f, 1.0f);

    antialiasing = false;
    finished = false;
    _facing = RIGHT;
    _flipped = 0;
    _curAnim = NULL;
    _curFrame = 0;
    _caf = 0;
    _frameTimer = 0;

    _mtx.setToIdentity();
    if (graphic.getWidth() && graphic.getHeight()) {
        loadGraphic(graphic);
    }
    else {
        createGraphic(10, 10);
    }
}

void Sprite::createGraphic(float w, float h, ColorA color)
{
    width = frameWidth = w;
    height = frameHeight = h;
    mColor = color;
    resetHelpers();
}

void Sprite::loadGraphic(bluegin::Graphic graphic, 
        bool animated, bool reverse, int w, int h, bool unique)
{
    Texture& tex = graphic.getTexture();
    if (!tex) {
        //  No texture associated with Graphic
        return;
    }

    this->graphic = graphic;
    _flipped = reverse ? graphic.getWidth() >> 1 : 0;
    if (w == 0) {
        if (animated)
            w = graphic.getHeight();
        else if (_flipped > 0) 
            w = graphic.getWidth() / 2;
        else
            w = graphic.getWidth();
    }
    width = frameWidth = w;
    if (h == 0) {
        if (animated) 
            h = width;
        else
            h = graphic.getHeight();
    }
    height = frameHeight = h;
    resetHelpers();
}

void Sprite::resetHelpers()
{
    frameRect.set(0, 0, frameWidth, frameHeight);
    calcOrigin();
    _caf = 0;

    refreshHulls();
}

void Sprite::calcOrigin()
{
    origin.x = frameWidth  / 2;
    origin.y = frameHeight / 2;
}

Direction_t Sprite::getFacing()
{
    return _facing;
}

Direction_t Sprite::setFacing(Direction_t dir)
{
    bool c = _facing != dir;
    _facing = dir;
    if (c) calcFrame();
    return _facing;
}

float Sprite::getAlpha()
{
    return mColor.a;
}

void Sprite::setAlpha(float alpha)
{
    alpha = math<float>::clamp(alpha, 0, 1.0f);
    if (mColor.a == alpha) return;
    calcFrame();
}

ColorA Sprite::getColor()
{
    return mColor;
}

void Sprite::setColor(ColorA color)
{
    mColor = color;    
}

void draw(Sprite& brush, ci::Vec2f pos)
{
    //  not implemented
}

void Sprite::updateAnimation()
{
    if (_curAnim != NULL && _curAnim->delay > 0 && (_curAnim->looped || !finished))
    {
        _frameTimer += FlxG.elapsed;
        if (_frameTimer > _curAnim->delay)
        {
            _frameTimer -= _curAnim->delay;
            if (_curFrame == _curAnim->frames.size()-1)
            {
                if (_curAnim->looped) _curFrame = 0;
                finished = true;
            }
            else
                _curFrame++;
            _caf = _curAnim->frames[_curFrame];
            calcFrame();
        }
    }
}

void Sprite::update()
{
    updateMotion();
    updateAnimation();
    updateFlickering();
}

void Sprite::renderSprite()
{
    if (!onScreen())
        return;

    // getScreenXY(_point);
    _mtx.setToIdentity();
    _mtx.translate(Vec3f(_point.x + origin.x, _point.y + origin.y, 0));
    if (angle != 0)
        _mtx.rotate(Vec3f(0, 0, 1.0f), M_PI * 2 * (angle / 360.0f));
    _mtx.scale(Vec3f(scale.x, scale.y, 1.0f));
    _mtx.translate(Vec3f(-origin.x, -origin.y, 0));

    //  draw
    BatchState batch(graphic.getTexture(), false);
    FlxG.renderer->setBatchState(batch);
    Rectf texRect = graphic.mapFrameRect(frameRect, (getFacing() == LEFT));
    FlxG.renderer->renderQuad(mColor, getFrameWidth(), getFrameHeight(), texRect, &_mtx, NULL);
}

// render implementation
void Sprite::render()
{
    renderSprite();
}

bool Sprite::overlapsPoint(Vec2f pt)
{
    pt.x -= FlxG.scroll.x;
    pt.y -= FlxG.scroll.y;
    if ((pt.x <= _point.x) || (pt.x >= _point.x+frameWidth) 
     || (pt.y <= _point.y) || (pt.y >= _point.y+frameHeight))
        return false;
    return true;
}

void Sprite::addAnimation(const char* name, int frameCount, int frames[], float framerate, bool looped)
{
    vector<int> vframes;
    vframes.resize(frameCount);
    std::copy(frames, frames+frameCount, vframes.begin());
    _animations.push_back(Anim(string(name), vframes, framerate, looped));
}

// void addAnimationCallback();
void Sprite::play(const char* name, bool force)
{
    string sname(name);
    if (!force && (_curAnim != NULL) && (name == _curAnim->name)) return;
    _curFrame = 0;
    _caf = 0;
    _frameTimer = 0;
    for (vector<Anim>::iterator it = _animations.begin(); it != _animations.end(); ++it) {
        if (it->name == name) {
            _curAnim = &(*it);
            finished = (_curAnim <= 0);
            _caf = _curAnim->frames[_curFrame];
            calcFrame();
            break;
        }
    }
}

void Sprite::randomFrame()
{
    _curAnim = NULL;
    _caf = ((int) FlxU::random() * (graphic.getWidth() / frameWidth));
    calcFrame();
}

int Sprite::getFrame()
{
    return _caf;
}

void Sprite::setFrame(int frame)
{
    _curAnim = NULL;
    _caf = frame;
    calcFrame();
}

Vec2d& Sprite::getScreenXY(Vec2d& pt)
{
    // XXX removed floor call on x/y
    pt.x = x + FlxG.scroll.x*scrollFactor.x - offset.x;
    pt.y = y + FlxG.scroll.y*scrollFactor.y - offset.y;
    return pt;
}

void Sprite::calcFrame()
{
    int rx = _caf * frameWidth;
    int ry = 0;

    //Handle sprite sheets
    int w = graphic.getWidth();
    if (rx >= w)
    {
        ry = int(rx / w) * frameHeight;
        rx %= w;
    }

    //Update display bitmap
    frameRect.set(rx, ry, rx+frameWidth, ry+frameHeight);
    // if(_callback != null) 
    //     _callback(_curAnim.name,_curFrame,_caf);
}

