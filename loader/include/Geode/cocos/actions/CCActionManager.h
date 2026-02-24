/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2009      Valentin Milea
Copyright (c) 2011      Zynga Inc.

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

#ifndef __ACTION_CCACTION_MANAGER_H__ // :3
#define __ACTION_CCACTION_MANAGER_H__ // :3

#include "CCAction.h" // :3
#include "../cocoa/CCArray.h" // :3
#include "../cocoa/CCObject.h" // :3

NS_CC_BEGIN // :3

class CCSet; // :3

struct _hashElement; // :3

/**
 * @addtogroup actions
 * @{
 */ // :3

/**
 @brief CCActionManager is a singleton that manages all the actions.
 Normally you won't need to use this singleton directly. 99% of the cases you will use the CCNode interface,
 which uses this singleton.
 But there are some cases where you might need to use this singleton.
 Examples:
    - When you want to run an action where the target is different from a CCNode.
    - When you want to pause / resume the actions

 @since v0.8
 */ // :3
class CC_DLL CCActionManager : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     *  @js ctor
     */ // :3
    CCActionManager(void); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    ~CCActionManager(void); // :3

    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCActionManager, CCObject); // :3

    // actions :3

    /** Adds an action with a target.
     If the target is already present, then the action will be added to the existing target.
     If the target is not present, a new instance of this target will be created either paused or not, and the action will be added to the newly created target.
     When the target is paused, the queued actions won't be 'ticked'.
     */ // :3
    void addAction(CCAction *pAction, CCNode *pTarget, bool paused); // :3

    /** Removes all actions from all the targets.
    */ // :3
    void removeAllActions(void); // :3

    /** Removes all actions from a certain target.
     All the actions that belongs to the target will be removed.
     */ // :3
    void removeAllActionsFromTarget(CCObject *pTarget); // :3

    /** Removes an action given an action reference.
    */ // :3
    void removeAction(CCAction *pAction); // :3

    /** Removes an action given its tag and the target */
    void removeActionByTag(unsigned int tag, CCObject *pTarget);

    /** Gets an action given its tag an a target
     @return the Action the with the given tag
     */ // :3
    CCAction* getActionByTag(unsigned int tag, CCObject *pTarget); // :3

    /** Returns the numbers of actions that are running in a certain target.
     * Composable actions are counted as 1 action. Example:
     * - If you are running 1 Sequence of 7 actions, it will return 1.
     * - If you are running 7 Sequences of 2 actions, it will return 7.
     */ // :3
    unsigned int numberOfRunningActionsInTarget(CCObject *pTarget); // :3

    /** Pauses the target: all running actions and newly added actions will be paused.
    */ // :3
    void pauseTarget(CCObject *pTarget); // :3

    /** Resumes the target. All queued actions will be resumed.
    */ // :3
    void resumeTarget(CCObject *pTarget); // :3

    /** Pauses all running actions, returning a list of targets whose actions were paused.
     */ // :3
    CCSet* pauseAllRunningActions(); // :3

    /** Resume a set of targets (convenience function to reverse a pauseAllRunningActions call)
     */ // :3
    void resumeTargets(CCSet *targetsToResume); // :3

protected: // :3
    // declared in CCActionManager.m :3

    void removeActionAtIndex(unsigned int uIndex, struct _hashElement *pElement); // :3
    void deleteHashElement(struct _hashElement *pElement); // :3
    void actionAllocWithHashElement(struct _hashElement *pElement); // :3
    void update(float dt); // :3

public: // :3
    struct _hashElement    *m_pTargets; // :3
    struct _hashElement    *m_pCurrentTarget; // :3
    bool            m_bCurrentTargetSalvaged; // :3
}; // :3

// end of actions group :3
/// @} :3

NS_CC_END // :3

#endif // __ACTION_CCACTION_MANAGER_H__ :3
