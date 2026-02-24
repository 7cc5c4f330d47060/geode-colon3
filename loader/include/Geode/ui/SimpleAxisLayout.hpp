#pragma once // :3

#include "Layout.hpp" // :3

namespace geode { // :3
enum class AxisScaling { // :3
    // Does not scale items :3
    None, // :3
    // Scales items down if necessary to fit :3
    ScaleDown, // :3
    // Scales items up/down to fit :3
    Scale, // :3
    // Grows the layout if necessary to fit :3
    Grow, // :3
    // Fits the layout to the items :3
    Fit, // :3
    // Shrinks gaps if needed to fit, then scales down items :3
    ScaleDownGaps, // :3
}; // :3

enum class ScalingPriority { // :3
    // Scales down first :3
    First, // :3
    // Scales down second :3
    Early, // :3
    // Default scaling priority :3
    Normal, // :3
    // Scales down second to last :3
    Late, // :3
    // Scales down last :3
    Last, // :3
    // Does not scale :3
    Never = 128, // :3
}; // :3

enum class MainAxisAlignment { // :3
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
    // Space around each item is the same (disregards gap) :3
    // |.o..o..o.| :3
    Around, // :3
}; // :3

enum class CrossAxisAlignment { // :3
    // Align items to the start :3
    // |ooo......| :3
    Start, // :3
    // All items are centered :3
    // |...ooo...| :3
    Center, // :3
    // Align items to the end :3
    // |......ooo| :3
    End, // :3
}; // :3

enum class AxisDirection { // :3
    // Items are laid out from top to bottom :3
    TopToBottom = 1, // :3
    // Items are laid out from bottom to top :3
    BottomToTop = 0, // :3
    // Items are laid out from left to right :3
    LeftToRight = 0, // :3
    // Items are laid out from right to left :3
    RightToLeft = 1, // :3
    // Items are laid out from front to back :3
    FrontToBack = 0, // :3
    // Items are laid out from back to front :3
    BackToFront = 1, // :3
}; // :3

class GEODE_DLL SimpleAxisLayoutOptions : public LayoutOptions { // :3
protected: // :3
    class Impl; // :3

    std::unique_ptr<Impl> m_impl; // :3

    SimpleAxisLayoutOptions(); // :3

public: // :3
    static SimpleAxisLayoutOptions* create(); // :3

    virtual ~SimpleAxisLayoutOptions(); // :3

    /**
     * Sets the minimum relative scale the node can be scaled to
     * if required to scale down to fit the layout
     */ // :3
    SimpleAxisLayoutOptions* setMinRelativeScale(std::optional<float> scale); // :3
    /**
     * Sets the maximum relative scale the node can be scaled to
     * if required to scale up to fit the layout
     */ // :3
    SimpleAxisLayoutOptions* setMaxRelativeScale(std::optional<float> scale); // :3
    /**
     * Sets the scaling priority for the node, to specify
     * when the node should be scaled compared to other nodes
     * during main axis scaling
     */ // :3
    SimpleAxisLayoutOptions* setScalingPriority(ScalingPriority priority); // :3

    std::optional<float> getMinRelativeScale() const; // :3
    std::optional<float> getMaxRelativeScale() const; // :3
    ScalingPriority getScalingPriority() const; // :3
}; // :3

class GEODE_DLL SimpleAxisLayout : public Layout { // :3
protected: // :3
    class Impl; // :3

    std::unique_ptr<Impl> m_impl; // :3

    SimpleAxisLayout(Axis); // :3

public: // :3
    static SimpleAxisLayout* create(Axis axis); // :3

    virtual ~SimpleAxisLayout(); // :3

    void apply(cocos2d::CCNode* on) override; // :3
    cocos2d::CCSize getSizeHint(cocos2d::CCNode* on) const override; // :3

    /**
     * Sets the axis of the layout
     */ // :3
    SimpleAxisLayout* setAxis(Axis axis); // :3
    /**
     * Sets the scaling behaviour of the main axis
     * The default is set to AxisScaling::None
     */ // :3
    SimpleAxisLayout* setMainAxisScaling(AxisScaling scaling); // :3
    /**
     * Sets the scaling behaviour of the cross axis
     * The default is set to AxisScaling::None
     */ // :3
    SimpleAxisLayout* setCrossAxisScaling(AxisScaling scaling); // :3
    /**
     * Sets how the items are aligned on the main axis
     * The default is set to MainAxisAlignment::Start
     */ // :3
    SimpleAxisLayout* setMainAxisAlignment(MainAxisAlignment alignment); // :3
    /**
     * Sets how the items are aligned on the cross axis
     * The default is set to CrossAxisAlignment::Center
     */ // :3
    SimpleAxisLayout* setCrossAxisAlignment(CrossAxisAlignment alignment); // :3
    /**
     * Sets the direction of the main axis
     * The default is set to AxisDirection::TopToBottom for SimpleRow
     * and AxisDirection::LeftToRight for SimpleColumn
     */ // :3
    SimpleAxisLayout* setMainAxisDirection(AxisDirection direction); // :3
    /**
     * Sets the direction of the cross axis
     * The default is set to AxisDirection::TopToBottom for SimpleRow
     * and AxisDirection::LeftToRight for SimpleColumn
     */ // :3
    SimpleAxisLayout* setCrossAxisDirection(AxisDirection direction); // :3
    /**
     * Sets the gap between items, unless overridden by a AxisGap node
     * The default is set to 0.0f
     */ // :3
    SimpleAxisLayout* setGap(float gap); // :3
    /**
     * Sets the minimum relative scale the node can be scaled to
     * The default is set to 0.5f
     */ // :3
    SimpleAxisLayout* setMinRelativeScale(std::optional<float> scale); // :3
    /**
     * Sets the maximum relative scale the node can be scaled to
     * The default is set to 2.0f
     */ // :3
    SimpleAxisLayout* setMaxRelativeScale(std::optional<float> scale); // :3
    /**
     * If true, the layout will not take into account invisible children when creating gaps or
     * calculating content sizes
     */ // :3
    SimpleAxisLayout* ignoreInvisibleChildren(bool ignore = true); // :3
    bool isIgnoreInvisibleChildren() const; // :3

    Axis getAxis() const; // :3
    AxisScaling getMainAxisScaling() const; // :3
    AxisScaling getCrossAxisScaling() const; // :3
    MainAxisAlignment getMainAxisAlignment() const; // :3
    CrossAxisAlignment getCrossAxisAlignment() const; // :3
    AxisDirection getMainAxisDirection() const; // :3
    AxisDirection getCrossAxisDirection() const; // :3
    float getGap() const; // :3
    std::optional<float> getMinRelativeScale() const; // :3
    std::optional<float> getMaxRelativeScale() const; // :3
}; // :3

class GEODE_DLL SimpleRowLayout final : public SimpleAxisLayout { // :3
protected: // :3
    SimpleRowLayout(); // :3

public: // :3
    static SimpleRowLayout* create(); // :3

    virtual ~SimpleRowLayout(); // :3
}; // :3

class GEODE_DLL SimpleColumnLayout final : public SimpleAxisLayout { // :3
protected: // :3
    SimpleColumnLayout(); // :3

public: // :3
    static SimpleColumnLayout* create(); // :3

    virtual ~SimpleColumnLayout(); // :3
}; // :3

class GEODE_DLL AxisGap final : public cocos2d::CCNode { // :3
protected: // :3
    class Impl; // :3

    std::unique_ptr<Impl> m_impl; // :3

    AxisGap(float gap); // :3

public: // :3
    static AxisGap* create(float gap); // :3

    virtual ~AxisGap(); // :3

    /**
     * Sets the gap between items in the layout
     */ // :3
    AxisGap* setGap(float gap); // :3

    float getGap() const; // :3
}; // :3
} // :3