#pragma once

#include "flx/flx.h"

namespace flx {

class Object;
class List;
// typedef shared_ptr<List> ListPtr;

class List
{
public:
    Object* object;
    List* next;

    List() : object(NULL), next(NULL) { }
    ~List();

    static List* create();
    static void release(List*);
};

}
