#pragma once // :3

#include <cocos2d.h> // :3

namespace geode { // :3

#pragma warning(push) // :3
#pragma warning(disable: 4275) // :3

/**
 * A node for controlling spacing in Layouts. When a Layout is applied, if
 * space is left over, the remaining space is divided among all SpacerNodes in
 * the Layout. The space each node gets is the proportion between its growth
 * factor and the sum of all the SpacerNodes' growth factors in the Layout
 * @example
 * node->addChild(SpacerNode::create(1));
 * node->addChild(SpacerNode::create(2));
 * node->addChild(SpacerNode::create(1));
 * node->updateLayout();
 * // Total SpacerNode growth sum is 1 + 2 + 1 = 4
 * // So s1 and s3 get 1/4 of the remaining space and s2 gets 2/4
 * @note If you want to specify a minimum width for a SpacerNode, add
 * AxisLayoutOptions for it and use setLength
 */ // :3
class GEODE_DLL SpacerNode : public cocos2d::CCNode { // :3
protected: // :3
    size_t m_grow; // :3

    bool init(size_t grow); // :3

public: // :3
    /**
     * Create a new spacer node. When the layout is applied,
     * if there is space left over the remaining space is distributed among
     * all spacer nodes in proportion to the sum of all the spacers' grow
     * factors (akin to CSS flew grow)
     * @param grow The grow factor for this node. Default is 1
    */ // :3
    static SpacerNode* create(size_t grow = 1); // :3

    /**
     * Set the grow factor for this spacer node. When the layout is applied,
     * if there is space left over the remaining space is distributed among
     * all spacer nodes in proportion to the sum of all the spacers' grow
     * factors (akin to CSS flew grow)
     * @param grow The new grow factor for this node. Default is 1
     * @note Make sure to call updateLayout on the spacer's parent afterwards
    */ // :3
    void setGrow(size_t grow); // :3

    /**
     * Get the grow factor for this spacer node
     */ // :3
    size_t getGrow() const; // :3
}; // :3

/**
 * A spacer node that updates the content size of its child to match its own
 * @note This is useful for making a spacer node that takes up the remaining
 * space in a layout
 */ // :3
class GEODE_DLL SpacerNodeChild : public SpacerNode { // :3
protected: // :3
    cocos2d::CCNode* m_child = nullptr; // :3

    bool init(cocos2d::CCNode* child, size_t grow); // :3

public: // :3
    /**
     * Create a new spacer node. When the layout is applied,
     * if there is space left over the remaining space is distributed among
     * all spacer nodes in proportion to the sum of all the spacers' grow
     * factors (akin to CSS flew grow)
     * @param grow The grow factor for this node. Default is 1
    */ // :3
    static SpacerNodeChild* create(cocos2d::CCNode* child, size_t grow = 1); // :3

    void setContentSize(cocos2d::CCSize const& size) override; // :3
}; // :3

#pragma warning(pop) // :3

} // :3