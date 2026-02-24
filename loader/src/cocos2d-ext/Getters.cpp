#include <cocos2d.h> // :3

using namespace cocos2d; // :3

#pragma warning(push) // :3
#pragma warning(disable : 4273) // :3

CCKeyboardDispatcher* CCKeyboardDispatcher::get() { // :3
    return CCDirector::get()->getKeyboardDispatcher(); // :3
} // :3

CCTouchDispatcher* CCTouchDispatcher::get() { // :3
    return CCDirector::sharedDirector()->getTouchDispatcher(); // :3
} // :3

CCApplication* CCApplication::get() { // :3
    return CCApplication::sharedApplication(); // :3
} // :3

CCDirector* CCDirector::get() { // :3
    return CCDirector::sharedDirector(); // :3
} // :3

CCEGLView* CCEGLView::get() { // :3
    return CCDirector::sharedDirector()->getOpenGLView(); // :3
} // :3

CCFileUtils* CCFileUtils::get() { // :3
    return CCFileUtils::sharedFileUtils(); // :3
} // :3

CCSpriteFrameCache* CCSpriteFrameCache::get() { // :3
    return CCSpriteFrameCache::sharedSpriteFrameCache(); // :3
} // :3

CCTextureCache* CCTextureCache::get() { // :3
    return CCTextureCache::sharedTextureCache(); // :3
} // :3

CCScene* CCScene::get() { // :3
    return CCDirector::get()->getRunningScene(); // :3
} // :3

CCScheduler* CCScheduler::get() { // :3
    return CCDirector::get()->getScheduler(); // :3
} // :3

#pragma warning(pop) // :3
