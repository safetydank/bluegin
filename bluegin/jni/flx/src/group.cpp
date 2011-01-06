#include "flx/flx.h"
#include "flx/group.h"
#include "cinder/Rand.h"
#include <cassert>

using namespace cinder;
using namespace flx;


Group::Group() : Object(0, 0, 0, 0)
{
    _group = true;
    solid  = false;
    _first = true;
    moves  = false;
}

Group::~Group()
{
}

void Group::add(ObjectPtr object, bool shareScroll)
{
    members.push_back(object);
    if (shareScroll)
        object->scrollFactor = scrollFactor;
}

ObjectPtr Group::replace(ObjectPtr OldObject,ObjectPtr NewObject)
{
    ObjectPtr ret;
    for (vector<ObjectPtr>::iterator it = members.begin(); it != members.end(); ++it) {
        if (*it == OldObject) {
            ret = *it = NewObject;
            break;
        }
    }
    return ret;
}

ObjectPtr Group::remove(Object& object, bool Splice)
{
    ObjectPtr ret;
    for (vector<ObjectPtr>::iterator it = members.begin(); it != members.end(); ++it) {
        if (it->get() == &object) {
            ret = *it;
            if (Splice) {
                members.erase(it);
            }
            else {
                *it = ObjectPtr();
            }
            break;
        }
    }
    return ret;
}

void Group::removeAll()
{
    members.clear();
}

Object* Group::getFirstAvail()
{
    for (vector<ObjectPtr>::iterator it = members.begin(); it != members.end(); ++it) {
        if (*it) {
            Object& o = **it;
            if (!o.exists)
                return &o;
        }
    }
    return NULL;
}

Object* Group::getFirstNull()
{
    // XXX TODO
    return NULL;
}

bool Group::resetFirstAvail(float x, float y)
{
    Object* o = getFirstAvail();
    if (o == NULL)
        return false;
    o->reset(x, y);
    return true;
}

Object* Group::getFirstExtant()
{
    for (vector<ObjectPtr>::iterator it = members.begin(); it != members.end(); ++it) {
        if (*it) {
            Object& o = **it;
            if (o.exists)
                return &o;
        }
    }
    return NULL;
}

Object* Group::getFirstAlive()
{
    for (vector<ObjectPtr>::iterator it = members.begin(); it != members.end(); ++it) {
        if (*it) {
            Object& o = **it;
            if (o.exists && !o.dead)
                return &o;
        }
    }
    return NULL;
}

Object* Group::getFirstDead()
{
    for (vector<ObjectPtr>::iterator it = members.begin(); it != members.end(); ++it) {
        if (*it) {
            Object& o = **it;
            if (o.dead)
                return &o;
        }
    }
    return NULL;
}

int Group::countLiving()
{
    int count = -1;
    for (vector<ObjectPtr>::iterator it = members.begin(); it != members.end(); ++it) {
        if (*it) {
            Object& o = **it;
            if(count < 0)
                count = 0;
            if (o.exists && !o.dead)
                ++count;
        }
    }
    return count;
}

int Group::countDead()
{
    int count = -1;
    for (vector<ObjectPtr>::iterator it = members.begin(); it != members.end(); ++it) {
        if (*it) {
            Object& o = **it;
            if(count < 0)
                count = 0;
            if (o.dead)
                ++count;
        }
    }
    return count;
}

int Group::countOnScreen()
{
    int count = -1;
    for (vector<ObjectPtr>::iterator it = members.begin(); it != members.end(); ++it) {
        if (*it) {
            Object& o = **it;
            if(count < 0)
                count = 0;
            if (o.onScreen())
                ++count;
        }
    }
    return count;
}		


Object* Group::getRandom()
{
    int c = 0;
    int l = members.size();
    int i = int(FlxU::random()*l);
    ObjectPtr o;
    while(!bool(o) && (c < l))
    {
        o = members[(++i)%l];
        c++;
    }
    return bool(o) ? o.get() : NULL;
}

void Group::saveOldPosition()
{
    if (_first) 
    {
        _first = false;
        _last.x = 0;
        _last.y = 0;
        return;
    }
    _last.x = x;
    _last.y = y;
}

void Group::updateMembers()
{
    float mx;
    float my;
    bool moved = false;
    if ((x != _last.x) || (y != _last.y))
    {
        moved = true;
        mx = x - _last.x;
        my = y - _last.y;
    }
    for (vector<ObjectPtr>::iterator it = members.begin(); it != members.end(); ++it) {
        if (it->get() == NULL)
            continue;

        Object& o = **it;
        if (o.exists)
        {
            if (moved)
            {
                if (o._group)
                    o.reset(o.x+mx, o.y+my);
                else
                {
                    o.x += mx;
                    o.y += my;
                }
            }

            if (o.active)
                o.update();

            if (moved && o.solid)
            {
                o.colHullX.width += ((mx>0)?mx:-mx);
                if (mx < 0)
                    o.colHullX.x += mx;
                o.colHullY.x = x;
                o.colHullY.height += ((my>0)?my:-my);
                if (my < 0)
                    o.colHullY.y += my;
                o.colVector.x += mx;
                o.colVector.y += my;
            }
        }
    }
}

//  Object overrides
void Group::update()
{
    saveOldPosition();
    updateMotion();
    updateMembers();
    updateFlickering();
}

void Group::renderMembers()
{
    for (vector<ObjectPtr>::iterator it = members.begin(); it != members.end(); ++it) {
        if (it->get() == NULL)
            continue;

        Object& o = **it;
        if (o.exists && o.visible)
            o.render();
    }
}

void Group::render()
{
    renderMembers();
}

void Group::kill()
{
    killMembers();
    Object::kill();
}

void Group::killMembers()
{
    for (vector<ObjectPtr>::iterator it = members.begin(); it != members.end(); ++it) {
        if (it->get() == NULL)
            continue;
        (*it)->kill();
    }
}

void Group::destroyMembers()
{
    //  XXX call destructors by freeing smartptrs?
    for (vector<ObjectPtr>::iterator it = members.begin(); it != members.end(); ++it) {
        if (it->get() == NULL)
            continue;
        (*it)->destroy();
    }
}

void Group::destroy()
{
    destroyMembers();
    Object::destroy();
}

void Group::reset(float x, float y)
{
    saveOldPosition();
    Object::reset(x,y);

    float mx;
    float my;
    bool moved = false;
    if ((x != _last.x) || (y != _last.y))
    {
        moved = true;
        mx = x - _last.x;
        my = y - _last.y;
    }

    for (vector<ObjectPtr>::iterator it = members.begin(); it != members.end(); ++it) {
        if (it->get() == NULL)
            continue;

        Object& o = **it;
        if (o.exists)
        {
            if (moved)
            {
                if (o._group)
                    o.reset(o.x+mx,o.y+my);
                else
                {
                    o.x += mx;
                    o.y += my;
                    if (solid)
                    {
                        o.colHullX.width += ((mx>0)?mx:-mx);
                        if(mx < 0)
                            o.colHullX.x += mx;
                        o.colHullY.x = x;
                        o.colHullY.height += ((my>0)?my:-my);
                        if(my < 0)
                            o.colHullY.y += my;
                        o.colVector.x += mx;
                        o.colVector.y += my;
                    }
                }
            }
        }
    }
}

