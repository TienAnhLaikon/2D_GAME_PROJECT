#include "BaseItem.h"

bool BaseItem::init()
{
    if (!Node::init())
    {
        return false;
    }
    return true;
}
