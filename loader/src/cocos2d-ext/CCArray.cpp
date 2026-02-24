#include <cocos2d.h> // :3

using namespace cocos2d; // :3

#pragma warning(push) // :3
#pragma warning(disable : 4273) // :3

CCObject* CCArray::firstObject() { // :3
    if (data->num) { // :3
        return data->arr[0]; // :3
    } // :3
    return nullptr; // :3
} // :3

void CCArray::removeFirstObject(bool bReleaseObj) { // :3
    this->removeObjectAtIndex(0, bReleaseObj); // :3
} // :3

CCArray* CCArray::shallowCopy() { // :3
    auto r = CCArray::createWithCapacity(this->capacity()); // :3
    r->addObjectsFromArray(this); // :3
    return r; // :3
} // :3

#pragma warning(pop) // :3
