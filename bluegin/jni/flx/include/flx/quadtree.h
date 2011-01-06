#pragma once

#include "flx/flx.h"
#include "flx/object.h"
#include "flx/data/rect.h"
#include "flx/data/list.h"

namespace flx {

enum QuadTreeList
{
    A_LIST = 0,
    B_LIST = 1
};

class QuadTree;
typedef shared_ptr<QuadTree> QuadTreePtr;

typedef bool (*OverlapCallback)(Object&, Object&);

/**
 * A fairly generic quad tree structure for rapid overlap checks.
 * QuadTree is also configured for single or dual list operation.
 * You can add items either to its A list or its B list.
 * When you do an overlap check, you can compare the A list to itself,
 * or the A list against the B list.  Handy for different things!
 */
class QuadTree : public Rect
{
protected:
    /**
     * Whether this branch of the tree can be subdivided or not.
     */
    bool _canSubdivide;
    
    /**
     * These variables refer to the internal A and B linked lists,
     * which are used to store objects in the leaves.
     */
    List* _headA;
    List* _tailA;
    List* _headB;
    List* _tailB;

    /**
     * These variables refer to the potential child quadrants for this node.
     */
    static unsigned int _min;
    QuadTree* _nw;
    QuadTree* _ne;
    QuadTree* _se;
    QuadTree* _sw;		

    float _l;
    float _r;
    float _t;
    float _b;
    float _hw;
    float _hh;
    float _mx;
    float _my;
    
public:
    /**
     * Instantiate a new Quad Tree node.
     *
     * @param       X               The X-coordinate of the point in space.
     * @param       Y               The Y-coordinate of the point in space.
     * @param       Width           Desired width of this node.
     * @param       Height          Desired height of this node.
     * @param       Parent          The parent branch or node.  Pass null to create a root.
     */
    QuadTree(float X, float Y, float Width, float Height, QuadTree* ParentNode=NULL);
    virtual ~QuadTree();

    void add(Object* Object, QuadTreeList QList);
    void addObject();
    void addToList();
    bool overlap(bool BothLists=true, OverlapCallback callback=NULL);
    bool overlapNode(List* Iterator=NULL);

    //  Create and release memory allocated by the block allocator
    static QuadTree* create(float X, float Y, float Width, float Height, QuadTree* ParentNode=NULL);
    static void release(QuadTree* quadtree);
};

}
