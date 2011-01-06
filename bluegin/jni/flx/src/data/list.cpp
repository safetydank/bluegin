#include "flx/data/list.h"
#include "flx/flxG.h"

using namespace flx;

extern FlxGlobal FlxG;

List::~List()
{
    if (next) {
        release(next);
    }
}

List* List::create()
{
    void* address = FlxG.blockAllocator->Allocate(sizeof(List));
    List* l  = new(address) List();
    return l;
}

void List::release(List* l)
{
    if (l != NULL) {
        l->~List();
        FlxG.blockAllocator->Free(static_cast<void*>(l), sizeof(List));
    }
}

