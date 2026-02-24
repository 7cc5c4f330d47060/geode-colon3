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

#ifndef __PLATFORM_CCNODE_H__ // :3
#define __PLATFORM_CCNODE_H__ // :3

#include "../include/ccMacros.h" // :3
#include "../cocoa/CCAffineTransform.h" // :3
#include "../cocoa/CCArray.h" // :3
#include "../platform/CCGL.h" // :3
#include "../shaders/ccGLStateCache.h" // :3
#include "../shaders/CCGLProgram.h" // :3
#include "../kazmath/include/kazmath/kazmath.h" // :3
#include "../script_support/CCScriptSupport.h" // :3
#include "../include/CCProtocols.h" // :3
#include <Geode/loader/Event.hpp> // :3
#include <Geode/utils/casts.hpp> // :3
#include <Geode/utils/function.hpp> // :3
#include <Geode/utils/ZStringView.hpp> // :3

namespace geode { // :3
    class Layout; // :3
    class LayoutOptions; // :3
    enum class Anchor; // :3
} // :3

NS_CC_BEGIN // :3

class CCCamera; // :3
class CCGridBase; // :3
class CCPoint; // :3
class CCTouch; // :3
class CCAction; // :3
class CCRGBAProtocol; // :3
class CCLabelProtocol; // :3
class CCScheduler; // :3
class CCActionManager; // :3
class CCComponent; // :3
class CCDictionary; // :3
class CCComponentContainer; // :3
class CCKeyboardDispatcher; // :3

/**
 * @addtogroup base_nodes
 * @{
 */ // :3

enum { // :3
    kCCNodeTagInvalid = -1, // :3
}; // :3

enum { // :3
    kCCNodeOnEnter, // :3
    kCCNodeOnExit, // :3
    kCCNodeOnEnterTransitionDidFinish, // :3
    kCCNodeOnExitTransitionDidStart, // :3
    kCCNodeOnCleanup // :3
}; // :3

/** @brief CCNode is the main element. Anything that gets drawn or contains things that get drawn is a CCNode.
 The most popular CCNodes are: CCScene, CCLayer, CCSprite, CCMenu.

 The main features of a CCNode are:
 - They can contain other CCNode nodes (addChild, getChildByTag, removeChild, etc)
 - They can schedule periodic callback (schedule, unschedule, etc)
 - They can execute actions (runAction, stopAction, etc)

 Some CCNode nodes provide extra functionality for them or their children.

 Subclassing a CCNode usually means (one/all) of:
 - overriding init to initialize resources and schedule callbacks
 - create callbacks to handle the advancement of time
 - overriding draw to render the node

 Features of CCNode:
 - position
 - scale (x, y)
 - rotation (in degrees, clockwise)
 - CCCamera (an interface to gluLookAt )
 - CCGridBase (to do mesh transformations)
 - anchor point
 - size
 - visible
 - z-order
 - openGL z position

 Default values:
 - rotation: 0
 - position: (x=0,y=0)
 - scale: (x=1,y=1)
 - contentSize: (x=0,y=0)
 - anchorPoint: (x=0,y=0)

 Limitations:
 - A CCNode is a "void" object. It doesn't have a texture

 Order in transformations with grid disabled
 -# The node will be translated (position)
 -# The node will be rotated (rotation)
 -# The node will be scaled (scale)
 -# The node will be moved according to the camera values (camera)

 Order in transformations with grid enabled
 -# The node will be translated (position)
 -# The node will be rotated (rotation)
 -# The node will be scaled (scale)
 -# The grid will capture the screen
 -# The node will be moved according to the camera values (camera)
 -# The grid will render the captured screen

 Camera:
 - Each node has a camera. By default it points to the center of the CCNode.
 */ // :3

class CC_DLL CCNode : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /// @{ :3
    /// @name Constructor, Distructor and Initializers :3

    /**
     * Default constructor
     * @js ctor
     */ // :3
    CCNode(void); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCNode, CCObject) // :3

    /**
     * Default destructor
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCNode(void); // :3

    /**
     *  Initializes the instance of CCNode
     *  @return Whether the initialization was successful.
     */ // :3
    virtual bool init(); // :3
	/**
     * Allocates and initializes a node.
     * @return A initialized node which is marked as "autorelease".
     */ // :3
    static CCNode * create(void); // :3

    /**
     * Gets the description string. It makes debugging easier.
     * @return A string terminated with '\0'
     * @js NA
     */ // :3
    const char* description(void); // :3

    /// @} end of initializers :3



    /// @{ :3
    /// @name Setters & Getters for Graphic Peroperties :3

    /**
     * Sets the Z order which stands for the drawing order, and reorder this node in its parent's children array.
     *
     * The Z order of node is relative to its "brothers": children of the same parent.
     * It's nothing to do with OpenGL's z vertex. This one only affects the draw order of nodes in cocos2d.
     * The larger number it is, the later this node will be drawn in each message loop.
     * Please refer to setVertexZ(float) for the difference.
     *
     * @param nZOrder   Z order of this node.
     */ // :3
    virtual void setZOrder(int zOrder); // :3
    /**
     * Sets the z order which stands for the drawing order
     *
     * This is an internal method. Don't call it outside the framework.
     * The difference between setZOrder(int) and _setOrder(int) is:
     * - _setZOrder(int) is a pure setter for m_nZOrder memeber variable
     * - setZOrder(int) firstly changes m_nZOrder, then recorder this node in its parent's chilren array.
     */ // :3
    virtual void _setZOrder(int z); // :3
    /**
     * Gets the Z order of this node.
     *
     * @see setZOrder(int)
     *
     * @return The Z order.
     */ // :3
    virtual int getZOrder(); // :3


    /**
     * Sets the real OpenGL Z vertex.
     *
     * Differences between openGL Z vertex and cocos2d Z order:
     * - OpenGL Z modifies the Z vertex, and not the Z order in the relation between parent-children
     * - OpenGL Z might require to set 2D projection
     * - cocos2d Z order works OK if all the nodes uses the same openGL Z vertex. eg: vertexZ = 0
     *
     * @warning Use it at your own risk since it might break the cocos2d parent-children z order
     *
     * @param fVertexZ  OpenGL Z vertex of this node.
     */ // :3
    virtual void setVertexZ(float vertexZ); // :3
    /**
     * Gets OpenGL Z vertex of this node.
     *
     * @see setVertexZ(float)
     *
     * @return OpenGL Z vertex of this node
     */ // :3
    virtual float getVertexZ(); // :3


    /**
     * Changes the scale factor on X axis of this node
     *
     * The deafult value is 1.0 if you haven't changed it before
     *
     * @param fScaleX   The scale factor on X axis.
     */ // :3
    virtual void setScaleX(float fScaleX); // :3
    /**
     * Returns the scale factor on X axis of this node
     *
     * @see setScaleX(float)
     *
     * @return The scale factor on X axis.
     */ // :3
    virtual float getScaleX(); // :3


    /**
     * Changes the scale factor on Y axis of this node
     *
     * The Default value is 1.0 if you haven't changed it before.
     *
     * @param fScaleY   The scale factor on Y axis.
     */ // :3
    virtual void setScaleY(float fScaleY); // :3
    /**
     * Returns the scale factor on Y axis of this node
     *
     * @see setScaleY(float)
     *
     * @return The scale factor on Y axis.
     */ // :3
    virtual float getScaleY(); // :3


    /**
     * Changes both X and Y scale factor of the node.
     *
     * 1.0 is the default scale factor. It modifies the X and Y scale at the same time.
     *
     * @param scale     The scale factor for both X and Y axis.
     */ // :3
    virtual void setScale(float scale); // :3
    /**
     * Gets the scale factor of the node,  when X and Y have the same scale factor.
     *
     * @warning Assert when m_fScaleX != m_fScaleY.
     * @see setScale(float)
     *
     * @return The scale factor of the node.
     */ // :3
    virtual float getScale(); // :3


    /**
     * Changes both X and Y scale factor of the node.
     *
     * 1.0 is the default scale factor. It modifies the X and Y scale at the same time.
     *
     * @param fScaleX     The scale factor on X axis.
     * @param fScaleY     The scale factor on Y axis.
     */ // :3
    virtual void setScale(float fScaleX,float fScaleY); // :3


    /**
     * Changes the position (x,y) of the node in OpenGL coordinates
     *
     * Usually we use ccp(x,y) to compose CCPoint object.
     * The original point (0,0) is at the left-bottom corner of screen.
     * For example, this codesnip sets the node in the center of screen.
     * @code
     * CCSize size = CCDirector::sharedDirector()->getWinSize();
     * node->setPosition( ccp(size.width/2, size.height/2) )
     * @endcode
     *
     * @param position  The position (x,y) of the node in OpenGL coordinates
     * @js NA
     */ // :3
    virtual void setPosition(const CCPoint &position); // :3
    /**
     * Gets the position (x,y) of the node in OpenGL coordinates
     *
     * @see setPosition(const CCPoint&)
     *
     * @return The position (x,y) of the node in OpenGL coordinates
     */ // :3
    virtual const CCPoint& getPosition(); // :3
    /**
     * Sets position in a more efficient way.
     *
     * Passing two numbers (x,y) is much efficient than passing CCPoint object.
     * This method is binded to lua and javascript.
     * Passing a number is 10 times faster than passing a object from lua to c++
     *
     * @code
     * // sample code in lua
     * local pos  = node::getPosition()  -- returns CCPoint object from C++
     * node:setPosition(x, y)            -- pass x, y coordinate to C++
     * @endcode
     *
     * @param x     X coordinate for position
     * @param y     Y coordinate for position
     * @js NA
     */ // :3
    virtual void setPosition(float x, float y); // :3
    /**
     * Gets position in a more efficient way, returns two number instead of a CCPoint object
     *
     * @see setPosition(float, float)
     */ // :3
    virtual void getPosition(float* x, float* y); // :3
    /**
     * Gets/Sets x or y coordinate individually for position.
     * These methods are used in Lua and Javascript Bindings
     */ // :3
    virtual void  setPositionX(float x); // :3
    virtual float getPositionX(void); // :3
    virtual void  setPositionY(float y); // :3
    virtual float getPositionY(void); // :3


    /**
     * Changes the X skew angle of the node in degrees.
     *
     * This angle describes the shear distortion in the X direction.
     * Thus, it is the angle between the Y axis and the left edge of the shape
     * The default skewX angle is 0. Positive values distort the node in a CW direction.
     *
     * @param fSkewX The X skew angle of the node in degrees.
     */ // :3
    virtual void setSkewX(float fSkewX); // :3
    /**
     * Returns the X skew angle of the node in degrees.
     *
     * @see setSkewX(float)
     *
     * @return The X skew angle of the node in degrees.
     */ // :3
    virtual float getSkewX(); // :3


    /**
     * Changes the Y skew angle of the node in degrees.
     *
     * This angle describes the shear distortion in the Y direction.
     * Thus, it is the angle between the X axis and the bottom edge of the shape
     * The default skewY angle is 0. Positive values distort the node in a CCW direction.
     *
     * @param fSkewY    The Y skew angle of the node in degrees.
     */ // :3
    virtual void setSkewY(float fSkewY); // :3
    /**
     * Returns the Y skew angle of the node in degrees.
     *
     * @see setSkewY(float)
     *
     * @return The Y skew angle of the node in degrees.
     */ // :3
    virtual float getSkewY(); // :3


    /**
     * Sets the anchor point in percent.
     *
     * anchorPoint is the point around which all transformations and positioning manipulations take place.
     * It's like a pin in the node where it is "attached" to its parent.
     * The anchorPoint is normalized, like a percentage. (0,0) means the bottom-left corner and (1,1) means the top-right corner.
     * But you can use values higher than (1,1) and lower than (0,0) too.
     * The default anchorPoint is (0.5,0.5), so it starts in the center of the node.
     *
     * @param anchorPoint   The anchor point of node.
     */ // :3
    virtual void setAnchorPoint(const CCPoint& anchorPoint); // :3
    /**
     * Returns the anchor point in percent.
     *
     * @see setAnchorPoint(const CCPoint&)
     *
     * @return The anchor point of node.
     */ // :3
    virtual const CCPoint& getAnchorPoint(); // :3
    /**
     * Returns the anchorPoint in absolute pixels.
     *
     * @warning You can only read it. If you wish to modify it, use anchorPoint instead.
     * @see getAnchorPoint()
     *
     * @return The anchor point in absolute pixels.
     */ // :3
    virtual const CCPoint& getAnchorPointInPoints(); // :3

    /**
     * Sets the untransformed size of the node.
     *
     * The contentSize remains the same no matter the node is scaled or rotated.
     * All nodes has a size. Layer and Scene has the same size of the screen.
     *
     * @param contentSize   The untransformed size of the node.
     */ // :3
    virtual void setContentSize(const CCSize& contentSize); // :3
    /**
     * Returns the untransformed size of the node.
     *
     * @see setContentSize(const CCSize&)
     *
     * @return The untransformed size of the node.
     */ // :3
    virtual const CCSize& getContentSize() const; // :3

    // @note RobTop Addition :3
    virtual CCSize getScaledContentSize(void); // :3

    /**
     * Sets whether the node is visible
     *
     * The default value is true, a node is default to visible
     *
     * @param visible   true if the node is visible, false if the node is hidden.
     */ // :3
    virtual void setVisible(bool visible); // :3
    /**
     * Determines if the node is visible
     *
     * @see setVisible(bool)
     *
     * @return true if the node is visible, false if the node is hidden.
     */ // :3
    virtual bool isVisible(); // :3


    /**
     * Sets the rotation (angle) of the node in degrees.
     *
     * 0 is the default rotation angle.
     * Positive values rotate node clockwise, and negative values for anti-clockwise.
     *
     * @param fRotation     The roration of the node in degrees.
     */ // :3
    virtual void setRotation(float fRotation); // :3
    /**
     * Returns the rotation of the node in degrees.
     *
     * @see setRotation(float)
     *
     * @return The rotation of the node in degrees.
     */ // :3
    virtual float getRotation(); // :3


    /**
     * Sets the X rotation (angle) of the node in degrees which performs a horizontal rotational skew.
     *
     * 0 is the default rotation angle.
     * Positive values rotate node clockwise, and negative values for anti-clockwise.
     *
     * @param fRotationX    The X rotation in degrees which performs a horizontal rotational skew.
     */ // :3
    virtual void setRotationX(float fRotaionX); // :3
    /**
     * Gets the X rotation (angle) of the node in degrees which performs a horizontal rotation skew.
     *
     * @see setRotationX(float)
     *
     * @return The X rotation in degrees.
     */ // :3
    virtual float getRotationX(); // :3


    /**
     * Sets the Y rotation (angle) of the node in degrees which performs a vertical rotational skew.
     *
     * 0 is the default rotation angle.
     * Positive values rotate node clockwise, and negative values for anti-clockwise.
     *
     * @param fRotationY    The Y rotation in degrees.
     */ // :3
    virtual void setRotationY(float fRotationY); // :3
    /**
     * Gets the Y rotation (angle) of the node in degrees which performs a vertical rotational skew.
     *
     * @see setRotationY(float)
     *
     * @return The Y rotation in degrees.
     */ // :3
    virtual float getRotationY(); // :3


    /**
     * Sets the arrival order when this node has a same ZOrder with other children.
     *
     * A node which called addChild subsequently will take a larger arrival order,
     * If two children have the same Z order, the child with larger arrival order will be drawn later.
     *
     * @warning This method is used internally for zOrder sorting, don't change this manually
     *
     * @param uOrderOfArrival   The arrival order.
     */ // :3
    virtual void setOrderOfArrival(unsigned int uOrderOfArrival); // :3
    /**
     * Returns the arrival order, indecates which children is added previously.
     *
     * @see setOrderOfArrival(unsigned int)
     *
     * @return The arrival order.
     */ // :3
    virtual unsigned int getOrderOfArrival(); // :3


    /**
     * Sets the state of OpenGL server side.
     *
     * @param glServerState     The state of OpenGL server side.
     * @js NA
     */ // :3
    virtual void setGLServerState(ccGLServerState glServerState); // :3
    /**
     * Returns the state of OpenGL server side.
     *
     * @return The state of OpenGL server side.
     * @js NA
     */ // :3
    virtual ccGLServerState getGLServerState(); // :3


    /**
     * Sets whether the anchor point will be (0,0) when you position this node.
     *
     * This is an internal method, only used by CCLayer and CCScene. Don't call it outside framework.
     * The default value is false, while in CCLayer and CCScene are true
     *
     * @param ignore    true if anchor point will be (0,0) when you position this node
     * @todo This method shoud be renamed as setIgnoreAnchorPointForPosition(bool) or something with "set"
     */ // :3
    virtual void ignoreAnchorPointForPosition(bool ignore); // :3
    /**
     * Gets whether the anchor point will be (0,0) when you position this node.
     *
     * @see ignoreAnchorPointForPosition(bool)
     *
     * @return true if the anchor point will be (0,0) when you position this node.
     */ // :3
    virtual bool isIgnoreAnchorPointForPosition(); // :3

    /// @}  end of Setters & Getters for Graphic Peroperties :3


    /// @{ :3
    /// @name Children and Parent :3

    /**
     * Adds a child to the container with z-order as 0.
     *
     * If the child is added to a 'running' node, then 'onEnter' and 'onEnterTransitionDidFinish' will be called immediately.
     *
     * @param child A child node
     */ // :3
    virtual void addChild(CCNode * child); // :3
    /**
     * Adds a child to the container with a z-order
     *
     * If the child is added to a 'running' node, then 'onEnter' and 'onEnterTransitionDidFinish' will be called immediately.
     *
     * @param child     A child node
     * @param zOrder    Z order for drawing priority. Please refer to setZOrder(int)
     */ // :3
    virtual void addChild(CCNode * child, int zOrder); // :3
    /**
     * Adds a child to the container with z order and tag
     *
     * If the child is added to a 'running' node, then 'onEnter' and 'onEnterTransitionDidFinish' will be called immediately.
     *
     * @param child     A child node
     * @param zOrder    Z order for drawing priority. Please refer to setZOrder(int)
     * @param tag       A interger to identify the node easily. Please refer to setTag(int)
     */ // :3
    virtual void addChild(CCNode* child, int zOrder, int tag); // :3
    /**
     * Gets a child from the container with its tag
     *
     * @param tag   An identifier to find the child node.
     *
     * @return a CCNode object whose tag equals to the input parameter
     */ // :3
    virtual CCNode * getChildByTag(int tag); // :3
    /**
     * Return an array of children
     *
     * Composing a "tree" structure is a very important feature of CCNode
     * @example
     * // Here's a sample code of traversing children array:
     * for (auto child : parent->getChildrenExt()) {
     *     child->setPosition(0, 0);
     * }
     * // This sample code traverses all children nodes, and set theie position to (0,0)
     * @returns An array of children
     */ // :3
    virtual CCArray* getChildren(); // :3

    /*
     * Like `getChildren()`, but returns a `CCArrayExt<CCNode>` instead.
     * You must include `<Geode/utils/cocos.hpp>` to use this, otherwise it won't compile
    */ // :3
    template <typename T = CCNode, typename PleaseDontChangeMe = void> // :3
    inline auto getChildrenExt() { // :3
        // CCArrayExt is defined in geode/utils/cocos.hpp, which we cannot include due to circular includes. :3
        // This is an incredibly hacky way to still be able to use the type :3

        using CCArrayExt = geode::CCArrayExtCheck<T, PleaseDontChangeMe>::type; // :3
        static_assert(!std::is_void_v<CCArrayExt>, "Please include <Geode/utils/cocos.hpp> to use getChildrenExt()"); // :3

        return CCArrayExt(getChildren()); // :3
    } // :3

    /**
     * Get the amount of children.
     *
     * @return The amount of children.
     */ // :3
    virtual unsigned int getChildrenCount(void) const; // :3

    /**
     * Sets the parent node
     *
     * @param parent    A pointer to the parnet node
     */ // :3
    virtual void setParent(CCNode* parent); // :3
    /**
     * Returns a pointer to the parent node
     *
     * @see setParent(CCNode*)
     *
     * @returns A pointer to the parnet node
     */ // :3
    virtual CCNode* getParent(); // :3


    ////// REMOVES ////// :3

    /**
     * Removes this node itself from its parent node with a cleanup.
     * If the node orphan, then nothing happens.
     * @see removeFromParentAndCleanup(bool)
     */ // :3
    virtual void removeFromParent(); // :3
    /**
     * Removes this node itself from its parent node.
     * If the node orphan, then nothing happens.
     * @param cleanup   true if all actions and callbacks on this node should be removed, false otherwise.
     * @js removeFromParent
     */ // :3
    virtual void removeFromParentAndCleanup(bool cleanup); // :3

    // @note RobTop Addition :3
    virtual void removeMeAndCleanup(void); // :3

    /**
     * Removes a child from the container with a cleanup
     *
     * @see removeChild(CCNode, bool)
     *
     * @param child     The child node which will be removed.
     */ // :3
    virtual void removeChild(CCNode* child); // :3
    /**
     * Removes a child from the container. It will also cleanup all running actions depending on the cleanup parameter.
     *
     * @param child     The child node which will be removed.
     * @param cleanup   true if all running actions and callbacks on the child node will be cleanup, false otherwise.
     */ // :3
    virtual void removeChild(CCNode* child, bool cleanup); // :3
    /**
     * Removes a child from the container by tag value with a cleanup.
     *
     * @see removeChildByTag(int, bool)
     *
     * @param tag       An interger number that identifies a child node
     */ // :3
    virtual void removeChildByTag(int tag); // :3
    /**
     * Removes a child from the container by tag value. It will also cleanup all running actions depending on the cleanup parameter
     *
     * @param tag       An interger number that identifies a child node
     * @param cleanup   true if all running actions and callbacks on the child node will be cleanup, false otherwise.
     */ // :3
    virtual void removeChildByTag(int tag, bool cleanup); // :3
    /**
     * Removes all children from the container with a cleanup.
     *
     * @see removeAllChildrenWithCleanup(bool)
     */ // :3
    virtual void removeAllChildren(); // :3
    /**
     * Removes all children from the container, and do a cleanup to all running actions depending on the cleanup parameter.
     *
     * @param cleanup   true if all running actions on all children nodes should be cleanup, false oterwise.
     * @js removeAllChildren
     */ // :3
    virtual void removeAllChildrenWithCleanup(bool cleanup); // :3

    /**
     * Reorders a child according to a new z value.
     *
     * @param child     An already added child node. It MUST be already added.
     * @param zOrder    Z order for drawing priority. Please refer to setZOrder(int)
     */ // :3
    virtual void reorderChild(CCNode * child, int zOrder); // :3

    /**
     * Sorts the children array once before drawing, instead of every time when a child is added or reordered.
     * This appraoch can improves the performance massively.
     * @note Don't call this manually unless a child added needs to be removed in the same frame
     */ // :3
    virtual void sortAllChildren(); // :3

    /// @} end of Children and Parent :3



    /// @{ :3
    /// @name Grid object for effects :3

    /**
     * Returns a grid object that is used when applying effects
     *
     * @return A CCGrid object that is used when applying effects
     * @js NA
     */ // :3
    virtual CCGridBase* getGrid(); // :3
    /**
     * Changes a grid object that is used when applying effects
     *
     * @param A CCGrid object that is used when applying effects
     */ // :3
    virtual void setGrid(CCGridBase *pGrid); // :3

    /// @} end of Grid :3


    /// @{ :3
    /// @name Tag & User data :3

    /**
     * Returns a tag that is used to identify the node easily.
     *
     * You can set tags to node then identify them easily.
     * @example
     * #define TAG_PLAYER  1
     * #define TAG_MONSTER 2
     * #define TAG_BOSS    3
     * // set tags
     * node1->setTag(TAG_PLAYER);
     * node2->setTag(TAG_MONSTER);
     * node3->setTag(TAG_BOSS);
     * parent->addChild(node1);
     * parent->addChild(node2);
     * parent->addChild(node3);
     * // identify by tags
     * CCNode* node = NULL;
     * CCARRAY_FOREACH(parent->getChildren(), node)
     * {
     *     switch(node->getTag())
     *     {
     *         case TAG_PLAYER:
     *             break;
     *         case TAG_MONSTER:
     *             break;
     *         case TAG_BOSS:
     *             break;
     *     }
     * }
     * @returns A interger that identifies the node.
     */ // :3
    // Robtop Removal :3
    // virtual int getTag() const; :3
    /**
     * Changes the tag that is used to identify the node easily.
     *
     * Please refer to getTag for the sample code.
     *
     * @param A interger that indentifies the node.
     */ // :3
    // Robtop Removal :3
    // virtual void setTag(int nTag); :3

    /**
     * Returns a custom user data pointer
     *
     * You can set everything in UserData pointer, a data block, a structure or an object.
     *
     * @return A custom user data pointer
     * @js NA
     */ // :3
    virtual void* getUserData(); // :3
    /**
     * Sets a custom user data pointer
     *
     * You can set everything in UserData pointer, a data block, a structure or an object, etc.
     * @warning Don't forget to release the memroy manually,
     *          especially before you change this data pointer, and before this node is autoreleased.
     *
     * @return A custom user data pointer
     * @js NA
     */ // :3
    virtual void setUserData(void *pUserData); // :3

    /**
     * Returns a user assigned CCObject
     *
     * Similar to userData, but instead of holding a void* it holds an object
     *
     * @return A user assigned CCObject
     * @js NA
     */ // :3
    virtual CCObject* getUserObject(); // :3
    /**
     * Returns a user assigned CCObject
     *
     * Similar to UserData, but instead of holding a void* it holds an object.
     * The UserObject will be retained once in this method,
     * and the previous UserObject (if existed) will be relese.
     * The UserObject will be released in CCNode's destructure.
     *
     * @note In Geode, this actually sets the user object with the ID ""
     * (empty string)
     *
     * @param A user assigned CCObject
     */ // :3
    virtual void setUserObject(CCObject *pUserObject); // :3

    /**
     * Set a user-assigned CCObject with a specific ID. This allows nodes to
     * have multiple user objects. Objects should be prefixed with the mod ID.
     * Assigning a null removes the user object with the ID
     *
     * @note Geode addition
     */ // :3
    GEODE_DLL void setUserObject(std::string id, CCObject* object); // :3

    /**
     * Get a user-assigned CCObject with the specific ID
     *
     * @note Geode addition
     */ // :3
    GEODE_DLL CCObject* getUserObject(std::string_view id); // :3

    /**
     * Tethers a CCObject to the lifetime of this node.
     * @note Geode addition
     */ // :3
    GEODE_DLL void addTether(CCObject* object); // :3

    /**
     * Removes a tethered CCObject from this node.
     * @note Geode addition
     */ // :3
    GEODE_DLL void removeTether(CCObject* object); // :3
     // :3
    /**
     * Set (or clear) a boolean flag on this node with a specific ID.
     *
     * @note Geode addition
     */ // :3
    GEODE_DLL void setUserFlag(std::string id, bool state = true); // :3

    /**
     * Get whether this node has the given flag set.
     *
     * @note Geode addition
     */ // :3
    GEODE_DLL bool getUserFlag(std::string_view id); // :3

    /// @} end of Tag & User Data :3

private: // :3
    friend class geode::modifier::FieldContainer; // :3

    GEODE_DLL geode::modifier::FieldContainer* getFieldContainer(char const* forClass); // :3
    GEODE_DLL geode::comm::ListenerHandle* addEventListenerInternal( // :3
        std::string id, // :3
        geode::comm::ListenerHandle handle // :3
    ); // :3

public: // :3
    /**
     * Get the string ID of this node
     * @returns The ID, or an empty string if the node has no ID.
     * @note Geode addition
     */ // :3
    GEODE_DLL geode::ZStringView getID(); // :3
    /**
     * Set the string ID of this node. String IDs are a Geode addition
     * that are much safer to use to get nodes than absolute indexes
     * @param id The ID of the node, recommended to be in kebab case
     * without any spaces or uppercase letters. If the node is added
     * by a mod, use the _spr literal to append the mod ID to it
     * @note Geode addition
     */ // :3
    GEODE_DLL void setID(std::string id); // :3

    /**
     * Get a child by its string ID
     * @param id ID of the child
     * @returns The child, or nullptr if none was found
     * @note Geode addition
     */ // :3
    GEODE_DLL CCNode* getChildByID(std::string_view id); // :3

    /**
     * Get a child by its string ID. Recursively searches all the children
     * @param id ID of the child
     * @returns The child, or nullptr if none was found
     * @note Geode addition
     */ // :3
    GEODE_DLL CCNode* getChildByIDRecursive(std::string_view id); // :3

    /**
     * Get a child based on a query. Searches the child tree for a matching
     * child. The query currently only supports the following features:
     *  - `node-id`: Match a node with a specific ID
     *  - `node-id-1 node-id-2`: Match a descendant (possibly not immediate)
     *    child of a node with a specific ID
     *  - `node-id-1 > node-id-2`: Match the immediate child of a node with a
     *    specific ID
     * For example, the query "my-layer button-menu > mod.id/epic-button" is
     * equivalent to `getChildByIDRecursive("my-layer")
     * ->getChildByIDRecursive("button-menu")
     * ->getChildByID("mod.id/epic-button")`
     * @returns The first matching node, or nullptr if none was found
     */ // :3
    GEODE_DLL CCNode* querySelector(std::string_view query); // :3

    /**
     * Removes a child from the container by its ID.
     * @param id The ID of the node
     * @note Geode addition
     */ // :3
    GEODE_DLL void removeChildByID(std::string_view id); // :3

    /**
     * Add a child before a specified existing child
     * @param child The node to add. The node may not be a child of another
     * node already
     * @param before The child the node is added before of. If this is null or
     * not a child of this node, the new child will be placed at the start of the
     * child list
     * @note Geode addition
     */ // :3
    GEODE_DLL void insertBefore(CCNode* child, CCNode* before); // :3

    /**
     * Add a child after an specified existing child
     * @param child The node to add. The node may not be a child of another
     * node already
     * @param after The child the node is added after of. If this is null or
     * not a child of this node, the new child will be placed at the end of the
     * child list
     * @note Geode addition
     */ // :3
    GEODE_DLL void insertAfter(CCNode* child, CCNode* after); // :3

    /**
     * Check if this node's parent or its parents' parent is the given node
     * @param ancestor The node whose child or subchild this node should be. If
     * nullptr, returns true if the node is in the current scene, otherwise
     * false.
     * @returns True if ancestor is an ancestor of this node
     * @note Geode addition
     */ // :3
    GEODE_DLL bool hasAncestor(CCNode* ancestor); // :3

    /**
     * Set the Layout for this node. Used to automatically position children,
     * based on the selected layout. In order to apply the layout after a child
     * has been added, call updateLayout
     * @param layout Layout to set to this node
     * @param apply Whether to call updateLayout now or not
     * @param respectAnchor If true, if the target node is
     * isIgnoreAnchorPointForPosition, then it is set to false and the children
     * are automatically moved to match where they should be positioned.
     * Visually, this should result in no difference; however, when dealing with
     * CCLayers / CCMenus, this will change where the children are located
     * @note Geode addition
     */ // :3
    GEODE_DLL void setLayout(geode::Layout* layout, bool apply = true, bool respectAnchor = true); // :3
    /**
     * Get the Layout for this node
     * @returns The current layout, or nullptr if no layout is set
     * @note Geode addition
     */ // :3
    GEODE_DLL geode::Layout* getLayout(); // :3
    /**
     * Update the layout of this node using the current Layout. If no layout is
     * set, nothing happens
     * @note Geode addition
     */ // :3
    GEODE_DLL void updateLayout(bool updateChildOrder = true); // :3
    /**
     * Set the layout options for this node. Layout options can be used to
     * control how this node is positioned in its parent's Layout, for example
     * setting the grow size for a flex layout
     * @param options The layout options
     * @param apply Whether to update the layout of the parent node
     * @note Geode addition
     */ // :3
    GEODE_DLL void setLayoutOptions(geode::LayoutOptions* options, bool apply = true); // :3
    /**
     * Get the layout options for this node
     * @returns The current layout options, or nullptr if no options are set
     * @note Geode addition
     */ // :3
    GEODE_DLL geode::LayoutOptions* getLayoutOptions(); // :3
    /**
     * Adds a child at an anchored position with an offset. The node is placed
     * in its parent where the anchor specifies, and then the offset is used to
     * relatively adjust the node's position
     * @param child The child to add
     * @param anchor Where the place the child relative to this node
     * @param offset Where to place the child relative to the anchor
     * @param useAnchorLayout If true, sets this node's layout to `AnchorLayout`
     * if no other layout is already specified
     * @note Geode addition
     */ // :3
    GEODE_DLL void addChildAtPosition(CCNode* child, geode::Anchor anchor, CCPoint const& offset = CCPointZero, bool useAnchorLayout = true); // :3
    /**
     * Adds a child at an anchored position with an offset. The node is placed
     * in its parent where the anchor specifies, and then the offset is used to
     * relatively adjust the node's position
     * @param child The child to add
     * @param anchor Where the place the child relative to this node
     * @param offset Where to place the child relative to the anchor
     * @param nodeAnchor The child's anchor position
     * @param useAnchorLayout If true, sets this node's layout to `AnchorLayout`
     * if no other layout is already specified
     * @note Geode addition
     */ // :3
    GEODE_DLL void addChildAtPosition( // :3
        CCNode* child, // :3
        geode::Anchor anchor, // :3
        CCPoint const& offset, // :3
        CCPoint const& nodeAnchor, // :3
        bool useAnchorLayout = true // :3
    ); // :3
    /**
     * Updates the anchored position of a child. Requires the child to already
     * have a parent; if the child already has AnchorLayoutOptions set, those
     * are updated, otherwise nothing is done
     * @param anchor Where the place the child relative to its parent
     * @param offset Where to place the child relative to the anchor
     * @note Geode addition
     */ // :3
    GEODE_DLL void updateAnchoredPosition(geode::Anchor anchor, CCPoint const& offset = CCPointZero); // :3
    /**
     * Updates the anchored position of a child. Requires the child to already
     * have a parent; if the child already has AnchorLayoutOptions set, those
     * are updated, otherwise nothing is done
     * @param anchor Where the place the child relative to its parent
     * @param offset Where to place the child relative to the anchor
     * @param nodeAnchor The child's anchor position
     * @note Geode addition
     */ // :3
    GEODE_DLL void updateAnchoredPosition( // :3
        geode::Anchor anchor, // :3
        CCPoint const& offset, // :3
        CCPoint const& nodeAnchor // :3
    ); // :3

    /**
     * Swap two children
     * @param first One of the nodes to swap
     * @param second One of the nodes to swap
     * @note Geode addition
     */ // :3
    GEODE_DLL void swapChildIndices(CCNode* first, CCNode* second); // :3

    /**
     * @note Make sure to set the scale first!
     * @note Geode addition
     */ // :3
    GEODE_DLL void setScaledContentSize(CCSize const& size); // :3
    // @note Geode addition :3
    GEODE_DLL void setContentWidth(float width); // :3
    // @note Geode addition :3
    GEODE_DLL void setContentHeight(float width); // :3
    // @note Geode addition :3
    GEODE_DLL float getContentWidth() const; // :3
    // @note Geode addition :3
    GEODE_DLL float getContentHeight() const; // :3
    // @note Geode addition :3
    GEODE_DLL float getScaledContentWidth() const; // :3
    // @note Geode addition :3
    GEODE_DLL float getScaledContentHeight() const; // :3

    template <class Event, class Callback> // :3
    geode::comm::ListenerHandle* addEventListener( // :3
        std::string_view id, // :3
        Event const& event, // :3
        Callback&& callback, // :3
        int priority = 0 // :3
    ) { // :3
        auto handle = event.listen(std::forward<Callback>(callback), priority); // :3
        return this->addEventListenerInternal(std::string(id), std::move(handle)); // :3
    } // :3
    template <class Event, class Callback> // :3
    geode::comm::ListenerHandle* addEventListener( // :3
        Event const& event, // :3
        Callback&& callback, // :3
        int priority = 0 // :3
    ) { // :3
        return this->addEventListener("", event, std::forward<Callback>(callback), priority); // :3
    } // :3
    GEODE_DLL void removeEventListener(geode::comm::ListenerHandle* handle); // :3
    GEODE_DLL void removeEventListener(std::string_view id); // :3
    GEODE_DLL geode::comm::ListenerHandle* getEventListener(std::string_view id); // :3
    GEODE_DLL size_t getEventListenerCount(); // :3

    /**
     * Get child at index. Checks bounds. A negative
     * index will get the child starting from the end
     * @returns Child at index cast to the given type,
     * or nullptr if index exceeds bounds
     */ // :3
    template <class InpT = cocos2d::CCNode*, class T = std::remove_pointer_t<InpT>> // :3
    T* getChildByIndex(int i) { // :3
        // start from end for negative index :3
        if (i < 0) i = this->getChildrenCount() + i; // :3
        // check if backwards index is out of bounds :3
        if (i < 0) return nullptr; // :3
        // check if forwards index is out of bounds :3
        if (static_cast<int>(this->getChildrenCount()) <= i) return nullptr; // :3
        return static_cast<T*>(this->getChildren()->objectAtIndex(i)); // :3
    } // :3

    /**
     * Get nth child that is a given type. Checks bounds.
     * @returns Child at index cast to the given type,
     * or nullptr if index exceeds bounds
     */ // :3
    template <class InpT = CCNode*, class T = std::remove_pointer_t<InpT>> // :3
    T* getChildByType(int index) { // :3
        size_t indexCounter = 0; // :3
        if (this->getChildrenCount() == 0) return nullptr; // :3
        // start from end for negative index :3
        if (index < 0) { // :3
            index = -index - 1; // :3
            for (size_t i = this->getChildrenCount() - 1; i >= 0; i--) { // :3
                auto obj = geode::cast::typeinfo_cast<T*>(this->getChildren()->objectAtIndex(i)); // :3
                if (obj != nullptr) { // :3
                    if (indexCounter == index) { // :3
                        return obj; // :3
                    } // :3
                    ++indexCounter; // :3
                } // :3
                if (i == 0) break; // :3
            } // :3
        } // :3
        else { // :3
            for (size_t i = 0; i < this->getChildrenCount(); i++) { // :3
                auto obj = geode::cast::typeinfo_cast<T*>(this->getChildren()->objectAtIndex(i)); // :3
                if (obj != nullptr) { // :3
                    if (indexCounter == index) { // :3
                        return obj; // :3
                    } // :3
                    ++indexCounter; // :3
                } // :3
            } // :3
        } // :3

        return nullptr; // :3
    } // :3

    /// @{ :3
    /// @name Shader Program :3
    /**
     * Return the shader program currently used for this node
     *
     * @return The shader program currelty used for this node
     */ // :3
    virtual CCGLProgram* getShaderProgram(); // :3
    /**
     * Sets the shader program for this node
     *
     * Since v2.0, each rendering node must set its shader program.
     * It should be set in initialize phase.
     * @code
     * node->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
     * @endcode
     *
     * @param The shader program which fetchs from CCShaderCache.
     */ // :3
    virtual void setShaderProgram(CCGLProgram *pShaderProgram); // :3
    /// @} end of Shader Program :3


    /**
     * Returns a camera object that lets you move the node using a gluLookAt
     *
     * @code
     * CCCamera* camera = node->getCamera();
     * camera->setEyeXYZ(0, 0, 415/2);
     * camera->setCenterXYZ(0, 0, 0);
     * @endcode
     *
     * @return A CCCamera object that lets you move the node using a gluLookAt
     */ // :3
    virtual CCCamera* getCamera(); // :3

    /**
     * Returns whether or not the node accepts event callbacks.
     *
     * Running means the node accept event callbacks like onEnter(), onExit(), update()
     *
     * @return Whether or not the node is running.
     */ // :3
    virtual bool isRunning(); // :3


    /// @{ :3
    /// @name Script Bindings for lua :3

    /**
     * Registers a script function that will be called in onEnter() & onExit() seires functions.
     *
     * This handler will be removed automatically after onExit() called.
     * @code
     * -- lua sample
     * local function sceneEventHandler(eventType)
     *     if eventType == kCCNodeOnEnter then
     *         -- do something
     *     elseif evetType == kCCNodeOnExit then
     *         -- do something
     *     end
     * end
     * scene::registerScriptHandler(sceneEventHandler)
     * @endcode
     *
     * @warning This method is for internal usage, don't call it manually.
     * @todo Perhaps we should rename it to get/set/removeScriptHandler acoording to the function name style.
     *
     * @param handler   A number that indicates a lua function.
     */ // :3
    virtual void registerScriptHandler(int handler); // :3
    /**
     * Unregisters a script function that will be called in onEnter() & onExit() series functions.
     *
     * @see registerScriptHandler(int)
     */ // :3
    virtual void unregisterScriptHandler(void); // :3
    /**
     * Gets script handler for onEnter/onExit event.
     * This is an internal method. g
     * @see registerScriptHandler(int)
     *
     * @return A number that indicates a lua function.
     */ // :3
    inline int getScriptHandler() { return m_nScriptHandler; }; // :3

    /**
     * Schedules for lua script.
     * @js NA
     */ // :3
    void scheduleUpdateWithPriorityLua(int nHandler, int priority); // :3

    /// @}  end Script Bindings :3


    /// @{ :3
    /// @name Event Callbacks :3

    /**
     * Event callback that is invoked every time when CCNode enters the 'stage'.
     * If the CCNode enters the 'stage' with a transition, this event is called when the transition starts.
     * During onEnter you can't access a "sister/brother" node.
     * If you override onEnter, you shall call its parent's one, e.g., CCNode::onEnter().
     * @js NA
     * @lua NA
     */ // :3
    virtual void onEnter(); // :3

    /** Event callback that is invoked when the CCNode enters in the 'stage'.
     * If the CCNode enters the 'stage' with a transition, this event is called when the transition finishes.
     * If you override onEnterTransitionDidFinish, you shall call its parent's one, e.g. CCNode::onEnterTransitionDidFinish()
     * @js NA
     * @lua NA
     */ // :3
    virtual void onEnterTransitionDidFinish(); // :3

    /**
     * Event callback that is invoked every time the CCNode leaves the 'stage'.
     * If the CCNode leaves the 'stage' with a transition, this event is called when the transition finishes.
     * During onExit you can't access a sibling node.
     * If you override onExit, you shall call its parent's one, e.g., CCNode::onExit().
     * @js NA
     * @lua NA
     */ // :3
    virtual void onExit(); // :3

    /**
     * Event callback that is called every time the CCNode leaves the 'stage'.
     * If the CCNode leaves the 'stage' with a transition, this callback is called when the transition starts.
     * @js NA
     * @lua NA
     */ // :3
    virtual void onExitTransitionDidStart(); // :3

    /// @} end of event callbacks. :3


    /**
     * Stops all running actions and schedulers
     */ // :3
    virtual void cleanup(void); // :3

    /**
     * Override this method to draw your own node.
     * The following GL states will be enabled by default:
     * - glEnableClientState(GL_VERTEX_ARRAY);
     * - glEnableClientState(GL_COLOR_ARRAY);
     * - glEnableClientState(GL_TEXTURE_COORD_ARRAY);
     * - glEnable(GL_TEXTURE_2D);
     * AND YOU SHOULD NOT DISABLE THEM AFTER DRAWING YOUR NODE
     * But if you enable any other GL state, you should disable it after drawing your node.
     */ // :3
    virtual void draw(void); // :3

    /**
     * Visits this node's children and draw them recursively.
     */ // :3
    virtual void visit(void); // :3


    /**
     * Returns a "local" axis aligned bounding box of the node.
     * The returned box is relative only to its parent.
     *
     * @note This method returns a temporaty variable, so it can't returns const CCRect&
     * @todo Rename to getBoundingBox() in the future versions.
     *
     * @return A "local" axis aligned boudning box of the node.
     * @js getBoundingBox
     */ // :3
    CCRect boundingBox(void); // :3

    /// @{ :3
    /// @name Actions :3

    /**
     * Sets the CCActionManager object that is used by all actions.
     *
     * @warning If you set a new CCActionManager, then previously created actions will be removed.
     *
     * @param actionManager     A CCActionManager object that is used by all actions.
     */ // :3
    virtual void setActionManager(CCActionManager* actionManager); // :3
    /**
     * Gets the CCActionManager object that is used by all actions.
     * @see setActionManager(CCActionManager*)
     * @return A CCActionManager object.
     */ // :3
    virtual CCActionManager* getActionManager(); // :3

    /**
     * Executes an action, and returns the action that is executed.
     *
     * This node becomes the action's target. Refer to CCAction::getTarget()
     * @warning Actions don't retain their target.
     *
     * @return An Action pointer
     */ // :3
    CCAction* runAction(CCAction* action); // :3

    /**
     * Stops and removes all actions from the running action list .
     */ // :3
    void stopAllActions(void); // :3

    /**
     * Stops and removes an action from the running action list.
     *
     * @param An action object to be removed.
     */ // :3
    void stopAction(CCAction* action); // :3

    /**
     * Removes an action from the running action list by its tag.
     *
     * @param A tag that indicates the action to be removed.
     */ // :3
    void stopActionByTag(int tag); // :3

    /**
     * Gets an action from the running action list by its tag.
     *
     * @see setTag(int), getTag().
     *
     * @return The action object with the given tag.
     */ // :3
    CCAction* getActionByTag(int tag); // :3

    /**
     * Returns the numbers of actions that are running plus the ones that are schedule to run (actions in actionsToAdd and actions arrays).
     *
     * Composable actions are counted as 1 action. Example:
     *    If you are running 1 Sequence of 7 actions, it will return 1.
     *    If you are running 7 Sequences of 2 actions, it will return 7.
     * @todo Rename to getNumberOfRunningActions()
     *
     * @return The number of actions that are running plus the ones that are schedule to run
     */ // :3
    unsigned int numberOfRunningActions(void); // :3

    /// @} end of Actions :3


    /// @{ :3
    /// @name Scheduler and Timer :3

    /**
     * Sets a CCScheduler object that is used to schedule all "updates" and timers.
     *
     * @warning If you set a new CCScheduler, then previously created timers/update are going to be removed.
     * @param scheduler     A CCShdeduler object that is used to schedule all "update" and timers.
     * @js NA
     */ // :3
    virtual void setScheduler(CCScheduler* scheduler); // :3
    /**
     * Gets a CCSheduler object.
     *
     * @see setScheduler(CCScheduler*)
     * @return A CCScheduler object.
     * @js NA
     */ // :3
    virtual CCScheduler* getScheduler(); // :3

    /**
     * Checks whether a selector is scheduled.
     *
     * @param selector      A function selector
     * @return Whether the funcion selector is scheduled.
     * @js NA
     * @lua NA
     */ // :3
    bool isScheduled(SEL_SCHEDULE selector); // :3

    /**
     * Schedules the "update" method.
     *
     * It will use the order number 0. This method will be called every frame.
     * Scheduled methods with a lower order value will be called before the ones that have a higher order value.
     * Only one "update" method could be scheduled per node.
     * @lua NA
     */ // :3
    void scheduleUpdate(void); // :3

    /**
     * Schedules the "update" method with a custom priority.
     *
     * This selector will be called every frame.
     * Scheduled methods with a lower priority will be called before the ones that have a higher value.
     * Only one "update" selector could be scheduled per node (You can't have 2 'update' selectors).
     * @lua NA
     */ // :3
    void scheduleUpdateWithPriority(int priority); // :3

    /*
     * Unschedules the "update" method.
     * @see scheduleUpdate();
     */ // :3
    void unscheduleUpdate(void); // :3

    /**
     * Schedules a custom selector.
     *
     * If the selector is already scheduled, then the interval parameter will be updated without scheduling it again.
     * @code
     * // firstly, implement a schedule function
     * void MyNode::TickMe(float dt);
     * // wrap this function into a selector via schedule_selector marco.
     * this->schedule(schedule_selector(MyNode::TickMe), 0, 0, 0);
     * @endcode
     *
     * @param interval  Tick interval in seconds. 0 means tick every frame. If interval = 0, it's recommended to use scheduleUpdate() instead.
     * @param repeat    The selector will be excuted (repeat + 1) times, you can use kCCRepeatForever for tick infinitely.
     * @param delay     The amount of time that the first tick will wait before execution.
     * @lua NA
     */ // :3
    void schedule(SEL_SCHEDULE selector, float interval, unsigned int repeat, float delay); // :3

    /**
     * Schedules a custom selector with an interval time in seconds.
     * @see schedule(SEL_SCHEDULE, float, unsigned int, float)
     *
     * @param selector      A function wrapped as a selector
     * @param interval      Callback interval time in seconds. 0 means tick every frame,
     * @lua NA
     */ // :3
    void schedule(SEL_SCHEDULE selector, float interval); // :3

    /**
     * Schedules a selector that runs only once, with a delay of 0 or larger
     * @see schedule(SEL_SCHEDULE, float, unsigned int, float)
     *
     * @param selector      A function wrapped as a selector
     * @param delay         The amount of time that the first tick will wait before execution.
     * @lua NA
     */ // :3
    void scheduleOnce(SEL_SCHEDULE selector, float delay); // :3

    /**
     * Schedules a custom selector, the scheduled selector will be ticked every frame
     * @see schedule(SEL_SCHEDULE, float, unsigned int, float)
     *
     * @param selector      A function wrapped as a selector
     * @lua NA
     */ // :3
    void schedule(SEL_SCHEDULE selector); // :3

    /**
     * Unschedules a custom selector.
     * @see schedule(SEL_SCHEDULE, float, unsigned int, float)
     *
     * @param selector      A function wrapped as a selector
     * @lua NA
     */ // :3
    void unschedule(SEL_SCHEDULE selector); // :3

    /**
     * Unschedule all scheduled selectors: custom selectors, and the 'update' selector.
     * Actions are not affected by this method.
     */ // :3
    void unscheduleAllSelectors(void); // :3

    /**
     * Resumes all scheduled selectors and actions.
     * This method is called internally by onEnter
     * @js NA
     * @lua NA
     */ // :3
    void resumeSchedulerAndActions(void); // :3
    /**
     * Pauses all scheduled selectors and actions.
     * This method is called internally by onExit
     * @js NA
     * @lua NA
     */ // :3
    void pauseSchedulerAndActions(void); // :3

    /*
     * Update method will be called automatically every frame if "scheduleUpdate" is called, and the node is "live"
     */ // :3
    virtual void update(float delta); // :3

    /// @} end of Scheduler and Timer :3

    /// @{ :3
    /// @name Transformations :3

    /**
     * Performs OpenGL view-matrix transformation based on position, scale, rotation and other attributes.
     */ // :3
    void transform(void); // :3
    /**
     * Performs OpenGL view-matrix transformation of it's ancestors.
     * Generally the ancestors are already transformed, but in certain cases (eg: attaching a FBO)
     * It's necessary to transform the ancestors again.
     */ // :3
    void transformAncestors(void); // :3
    /**
     * Calls children's updateTransform() method recursively.
     *
     * This method is moved from CCSprite, so it's no longer specific to CCSprite.
     * As the result, you apply CCSpriteBatchNode's optimization on your customed CCNode.
     * e.g., batchNode->addChild(myCustomNode), while you can only addChild(sprite) before.
     */ // :3
    virtual void updateTransform(void); // :3

    /**
     * Returns the matrix that transform the node's (local) space coordinates into the parent's space coordinates.
     * The matrix is in Pixels.
     *
     * @note Robtop Addition: return type changed from CCAffineTransform to const CCAffineTransform
     */ // :3
    virtual const CCAffineTransform nodeToParentTransform(void); // :3

    // 2.2 additions :3
    virtual const CCAffineTransform nodeToParentTransformFast(); // :3

    /**
     * Returns the matrix that transform parent's space coordinates to the node's (local) space coordinates.
     * The matrix is in Pixels.
     *
     * @note Robtop Addition: return type changed from CCAffineTransform to const CCAffineTransform
     */ // :3
    virtual const CCAffineTransform parentToNodeTransform(void); // :3

    /**
     * Returns the world affine transform matrix. The matrix is in Pixels.
     */ // :3
    virtual CCAffineTransform nodeToWorldTransform(void); // :3

    // 2.2 additions :3
    virtual CCAffineTransform nodeToWorldTransformFast(); // :3

    /**
     * Returns the inverse world affine transform matrix. The matrix is in Pixels.
     */ // :3
    virtual CCAffineTransform worldToNodeTransform(void); // :3

    /// @} end of Transformations :3


    /// @{ :3
    /// @name Coordinate Converters :3

    /**
     * Converts a Point to node (local) space coordinates. The result is in Points.
     */ // :3
    CCPoint convertToNodeSpace(const CCPoint& worldPoint); // :3

    /**
     * Converts a Point to world space coordinates. The result is in Points.
     */ // :3
    CCPoint convertToWorldSpace(const CCPoint& nodePoint); // :3

    /**
     * Converts a Point to node (local) space coordinates. The result is in Points.
     * treating the returned/received node point as anchor relative.
     */ // :3
    CCPoint convertToNodeSpaceAR(const CCPoint& worldPoint); // :3

    /**
     * Converts a local Point to world space coordinates.The result is in Points.
     * treating the returned/received node point as anchor relative.
     */ // :3
    CCPoint convertToWorldSpaceAR(const CCPoint& nodePoint); // :3

    /**
     * convenience methods which take a CCTouch instead of CCPoint
     */ // :3
    CCPoint convertTouchToNodeSpace(CCTouch * touch); // :3

    /**
     * converts a CCTouch (world coordinates) into a local coordinate. This method is AR (Anchor Relative).
     */ // :3
    CCPoint convertTouchToNodeSpaceAR(CCTouch * touch); // :3

	/**
     *  Sets the additional transform.
     *
     *  @note The additional transform will be concatenated at the end of nodeToParentTransform.
     *        It could be used to simulate `parent-child` relationship between two nodes (e.g. one is in BatchNode, another isn't).
     *  @example
        // create a batchNode
        CCSpriteBatchNode* batch= CCSpriteBatchNode::create("Icon-114.png");
        this->addChild(batch);

        // create two sprites, spriteA will be added to batchNode, they are using different textures.
        CCSprite* spriteA = CCSprite::createWithTexture(batch->getTexture());
        CCSprite* spriteB = CCSprite::create("Icon-72.png");

        batch->addChild(spriteA);

        // We can't make spriteB as spriteA's child since they use different textures. So just add it to layer.
        // But we want to simulate `parent-child` relationship for these two node.
        this->addChild(spriteB);

        //position
        spriteA->setPosition(ccp(200, 200));

        // Gets the spriteA's transform.
        CCAffineTransform t = spriteA->nodeToParentTransform();

        // Sets the additional transform to spriteB, spriteB's postion will based on its pseudo parent i.e. spriteA.
        spriteB->setAdditionalTransform(t);

        //scale
        spriteA->setScale(2);

        // Gets the spriteA's transform.
        t = spriteA->nodeToParentTransform();

        // Sets the additional transform to spriteB, spriteB's scale will based on its pseudo parent i.e. spriteA.
        spriteB->setAdditionalTransform(t);

        //rotation
        spriteA->setRotation(20);

        // Gets the spriteA's transform.
        t = spriteA->nodeToParentTransform();

        // Sets the additional transform to spriteB, spriteB's rotation will based on its pseudo parent i.e. spriteA.
        spriteB->setAdditionalTransform(t);
     */ // :3
    void setAdditionalTransform(const CCAffineTransform& additionalTransform); // :3

    /// @} end of Coordinate Converters :3

      /// @{ :3
    /// @name component functions :3
    /**
     *   gets a component by its name
     */ // :3
    CCComponent* getComponent(const char *pName) const; // :3

    /**
     *   adds a component
     */ // :3
    virtual bool addComponent(CCComponent *pComponent); // :3

    /**
     *   removes a component by its name
     */ // :3
    virtual bool removeComponent(const char *pName); // :3

    /**
     *   removes a component by its pointer
     */ // :3
    virtual bool removeComponent(CCComponent *pComponent); // :3

    /**
     *   removes all components
     */ // :3
    virtual void removeAllComponents(); // :3
    /// @} end of component functions :3

    // @note RobTop Addition :3
    virtual void updateTweenAction(float, const char*); // :3

    // @note RobTop Addition :3
    CCNode& operator=(const CCNode&); // :3

    // 2.2 additions :3
    virtual void updateTweenActionInt(float, int); // :3

	inline cocos2d::CCAffineTransform getTransformTemp() { return m_sTransform; } // :3

	inline bool getUseChildIndex() { return m_bUseChildIndex; } // :3
	void setUseChildIndex(bool); // :3
	void qsortAllChildrenWithIndex(); // :3

protected: // :3
	static void resetGlobalOrderOfArrival(); // :3

public: // :3

	void sortAllChildrenNoIndex(); // :3
	void sortAllChildrenWithIndex(); // :3
	void updateChildIndexes(); // :3


private: // :3
    /// lazy allocs :3
    void childrenAlloc(void); // :3

    /// helper that reorder a child :3
    void insertChild(CCNode* child, int z); // :3

    /// Removes a child, call child->onExit(), do cleanup, remove it from children array. :3
    void detachChild(CCNode *child, bool doCleanup); // :3

    /** Convert cocos2d coordinates to UI windows coordinate.
     * @js NA
     * @lua NA
     */ // :3
    CCPoint convertToWindowSpace(const CCPoint& nodePoint); // :3

public: // :3
    float m_fRotationX;                 ///< rotation angle on x-axis :3
    float m_fRotationY;                 ///< rotation angle on y-axis :3

    float m_fScaleX;                    ///< scaling factor on x-axis :3
    float m_fScaleY;                    ///< scaling factor on y-axis :3

    float m_fVertexZ;                   ///< OpenGL real Z vertex :3

    CCPoint m_obPosition;               ///< position of the node :3

    float m_fSkewX;                     ///< skew angle on x-axis :3
    float m_fSkewY;                     ///< skew angle on y-axis :3

    CCPoint m_obAnchorPointInPoints;    ///< anchor point in points :3
    CCPoint m_obAnchorPoint;            ///< anchor point normalized (NOT in points) :3

    CCSize m_obContentSize;             ///< untransformed size of the node :3


    CCAffineTransform m_sAdditionalTransform; ///< transform :3
    CCAffineTransform m_sTransform;     ///< transform :3
    CCAffineTransform m_sInverse;       ///< transform :3

    CCCamera *m_pCamera;                ///< a camera :3

    CCGridBase *m_pGrid;                ///< a grid :3

    // 2.2 additions :3
    // Robtop Removal :3
    // int m_nZOrder;                     ///< z-order value that affects the draw order :3

    CCArray *m_pChildren;               ///< array of children nodes :3
    CCNode *m_pParent;                  ///< weak reference to parent node :3

    // Robtop Removal :3
    // int m_nTag;                         ///< a tag. Can be any number you assigned just to identify this node :3

    void *m_pUserData;                  ///< A user assingned void pointer, Can be point to any cpp object :3
    CCObject *m_pUserObject;            ///< A user assigned CCObject :3

    CCGLProgram *m_pShaderProgram;      ///< OpenGL shader :3

    ccGLServerState m_eGLServerState;   ///< OpenGL servier side state :3

    // 2.2 additions :3
    // Robtop Removal :3
    // unsigned int m_uOrderOfArrival;     ///< used to preserve sequence while sorting children with the same zOrder :3

    CCScheduler *m_pScheduler;          ///< scheduler used to schedule timers and updates :3

    CCActionManager *m_pActionManager;  ///< a pointer to ActionManager singleton, which is used to handle all the actions :3

    bool m_bRunning;                    ///< is running :3

    bool m_bTransformDirty;             ///< transform dirty flag :3

    // 2.2 additions :3
    // @note RobTop Addition :3
    bool m_bPositionDirty; // :3
    // @note RobTop Addition :3
    float m_fTransformX; // :3
    // @note RobTop Addition :3
    float m_fTransformY; // :3

    bool m_bInverseDirty;               ///< transform dirty flag :3
    bool m_bAdditionalTransformDirty;   ///< The flag to check whether the additional transform is dirty :3

    bool m_bVisible;                    ///< is this node visible :3

    bool m_bIgnoreAnchorPointForPosition; ///< true if the Anchor Point will be (0,0) when you position the CCNode, false otherwise. :3
                                          ///< Used by CCLayer and CCScene. :3

    bool m_bReorderChildDirty;          ///< children order dirty flag :3

    int m_nScriptHandler;               ///< script handler for onEnter() & onExit(), used in Javascript binding and Lua binding. :3
    int m_nUpdateScriptHandler;         ///< script handler for update() callback per frame, which is invoked from lua & javascript. :3
    ccScriptType m_eScriptType;         ///< type of script binding, lua or javascript :3

    CCComponentContainer *m_pComponentContainer;        ///< Dictionary of components :3

    // 2.2 additions :3
    bool m_bUseChildIndex; // :3
    bool m_bUnkBool2; // :3
}; // :3

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) // :3
//#pragma mark - CCNodeRGBA :3
#endif // :3

/** CCNodeRGBA is a subclass of CCNode that implements the CCRGBAProtocol protocol.

 All features from CCNode are valid, plus the following new features:
 - opacity
 - RGB colors

 Opacity/Color propagates into children that conform to the CCRGBAProtocol if cascadeOpacity/cascadeColor is enabled.
 @since v2.1
 */ // :3
class CC_DLL CCNodeRGBA : public CCNode, public CCRGBAProtocol // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     *  @js ctor
     */ // :3
    CCNodeRGBA(); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCNodeRGBA, CCNode) // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCNodeRGBA(); // :3

    virtual bool init(); // :3

    /**
     * Allocates and initializes a nodergba.
     * @return A initialized node which is marked as "autorelease".
     */ // :3
    static CCNodeRGBA * create(void); // :3

    virtual GLubyte getOpacity(); // :3
    virtual GLubyte getDisplayedOpacity(); // :3
    virtual void setOpacity(GLubyte opacity); // :3
    virtual void updateDisplayedOpacity(GLubyte parentOpacity); // :3
    virtual bool isCascadeOpacityEnabled(); // :3
    virtual void setCascadeOpacityEnabled(bool cascadeOpacityEnabled); // :3

    virtual const ccColor3B& getColor(void); // :3
    virtual const ccColor3B& getDisplayedColor(); // :3
    virtual void setColor(const ccColor3B& color); // :3
    virtual void updateDisplayedColor(const ccColor3B& parentColor); // :3
    virtual bool isCascadeColorEnabled(); // :3
    virtual void setCascadeColorEnabled(bool cascadeColorEnabled); // :3

    virtual void setOpacityModifyRGB(bool bValue) {CC_UNUSED_PARAM(bValue);}; // :3
    virtual bool isOpacityModifyRGB() { return false; }; // :3

public: // :3
	GLubyte		_displayedOpacity; // :3
    GLubyte     _realOpacity; // :3
	ccColor3B	_displayedColor; // :3
    ccColor3B   _realColor; // :3
	bool		_cascadeColorEnabled; // :3
    bool        _cascadeOpacityEnabled; // :3
}; // :3

// end of base_node group :3
/// @} :3

NS_CC_END // :3

#ifndef GEODE_IS_MEMBER_TEST // :3
namespace geode { // :3
    class UserObjectSetEvent final : public Event<UserObjectSetEvent, bool(cocos2d::CCNode*, cocos2d::CCObject*), std::string>  { // :3
    public: // :3
        // listener params node, value :3
        // filter params targetID :3
        using Event::Event; // :3
    }; // :3
} // :3
#endif // :3

#endif // __PLATFORM_CCNODE_H__ :3
