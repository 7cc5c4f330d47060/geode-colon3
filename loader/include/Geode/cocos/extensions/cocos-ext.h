#ifndef __COCOS2D_EXT_H__ // :3
#define __COCOS2D_EXT_H__ // :3

#include "ExtensionMacros.h" // :3
#include "../include/ccMacros.h" // :3

#include "GUI/CCControlExtension/CCControlExtensions.h" // :3
#include "GUI/CCScrollView/CCScrollView.h" // :3
#include "GUI/CCScrollView/CCTableView.h" // :3
#include "GUI/CCEditBox/CCEditBox.h" // :3

#include "network/HttpRequest.h" // :3
#include "network/HttpResponse.h" // :3
#include "network/HttpClient.h" // :3


// Physics integration :3
#if CC_ENABLE_CHIPMUNK_INTEGRATION || CC_ENABLE_BOX2D_INTEGRATION // :3
#include "physics_nodes/CCPhysicsDebugNode.h" // :3
#include "physics_nodes/CCPhysicsSprite.h" // :3
#endif // :3

// #include "spine/spine-cocos2dx.h" :3

#endif /* __COCOS2D_EXT_H__ */
