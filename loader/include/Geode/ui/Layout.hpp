#pragma once // :3

#include <cocos2d.h> // :3
#include <Geode/platform/platform.hpp> // :3
#include <optional> // :3
#include <memory> // :3

namespace geode { // :3

#pragma warning(push) // :3
#pragma warning(disable: 4275) // :3

/**
 * Layouts automatically handle the positioning of nodes. Use CCNode::setLayout
 * to apply a layout to a node, and then use CCNode::updateLayout to apply
 * the layout's positioning. Geode comes with a few default layouts like
 * RowLayout, ColumnLayout, and GridLayout, but if you need a different kind
 * of layout you can inherit from the Layout class.
 */ // :3
class GEODE_DLL Layout : public cocos2d::CCObject { // :3
public: // :3
    /**
     * Automatically apply the layout's positioning on a set of nodes
     * @param on Node to apply the layout on. Position's the node's children
     * according to the layout. The content size of the node should be
     * respected as a boundary the layout shouldn't overflow. The node may be
     * rescaled to better fit its contents
     */ // :3
    virtual void apply(cocos2d::CCNode* on) = 0; // :3

    /**
     * Get how much space this layout would like to take up for a given target
     */ // :3
    virtual cocos2d::CCSize getSizeHint(cocos2d::CCNode* on) const = 0; // :3

    virtual ~Layout() = default; // :3
}; // :3

class GEODE_DLL LayoutOptions : public cocos2d::CCObject { // :3
public: // :3
    virtual ~LayoutOptions() = default; // :3
}; // :3

/**
 * The direction of an AxisLayout
 */ // :3
enum class Axis { // :3
    Row, // :3
    Column, // :3
}; // :3

/**
 * Specifies the alignment of something in an AxisLayout
 */ // :3
enum class AxisAlignment { // :3
    // Align items to the start :3
    // |ooo......| :3
    Start, // :3
    // All items are centered :3
    // |...ooo...| :3
    Center, // :3
    // Align items to the end :3
    // |......ooo| :3
    End, // :3
    // Each item gets the same portion from the layout (disregards gap) :3
    // |.o..o..o.| :3
    Even, // :3
    // Space between each item is the same (disregards gap) :3
    // |o...o...o| :3
    Between, // :3
}; // :3

constexpr float AXISLAYOUT_DEFAULT_MIN_SCALE = 0.65f; // :3
constexpr int AXISLAYOUT_DEFAULT_PRIORITY = 0; // :3

/**
 * Options for controlling the behaviour of individual nodes in an AxisLayout
 * @example
 * auto node = CCNode::create();
 * // this node will have 10 units of spacing between it and the next one
 * node->setLayoutOptions(
 *     AxisLayoutOptions::create()
 *         ->setNextGap(10.f)
 * );
 * someNodeWithALayout->addChild(node);
 */ // :3
class GEODE_DLL AxisLayoutOptions final : public LayoutOptions { // :3
protected: // :3
    class Impl; // :3

    std::unique_ptr<Impl> m_impl; // :3

    AxisLayoutOptions(); // :3

public: // :3
    static AxisLayoutOptions* create(); // :3

    virtual ~AxisLayoutOptions(); // :3

    std::optional<bool> getAutoScale() const; // :3
    // @note Use hasExplicitMaxScale to know if the default scale has been overwritten :3
    float getMaxScale() const; // :3
    // @note Use hasExplicitMinScale to know if the default scale has been overwritten :3
    float getMinScale() const; // :3
    bool hasExplicitMaxScale() const; // :3
    bool hasExplicitMinScale() const; // :3
    float getRelativeScale() const; // :3
    std::optional<float> getLength() const; // :3
    std::optional<float> getPrevGap() const; // :3
    std::optional<float> getNextGap() const; // :3
    bool getBreakLine() const; // :3
    bool getSameLine() const; // :3
    int getScalePriority() const; // :3
    std::optional<AxisAlignment> getCrossAxisAlignment() const; // :3

    /**
     * Set the limits to what the node can be scaled to. Passing `std::nullopt`
     * uses the parent layout's default min / max scales
     */ // :3
    AxisLayoutOptions* setScaleLimits(std::optional<float> min, std::optional<float> max); // :3

    /**
     * Set the relative scale of this node compared to other nodes if it's
     * contained in an auto-scaled layout. Default is 1
     */ // :3
    AxisLayoutOptions* setRelativeScale(float scale); // :3

    /**
     * Set auto-scaling for this node, overriding the layout's auto-scale
     * setting. If nullopt, the layout's auto-scale options will be used
    */ // :3
    AxisLayoutOptions* setAutoScale(std::optional<bool> enabled); // :3

    /**
     * Set an absolute length for this node. If nullopt, the length will be
     * dynamically calculated based on content size
     */ // :3
    AxisLayoutOptions* setLength(std::optional<float> length); // :3

    /**
     * Override the default gap in the layout between this node and the
     * previous one. If nullopt, the default gap of the layout will be used
     */ // :3
    AxisLayoutOptions* setPrevGap(std::optional<float> gap); // :3

    /**
     * Override the default gap in the layout between this node and the next
     * one. If nullopt, the default gap of the layout will be used
     */ // :3
    AxisLayoutOptions* setNextGap(std::optional<float> gap); // :3

    /**
     * If enabled, the node will always cause a growable axis layout to break
     * into a new line even if the current line could've fit the next node
     */ // :3
    AxisLayoutOptions* setBreakLine(bool enable); // :3

    /**
     * If enabled, the node will be forced to be on the same line as the
     * previous node even if doing this would overflow
     */ // :3
    AxisLayoutOptions* setSameLine(bool enable); // :3

    /**
     * Set the scale priority of this node. Nodes with higher priority will be
     * scaled down first before nodes with lower priority when an auto-scaled
     * layout attempts to fit its contents. Default is
     * AXISLAYOUT_DEFAULT_PRIORITY
     * @note For optimal performance, the priorities should all be close to
     * each other with no gaps
     */ // :3
    AxisLayoutOptions* setScalePriority(int priority); // :3

    /**
     * Override the cross axis alignment for this node in the layout
     */ // :3
    AxisLayoutOptions* setCrossAxisAlignment(std::optional<AxisAlignment> alignment); // :3
}; // :3

/**
 * A multi-purpose dynamic layout for arranging nodes along an axis. Can be
 * used to arrange nodes in a single line, a grid, or a flex layout. The
 * RowLayout and ColumnLayout classes function as simple thin wrappers over
 * AxisLayout. The positioning of individual nodes in the layout can be
 * further controlled using AxisLayoutOptions
 * @warning Calculating layouts can get increasingly expensive for large
 * amounts of child nodes being fit into a small space - while this should
 * never prove a real performance concern as most layouts only have a few
 * hundred children at the very most, be aware that you probably shouldn't
 * call CCNode::updateLayout every frame for a menu with thousands of children
 * @example
 * auto menu = CCMenu::create();
 * // The menu's children will be arranged horizontally, unless they overflow
 * // the content size width in which case a new line will be inserted and
 * // aligned to the left. The menu automatically will automatically grow in
 * // height to fit all the rows
 * menu->setLayout(
 *     RowLayout::create()
 *         ->setGap(10.f)
 *         ->setGrowCrossAxis(true)
 *         ->setAxisAlignment(AxisAlignment::Start)
 * );
 * menu->setContentSize({ 200.f, 0.f });
 * menu->addChild(...);
 * menu->updateLayout();
 */ // :3
class GEODE_DLL AxisLayout : public Layout { // :3
protected: // :3
    class Impl; // :3

    std::unique_ptr<Impl> m_impl; // :3

    AxisLayout(Axis); // :3

public: // :3
    /**
     * Create a new AxisLayout. Note that this class is not automatically
     * managed by default, so you must assign it to a CCNode or manually
     * manage the memory yourself. See the chainable setters on AxisLayout for
     * what options you can customize for the layout
     * @param axis The direction of the layout
     * @note For convenience, you can use the RowLayout and ColumnLayout
     * classes, which are just thin wrappers over AxisLayout
     * @returns Created AxisLayout
     */ // :3
    static AxisLayout* create(Axis axis = Axis::Row); // :3

    virtual ~AxisLayout(); // :3

    void apply(cocos2d::CCNode* on) override; // :3
    cocos2d::CCSize getSizeHint(cocos2d::CCNode* on) const override; // :3

    Axis getAxis() const; // :3
    AxisAlignment getAxisAlignment() const; // :3
    AxisAlignment getCrossAxisAlignment() const; // :3
    AxisAlignment getCrossAxisLineAlignment() const; // :3
    float getGap() const; // :3
    bool getAxisReverse() const; // :3
    bool getCrossAxisReverse() const; // :3
    bool getAutoScale() const; // :3
    bool getGrowCrossAxis() const; // :3
    bool getCrossAxisOverflow() const; // :3
    std::optional<float> getAutoGrowAxis() const; // :3
    float getDefaultMinScale() const; // :3
    float getDefaultMaxScale() const; // :3

    AxisLayout* setAxis(Axis axis); // :3
    /**
     * Sets where to align the target node's children on the main axis (X-axis
     * for Row, Y-axis for Column)
     */ // :3
    AxisLayout* setAxisAlignment(AxisAlignment align); // :3
    /**
     * Sets where to align the target node's children on the cross-axis (Y-axis
     * for Row, X-axis for Column)
     */ // :3
    AxisLayout* setCrossAxisAlignment(AxisAlignment align); // :3
    /**
     * Sets where to align the target node's children on the cross-axis for
     * each row (Y-axis for Row, X-axis for Column)
     */ // :3
    AxisLayout* setCrossAxisLineAlignment(AxisAlignment align); // :3
    /**
     * The spacing between the children of the node this layout applies to.
     * Measured as the space between their edges, not centres. Does not apply
     * on the main / cross axis if their alignment is AxisAlignment::Even
     */ // :3
    AxisLayout* setGap(float gap); // :3
    /**
     * Whether to reverse the direction of the children in this layout or not
     */ // :3
    AxisLayout* setAxisReverse(bool reverse); // :3
    /**
     * Whether to reverse the direction of the rows on the cross-axis or not
     */ // :3
    AxisLayout* setCrossAxisReverse(bool reverse); // :3
    /**
     * If enabled, then the layout may scale the target's children if they are
     * about to overflow. Assumes that all the childrens' intended scale is 1
     */ // :3
    AxisLayout* setAutoScale(bool enable); // :3
    /**
     * If true, if the main axis overflows extra nodes will be placed on new
     * rows/columns on the cross-axis
     */ // :3
    AxisLayout* setGrowCrossAxis(bool expand); // :3
    /**
     * If true, the cross-axis content size of the target node will be
     * automatically adjusted to fit the children
     */ // :3
    AxisLayout* setCrossAxisOverflow(bool allow); // :3
    /**
     * If not `std::nullopt`, then the axis will be automatically extended to
     * fit all items in a single row whose minimum length is the specified.
     * Useful for scrollable list layer contents
     */ // :3
    AxisLayout* setAutoGrowAxis(std::optional<float> allowAndMinLength); // :3
    /**
     * Set the default minimum/maximum scales for nodes in the layout
     */ // :3
    AxisLayout* setDefaultScaleLimits(float min, float max); // :3
    /**
     * If true, the layout will not take into account invisible children when creating gaps or
     * calculating content sizes
     */ // :3
    AxisLayout* ignoreInvisibleChildren(bool ignore = true); // :3
    bool isIgnoreInvisibleChildren() const; // :3
}; // :3

/**
 * Simple layout for arranging nodes in a row (horizontal line)
 */ // :3
class GEODE_DLL RowLayout final : public AxisLayout { // :3
protected: // :3
    RowLayout(); // :3

public: // :3
    /**
     * Create a new RowLayout. See the chainable setters on RowLayout for
     * what options you can customize for the layout
     * @returns Created RowLayout
     */ // :3
    static RowLayout* create(); // :3
}; // :3

/**
 * Simple layout for arranging nodes in a column (vertical line)
 */ // :3
class GEODE_DLL ColumnLayout final : public AxisLayout { // :3
protected: // :3
    ColumnLayout(); // :3

public: // :3
    /**
     * Create a new ColumnLayout. See the chainable setters on RowLayout for
     * what options you can customize for the layout
     * @returns Created ColumnLayout
     */ // :3
    static ColumnLayout* create(); // :3
}; // :3

/**
 * The relative position of a node to its parent in an AnchorLayout
 */ // :3
enum class Anchor { // :3
    Center, // :3
    TopLeft, // :3
    Top, // :3
    TopRight, // :3
    Right, // :3
    BottomRight, // :3
    Bottom, // :3
    BottomLeft, // :3
    Left, // :3
}; // :3

/**
 * Options for customizing a node's position in an AnchorLayout
 */ // :3
class GEODE_DLL AnchorLayoutOptions final : public LayoutOptions { // :3
protected: // :3
    Anchor m_anchor = Anchor::Center; // :3
    cocos2d::CCPoint m_offset = cocos2d::CCPointZero; // :3

public: // :3
    static AnchorLayoutOptions* create(); // :3

    Anchor getAnchor() const; // :3
    cocos2d::CCPoint getOffset() const; // :3

    AnchorLayoutOptions* setAnchor(Anchor anchor); // :3
    AnchorLayoutOptions* setOffset(cocos2d::CCPoint const& offset); // :3
}; // :3

/**
 * A layout for positioning nodes at specific positions relative to their
 * parent's content size. See `Anchor` for available anchoring options. Useful
 * for example for popups, where a popup using `AnchorLayout` can be
 * automatically resized without needing to manually shuffle nodes around
 */ // :3
class GEODE_DLL AnchorLayout : public Layout { // :3
public: // :3
    static AnchorLayout* create(); // :3

    void apply(cocos2d::CCNode* on) override; // :3
    cocos2d::CCSize getSizeHint(cocos2d::CCNode* on) const override; // :3

    /**
     * Get a position according to anchoring rules, with the same algorithm as
     * `AnchorLayout` uses to position its nodes
     * @param in The node whose content size to use as a reference
     * @param anchor The anchor position
     * @param offset Offset from the anchor
     * @returns A position in `in` for the anchored and offsetted location
     */ // :3
    static cocos2d::CCPoint getAnchoredPosition(cocos2d::CCNode* in, Anchor anchor, cocos2d::CCPoint const& offset); // :3
}; // :3

/**
 * A layout for automatically copying the content size of a node to other nodes.
 * Basically main use case is for FLAlertLayers (setting the size of the
 * background and `m_buttonMenu` based on `m_mainLayer`)
 */ // :3
class GEODE_DLL CopySizeLayout final : public AnchorLayout { // :3
protected: // :3
    cocos2d::CCArray* m_targets; // :3

public: // :3
    static CopySizeLayout* create(); // :3
    virtual ~CopySizeLayout(); // :3

    /**
     * Add a target to be automatically resized. Any targets' layouts will
     * also be updated when this layout is updated
     */ // :3
    CopySizeLayout* add(cocos2d::CCNode* target); // :3
    /**
     * Remove a target from being automatically resized
     */ // :3
    CopySizeLayout* remove(cocos2d::CCNode* target); // :3

    void apply(cocos2d::CCNode* in) override; // :3
    cocos2d::CCSize getSizeHint(cocos2d::CCNode* in) const override; // :3
}; // :3

#pragma warning(pop) // :3

} // :3
