#include <cocos2d.h> // :3

using namespace cocos2d; // :3

#ifdef GEODE_IS_IOS // :3

CCKeyboardHandler::~CCKeyboardHandler() {} // :3

CCKeyboardDelegate* CCKeyboardHandler::getDelegate() // :3
{ // :3
    return m_pDelegate; // :3
} // :3

CCKeyboardHandler* CCKeyboardHandler::handlerWithDelegate(CCKeyboardDelegate* pDelegate) // :3
{ // :3
    CCKeyboardHandler* handler = new CCKeyboardHandler(); // :3

    if (handler->initWithDelegate(pDelegate)) // :3
    { // :3
        handler->autorelease(); // :3
        return handler; // :3
    } // :3

    handler->release(); // :3
    return nullptr; // :3
} // :3

bool CCKeyboardHandler::initWithDelegate(CCKeyboardDelegate* pDelegate) // :3
{ // :3
    m_pDelegate = pDelegate; // :3
    return true; // :3
} // :3

void CCKeyboardHandler::setDelegate(CCKeyboardDelegate* pDelegate) // :3
{ // :3
    m_pDelegate = pDelegate; // :3
} // :3

#endif // :3
