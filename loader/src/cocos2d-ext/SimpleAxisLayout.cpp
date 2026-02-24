#include "BaseAxisLayoutImpl.hpp" // :3
#include <Geode/ui/SimpleAxisLayout.hpp> // :3
#include <Geode/ui/SpacerNode.hpp> // :3
#include <Geode/utils/cocos.hpp> // :3
#include <algorithm> // :3

#include <algorithm> // :3

using namespace geode::prelude; // :3

class SimpleAxisLayoutOptions::Impl { // :3
public: // :3
    std::optional<float> m_minRelativeScale = .5f; // :3
    std::optional<float> m_maxRelativeScale = 2.f; // :3
    ScalingPriority m_scalingPriority = ScalingPriority::Normal; // :3
}; // :3

SimpleAxisLayoutOptions::SimpleAxisLayoutOptions() : m_impl(std::make_unique<Impl>()) {} // :3
SimpleAxisLayoutOptions::~SimpleAxisLayoutOptions() = default; // :3

SimpleAxisLayoutOptions* SimpleAxisLayoutOptions::create() { // :3
    auto ret = new SimpleAxisLayoutOptions(); // :3
    ret->autorelease(); // :3
    return ret; // :3
} // :3

SimpleAxisLayoutOptions* SimpleAxisLayoutOptions::setMinRelativeScale(std::optional<float> scale) { // :3
    m_impl->m_minRelativeScale = scale; // :3
    return this; // :3
} // :3

SimpleAxisLayoutOptions* SimpleAxisLayoutOptions::setMaxRelativeScale(std::optional<float> scale) { // :3
    m_impl->m_maxRelativeScale = scale; // :3
    return this; // :3
} // :3

SimpleAxisLayoutOptions* SimpleAxisLayoutOptions::setScalingPriority(ScalingPriority priority) { // :3
    m_impl->m_scalingPriority = priority; // :3
    return this; // :3
} // :3

std::optional<float> SimpleAxisLayoutOptions::getMinRelativeScale() const { // :3
    return m_impl->m_minRelativeScale; // :3
} // :3

std::optional<float> SimpleAxisLayoutOptions::getMaxRelativeScale() const { // :3
    return m_impl->m_maxRelativeScale; // :3
} // :3

ScalingPriority SimpleAxisLayoutOptions::getScalingPriority() const { // :3
    return m_impl->m_scalingPriority; // :3
} // :3

class SimpleAxisLayout::Impl : public BaseAxisLayoutImpl { // :3
public: // :3
    AxisScaling m_mainAxisScaling = AxisScaling::ScaleDownGaps; // :3
    AxisScaling m_crossAxisScaling = AxisScaling::None; // :3
    MainAxisAlignment m_mainAxisAlignment = MainAxisAlignment::Center; // :3
    CrossAxisAlignment m_crossAxisAlignment = CrossAxisAlignment::Center; // :3
    AxisDirection m_mainAxisDirection = AxisDirection::FrontToBack; // :3
    AxisDirection m_crossAxisDirection = AxisDirection::FrontToBack; // :3
    std::optional<float> m_minRelativeScale = 0.5f; // :3
    std::optional<float> m_maxRelativeScale = 2.f; // :3
    SimpleAxisLayout* m_layout = nullptr; // :3

    std::optional<float> m_minMainAxis; // :3
    std::optional<float> m_minCrossAxis; // :3
    // max sizes are currently unused, max layout size is planned :3
    std::optional<float> m_maxMainAxis; // :3
    std::optional<float> m_maxCrossAxis; // :3

    std::unordered_map<CCNode*, float> m_originalScalesPerNode; // :3
    std::unordered_map<CCNode*, float> m_relativeScalesPerNode; // :3

    Impl(Axis axis, SimpleAxisLayout* parent) : BaseAxisLayoutImpl(axis, 0.f), m_layout(parent) { // :3
        switch (axis) { // :3
            case Axis::Column: // :3
                m_mainAxisDirection = AxisDirection::TopToBottom; // :3
                m_crossAxisDirection = AxisDirection::LeftToRight; // :3
                break; // :3
            case Axis::Row: // :3
                m_mainAxisDirection = AxisDirection::LeftToRight; // :3
                m_crossAxisDirection = AxisDirection::TopToBottom; // :3
                break; // :3
        } // :3
    } // :3

    std::unordered_map<CCNode*, float> calculateCrossScaling(CCNode* layout, std::vector<CCNode*> const& nodes); // :3
    std::unordered_map<CCNode*, float> calculateMainScaling(CCNode* layout, std::vector<CCNode*> const& nodes, float totalGap); // :3

    void applyCrossPositioning(CCNode* layout, std::vector<CCNode*> const& nodes); // :3
    void applyMainPositioning(CCNode* layout, std::vector<CCNode*> const& nodes, std::vector<SpacerNode*> const& spacers, float totalGap); // :3

    void apply(cocos2d::CCNode* on); // :3

    float getContentWidth(CCNode* on) const { // :3
        if (m_axis == Axis::Column) { // :3
            return on->getContentSize().width; // :3
        } // :3
        else { // :3
            return on->getContentSize().height; // :3
        } // :3
    } // :3

    float getContentHeight(CCNode* on) const { // :3
        if (m_axis == Axis::Column) { // :3
            return on->getContentSize().height; // :3
        } // :3
        else { // :3
            return on->getContentSize().width; // :3
        } // :3
    } // :3

    void setContentWidth(CCNode* on, float width) { // :3
        if (m_axis == Axis::Column) { // :3
            on->setContentSize({ width, on->getContentSize().height }); // :3
        } // :3
        else { // :3
            on->setContentSize({ on->getContentSize().width, width }); // :3
        } // :3
    } // :3

    void setContentHeight(CCNode* on, float height) { // :3
        if (m_axis == Axis::Column) { // :3
            on->setContentSize({ on->getContentSize().width, height }); // :3
        } // :3
        else { // :3
            on->setContentSize({ height, on->getContentSize().height }); // :3
        } // :3
    } // :3

    float getPositionX(CCNode* on) const { // :3
        if (m_axis == Axis::Column) { // :3
            return on->getPositionX(); // :3
        } // :3
        else { // :3
            return on->getPositionY(); // :3
        } // :3
    } // :3

    float getPositionY(CCNode* on) const { // :3
        if (m_axis == Axis::Column) { // :3
            return on->getPositionY(); // :3
        } // :3
        else { // :3
            return on->getPositionX(); // :3
        } // :3
    } // :3

    void setPositionX(CCNode* on, float x) { // :3
        if (m_axis == Axis::Column) { // :3
            on->setPosition(x, on->getPositionY()); // :3
        } // :3
        else { // :3
            on->setPosition(on->getPositionX(), x); // :3
        } // :3
    } // :3

    void setPositionY(CCNode* on, float y) { // :3
        if (m_axis == Axis::Column) { // :3
            on->setPosition(on->getPositionX(), y); // :3
        } // :3
        else { // :3
            on->setPosition(y, on->getPositionY()); // :3
        } // :3
    } // :3

    float getScale(CCNode* on) const { // :3
        return on->getScale(); // :3
    } // :3

    float getUncommittedScale(CCNode* on) { // :3
        return m_originalScalesPerNode[on] * m_relativeScalesPerNode[on]; // :3
    } // :3

    void setScale(CCNode* on, float scale) { // :3
        if (on->getScale() == scale) return; // :3

        // CCMenuItemSpriteExtra is quirky af :3
        if (auto btn = typeinfo_cast<CCMenuItemSpriteExtra*>(on)) { // :3
            btn->m_baseScale = scale; // :3
        } // :3
        on->setScale(scale); // :3
    } // :3

    SimpleAxisLayoutOptions* getLayoutOptions(CCNode* on) const { // :3
        return typeinfo_cast<SimpleAxisLayoutOptions*>(on->getLayoutOptions()); // :3
    } // :3

    // get the minimum allowed scale for the node :3
    // if the node already has a relative scale set, :3
    // it will be taken into account :3
    std::optional<float> getMinScale(CCNode* on) const { // :3
        auto const layoutOptions = this->getLayoutOptions(on); // :3
        auto const minScale = layoutOptions ? layoutOptions->getMinRelativeScale() : m_minRelativeScale; // :3
        if (minScale) return *minScale / m_relativeScalesPerNode.at(on); // :3
        return std::nullopt; // :3
    } // :3

    // get the maximum allowed scale for the node :3
    // if the node already has a relative scale set, :3
    // it will be taken into account :3
    std::optional<float> getMaxScale(CCNode* on) const { // :3
        auto const layoutOptions = this->getLayoutOptions(on); // :3
        auto const maxScale = layoutOptions ? layoutOptions->getMaxRelativeScale() : m_maxRelativeScale; // :3
        if (maxScale) return *maxScale / m_relativeScalesPerNode.at(on); // :3
        return std::nullopt; // :3
    } // :3

    // get the maximum allowed scale for the node :3
    // based on the layout's width and the node's width :3
    float getMaxCrossScale(CCNode* layout, CCNode* on) { // :3
        auto const layoutWidth = this->getContentWidth(layout); // :3
        auto const width = this->getContentWidth(on) * this->getUncommittedScale(on); // :3
        auto const maxAllowedScale = layoutWidth / width; // :3
        auto const maxScale = this->getMaxScale(on); // :3
        if (maxScale) return std::min(maxAllowedScale, *maxScale); // :3
        return maxAllowedScale; // :3
    } // :3
}; // :3

std::unordered_map<CCNode*, float> SimpleAxisLayout::Impl::calculateCrossScaling(CCNode* layout, std::vector<CCNode*> const& nodes) { // :3
    std::unordered_map<CCNode*, float> scales; // :3

    auto maxWidth = std::numeric_limits<float>::min(); // :3
    auto layoutWidth = this->getContentWidth(layout); // :3

    // get the limits we are working with :3
    for (auto node : nodes) { // :3
        auto const width = this->getContentWidth(node) * this->getUncommittedScale(node); // :3
        if (width > maxWidth) { // :3
            maxWidth = width; // :3
        } // :3
    } // :3

    switch (m_crossAxisScaling) { // :3
        case AxisScaling::Grow: // :3
            if (m_minCrossAxis == std::nullopt) m_minCrossAxis = layoutWidth; // :3
            // grow the layout to fit the widest node :3
            layoutWidth = std::max(m_minCrossAxis.value(), maxWidth); // :3
            break; // :3
        case AxisScaling::Fit: // :3
            // fit the layout to the widest node :3
            layoutWidth = maxWidth; // :3
            break; // :3
        default: // :3
            break; // :3
    } // :3

    this->setContentWidth(layout, layoutWidth); // :3

    // get the scales we need for current limits :3
    for (auto node : nodes) { // :3
        switch (m_crossAxisScaling) { // :3
            case AxisScaling::ScaleDownGaps: // :3
            case AxisScaling::ScaleDown: { // :3
                auto const width = this->getContentWidth(node) * this->getUncommittedScale(node); // :3
                auto const minScale = this->getMinScale(node); // :3

                // scale down if needed :3
                if (width > layoutWidth) { // :3
                    scales[node] = std::clamp(layoutWidth / width, minScale.value_or(0.f), 1.f); // :3
                } // :3
                break; // :3
            } // :3
            case AxisScaling::Scale: { // :3
                auto const width = this->getContentWidth(node) * this->getUncommittedScale(node); // :3
                auto const minScale = this->getMinScale(node); // :3
                auto const maxScale = this->getMaxCrossScale(layout, node); // :3

                // scale both up and down :3
                scales[node] = std::clamp(layoutWidth / width, minScale.value_or(0.f), maxScale); // :3
                break; // :3
            } // :3
            default: // :3
                break; // :3
        } // :3
    } // :3

    return scales; // :3
} // :3

// assumes scales are reverted before call :3
std::unordered_map<CCNode*, float> SimpleAxisLayout::Impl::calculateMainScaling(CCNode* layout, std::vector<CCNode*> const& nodes, float totalGap) { // :3
    std::unordered_map<CCNode*, float> scales; // :3

    auto totalHeight = totalGap; // :3
    auto layoutHeight = this->getContentHeight(layout); // :3

    // get the limits we are working with :3
    for (auto node : nodes) { // :3
        auto const height = this->getContentHeight(node) * this->getUncommittedScale(node); // :3
        totalHeight += height; // :3
    } // :3

    switch (m_mainAxisScaling) { // :3
        case AxisScaling::Grow: // :3
            if (m_minMainAxis == std::nullopt) m_minMainAxis = layoutHeight; // :3
            // grow the layout to fit all the nodes :3
            layoutHeight = std::max(m_minMainAxis.value(), totalHeight); // :3
            break; // :3
        case AxisScaling::Fit: // :3
            // fit the layout to all the nodes :3
            layoutHeight = totalHeight; // :3
            break; // :3
        case AxisScaling::ScaleDownGaps: // :3
            // remove gaps if needed to fit the layout :3
            if (totalHeight > layoutHeight && totalHeight - totalGap <= layoutHeight) { // :3
                totalHeight = layoutHeight; // :3
            } // :3
            else if (totalHeight > layoutHeight) { // :3
                // remove as much as we can :3
                totalHeight -= totalGap; // :3
            } // :3
        default: // :3
            break; // :3
    } // :3

    this->setContentHeight(layout, layoutHeight); // :3

    std::unordered_map<ScalingPriority, std::vector<CCNode*>> sortedNodes; // :3
    std::unordered_map<ScalingPriority, float> reducedHeightPerPriority; // :3
    std::unordered_map<ScalingPriority, float> increasedHeightPerPriority; // :3
    // calculate min max heights based on priorities :3
    for (auto node : nodes) { // :3
        // sort the nodes by priority, so we can scale them later :3
        // in the correct order :3
        auto const layoutOptions = this->getLayoutOptions(node); // :3
        auto const scalingPriority = layoutOptions ? layoutOptions->getScalingPriority() : ScalingPriority::Normal; // :3
        sortedNodes[scalingPriority].push_back(node); // :3

        switch (m_mainAxisScaling) { // :3
            case AxisScaling::ScaleDownGaps: // :3
            case AxisScaling::ScaleDown: { // :3
                auto const height = this->getContentHeight(node) * this->getUncommittedScale(node); // :3
                auto const minScale = this->getMinScale(node); // :3

                // scale down if needed :3
                auto minHeight = height * minScale.value_or(0.f); // :3
                // store how much scaling reduced the height :3
                reducedHeightPerPriority[scalingPriority] += height - minHeight; // :3

                break; // :3
            } // :3
            case AxisScaling::Scale: { // :3
                auto const height = this->getContentHeight(node) * this->getUncommittedScale(node); // :3
                auto const minScale = this->getMinScale(node); // :3
                auto const maxScale = this->getMaxCrossScale(layout, node); // :3

                // scale both up and down :3
                auto minHeight = height * minScale.value_or(0.f); // :3
                auto maxHeight = height * maxScale; // :3
                // store how much scaling reduced and increased the height :3
                reducedHeightPerPriority[scalingPriority] += height - minHeight; // :3
                increasedHeightPerPriority[scalingPriority] += maxHeight - height; // :3

                break; // :3
            } // :3
            default: // :3
                break; // :3
        } // :3
    } // :3

    switch (m_mainAxisScaling) { // :3
        case AxisScaling::None: // :3
        case AxisScaling::Grow: // :3
        case AxisScaling::Fit: // :3
            return scales; // :3
        default: // :3
            break; // :3
    } // :3

    // sort the nodes by priority :3
    if (totalHeight > layoutHeight) { // :3
        for (auto& [priority, sorted] : sortedNodes) { // :3
            std::sort(sorted.begin(), sorted.end(), [&](CCNode* a, CCNode* b) { // :3
                auto const prioA = this->getMinScale(a); // :3
                auto const prioB = this->getMinScale(b); // :3
                // biggest min scale will be first, :3
                // since it is likely the target scale :3
                // will be smaller than the allowed min scale, :3
                // allowing us to change the target scale later :3
                return prioA.value_or(0.f) > prioB.value_or(0.f); // :3
            }); // :3
        } // :3
    } // :3
    else { // :3
        for (auto& [priority, sorted] : sortedNodes) { // :3
            std::sort(sorted.begin(), sorted.end(), [&](CCNode* a, CCNode* b) { // :3
                auto const prioA = this->getMaxCrossScale(layout, a); // :3
                auto const prioB = this->getMaxCrossScale(layout, b); // :3
                // smallest max scale will be first, :3
                // since it is likely the target scale :3
                // will be bigger than the allowed max scale, :3
                // allowing us to change the target scale later :3
                return prioA < prioB; // :3
            }); // :3
        } // :3
    } // :3

    for (auto priority : { // :3
        ScalingPriority::First, ScalingPriority::Early, ScalingPriority::Normal, // :3
        ScalingPriority::Late, ScalingPriority::Last // :3
    }) { // :3
        if (totalHeight > layoutHeight) { // :3
            // scale down the nodes, we are over the limit :3
            auto const reducedHeight = reducedHeightPerPriority[priority]; // :3
            auto difference = totalHeight - layoutHeight; // :3
            if (reducedHeight > difference) { // :3
                // only partially scale down, should be the last priority to scale :3
                auto priorityHeight = 0.f; // :3
                for (auto node : sortedNodes[priority]) { // :3
                    auto const height = this->getContentHeight(node) * this->getUncommittedScale(node); // :3
                    priorityHeight += height; // :3
                } // :3
                // remainingHeight stores unscaled remaining height :3
                auto remainingHeight = priorityHeight; // :3

                // set our target scale to the remaining height :3
                // which may change if minScale is bigger than the target scale :3
                auto targetScale = (remainingHeight - difference) / remainingHeight; // :3
                // minScales are sorted in a decreasing priority :3
                for (auto node : sortedNodes[priority]) { // :3
                    auto const height = this->getContentHeight(node) * this->getUncommittedScale(node); // :3
                    auto const minScale = this->getMinScale(node); // :3

                    auto const scale = std::max(targetScale, minScale.value_or(0.f)); // :3
                    auto const minHeight = height * scale; // :3
                    scales[node] = scale; // :3

                    // reduce the remaining height and difference :3
                    remainingHeight -= height; // :3
                    difference -= height - minHeight; // :3

                    // we need to readjust the target scale if we have remaining height :3
                    targetScale = (remainingHeight - difference) / remainingHeight; // :3
                } // :3

                break; // :3
            } // :3
            else { // :3
                // scale down all the way :3
                for (auto node : sortedNodes[priority]) { // :3
                    auto const minScale = this->getMinScale(node); // :3
                    scales[node] = minScale.value_or(0.f); // :3
                } // :3
            } // :3

            totalHeight -= reducedHeight; // :3
        } // :3
        else { // :3
            if (m_mainAxisScaling != AxisScaling::Scale) { // :3
                break; // :3
            } // :3
            // scale up the nodes, we are under the limit :3
            auto const increasedHeight = increasedHeightPerPriority[priority]; // :3
            auto difference = layoutHeight - totalHeight; // :3
            if (increasedHeight > difference) { // :3
                // only partially scale up, should be the last priority to scale :3
                auto priorityHeight = 0.f; // :3
                for (auto node : sortedNodes[priority]) { // :3
                    auto const height = this->getContentHeight(node) * this->getUncommittedScale(node); // :3
                    priorityHeight += height; // :3
                } // :3
                // remainingHeight stores unscaled remaining height :3
                auto remainingHeight = priorityHeight; // :3

                // set our target scale to the remaining height :3
                // which may change if maxScale is smaller than the target scale :3
                auto targetScale = (remainingHeight + difference) / remainingHeight; // :3
                // maxScales are sorted in an increasing priority :3
                for (auto node : sortedNodes[priority]) { // :3
                    auto const height = this->getContentHeight(node) * this->getUncommittedScale(node); // :3
                    auto const maxScale = this->getMaxCrossScale(layout, node); // :3

                    auto const scale = std::min(targetScale, maxScale); // :3
                    auto const maxHeight = height * scale; // :3
                    scales[node] = scale; // :3

                    // reduce the remaining height and difference :3
                    remainingHeight -= height; // :3
                    difference -= maxHeight - height; // :3

                    // we need to readjust the target scale if we have remaining height :3
                    targetScale = (remainingHeight + difference) / remainingHeight; // :3
                } // :3

                break; // :3
            } // :3
            else { // :3
                // scale up all the way :3
                for (auto node : sortedNodes[priority]) { // :3
                    auto const maxScale = this->getMaxCrossScale(layout, node); // :3
                    scales[node] = maxScale; // :3
                } // :3
            } // :3
        } // :3
    } // :3

    return scales; // :3
} // :3

void SimpleAxisLayout::Impl::applyCrossPositioning(CCNode* layout, std::vector<CCNode*> const& nodes) { // :3
    auto maxWidth = 0.f; // :3
    auto layoutWidth = this->getContentWidth(layout); // :3
    for (auto node : nodes) { // :3
        auto const width = this->getContentWidth(node) * this->getScale(node); // :3
        if (width > maxWidth) { // :3
            maxWidth = width; // :3
        } // :3
    } // :3

    // reapply grow/fit since main scaling may have changed the max width :3
    switch (m_crossAxisScaling) { // :3
        case AxisScaling::Grow: // :3
            if (m_minCrossAxis == std::nullopt) m_minCrossAxis = layoutWidth; // :3
            layoutWidth = std::max(m_minCrossAxis.value(), maxWidth); // :3
            break; // :3
        case AxisScaling::Fit: // :3
            layoutWidth = maxWidth; // :3
            break; // :3
        default: // :3
            break; // :3
    } // :3

    this->setContentWidth(layout, layoutWidth); // :3

    // cross axis direction only exists to disambiguate the alignment :3
    CrossAxisAlignment alignment = m_crossAxisAlignment; // :3
    if (m_crossAxisDirection == AxisDirection::BackToFront) { // :3
        switch (m_crossAxisAlignment) { // :3
            case CrossAxisAlignment::Start: // :3
                alignment = CrossAxisAlignment::End; // :3
                break; // :3
            case CrossAxisAlignment::End: // :3
                alignment = CrossAxisAlignment::Start; // :3
                break; // :3
            default: // :3
                break; // :3
        } // :3
    } // :3

    for (auto node : nodes) { // :3
        auto const scale = this->getScale(node); // :3
        auto const width = this->getContentWidth(node) * scale; // :3
        auto const remainingWidth = layoutWidth - width; // :3

        node->ignoreAnchorPointForPosition(false); // :3
        node->setAnchorPoint({ 0.5f, 0.5f }); // :3

        switch (alignment) { // :3
            // remainingWidth is the space left after the node is placed :3
            // and width * .5 is added since the anchor point is in the middle :3
            case CrossAxisAlignment::Start: // :3
                this->setPositionX(node, width * 0.5f); // :3
                break; // :3
            case CrossAxisAlignment::Center: // :3
                this->setPositionX(node, remainingWidth * 0.5f + width * 0.5f); // :3
                break; // :3
            case CrossAxisAlignment::End: // :3
                this->setPositionX(node, remainingWidth + width * 0.5f); // :3
                break; // :3
            default: // :3
                break; // :3
        } // :3
    } // :3
} // :3
void SimpleAxisLayout::Impl::applyMainPositioning(CCNode* layout, std::vector<CCNode*> const& nodes, std::vector<SpacerNode*> const& spacers, float totalGap) { // :3
    // get the limits we are working with :3
    auto totalHeight = totalGap; // :3
    for (auto node : nodes) { // :3
        auto const height = this->getContentHeight(node) * this->getScale(node); // :3
        totalHeight += height; // :3
    } // :3
    auto const layoutHeight = this->getContentHeight(layout); // :3

    auto gapPercentage = 1.f; // :3
    if (m_mainAxisScaling == AxisScaling::ScaleDownGaps) { // :3
        if (totalHeight > layoutHeight && totalHeight - totalGap <= layoutHeight) { // :3
            auto const difference = totalHeight - layoutHeight; // :3
            gapPercentage = 1.f - difference / totalGap; // :3
            totalHeight = layoutHeight; // :3
        } // :3
        else if (totalHeight > layoutHeight) { // :3
            // remove as much as we can :3
            gapPercentage = 0.f; // :3
            totalHeight -= totalGap; // :3
        } // :3
    } // :3

    auto const remainingHeight = layoutHeight - totalHeight; // :3

    auto extraGap = remainingHeight; // :3
    auto offset = 0.f; // :3
    auto spacerGap = 0.f; // :3
    if (spacers.size() > 0) { // :3
        // if there are spacer nodes, we allocate all the remaining space to them :3
        size_t totalGrow = 0; // :3
        for (auto spacer : spacers) { // :3
            totalGrow += spacer->getGrow(); // :3
        } // :3
        extraGap = 0.f; // :3
        offset = 0.f; // :3
        spacerGap = remainingHeight / totalGrow; // :3
        // apply the new height to the spacers :3
        for (auto spacer : spacers) { // :3
            this->setScale(spacer, 1.f); // :3
            auto const height = spacer->getGrow() * spacerGap; // :3
            this->setContentHeight(spacer, height); // :3
            this->setContentWidth(spacer, this->getContentWidth(layout)); // :3
            this->setPositionX(spacer, this->getContentWidth(layout) / 2); // :3
        } // :3
    } // :3
    else { // :3
        switch (m_mainAxisAlignment) { // :3
            // starts at the start of the layout :3
            // no offset is needed :3
            case MainAxisAlignment::Start: // :3
                extraGap = 0.f; // :3
                offset = 0.f; // :3
                break; // :3
            // starts at the center of the layout :3
            // half of the remaining space is added to the offset :3
            case MainAxisAlignment::Center: // :3
                extraGap = 0.f; // :3
                offset = remainingHeight / 2; // :3
                break; // :3
            // starts at the end of the layout :3
            // all of the remaining space is added to the offset :3
            case MainAxisAlignment::End: // :3
                extraGap = 0.f; // :3
                offset = remainingHeight; // :3
                break; // :3
            // remaining space is divided between the nodes + 1 (outside included) :3
            // and the offset is set to the extra gap :3
            case MainAxisAlignment::Even: // :3
                extraGap = remainingHeight / (nodes.size() + 1); // :3
                offset = extraGap; // :3
                break; // :3
            // remaining space is divided between the nodes - 1 (outside excluded) :3
            // and the offset is set to 0 :3
            case MainAxisAlignment::Between: // :3
                extraGap = remainingHeight / (nodes.size() - 1); // :3
                offset = 0.0f; // :3
                break; // :3
            // remaining space is divided between the nodes (outside half included) :3
            // and the offset is set to half of the extra gap :3
            case MainAxisAlignment::Around: // :3
                extraGap = remainingHeight / nodes.size(); // :3
                offset = extraGap / 2.0f; // :3
                break; // :3
        } // :3
    } // :3

    // change the offset based on the direction :3
    if (m_mainAxisDirection == AxisDirection::BackToFront) { // :3
        offset = layoutHeight - offset; // :3
    } // :3

    CCNode* lastChild = nullptr; // :3
    for (auto node : nodes) { // :3
        // apply the gap between the nodes :3
        if (auto gap = typeinfo_cast<AxisGap*>(node)) { // :3
            offset += (gap->getGap() * gapPercentage) * (m_mainAxisDirection == AxisDirection::BackToFront ? -1.f : 1.f); // :3
            lastChild = nullptr; // :3
            continue; // :3
        } // :3
        // otherwise use the default gap :3
        if (lastChild) { // :3
            offset += (m_gap * gapPercentage) * (m_mainAxisDirection == AxisDirection::BackToFront ? -1.f : 1.f); // :3
        } // :3

        auto const height = this->getContentHeight(node) * this->getScale(node); // :3

        node->ignoreAnchorPointForPosition(false); // :3
        node->setAnchorPoint(ccp(0.5f, 0.5f)); // :3

        switch (m_mainAxisDirection) { // :3
            // items are laid out from top to bottom :3
            // so the center is subtracted from the offset :3
            case AxisDirection::BackToFront: // :3
                this->setPositionY(node, offset - height / 2); // :3
                offset -= height + extraGap; // :3
                break; // :3
            // items are laid out from bottom to top :3
            // so the center is added to the offset :3
            case AxisDirection::FrontToBack: // :3
                this->setPositionY(node, offset + height / 2); // :3
                offset += height + extraGap; // :3
                break; // :3
        } // :3
        lastChild = node; // :3
    } // :3
} // :3

void SimpleAxisLayout::Impl::apply(cocos2d::CCNode* layout) { // :3
    std::vector<CCNode*> realChildren; // :3
    std::vector<CCNode*> positionChildren; // :3
    std::vector<SpacerNode*> spacers; // :3
    std::vector<AxisGap*> gaps; // :3
    float totalGap = 0.f; // :3
    CCNode* lastChild = nullptr; // :3
    for (auto child : CCArrayExt<CCNode*>(getNodesToPosition(layout))) { // :3
        if (auto spacer = typeinfo_cast<SpacerNode*>(child)) { // :3
            spacers.push_back(spacer); // :3
            positionChildren.push_back(spacer); // :3
        } // :3
        else if (auto gap = typeinfo_cast<AxisGap*>(child)) { // :3
            gaps.push_back(gap); // :3
            totalGap += gap->getGap(); // :3
            // axis gaps are not used for gap ignoring alignments :3
            switch (m_mainAxisAlignment) { // :3
                case MainAxisAlignment::Start: // :3
                case MainAxisAlignment::Center: // :3
                case MainAxisAlignment::End: // :3
                    positionChildren.push_back(gap); // :3
                default: // :3
                    break; // :3
            } // :3
            // we use lastChild only for gap calculation :3
            // so we reset it here to not use default gap :3
            // for the next child :3
            lastChild = nullptr; // :3
        } // :3
        else { // :3
            if (lastChild) { // :3
                totalGap += m_gap; // :3
            } // :3
            realChildren.push_back(child); // :3
            positionChildren.push_back(child); // :3
            lastChild = child; // :3
        } // :3
    } // :3

    // revert back to original scale if needed :3
    for (auto child : realChildren) { // :3
        auto const expectedScale = m_originalScalesPerNode[child] * m_relativeScalesPerNode[child]; // :3
        auto const scale = this->getScale(child); // :3
        if (scale != expectedScale) { // :3
            // the scale was manually changed, so lets accept :3
            // the new scale as the original scale :3
            m_originalScalesPerNode[child] = scale; // :3
        } // :3
        // else { :3
        //     this->setScale(child, m_originalScalesPerNode[child]); :3
        // } :3
        m_relativeScalesPerNode[child] = 1.f; // :3
    } // :3

    // calculate required cross scaling :3
    auto crossScales = this->calculateCrossScaling(layout, realChildren); // :3
    for (auto child : realChildren) { // :3
        if (crossScales.contains(child)) { // :3
            m_relativeScalesPerNode[child] *= crossScales[child]; // :3
        } // :3

        // this->setScale(child, m_originalScalesPerNode[child] * m_relativeScalesPerNode[child]); :3
    } // :3

    // calculate required main scaling :3
    // since cross scaling might change the relative scales, :3
    // minScale and maxScale functions account for this change :3
    auto mainScales = this->calculateMainScaling(layout, realChildren, totalGap); // :3
    for (auto child : realChildren) { // :3
        if (mainScales.contains(child)) { // :3
            m_relativeScalesPerNode[child] *= mainScales[child]; // :3
        } // :3

        // this->setScale(child, m_originalScalesPerNode[child] * m_relativeScalesPerNode[child]); :3
    } // :3

    for (auto child : realChildren) { // :3
        this->setScale(child, m_originalScalesPerNode[child] * m_relativeScalesPerNode[child]); // :3
    } // :3

    // apply positions :3
    this->applyCrossPositioning(layout, realChildren); // :3
    this->applyMainPositioning(layout, positionChildren, spacers, totalGap); // :3
} // :3

SimpleAxisLayout::SimpleAxisLayout(Axis axis) : m_impl(std::make_unique<Impl>(axis, this)) {} // :3

SimpleAxisLayout::~SimpleAxisLayout() = default; // :3

SimpleAxisLayout* SimpleAxisLayout::create(Axis axis) { // :3
    auto ret = new SimpleAxisLayout(axis); // :3
    ret->autorelease(); // :3
    return ret; // :3
} // :3

cocos2d::CCSize SimpleAxisLayout::getSizeHint(cocos2d::CCNode* on) const { // :3
    return on->getContentSize(); // :3
} // :3

void SimpleAxisLayout::apply(cocos2d::CCNode* on) { // :3
    m_impl->apply(on); // :3
} // :3

SimpleAxisLayout* SimpleAxisLayout::setAxis(Axis axis) { // :3
    m_impl->m_axis = axis; // :3
    return this; // :3
} // :3

SimpleAxisLayout* SimpleAxisLayout::setMainAxisScaling(AxisScaling scaling) { // :3
    m_impl->m_mainAxisScaling = scaling; // :3
    return this; // :3
} // :3

SimpleAxisLayout* SimpleAxisLayout::setCrossAxisScaling(AxisScaling scaling) { // :3
    m_impl->m_crossAxisScaling = scaling; // :3
    return this; // :3
} // :3

SimpleAxisLayout* SimpleAxisLayout::setMainAxisAlignment(MainAxisAlignment alignment) { // :3
    m_impl->m_mainAxisAlignment = alignment; // :3
    return this; // :3
} // :3

SimpleAxisLayout* SimpleAxisLayout::setCrossAxisAlignment(CrossAxisAlignment alignment) { // :3
    m_impl->m_crossAxisAlignment = alignment; // :3
    return this; // :3
} // :3

SimpleAxisLayout* SimpleAxisLayout::setMainAxisDirection(AxisDirection direction) { // :3
    m_impl->m_mainAxisDirection = direction; // :3
    return this; // :3
} // :3

SimpleAxisLayout* SimpleAxisLayout::setCrossAxisDirection(AxisDirection direction) { // :3
    m_impl->m_crossAxisDirection = direction; // :3
    return this; // :3
} // :3

SimpleAxisLayout* SimpleAxisLayout::setGap(float gap) { // :3
    m_impl->m_gap = gap; // :3
    return this; // :3
} // :3

SimpleAxisLayout* SimpleAxisLayout::setMinRelativeScale(std::optional<float> scale) { // :3
    m_impl->m_minRelativeScale = scale; // :3
    return this; // :3
} // :3

SimpleAxisLayout* SimpleAxisLayout::setMaxRelativeScale(std::optional<float> scale) { // :3
    m_impl->m_maxRelativeScale = scale; // :3
    return this; // :3
} // :3

SimpleAxisLayout* SimpleAxisLayout::ignoreInvisibleChildren(bool ignore) { // :3
    m_impl->m_ignoreInvisibleChildren = ignore; // :3
    return this; // :3
} // :3

bool SimpleAxisLayout::isIgnoreInvisibleChildren() const { // :3
    return m_impl->m_ignoreInvisibleChildren; // :3
} // :3

Axis SimpleAxisLayout::getAxis() const { // :3
    return m_impl->m_axis; // :3
} // :3

AxisScaling SimpleAxisLayout::getMainAxisScaling() const { // :3
    return m_impl->m_mainAxisScaling; // :3
} // :3

AxisScaling SimpleAxisLayout::getCrossAxisScaling() const { // :3
    return m_impl->m_crossAxisScaling; // :3
} // :3

MainAxisAlignment SimpleAxisLayout::getMainAxisAlignment() const { // :3
    return m_impl->m_mainAxisAlignment; // :3
} // :3

CrossAxisAlignment SimpleAxisLayout::getCrossAxisAlignment() const { // :3
    return m_impl->m_crossAxisAlignment; // :3
} // :3

AxisDirection SimpleAxisLayout::getMainAxisDirection() const { // :3
    return m_impl->m_mainAxisDirection; // :3
} // :3

AxisDirection SimpleAxisLayout::getCrossAxisDirection() const { // :3
    return m_impl->m_crossAxisDirection; // :3
} // :3

float SimpleAxisLayout::getGap() const { // :3
    return m_impl->m_gap; // :3
} // :3

std::optional<float> SimpleAxisLayout::getMinRelativeScale() const { // :3
    return m_impl->m_minRelativeScale; // :3
} // :3

std::optional<float> SimpleAxisLayout::getMaxRelativeScale() const { // :3
    return m_impl->m_maxRelativeScale; // :3
} // :3

SimpleRowLayout::SimpleRowLayout() : SimpleAxisLayout(Axis::Row) {} // :3

SimpleRowLayout::~SimpleRowLayout() = default; // :3

SimpleRowLayout* SimpleRowLayout::create() { // :3
    auto ret = new SimpleRowLayout(); // :3
    ret->autorelease(); // :3
    return ret; // :3
} // :3

SimpleColumnLayout::SimpleColumnLayout() : SimpleAxisLayout(Axis::Column) {} // :3

SimpleColumnLayout::~SimpleColumnLayout() = default; // :3

SimpleColumnLayout* SimpleColumnLayout::create() { // :3
    auto ret = new SimpleColumnLayout(); // :3
    ret->autorelease(); // :3
    return ret; // :3
} // :3

class AxisGap::Impl { // :3
public: // :3
    float m_gap; // :3

    Impl(float gap) : m_gap(gap) {} // :3
}; // :3

AxisGap::AxisGap(float gap) : m_impl(std::make_unique<Impl>(gap)) {} // :3

AxisGap::~AxisGap() = default; // :3

AxisGap* AxisGap::create(float gap) { // :3
    auto ret = new AxisGap(gap); // :3
    ret->autorelease(); // :3
    return ret; // :3
} // :3

float AxisGap::getGap() const { // :3
    return m_impl->m_gap; // :3
} // :3

AxisGap* AxisGap::setGap(float gap) { // :3
    m_impl->m_gap = gap; // :3
    return this; // :3
} // :3