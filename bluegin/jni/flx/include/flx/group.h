#pragma once

#include "flx/object.h"
#include "cinder/Vector.h"

namespace flx {
    
/**
 * This is an organizational class that can update and render a bunch of <code>Object</code>s.
 * NOTE: Although <code>Group</code> extends <code>Object</code>, it will not automatically
 * add itself to the global collisions quad tree, it will only add its members.
 */
class Group : public Object
{
public:
    Group();
    virtual ~Group();

    /**
     * Adds a new <code>Object</code> subclass (Sprite, Tileblock, etc) to the list of children
     *
     * @param	Object			The object you want to add
     * @param	ShareScroll		Whether or not this Object should sync up with this layer's scrollFactor
     */
    void      add(ObjectPtr object, bool shareScroll=false);

    /**
     * Replaces an existing <code>Object</code> with a new one.
     * 
     * @param	OldObject	The object you want to replace.
     * @param	NewObject	The new object you want to use instead.
     * 
     * @return	The new object.
     */
    ObjectPtr replace(ObjectPtr OldObject,ObjectPtr NewObject);

    /**
     * Removes an object from the group.
     * 
     * @param	Object	The <code>Object</code> you want to remove.
     * @param	Splice	Whether the object should be cut from the array entirely or not.
     * 
     * @return	The removed object.
     */
    ObjectPtr remove(Object& object, bool Splice=false);

    /**
     * Remove all objects from this group
     */
    void removeAll();

    /**
     * Call this function to retrieve the first object with exists == false in the group.
     * This is handy for recycling in general, e.g. respawning enemies.
     * 
     * @return	A <code>Object</code> currently flagged as not existing.
     */
    Object* getFirstAvail();

    /**
     * Finds the first object with exists == false and calls reset on it.
     * 
     * @param	X	The new X position of this object.
     * @param	Y	The new Y position of this object.
     * 
     * @return	Whether a suitable <code>Object</code> was found and reset.
     */
    bool    resetFirstAvail(float x, float y);

    /**
     * Call this function to retrieve the first object with exists == true in the group.
     * This is handy for checking if everything's wiped out, or choosing a squad leader, etc.
     * 
     * @return	A <code>Object</code> currently flagged as existing.
     */
    Object* getFirstExtant();

    /**
     * Call this function to retrieve the first object with dead == false in the group.
     * This is handy for checking if everything's wiped out, or choosing a squad leader, etc.
     * 
     * @return	A <code>Object</code> currently flagged as not dead.
     */
    Object* getFirstAlive();

    /**
     * Call this function to retrieve the first index set to 'null'.
     * Returns -1 if no index stores a null object.
     * 
     * @return	An <code>int</code> indicating the first null slot in the group.
     */
    Object* getFirstNull();

    /**
     * Call this function to retrieve the first object with dead == true in the group.
     * This is handy for checking if everything's wiped out, or choosing a squad leader, etc.
     * 
     * @return	A <code>Object</code> currently flagged as dead.
     */
    Object* getFirstDead();

    /**
     * Call this function to find out how many members of the group are not dead.
     * 
     * @return	The number of <code>Object</code>s flagged as not dead.  Returns -1 if group is empty.
     */
    int countLiving();

    /**
     * Call this function to find out how many members of the group are dead.
     * 
     * @return	The number of <code>Object</code>s flagged as dead.  Returns -1 if group is empty.
     */
    int countDead();

    /**
     * Returns a count of how many objects in this group are on-screen right now.
     * 
     * @return	The number of <code>Object</code>s that are on screen.  Returns -1 if group is empty.
     */
    int countOnScreen();

    /**
     * Returns a member at random from the group.
     * 
     * @return	A <code>Object</code> from the members list.
     */
    Object* getRandom();

    /**
     * Internal function, helps with the moving/updating of group members.
     */
    void saveOldPosition();

    /**
     * Internal function that actually goes through and updates all the group members.
     * Depends on <code>saveOldPosition()</code> to set up the correct values in <code>_last</code> in order to work properly.
     */
    virtual void updateMembers();

    /**
     * Internal function that actually loops through and renders all the group members.
     */
    void renderMembers();

    //  Object overrides

    /**
     * Automatically goes through and calls update on everything you added,
     * override this function to handle custom input and perform collisions.
     */
    virtual void update();

    /**
     * Automatically goes through and calls render on everything you added,
     * override this loop to control render order manually.
     */
    virtual void render();

    /**
     * Calls kill on the group and all its members.
     */
    virtual void kill();

    /**
     * If the group's position is reset, we want to reset all its members too.
     * 
     * @param	X	The new X position of this object.
     * @param	Y	The new Y position of this object.
     */
    virtual void reset(float x, float y);

    /**
     * Override this function to handle any deleting or "shutdown" type operations you might need,
     * such as removing traditional Flash children like Sprite objects.
     */
    virtual void destroy();

    /**
     * Vector of all the <code>Object</code>s that exist in this layer.
     */
    vector<ObjectPtr> members;

protected:
    /**
     * Helpers for moving/updating group members.
     */
    ci::Vec2f _last;
    bool _first;

    /**
     * Internal function that calls kill on all members.
     */
    void killMembers();

    /**
     * Internal function that actually loops through and destroys each member.
     */
    void destroyMembers();
};

typedef shared_ptr<Group> GroupPtr;

}
