#define FLXU_STATIC
#include "flx/flx.h"
#include "flx/object.h"
#include "flx/quadtree.h"
#include "flx/flxG.h"

#include "cinder/Rand.h"

#include <cstdlib>

using namespace bluegin;
using namespace cinder;
using namespace flx;

//  globals
FlxGlobal FlxG;

static QuadTree* quadTree = NULL;
static Rect quadTreeBounds;

namespace flx { namespace FlxU {

    void setWorldBounds(float X, float Y, float Width, float Height, int Divisions)
    {
        quadTreeBounds.x = X;
        quadTreeBounds.y = Y;
        if(Width > 0)
            quadTreeBounds.width = Width;
        if(Height > 0)
            quadTreeBounds.height = Height;
        if(Divisions > 0)
            FlxU::quadTreeDivisions = Divisions;
    }

    Vec2f rotatePoint(float X, float Y, float PivotX, float PivotY, float Angle, Vec2f* dst)
    {
        Vec2f P;
        float radians = -Angle / 180 * M_PI;
        float dx = X-PivotX;
        float dy = PivotY-Y;
        P.x = PivotX + cos(radians)*dx - sin(radians)*dy;
        P.y = PivotY - (sin(radians)*dx + cos(radians)*dy);
        if (dst != 0) {
            dst->x = P.x;
            dst->y = P.y;
        }
        return P;
    }

    float computeVelocity(float Velocity, float Acceleration, float Drag, float Max)
    {
        if (Acceleration != 0)
            Velocity += Acceleration * FlxG.elapsed;
        else if (Drag != 0)
        {
            float d = Drag*FlxG.elapsed;
            if(Velocity - d > 0)
                Velocity -= d;
            else if(Velocity + d < 0)
                Velocity += d;
            else
                Velocity = 0;
        }
        if ((Velocity != 0) && (Max != 10000))
        {
            if (Velocity > Max)
                Velocity = Max;
            else if (Velocity < -Max)
                Velocity = -Max;
        }
        return Velocity;
    }

    float random()
    {
        // return Rand::randFloat();
        return float(rand()) / RAND_MAX;
    }

    bool overlap(Object& Object1, Object& Object2, OverlapCallback Callback)
    {
        if( !Object1.exists || !Object2.exists )
            return false;
        if (quadTree) {
            QuadTree::release(quadTree);
        }
        quadTree = QuadTree::create(quadTreeBounds.x,quadTreeBounds.y,
                                    quadTreeBounds.width,quadTreeBounds.height);
        quadTree->add(&Object1,A_LIST);
        if(&Object1 == &Object2)
            return quadTree->overlap(false,Callback);
        quadTree->add(&Object2,B_LIST);
        return quadTree->overlap(true,Callback);
    }

    bool collide(Object& Object1, Object& Object2)
    {
        if( !Object1.exists || !Object2.exists )
            return false;
        if (quadTree) {
            QuadTree::release(quadTree);
        }
        quadTree = QuadTree::create(quadTreeBounds.x,quadTreeBounds.y,
                                    quadTreeBounds.width,quadTreeBounds.height);
        quadTree->add(&Object1, A_LIST);
        bool match = (&Object1 == &Object2);
        if(!match)
            quadTree->add(&Object2, B_LIST);
        bool cx = quadTree->overlap(!match, solveXCollision);
        bool cy = quadTree->overlap(!match, solveYCollision);
        return cx || cy;			
    }


    bool solveXCollision(Object& Object1, Object& Object2)
    {
        //Avoid messed up collisions ahead of time
        float o1 = Object1.colVector.x;
        float o2 = Object2.colVector.x;
        if(o1 == o2)
            return false;

        //Give the objects a heads up that we're about to resolve some collisions
        Object1.preCollide(Object2);
        Object2.preCollide(Object1);

        //Basic resolution variables
        float overlap;
        bool hit = false;
        bool p1hn2;

        //Directional variables
        bool obj1Stopped = o1 == 0;
        bool obj1MoveNeg = o1 < 0;
        bool obj1MovePos = o1 > 0;
        bool obj2Stopped = o2 == 0;
        bool obj2MoveNeg = o2 < 0;
        bool obj2MovePos = o2 > 0;

        //Offset loop variables
        int i1;
        int i2;
        Rect& obj1Hull = Object1.colHullX;
        Rect& obj2Hull = Object2.colHullX;
        vector<Vec2f>& co1 = Object1.colOffsets;
        vector<Vec2f>& co2 = Object2.colOffsets;
        int l1 = co1.size();
        int l2 = co2.size();
        float ox1;
        float oy1;
        float ox2;
        float oy2;
        float r1;
        float r2;
        float sv1;
        float sv2;

        //Decide based on object's movement patterns if it was a right-side or left-side collision
        p1hn2 = ((obj1Stopped && obj2MoveNeg) || (obj1MovePos && obj2Stopped) || (obj1MovePos && obj2MoveNeg) || //the obvious cases
                (obj1MoveNeg && obj2MoveNeg && (((o1>0)?o1:-o1) < ((o2>0)?o2:-o2))) || //both moving left, obj2 overtakes obj1
                (obj1MovePos && obj2MovePos && (((o1>0)?o1:-o1) > ((o2>0)?o2:-o2))) ); //both moving right, obj1 overtakes obj2

        //Check to see if these objects allow these collisions
        if(p1hn2?(!Object1.collideRight || !Object2.collideLeft):(!Object1.collideLeft || !Object2.collideRight))
            return false;

        //this looks insane, but we're just looping through collision offsets on each object
        for(i1 = 0; i1 < l1; i1++)
        {
            ox1 = co1[i1].x;
            oy1 = co1[i1].y;
            obj1Hull.x += ox1;
            obj1Hull.y += oy1;
            for(i2 = 0; i2 < l2; i2++)
            {
                ox2 = co2[i2].x;
                oy2 = co2[i2].y;
                obj2Hull.x += ox2;
                obj2Hull.y += oy2;

                //See if it's a actually a valid collision
                if( (obj1Hull.x + obj1Hull.width  < obj2Hull.x + roundingError) ||
                    (obj1Hull.x + roundingError > obj2Hull.x + obj2Hull.width) ||
                    (obj1Hull.y + obj1Hull.height < obj2Hull.y + roundingError) ||
                    (obj1Hull.y + roundingError > obj2Hull.y + obj2Hull.height) )
                {
                    obj2Hull.x -= ox2;
                    obj2Hull.y -= oy2;
                    continue;
                }

                //Calculate the overlap between the objects
                if(p1hn2)
                {
                    if(obj1MoveNeg)
                        r1 = obj1Hull.x + Object1.colHullY.width;
                    else
                        r1 = obj1Hull.x + obj1Hull.width;
                    if(obj2MoveNeg)
                        r2 = obj2Hull.x;
                    else
                        r2 = obj2Hull.x + obj2Hull.width - Object2.colHullY.width;
                }
                else
                {
                    if(obj2MoveNeg)
                        r1 = -obj2Hull.x - Object2.colHullY.width;
                    else
                        r1 = -obj2Hull.x - obj2Hull.width;
                    if(obj1MoveNeg)
                        r2 = -obj1Hull.x;
                    else
                        r2 = -obj1Hull.x - obj1Hull.width + Object1.colHullY.width;
                }
                overlap = r1 - r2;

                //Last chance to skip out on a bogus collision resolution
                if( (overlap == 0) ||
                    ((!Object1.fixed && ((overlap>0)?overlap:-overlap) > obj1Hull.width*0.8)) ||
                    ((!Object2.fixed && ((overlap>0)?overlap:-overlap) > obj2Hull.width*0.8)) )
                {
                    obj2Hull.x -= ox2;
                    obj2Hull.y -= oy2;
                    continue;
                }
                hit = true;

                //Adjust the objects according to their flags and stuff
                sv1 = Object2.velocity.x;
                sv2 = Object1.velocity.x;
                if(!Object1.fixed && Object2.fixed)
                {
                    if(Object1._group)
                        Object1.reset(Object1.x - overlap,Object1.y);
                    else
                        Object1.x -= overlap;
                }
                else if(Object1.fixed && !Object2.fixed)
                {
                    if(Object2._group)
                        Object2.reset(Object2.x + overlap,Object2.y);
                    else
                        Object2.x += overlap;
                }
                else if(!Object1.fixed && !Object2.fixed)
                {
                    overlap /= 2;
                    if(Object1._group)
                        Object1.reset(Object1.x - overlap,Object1.y);
                    else
                        Object1.x -= overlap;
                    if(Object2._group)
                        Object2.reset(Object2.x + overlap,Object2.y);
                    else
                        Object2.x += overlap;
                    sv1 /= 2;
                    sv2 /= 2;
                }
                if(p1hn2)
                {
                    Object1.hitRight(Object2,sv1);
                    Object2.hitLeft(Object1,sv2);
                }
                else
                {
                    Object1.hitLeft(Object2,sv1);
                    Object2.hitRight(Object1,sv2);
                }

                //Adjust collision hulls if necessary
                if(!Object1.fixed && (overlap != 0))
                {
                    if(p1hn2)
                        obj1Hull.width -= overlap;
                    else
                    {
                        obj1Hull.x -= overlap;
                        obj1Hull.width += overlap;
                    }
                    Object1.colHullY.x -= overlap;
                }
                if(!Object2.fixed && (overlap != 0))
                {
                    if(p1hn2)
                    {
                        obj2Hull.x += overlap;
                        obj2Hull.width -= overlap;
                    }
                    else
                        obj2Hull.width += overlap;
                    Object2.colHullY.x += overlap;
                }
                obj2Hull.x -= ox2;
                obj2Hull.y -= oy2;
            }
            obj1Hull.x -= ox1;
            obj1Hull.y -= oy1;
        }

        return hit;
    }

    bool solveYCollision(Object& Object1, Object& Object2)
    {
        //Avoid messed up collisions ahead of time
        float o1 = Object1.colVector.y;
        float o2 = Object2.colVector.y;
        if(o1 == o2)
            return false;

        //Give the objects a heads up that we're about to resolve some collisions
        Object1.preCollide(Object2);
        Object2.preCollide(Object1);

        //Basic resolution variables
        float overlap;
        bool hit = false;
        bool p1hn2;

        //Directional variables
        bool obj1Stopped = o1 == 0;
        bool obj1MoveNeg = o1 < 0;
        bool obj1MovePos = o1 > 0;
        bool obj2Stopped = o2 == 0;
        bool obj2MoveNeg = o2 < 0;
        bool obj2MovePos = o2 > 0;

        //Offset loop variables
        int i1;
        int i2;
        Rect& obj1Hull = Object1.colHullY;
        Rect& obj2Hull = Object2.colHullY;
        vector<Vec2f>& co1 = Object1.colOffsets;
        vector<Vec2f>& co2 = Object2.colOffsets;
        int l1 = co1.size();
        int l2 = co2.size();
        float ox1;
        float oy1;
        float ox2;
        float oy2;
        float r1;
        float r2;
        float sv1;
        float sv2;

        //Decide based on object's movement patterns if it was a top or bottom collision
        p1hn2 = ((obj1Stopped && obj2MoveNeg) || (obj1MovePos && obj2Stopped) || (obj1MovePos && obj2MoveNeg) || //the obvious cases
                (obj1MoveNeg && obj2MoveNeg && (((o1>0)?o1:-o1) < ((o2>0)?o2:-o2))) || //both moving up, obj2 overtakes obj1
                (obj1MovePos && obj2MovePos && (((o1>0)?o1:-o1) > ((o2>0)?o2:-o2))) ); //both moving down, obj1 overtakes obj2

        //Check to see if these objects allow these collisions
        if(p1hn2?(!Object1.collideBottom || !Object2.collideTop):(!Object1.collideTop || !Object2.collideBottom))
            return false;

        //this looks insane, but we're just looping through collision offsets on each object
        for(i1 = 0; i1 < l1; i1++)
        {
            ox1 = co1[i1].x;
            oy1 = co1[i1].y;
            obj1Hull.x += ox1;
            obj1Hull.y += oy1;
            for(i2 = 0; i2 < l2; i2++)
            {
                ox2 = co2[i2].x;
                oy2 = co2[i2].y;
                obj2Hull.x += ox2;
                obj2Hull.y += oy2;

                //See if it's a actually a valid collision
                if( (obj1Hull.x + obj1Hull.width  < obj2Hull.x + roundingError) ||
                    (obj1Hull.x + roundingError > obj2Hull.x + obj2Hull.width) ||
                    (obj1Hull.y + obj1Hull.height < obj2Hull.y + roundingError) ||
                    (obj1Hull.y + roundingError > obj2Hull.y + obj2Hull.height) )
                {
                    obj2Hull.x -= ox2;
                    obj2Hull.y -= oy2;
                    continue;
                }

                //Calculate the overlap between the objects
                if(p1hn2)
                {
                    if(obj1MoveNeg)
                        r1 = obj1Hull.y + Object1.colHullX.height;
                    else
                        r1 = obj1Hull.y + obj1Hull.height;
                    if(obj2MoveNeg)
                        r2 = obj2Hull.y;
                    else
                        r2 = obj2Hull.y + obj2Hull.height - Object2.colHullX.height;
                }
                else
                {
                    if(obj2MoveNeg)
                        r1 = -obj2Hull.y - Object2.colHullX.height;
                    else
                        r1 = -obj2Hull.y - obj2Hull.height;
                    if(obj1MoveNeg)
                        r2 = -obj1Hull.y;
                    else
                        r2 = -obj1Hull.y - obj1Hull.height + Object1.colHullX.height;
                }
                overlap = r1 - r2;

                //Last chance to skip out on a bogus collision resolution
                if( (overlap == 0) ||
                    ((!Object1.fixed && ((overlap>0)?overlap:-overlap) > obj1Hull.height*0.8)) ||
                    ((!Object2.fixed && ((overlap>0)?overlap:-overlap) > obj2Hull.height*0.8)) )
                {
                    obj2Hull.x -= ox2;
                    obj2Hull.y -= oy2;
                    continue;
                }
                hit = true;

                //Adjust the objects according to their flags and stuff
                sv1 = Object2.velocity.y;
                sv2 = Object1.velocity.y;
                if(!Object1.fixed && Object2.fixed)
                {
                    if(Object1._group)
                        Object1.reset(Object1.x, Object1.y - overlap);
                    else
                        Object1.y -= overlap;
                }
                else if(Object1.fixed && !Object2.fixed)
                {
                    if(Object2._group)
                        Object2.reset(Object2.x, Object2.y + overlap);
                    else
                        Object2.y += overlap;
                }
                else if(!Object1.fixed && !Object2.fixed)
                {
                    overlap /= 2;
                    if(Object1._group)
                        Object1.reset(Object1.x, Object1.y - overlap);
                    else
                        Object1.y -= overlap;
                    if(Object2._group)
                        Object2.reset(Object2.x, Object2.y + overlap);
                    else
                        Object2.y += overlap;
                    sv1 /= 2;
                    sv2 /= 2;
                }
                if(p1hn2)
                {
                    Object1.hitBottom(Object2,sv1);
                    Object2.hitTop(Object1,sv2);
                }
                else
                {
                    Object1.hitTop(Object2,sv1);
                    Object2.hitBottom(Object1,sv2);
                }

                //Adjust collision hulls if necessary
                if(!Object1.fixed && (overlap != 0))
                {
                    if(p1hn2)
                    {
                        obj1Hull.y -= overlap;

                        //This code helps stuff ride horizontally moving platforms.
                        if(Object2.fixed && Object2.moves)
                        {
                            sv1 = Object2.colVector.x;
                            Object1.x += sv1;
                            obj1Hull.x += sv1;
                            Object1.colHullX.x += sv1;
                        }
                    }
                    else
                    {
                        obj1Hull.y -= overlap;
                        obj1Hull.height += overlap;
                    }
                }
                if(!Object2.fixed && (overlap != 0))
                {
                    if(p1hn2)
                    {
                        obj2Hull.y += overlap;
                        obj2Hull.height -= overlap;
                    }
                    else
                    {
                        obj2Hull.height += overlap;

                        //This code helps stuff ride horizontally moving platforms.
                        if(Object1.fixed && Object1.moves)
                        {
                            sv2 = Object1.colVector.x;
                            Object2.x += sv2;
                            obj2Hull.x += sv2;
                            Object2.colHullX.x += sv2;
                        }
                    }
                }
                obj2Hull.x -= ox2;
                obj2Hull.y -= oy2;
            }
            obj1Hull.x -= ox1;
            obj1Hull.y -= oy1;
        }

        return hit;
    }

    float getAngle(float X, float Y)
    {
        return atan2(Y,X) * 180.0f / M_PI;
    }
    
    ColorA color(unsigned int c)
    {
        ColorA ret;
        ret.a = (c >> 24) / 255.0f;
        //  Assume alpha is 1.0f if not provided
        if (ret.a == 0) ret.a = 1.0f;

        ret.r = ((c >> 16) & 0xFF) / 255.0f;
        ret.g = ((c >> 8)  & 0xFF) / 255.0f;
        ret.b = (c & 0xFF) / 255.0f;
        return ret;
    }

} }  // namespace flx::FlxU
