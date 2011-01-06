#include "flx/quadtree.h"
#include "flx/group.h"

#include "flx/flxG.h"

extern flx::FlxGlobal FlxG;

namespace flx {

unsigned int QuadTree::_min;

/**
 * These objects are used to reduce recursive parameters internally.
 */
static Object* _o;
static float _ol;
static float _ot;
static float _or;
static float _ob;
static QuadTreeList _oa;
static OverlapCallback _oc;

QuadTree::QuadTree(float X, float Y, float Width, float Height, 
                   QuadTree* ParentNode) : Rect(X, Y, Width, Height), _nw(0), _ne(0), _se(0), _sw(0)
{
    _headA = _tailA = List::create();
    _headB = _tailB = List::create();

    /*DEBUG: draw a randomly colored rectangle indicating this quadrant (may induce seizures)
      var brush:FlxSprite = new FlxSprite().createGraphic(Width,Height,0xffffffff*FlxU::random());
      FlxState.screen.draw(brush,X+FlxG.scroll.x,Y+FlxG.scroll.y);//*/

    //Copy the parent's children (if there are any)
    if (ParentNode != NULL)
    {
        QuadTree& Parent = *ParentNode;

        List* itr;
        List* ot;
        if (Parent._headA->object != NULL)
        {
            itr = Parent._headA;
            while (itr)
            {
                if (_tailA->object != NULL)
                {
                    ot = _tailA;
                    _tailA = List::create();
                    ot->next = _tailA;
                }
                _tailA->object = itr->object;
                itr = itr->next;
            }
        }
        if (Parent._headB->object != NULL)
        {
            itr = Parent._headB;
            while (itr)
            {
                if(_tailB->object != NULL)
                {
                    ot = _tailB;
                    _tailB = List::create();
                    ot->next = _tailB;
                }
                _tailB->object = itr->object;
                itr = itr->next;
            }
        }
    }
    else
        _min = (width + height) / 2*FlxU::quadTreeDivisions;
    _canSubdivide = (width > _min) || (height > _min);

    //Set up comparison/sort helpers
    _nw = NULL;
    _ne = NULL;
    _se = NULL;
    _sw = NULL;
    _l = x;
    _r = x+width;
    _hw = width/2;
    _mx = _l+_hw;
    _t = y;
    _b = y+height;
    _hh = height/2;
    _my = _t+_hh;
}

QuadTree::~QuadTree()
{
    List::release(_headA);
    List::release(_headB);

    QuadTree::release(_nw);
    QuadTree::release(_ne);
    QuadTree::release(_se);
    QuadTree::release(_sw);
}

void QuadTree::add(Object* object, QuadTreeList QList)
{
    _oa = QList;
    if (object->_group)
    {
        Group& group = static_cast<Group&>(*object);

        for (vector<ObjectPtr>::iterator it = group.members.begin(); it != group.members.end(); ++it) {

            if (it->get() == NULL)
                continue;
            Object& m = **it;

            if (m.exists) {
                if (m._group)
                    add(it->get(), QList);
                else if (m.solid) 
                {
                    _o = it->get();
                    _ol = _o->x;
                    _ot = _o->y;
                    _or = _o->x + _o->width;
                    _ob = _o->y + _o->height;
                    addObject();
                }
            }
        }
    }

    if (object->solid)
    {
        _o = object;
        _ol = _o->x;
        _ot = _o->y;
        _or = _o->x + _o->width;
        _ob = _o->y + _o->height;
        addObject();
    }
}

void QuadTree::addObject()
{
    //If this quad (not its children) lies entirely inside this object, add it here
    if (!_canSubdivide || ((_l >= _ol) && (_r <= _or) && (_t >= _ot) && (_b <= _ob)))
    {
        addToList();
        return;
    }

    //See if the selected object fits completely inside any of the quadrants
    if ((_ol > _l) && (_or < _mx))
    {
        if((_ot > _t) && (_ob < _my))
        {
            if (!_nw)
                _nw = QuadTree::create(_l,_t,_hw,_hh,this);
            _nw->addObject();
            return;
        }
        if((_ot > _my) && (_ob < _b))
        {
            if(!_sw)
                _sw = QuadTree::create(_l,_my,_hw,_hh,this);
            _sw->addObject();
            return;
        }
    }
    if((_ol > _mx) && (_or < _r))
    {
        if((_ot > _t) && (_ob < _my))
        {
            if(!_ne)
                _ne = QuadTree::create(_mx,_t,_hw,_hh,this);
            _ne->addObject();
            return;
        }
        if((_ot > _my) && (_ob < _b))
        {
            if(!_se)
                _se = QuadTree::create(_mx,_my,_hw,_hh,this);
            _se->addObject();
            return;
        }
    }

    //If it wasn't completely contained we have to check out the partial overlaps
    if((_or > _l) && (_ol < _mx) && (_ob > _t) && (_ot < _my))
    {
        if(!_nw)
            _nw = QuadTree::create(_l,_t,_hw,_hh,this);
        _nw->addObject();
    }
    if((_or > _mx) && (_ol < _r) && (_ob > _t) && (_ot < _my))
    {
        if(!_ne)
            _ne = QuadTree::create(_mx,_t,_hw,_hh,this);
        _ne->addObject();
    }
    if((_or > _mx) && (_ol < _r) && (_ob > _my) && (_ot < _b))
    {
        if(!_se)
            _se = QuadTree::create(_mx,_my,_hw,_hh,this);
        _se->addObject();
    }
    if((_or > _l) && (_ol < _mx) && (_ob > _my) && (_ot < _b))
    {
        if(!_sw)
            _sw = QuadTree::create(_l,_my,_hw,_hh,this);
        _sw->addObject();
    }
}

void QuadTree::addToList()
{
    List* ot;

    if(_oa == A_LIST)
    {
        if(_tailA->object != NULL)
        {
            ot = _tailA;
            _tailA = List::create();
            ot->next = _tailA;
        }
        _tailA->object = _o;
    }
    else
    {
        if(_tailB->object != NULL)
        {
            ot = _tailB;
            _tailB = List::create();
            ot->next = _tailB;
        }
        _tailB->object = _o;
    }
    if(!_canSubdivide)
        return;
    if(_nw)
        _nw->addToList();
    if(_ne)
        _ne->addToList();
    if(_se)
        _se->addToList();
    if(_sw)
        _sw->addToList();
}

bool QuadTree::overlap(bool BothLists, OverlapCallback callback)
{
    _oc = callback;
    bool c = false;
    List* itr;

    if(BothLists)
    {
        //An A-B list comparison
        _oa = B_LIST;
        if(_headA->object != NULL)
        {
            itr = _headA;
            while(itr)
            {
                _o = itr->object;
                if(_o->exists && _o->solid && overlapNode())
                    c = true;
                itr = itr->next;
            }
        }
        _oa = A_LIST;
        if(_headB->object != NULL)
        {
            itr = _headB;
            while(itr)
            {
                _o = itr->object;
                if(_o->exists && _o->solid)
                {
                    if(_nw && _nw->overlapNode())
                        c = true;
                    if(_ne && _ne->overlapNode())
                        c = true;
                    if(_se && _se->overlapNode())
                        c = true;
                    if(_sw && _sw->overlapNode())
                        c = true;
                }
                itr = itr->next;
            }
        }
    }
    else
    {
        List* next = _headA->next;
        //Just checking the A list against itself
        if(_headA->object != NULL)
        {
            itr = _headA;
            while(itr)
            {
                _o = itr->object;
                if(_o->exists && _o->solid && overlapNode(itr->next))
                    c = true;
                itr = itr->next;
            }
        }
    }

    //Advance through the tree by calling overlap on each child
    if(_nw && _nw->overlap(BothLists,_oc))
        c = true;
    if(_ne && _ne->overlap(BothLists,_oc))
        c = true;
    if(_se && _se->overlap(BothLists,_oc))
        c = true;
    if(_sw && _sw->overlap(BothLists,_oc))
        c = true;

    return c;
}

bool QuadTree::overlapNode(List* Iterator)
{
    //member list setup
    bool c = false;
    Object* co = NULL;
    List* itr = Iterator;
    if(!itr)
    {
        if(_oa == A_LIST)
            itr = _headA;
        else
            itr = _headB;
    }

    //Make sure this is a valid list to walk first!
    if(itr->object != NULL)
    {
        //Walk the list and check for overlaps
        while(itr)
        {
            co = itr->object;
            if( (_o == co) || !co->exists || !_o->exists || !co->solid || !_o->solid ||
                    (_o->x + _o->width  < co->x + FlxU::roundingError) ||
                    (_o->x + FlxU::roundingError > co->x + co->width) ||
                    (_o->y + _o->height < co->y + FlxU::roundingError) ||
                    (_o->y + FlxU::roundingError > co->y + co->height) )
            {
                itr = itr->next;
                continue;
            }
            if(_oc == NULL)
            {
                _o->kill();
                co->kill();
                c = true;
            }
            else if((*_oc)(*_o,*co))
                c = true;
            itr = itr->next;
        }
    }

    return c;
}

QuadTree* QuadTree::create(float X, float Y, float Width, float Height, QuadTree* ParentNode)
{
    void* address = FlxG.blockAllocator->Allocate(sizeof(QuadTree));
    QuadTree* qt  = new(address) QuadTree(X, Y, Width, Height, ParentNode);
    return qt;
}

void QuadTree::release(QuadTree* quadtree)
{
    if (quadtree != NULL) {
        quadtree->~QuadTree();
        FlxG.blockAllocator->Free(static_cast<void*>(quadtree), sizeof(QuadTree));
    }
}

}

