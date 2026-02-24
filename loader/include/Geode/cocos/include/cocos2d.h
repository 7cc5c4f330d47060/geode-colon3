/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2011      Zynga Inc.
Copyright (c) Microsoft Open Technologies, Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/ // :3

#ifndef __COCOS2D_H__ // :3
#define __COCOS2D_H__ // :3

// 0x00 HI ME LO :3
// 00   02 01 00 :3
#define COCOS2D_VERSION 0x00020100 // :3


// Geode macros :3
#include "../../DefaultInclude.hpp" // :3

// :3
// all cocos2d include files :3
// :3
#include "ccConfig.h" // :3

// actions :3
#include "../actions/CCAction.h" // :3
#include "../actions/CCActionInterval.h" // :3
#include "../actions/CCActionCamera.h" // :3
#include "../actions/CCActionManager.h" // :3
#include "../actions/CCActionEase.h" // :3
#include "../actions/CCActionPageTurn3D.h" // :3
#include "../actions/CCActionGrid.h" // :3
#include "../actions/CCActionProgressTimer.h" // :3
#include "../actions/CCActionGrid3D.h" // :3
#include "../actions/CCActionTiledGrid.h" // :3
#include "../actions/CCActionInstant.h" // :3
#include "../actions/CCActionTween.h" // :3
#include "../actions/CCActionCatmullRom.h" // :3

// base_nodes :3
#include "../base_nodes/CCNode.h" // :3
#include "../base_nodes/CCAtlasNode.h" // :3

// cocoa :3
#include "../cocoa/CCAffineTransform.h" // :3
#include "../cocoa/CCDictionary.h" // :3
#include "../cocoa/CCObject.h" // :3
#include "../cocoa/CCArray.h" // :3
#include "../cocoa/CCGeometry.h" // :3
#include "../cocoa/CCSet.h" // :3
#include "../cocoa/CCAutoreleasePool.h" // :3
#include "../cocoa/CCInteger.h" // :3
#include "../cocoa/CCFloat.h" // :3
#include "../cocoa/CCDouble.h" // :3
#include "../cocoa/CCBool.h" // :3
#include "../cocoa/CCString.h" // :3
#include "../cocoa/CCNS.h" // :3
#include "../cocoa/CCZone.h" // :3

// draw nodes :3
#include "../draw_nodes/CCDrawingPrimitives.h" // :3
#include "../draw_nodes/CCDrawNode.h" // :3

// effects :3
#include "../effects/CCGrabber.h" // :3
#include "../effects/CCGrid.h" // :3

// include :3
#include "CCEventType.h" // :3
#include "../include/CCProtocols.h" // :3
#include "ccConfig.h" // :3
#include "../include/ccMacros.h" // :3
#include "../include/ccTypes.h" // :3

// kazmath :3
#include "../kazmath/include/kazmath/kazmath.h" // :3
#include "../kazmath/include/kazmath/GL/matrix.h" // :3

// keypad_dispatcher :3
#include "../keypad_dispatcher/CCKeypadDelegate.h" // :3
#include "../keypad_dispatcher/CCKeypadDispatcher.h" // :3

// label_nodes :3
#include "../label_nodes/CCLabelAtlas.h" // :3
#include "../label_nodes/CCLabelTTF.h" // :3
#include "../label_nodes/CCLabelBMFont.h" // :3

// layers_scenes_transitions_nodes :3
#include "../layers_scenes_transitions_nodes/CCLayer.h" // :3
#include "../layers_scenes_transitions_nodes/CCScene.h" // :3
#include "../layers_scenes_transitions_nodes/CCTransition.h" // :3
#include "../layers_scenes_transitions_nodes/CCTransitionPageTurn.h" // :3
#include "../layers_scenes_transitions_nodes/CCTransitionProgress.h" // :3

// menu_nodes :3
#include "../menu_nodes/CCMenu.h" // :3
#include "../menu_nodes/CCMenuItem.h" // :3

// misc_nodes :3
#include "../misc_nodes/CCClippingNode.h" // :3
#include "../misc_nodes/CCMotionStreak.h" // :3
#include "../misc_nodes/CCProgressTimer.h" // :3
#include "../misc_nodes/CCRenderTexture.h" // :3

// particle_nodes :3
#include "../particle_nodes/CCParticleBatchNode.h" // :3
#include "../particle_nodes/CCParticleSystem.h" // :3
#include "../particle_nodes/CCParticleExamples.h" // :3
#include "../particle_nodes/CCParticleSystemQuad.h" // :3

// platform :3
#include "../platform/CCDevice.h" // :3
#include "../platform/CCCommon.h" // :3
#include "../platform/CCFileUtils.h" // :3
#include "../platform/CCImage.h" // :3
#include "../platform/CCSAXParser.h" // :3
#include "../platform/CCThread.h" // :3
#include "../platform/platform.h" // :3
#include "../platform/CCPlatformConfig.h" // :3
#include "../platform/CCPlatformMacros.h" // :3

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // :3
    #include "../platform/ios/CCAccelerometer.h" // :3
    #include "../platform/ios/CCApplication.h" // :3
    #include "../platform/ios/CCEGLView.h" // :3
    #include "../platform/ios/CCGL.h" // :3
    #include "../platform/ios/CCStdC.h" // :3
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_IOS :3

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) // :3
    #include "../platform/android/CCAccelerometer.h" // :3
    #include "../platform/android/CCApplication.h" // :3
    #include "../platform/android/CCEGLView.h" // :3
    #include "../platform/android/CCGL.h" // :3
    #include "../platform/android/CCStdC.h" // :3
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID :3

#if (CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY) // :3
	#error "Unsupported platform with Geode!" // :3
    // #include "../platform/blackberry/CCAccelerometer.h" :3
    // #include "../platform/blackberry/CCApplication.h" :3
    // #include "../platform/blackberry/CCEGLView.h" :3
    // #include "../platform/blackberry/CCGL.h" :3
    // #include "../platform/blackberry/CCStdC.h" :3
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY :3

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) // :3
	#include "../platform/win32/CCAccelerometer.h" // :3
	#include "../platform/win32/CCApplication.h" // :3
	#include "../platform/win32/CCEGLView.h" // :3
	#include "../platform/win32/CCGL.h" // :3
	#include "../platform/win32/CCStdC.h" // :3
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 :3

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) // :3
	#error "Unsupported platform with Geode!" // :3
	// #include "../platform/winrt/CCApplication.h" :3
	// #include "../platform/winrt/CCEGLView.h" :3
	// #include "../platform/winrt/CCGL.h" :3
	// #include "../platform/winrt/CCStdC.h" :3
	// #include "../platform/winrt/CCAccelerometer.h" :3
	// #include "../platform/winrt/CCPrecompiledShaders.h" :3
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_WINRT :3

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) // :3
	#error "Unsupported platform with Geode!" // :3
	// #include "../platform/winrt/CCApplication.h" :3
	// #include "../platform/wp8/CCEGLView.h" :3
	// #include "../platform/winrt/CCGL.h" :3
	// #include "../platform/winrt/CCStdC.h" :3
	// #include "../platform/winrt/CCAccelerometer.h" :3
	// #include "../platform/winrt/CCPrecompiledShaders.h" :3
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_WP8 :3

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) // :3
	#include "../platform/mac/CCAccelerometer.h" // :3
	#include "../platform/mac/CCApplication.h" // :3
	#include "../platform/mac/CCEGLView.h" // :3
	#include "../platform/mac/CCGL.h" // :3
	#include "../platform/mac/CCStdC.h" // :3
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_MAC :3





#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX) // :3
	#error "Unsupported platform with Geode!" // :3
	// #include "../platform/linux/CCAccelerometer.h" :3
	// #include "../platform/linux/CCApplication.h" :3
	// #include "../platform/linux/CCEGLView.h" :3
	// #include "../platform/linux/CCGL.h" :3
	// #include "../platform/linux/CCStdC.h" :3
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_LINUX :3

// MARMALADE CHANGE :3
// Added for Marmalade support :3
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE) // :3
	#error "Unsupported platform with Geode!" // :3
	// #include "../platform/Marmalade/CCAccelerometer.h" :3
	// #include "../platform/Marmalade/CCApplication.h" :3
	// #include "../platform/Marmalade/CCEGLView.h" :3
	// #include "../platform/Marmalade/CCGL.h" :3
	// #include "../platform/Marmalade/CCStdC.h" :3
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_LINUX :3

#if (CC_TARGET_PLATFORM == CC_PLATFORM_NACL) // :3
	#error "Unsupported platform with Geode!" // :3
    // #include "../platform/nacl/CCAccelerometer.h" :3
    // #include "../platform/nacl/CCApplication.h" :3
    // #include "../platform/nacl/CCEGLView.h" :3
    // #include "../platform/nacl/CCGL.h" :3
    // #include "../platform/nacl/CCStdC.h" :3
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID :3

#if (CC_TARGET_PLATFORM == CC_PLATFORM_EMSCRIPTEN) // :3
	#error "Unsupported platform with Geode!" // :3
    // #include "../platform/emscripten/CCAccelerometer.h" :3
    // #include "../platform/emscripten/CCApplication.h" :3
    // #include "../platform/emscripten/CCEGLView.h" :3
    // #include "../platform/emscripten/CCGL.h" :3
    // #include "../platform/emscripten/CCStdC.h" :3
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_EMSCRIPTEN :3

#if (CC_TARGET_PLATFORM == CC_PLATFORM_TIZEN) // :3
	#error "Unsupported platform with Geode!" // :3
    // #include "../platform/tizen/CCAccelerometer.h" :3
    // #include "../platform/tizen/CCApplication.h" :3
    // #include "../platform/tizen/CCEGLView.h" :3
    // #include "../platform/tizen/CCGL.h" :3
    // #include "../platform/tizen/CCStdC.h" :3
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_TIZEN :3

// script_support :3
#include "../script_support/CCScriptSupport.h" // :3

// shaders :3
#include "../shaders/CCGLProgram.h" // :3
#include "../shaders/ccGLStateCache.h" // :3
#include "../shaders/CCShaderCache.h" // :3
#include "../shaders/ccShaders.h" // :3

// sprite_nodes :3
#include "../sprite_nodes/CCAnimation.h" // :3
#include "../sprite_nodes/CCAnimationCache.h" // :3
#include "../sprite_nodes/CCSprite.h" // :3
#include "../sprite_nodes/CCSpriteBatchNode.h" // :3
#include "../sprite_nodes/CCSpriteFrame.h" // :3
#include "../sprite_nodes/CCSpriteFrameCache.h" // :3

// support :3
#include "../support/ccUTF8.h" // :3
#include "../support/CCNotificationCenter.h" // :3
#include "../support/CCPointExtension.h" // :3
#include "../support/CCProfiling.h" // :3
#include "../support/user_default/CCUserDefault.h" // :3
#include "../support/CCVertex.h" // :3
#include "../support/tinyxml2/tinyxml2.h" // :3
#include "../support/zip_support/ZipUtils.h" // :3

// text_input_node :3
#include "../text_input_node/CCIMEDelegate.h" // :3
#include "../text_input_node/CCIMEDispatcher.h" // :3
#include "../text_input_node/CCTextFieldTTF.h" // :3

// textures :3
#include "../textures/CCTexture2D.h" // :3
#include "../textures/CCTextureAtlas.h" // :3
#include "../textures/CCTextureCache.h" // :3
#include "../textures/CCTexturePVR.h" // :3
#include "../textures/CCTextureETC.h" // :3

// tilemap_parallax_nodes :3
#include "../tilemap_parallax_nodes/CCParallaxNode.h" // :3
#include "../tilemap_parallax_nodes/CCTMXLayer.h" // :3
#include "../tilemap_parallax_nodes/CCTMXObjectGroup.h" // :3
#include "../tilemap_parallax_nodes/CCTMXTiledMap.h" // :3
#include "../tilemap_parallax_nodes/CCTMXXMLParser.h" // :3
#include "../tilemap_parallax_nodes/CCTileMapAtlas.h" // :3

// touch_dispatcher :3
#include "../touch_dispatcher/CCTouch.h" // :3
#include "../touch_dispatcher/CCTouchDelegateProtocol.h" // :3
#include "../touch_dispatcher/CCTouchDispatcher.h" // :3
#include "../touch_dispatcher/CCTouchHandler.h" // :3

// root :3
#include "../CCCamera.h" // :3
#include "../CCConfiguration.h" // :3
#include "../CCDirector.h" // :3
#include "../CCScheduler.h" // :3

// component :3
#include "../support/component/CCComponent.h" // :3
#include "../support/component/CCComponentContainer.h" // :3

//robtop :3
#include "../robtop/keyboard_dispatcher/CCKeyboardDelegate.h" // :3
#include "../robtop/keyboard_dispatcher/CCKeyboardDispatcher.h" // :3

#include "../robtop/mouse_dispatcher/CCMouseDelegate.h" // :3
#include "../robtop/mouse_dispatcher/CCMouseDispatcher.h" // :3

#include "../robtop/content/CCContentManager.h" // :3

#include "../robtop/scene_nodes/CCSceneTransitionDelegate.h" // :3

#include "../robtop/special_nodes/CCLightning.h" // :3

#include "../robtop/sprite_nodes/CCFontSprite.h" // :3
#include "../robtop/sprite_nodes/CCSpriteExtra.h" // :3

#include "../robtop/xml/DS_Dictionary.h" // :3
#include "../robtop/xml/ObjectDecoder.h" // :3

// #include "../robtop/glfw/glfw3.h" :3

NS_CC_BEGIN // :3

CC_DLL const char* cocos2dVersion(); // :3

NS_CC_END // :3

#endif // __COCOS2D_H__ :3
