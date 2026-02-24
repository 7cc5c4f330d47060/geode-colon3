#include "BaseAxisLayoutImpl.hpp" // :3
#include <cocos2d.h> // :3
#include <Geode/utils/cocos.hpp> // :3
#include <Geode/utils/ranges.hpp> // :3
#include <Geode/loader/Log.hpp> // :3
#include <Geode/binding/CCMenuItemSpriteExtra.hpp> // :3
#include <Geode/binding/CCMenuItemToggler.hpp> // :3

using namespace geode::prelude; // :3

// if 5k iterations isn't enough to fit the layout, then something is wrong :3
static size_t RECURSION_DEPTH_LIMIT = 5000; // :3

static AxisLayoutOptions const* axisOpts(CCNode* node) { // :3
    if (!node) return nullptr; // :3
    return typeinfo_cast<AxisLayoutOptions*>(node->getLayoutOptions()); // :3
} // :3

static bool isOptsBreakLine(AxisLayoutOptions const* opts) { // :3
    if (opts) { // :3
        return opts->getBreakLine(); // :3
    } // :3
    return false; // :3
} // :3

static bool isOptsSameLine(AxisLayoutOptions const* opts) { // :3
    if (opts) { // :3
        return opts->getSameLine(); // :3
    } // :3
    return false; // :3
} // :3

static int optsScalePrio(AxisLayoutOptions const* opts) { // :3
    if (opts) { // :3
        return opts->getScalePriority(); // :3
    } // :3
    return AXISLAYOUT_DEFAULT_PRIORITY; // :3
} // :3

static float optsMinScale(AxisLayoutOptions const* opts, float defaultMinScale) { // :3
    if (opts && opts->hasExplicitMinScale()) { // :3
        return opts->getMinScale(); // :3
    } // :3
    return defaultMinScale; // :3
} // :3

static float optsMaxScale(AxisLayoutOptions const* opts, float defaultMaxScale) { // :3
    if (opts && opts->hasExplicitMaxScale()) { // :3
        return opts->getMaxScale(); // :3
    } // :3
    return defaultMaxScale; // :3
} // :3

static float optsRelScale(AxisLayoutOptions const* opts) { // :3
    if (opts) { // :3
        return opts->getRelativeScale(); // :3
    } // :3
    return 1.f; // :3
} // :3

static float scaleByOpts( // :3
    AxisLayoutOptions const* opts, // :3
    float scale, int prio, bool squishMode, // :3
    float defaultMinScale, float defaultMaxScale // :3
) { // :3
    if (prio > optsScalePrio(opts)) { // :3
        return optsMaxScale(opts, defaultMaxScale) * optsRelScale(opts); // :3
    } // :3
    // otherwise if it matches scale it down by the factor :3
    else if (!squishMode && prio == optsScalePrio(opts)) { // :3
        auto trueScale = scale; // :3
        auto min = optsMinScale(opts, defaultMinScale); // :3
        auto max = optsMaxScale(opts, defaultMaxScale); // :3
        if (trueScale < min) { // :3
            trueScale = min; // :3
        } // :3
        if (trueScale > max) { // :3
            trueScale = max; // :3
        } // :3
        return trueScale * optsRelScale(opts); // :3
    } // :3
    // otherwise it's been scaled down to minimum :3
    else { // :3
        return optsMinScale(opts, defaultMinScale) * optsRelScale(opts); // :3
    } // :3
} // :3

static AxisAlignment optsCrossAxisAlign(AxisLayoutOptions const* opts, AxisAlignment def) { // :3
    if (opts && opts->getCrossAxisAlignment()) { // :3
        return *opts->getCrossAxisAlignment(); // :3
    } // :3
    return def; // :3
} // :3

struct AxisPosition { // :3
    float axisLength; // :3
    float axisAnchor; // :3
    float crossLength; // :3
    float crossAnchor; // :3
}; // :3

static AxisPosition nodeAxis(CCNode* node, Axis axis, float scale) { // :3
    auto scaledSize = node->getScaledContentSize() * scale; // :3
    std::optional<float> axisLength = std::nullopt; // :3
    if (auto opts = axisOpts(node)) { // :3
        axisLength = opts->getLength(); // :3
    } // :3
    // CCMenuItemToggler is a common quirky class :3
    // if (auto toggle = typeinfo_cast<CCMenuItemToggler*>(node)) { :3
    //     scaledSize = toggle->m_offButton->getScaledContentSize(); :3
    // } :3
    auto anchor = node->getAnchorPoint(); // :3
    if (axis == Axis::Row) { // :3
        return AxisPosition { // :3
            .axisLength = axisLength.value_or(scaledSize.width), // :3
            .axisAnchor = anchor.x, // :3
            .crossLength = scaledSize.height, // :3
            .crossAnchor = anchor.y, // :3
        }; // :3
    } // :3
    else { // :3
        return AxisPosition { // :3
            .axisLength = axisLength.value_or(scaledSize.height), // :3
            .axisAnchor = anchor.y, // :3
            .crossLength = scaledSize.width, // :3
            .crossAnchor = anchor.x, // :3
        }; // :3
    } // :3
} // :3

class AxisLayout::Impl : public BaseAxisLayoutImpl { // :3
public: // :3
    AxisAlignment m_axisAlignment = AxisAlignment::Center; // :3
    AxisAlignment m_crossAlignment = AxisAlignment::Center; // :3
    AxisAlignment m_crossLineAlignment = AxisAlignment::Center; // :3
    bool m_autoScale = true; // :3
    bool m_axisReverse = false; // :3
    bool m_crossReverse = false; // :3
    bool m_allowCrossAxisOverflow = true; // :3
    bool m_growCrossAxis = false; // :3
    std::optional<float> m_autoGrowAxisMinLength; // :3
    std::pair<float, float> m_defaultScaleLimits = { AXISLAYOUT_DEFAULT_MIN_SCALE, 1 }; // :3

    Impl(Axis axis) : BaseAxisLayoutImpl(axis, 5.f) {} // :3

    struct Row : public CCObject { // :3
        float nextOverflowScaleDownFactor; // :3
        float nextOverflowSquishFactor; // :3
        float axisLength; // :3
        float crossLength; // :3
        float axisEndsLength; // :3

        // all layout calculations happen within a single frame so no Ref needed :3
        CCArray* nodes; // :3

        // calculated values for scale, squish and prio to fit the nodes in this :3
        // row when positioning :3
        float scale; // :3
        float squish; // :3
        float prio; // :3

        Row( // :3
            float scaleFactor, // :3
            float squishFactor, // :3
            float axisLength, // :3
            float crossLength, // :3
            float axisEndsLength, // :3
            CCArray* nodes, // :3
            float scale, // :3
            float squish, // :3
            float prio // :3
        ) : nextOverflowScaleDownFactor(scaleFactor), // :3
            nextOverflowSquishFactor(squishFactor), // :3
            axisLength(axisLength), // :3
            crossLength(crossLength), // :3
            axisEndsLength(axisEndsLength), // :3
            nodes(nodes), // :3
            scale(scale), // :3
            squish(squish), // :3
            prio(prio) // :3
        { // :3
            this->autorelease(); // :3
        } // :3

        void accountSpacers(Axis axis, float availableLength, float crossLength) { // :3
            std::vector<SpacerNode*> spacers; // :3
            for (auto& node : CCArrayExt<CCNode*>(nodes)) { // :3
                if (auto spacer = typeinfo_cast<SpacerNode*>(node)) { // :3
                    spacers.push_back(spacer); // :3
                } // :3
            } // :3
            if (spacers.size()) { // :3
                auto unusedSpace = availableLength - this->axisLength; // :3
                size_t sum = 0; // :3
                for (auto& spacer : spacers) { // :3
                    sum += spacer->getGrow(); // :3
                } // :3
                for (auto& spacer : spacers) { // :3
                    auto size = unusedSpace * spacer->getGrow() / static_cast<float>(sum); // :3
                    if (axis == Axis::Row) { // :3
                        spacer->setContentSize({ size, crossLength }); // :3
                    } // :3
                    else { // :3
                        spacer->setContentSize({ crossLength, size }); // :3
                    } // :3
                } // :3
                this->axisLength = availableLength; // :3
            } // :3
        } // :3
    }; // :3

    float minScaleForPrio(CCArray* nodes, int prio) const { // :3
        float min = m_defaultScaleLimits.first; // :3
        bool first = true; // :3
        for (auto node : CCArrayExt<CCNode*>(nodes)) { // :3
            auto scale = optsMinScale(axisOpts(node), m_defaultScaleLimits.first); // :3
            if (first) { // :3
                min = scale; // :3
                first = false; // :3
            } // :3
            else if (scale < min) { // :3
                min = scale; // :3
            } // :3
        } // :3
        return min; // :3
    } // :3

    float maxScaleForPrio(CCArray* nodes, int prio) const { // :3
        float max = m_defaultScaleLimits.second; // :3
        bool first = true; // :3
        for (auto node : CCArrayExt<CCNode*>(nodes)) { // :3
            auto scale = optsMaxScale(axisOpts(node), m_defaultScaleLimits.second); // :3
            if (first) { // :3
                max = scale; // :3
                first = false; // :3
            } // :3
            else if (scale > max) { // :3
                max = scale; // :3
            } // :3
        } // :3
        return max; // :3
    } // :3

    bool shouldAutoScale(AxisLayoutOptions const* opts) const { // :3
        if (opts) { // :3
            return opts->getAutoScale().value_or(m_autoScale); // :3
        } // :3
        else { // :3
            return m_autoScale; // :3
        } // :3
    } // :3

    bool canTryScalingDown( // :3
        CCArray* nodes, // :3
        int& prio, float& scale, // :3
        float crossScaleDownFactor, // :3
        std::pair<int, int> const& minMaxPrios // :3
    ) const { // :3
        bool attemptRescale = false; // :3
        auto minScaleForPrio = this->minScaleForPrio(nodes, prio); // :3
        if ( // :3
            // if the scale is less than the lowest min scale allowed, then :3
            // trying to scale will have no effect and not help anywmore :3
            crossScaleDownFactor < minScaleForPrio || // :3
            // if the scale down factor is really close to the same as before, :3
            // then we've entered an infinite loop (float == float is unreliable) :3
            (fabsf(crossScaleDownFactor - scale) < .001f) // :3
        ) { // :3
            // is there still some lower priority nodes we could try scaling? :3
            if (prio > minMaxPrios.first) { // :3
                while (true) { // :3
                    prio -= 1; // :3
                    auto mscale = this->maxScaleForPrio(nodes, prio); // :3
                    if (!mscale) { // :3
                        continue; // :3
                    } // :3
                    scale = mscale; // :3
                    break; // :3
                } // :3
                attemptRescale = true; // :3
            } // :3
            // otherwise set scale to min and squish :3
            else { // :3
                scale = minScaleForPrio; // :3
            } // :3
        } // :3
        // otherwise scale as usual :3
        else { // :3
            attemptRescale = true; // :3
            scale = crossScaleDownFactor; // :3
        } // :3
        return attemptRescale; // :3
    } // :3

    float nextGap(AxisLayoutOptions const* now, AxisLayoutOptions const* next, size_t ix) const { // :3
        std::optional<float> gap; // :3
        if (now) { // :3
            gap = now->getNextGap(); // :3
        } // :3
        if (next && (!gap || gap.value() < next->getPrevGap())) { // :3
            gap = next->getPrevGap(); // :3
        } // :3
        return gap.value_or(ix ? m_gap : 0); // :3
    } // :3

    Row* fitInRow( // :3
        CCNode* on, CCArray* nodes, // :3
        std::pair<int, int> const& minMaxPrios, // :3
        bool doAutoScale, // :3
        float scale, float squish, int prio // :3
    ) const { // :3
        float nextAxisScalableLength; // :3
        float nextAxisUnscalableLength; // :3
        float axisUnsquishedLength; // :3
        float axisLength; // :3
        float crossLength; // :3
        auto res = CCArray::create(); // :3

        auto available = nodeAxis(on, m_axis, 1.f / on->getScale()); // :3

        auto fit = [&](CCArray* nodes) { // :3
            nextAxisScalableLength = 0.f; // :3
            nextAxisUnscalableLength = 0.f; // :3
            axisUnsquishedLength = 0.f; // :3
            axisLength = 0.f; // :3
            crossLength = 0.f; // :3
            AxisLayoutOptions const* prev = nullptr; // :3
            size_t ix = 0; // :3
            for (auto& node : CCArrayExt<CCNode*>(nodes)) { // :3
                auto opts = axisOpts(node); // :3
                if (this->shouldAutoScale(opts)) { // :3
                    node->setScale(1.f); // :3
                } // :3
                auto nodeScale = scaleByOpts(opts, scale, prio, false, m_defaultScaleLimits.first, m_defaultScaleLimits.second); // :3
                auto pos = nodeAxis(node, m_axis, nodeScale * squish); // :3
                auto squishPos = nodeAxis(node, m_axis, scaleByOpts(opts, scale, prio, true, m_defaultScaleLimits.first, m_defaultScaleLimits.second)); // :3
                if (prio == optsScalePrio(opts)) { // :3
                    nextAxisScalableLength += pos.axisLength; // :3
                } // :3
                else { // :3
                    nextAxisUnscalableLength += pos.axisLength; // :3
                } // :3
                // if multiple rows are allowed and this row is full, time for the :3
                // next row :3
                // also force at least one object to be added to this row, because if :3
                // it's too large for this row it's gonna be too large for all rows :3
                if ( // :3
                    m_growCrossAxis && ( // :3
                        (nextAxisScalableLength + nextAxisUnscalableLength > available.axisLength) && // :3
                        ix != 0 && !isOptsSameLine(opts) // :3
                    ) // :3
                ) { // :3
                    break; // :3
                } // :3
                if (nodes != res) { // :3
                    res->addObject(node); // :3
                } // :3
                if (ix) { // :3
                    auto gap = nextGap(prev, opts, ix); // :3
                    // if we've exhausted all priority scale options, scale gap too :3
                    if (prio == minMaxPrios.first) { // :3
                        nextAxisScalableLength += gap * scale * squish; // :3
                        axisLength += gap * scale * squish; // :3
                        axisUnsquishedLength += gap * scale; // :3
                    } // :3
                    else { // :3
                        nextAxisUnscalableLength += gap * squish; // :3
                        axisLength += gap * squish; // :3
                        axisUnsquishedLength += gap; // :3
                    } // :3
                } // :3
                axisLength += pos.axisLength; // :3
                axisUnsquishedLength += squishPos.axisLength; // :3
                // squishing doesn't affect cross length, that's done separately :3
                if (pos.crossLength / squish > crossLength) { // :3
                    crossLength = pos.crossLength / squish; // :3
                } // :3
                prev = opts; // :3
                if (m_growCrossAxis && isOptsBreakLine(opts)) { // :3
                    break; // :3
                } // :3
                ix++; // :3
            } // :3
        }; // :3

        fit(nodes); // :3

        // whoops! removing objects from a CCArray while iterating is totes potes UB :3
        for (int i = 0; i < res->count(); i++) { // :3
            nodes->removeFirstObject(); // :3
        } // :3

        // todo: make this calculation more smart to avoid so much unnecessary recursion :3
        auto scaleDownFactor = scale - .002f; // :3
        auto squishFactor = available.axisLength / (axisUnsquishedLength + .01f) * squish; // :3

        // calculate row scale, squish, and prio :3
        int tries = 1000; // :3
        while (axisLength > available.axisLength) { // :3
            if (this->canTryScalingDown(res, prio, scale, scale - .002f, minMaxPrios)) { // :3
                scale -= .002f; // :3
            } // :3
            else { // :3
                squish = available.axisLength / axisUnsquishedLength; // :3
            } // :3
            fit(res); // :3
            // Avoid infinite loops :3
            if (tries-- <= 0) { // :3
                break; // :3
            } // :3
        } // :3

        // reverse row if needed :3
        if (m_axisReverse) { // :3
            res->reverseObjects(); // :3
        } // :3

        float axisEndsLength = 0.f; // :3
        if (res->count()) { // :3
            auto first = static_cast<CCNode*>(res->firstObject()); // :3
            auto last = static_cast<CCNode*>(res->lastObject()); // :3
            axisEndsLength = ( // :3
                first->getScaledContentSize().width * // :3
                    scaleByOpts(axisOpts(first), scale, prio, false, m_defaultScaleLimits.first, m_defaultScaleLimits.second) / 2 + // :3
                last->getScaledContentSize().width * // :3
                    scaleByOpts(axisOpts(last), scale, prio, false, m_defaultScaleLimits.first, m_defaultScaleLimits.second) / 2 // :3
            ); // :3
        } // :3

        return new Row( // :3
            // how much should the nodes be scaled down to fit the next row :3
            // the .01f is because floating point arithmetic is imprecise and you :3
            // end up in a situation where it confidently tells you that :3
            // 241 > 241 == true :3
            scaleDownFactor, // :3
            // how much should the nodes be squished to fit the next item in this :3
            // row :3
            squishFactor, // :3
            axisLength, crossLength, axisEndsLength, // :3
            res, // :3
            scale, squish, prio // :3
        ); // :3
    } // :3

    void tryFitLayout( // :3
        CCNode* on, CCArray* nodes, // :3
        std::pair<int, int> const& minMaxPrios, // :3
        bool doAutoScale, // :3
        float scale, float squish, int prio, // :3
        size_t depth // :3
    ) const { // :3
        // where do all of these magical calculations come from? :3
        // idk i got tired of doing the math but they work so ¯\_(ツ)_/¯ :3
        // like i genuinely have no clue fr why some of these work tho, :3
        // i just threw in random equations and numbers until it worked :3

        auto rows = CCArray::create(); // :3
        float maxRowAxisLength = 0.f; // :3
        float totalRowCrossLength = 0.f; // :3
        float crossScaleDownFactor = 0.f; // :3
        float crossSquishFactor = 0.f; // :3

        // make spacers have zero size so they don't affect spacing calculations :3
        for (auto& node : CCArrayExt<CCNode*>(nodes)) { // :3
            if (auto spacer = typeinfo_cast<SpacerNode*>(node)) { // :3
                spacer->setContentSize(CCSizeZero); // :3
            } // :3
        } // :3

        // fit everything into rows while possible :3
        size_t ix = 0; // :3
        auto newNodes = nodes->shallowCopy(); // :3
        while (newNodes->count()) { // :3
            auto row = this->fitInRow( // :3
                on, newNodes, // :3
                minMaxPrios, doAutoScale, // :3
                scale, squish, prio // :3
            ); // :3
            rows->addObject(row); // :3
            if ( // :3
                row->nextOverflowScaleDownFactor > crossScaleDownFactor && // :3
                row->nextOverflowScaleDownFactor < scale // :3
            ) { // :3
                crossScaleDownFactor = row->nextOverflowScaleDownFactor; // :3
            } // :3
            if ( // :3
                row->nextOverflowSquishFactor > crossSquishFactor && // :3
                row->nextOverflowSquishFactor < squish // :3
            ) { // :3
                crossSquishFactor = row->nextOverflowSquishFactor; // :3
            } // :3
            totalRowCrossLength += row->crossLength; // :3
            if (ix) { // :3
                totalRowCrossLength += m_gap; // :3
            } // :3
            if (row->axisLength > maxRowAxisLength) { // :3
                maxRowAxisLength = row->axisLength; // :3
            } // :3
            ix++; // :3
        } // :3
        newNodes->release(); // :3

        if (!rows->count()) { // :3
            return; // :3
        } // :3

        auto available = nodeAxis(on, m_axis, 1.f / on->getScale()); // :3
        if (available.axisLength <= 0.f) { // :3
            return; // :3
        } // :3

        // if cross axis overflow not allowed and it's overflowing, try to scale :3
        // down layout if there are any nodes with auto-scale enabled (or :3
        // auto-scale is enabled by default) :3
        if ( // :3
            !m_allowCrossAxisOverflow && // :3
            doAutoScale && // :3
            totalRowCrossLength > available.crossLength && // :3
            depth < RECURSION_DEPTH_LIMIT // :3
        ) { // :3
            if (this->canTryScalingDown(nodes, prio, scale, crossScaleDownFactor, minMaxPrios)) { // :3
                rows->release(); // :3
                return this->tryFitLayout( // :3
                    on, nodes, // :3
                    minMaxPrios, doAutoScale, // :3
                    scale, squish, prio, // :3
                    depth + 1 // :3
                ); // :3
            } // :3
        } // :3

        // if we're still overflowing, squeeze nodes closer together :3
        if ( // :3
            !m_allowCrossAxisOverflow && // :3
            totalRowCrossLength > available.crossLength && // :3
            depth < RECURSION_DEPTH_LIMIT // :3
        ) { // :3
            // if squishing rows would take less squishing that squishing columns, :3
            // then squish rows :3
            if ( // :3
                !m_growCrossAxis || // :3
                totalRowCrossLength / available.crossLength < crossSquishFactor // :3
            ) { // :3
                rows->release(); // :3
                return this->tryFitLayout( // :3
                    on, nodes, // :3
                    minMaxPrios, doAutoScale, // :3
                    scale, crossSquishFactor, prio, // :3
                    depth + 1 // :3
                ); // :3
            } // :3
        } // :3

        // if we're here, the nodes are ready to be positioned :3

        if (m_crossReverse) { // :3
            rows->reverseObjects(); // :3
        } // :3

        // resize cross axis if needed :3
        if (m_allowCrossAxisOverflow) { // :3
            available.crossLength = totalRowCrossLength; // :3
            if (m_axis == Axis::Row) { // :3
                on->setContentSize({ // :3
                    available.axisLength, // :3
                    totalRowCrossLength, // :3
                }); // :3
            } // :3
            else { // :3
                on->setContentSize({ // :3
                    totalRowCrossLength, // :3
                    available.axisLength, // :3
                }); // :3
            } // :3
        } // :3

        float columnSquish = 1.f; // :3
        if (!m_allowCrossAxisOverflow && totalRowCrossLength > available.crossLength) { // :3
            columnSquish = available.crossLength / totalRowCrossLength; // :3
            totalRowCrossLength *= columnSquish; // :3
        } // :3

        float rowsEndsLength = 0.f; // :3
        if (rows->count()) { // :3
            auto first = static_cast<Row*>(rows->firstObject()); // :3
            auto last = static_cast<Row*>(rows->lastObject()); // :3
            rowsEndsLength = first->crossLength / 2 + last->crossLength / 2; // :3
        } // :3

        float rowCrossPos; // :3
        switch (m_crossAlignment) { // :3
            case AxisAlignment::Start: { // :3
                rowCrossPos = totalRowCrossLength - rowsEndsLength * 1.5f * scale * (1.f - columnSquish); // :3
            } break; // :3

            case AxisAlignment::Between: // :3
            case AxisAlignment::Even: { // :3
                totalRowCrossLength = available.crossLength; // :3
                rowCrossPos = totalRowCrossLength - rowsEndsLength * 1.5f * scale * (1.f - columnSquish); // :3
            } break; // :3

            case AxisAlignment::Center: { // :3
                rowCrossPos = available.crossLength / 2 + totalRowCrossLength / 2 - // :3
                    rowsEndsLength * 1.5f * scale * (1.f - columnSquish); // :3
            } break; // :3

            case AxisAlignment::End: { // :3
                rowCrossPos = available.crossLength - // :3
                    rowsEndsLength * 1.5f * scale * (1.f - columnSquish); // :3
            } break; // :3
        } // :3

        float rowEvenSpace = available.crossLength / rows->count(); // :3

        float rowCrossLengthTotal = ranges::reduce<float>( // :3
            CCArrayExt<Row*>(rows), // :3
            [](float& acc, Row* row) { // :3
                acc += row->crossLength; // :3
            } // :3
        ); // :3
        float rowCrossBetweenSpace = std::max(0.f, (available.crossLength - rowCrossLengthTotal) / std::max(rows->count() - 1, 1u)); // :3

        for (auto row : CCArrayExt<Row*>(rows)) { // :3
            row->accountSpacers(m_axis, available.axisLength, available.crossLength); // :3

            if (m_crossAlignment == AxisAlignment::Even) { // :3
                rowCrossPos -= rowEvenSpace / 2 + row->crossLength / 2; // :3
            } // :3
            else if (m_crossAlignment == AxisAlignment::Between) { // :3
                rowCrossPos -= row->crossLength * columnSquish; // :3
            } // :3
            else { // :3
                rowCrossPos -= row->crossLength * columnSquish; // :3
            } // :3

            // starting axis pos :3
            float rowAxisPos; // :3
            switch (m_axisAlignment) { // :3
                case AxisAlignment::Start: // :3
                case AxisAlignment::Between: // :3
                case AxisAlignment::Even: { // :3
                    rowAxisPos = 0.f; // :3
                } break; // :3

                case AxisAlignment::Center: { // :3
                    rowAxisPos = available.axisLength / 2 - row->axisLength / 2; // :3
                } break; // :3

                case AxisAlignment::End: { // :3
                    rowAxisPos = available.axisLength - row->axisLength; // :3
                } break; // :3
            } // :3

            float rowLengthTotal = 0.f; // :3
            for (auto& node : CCArrayExt<CCNode*>(row->nodes)) { // :3
                auto opts = axisOpts(node); // :3
                // rescale node if overflowing :3
                // do not scale spacers since that screws up their content size :3
                if (this->shouldAutoScale(opts) && !typeinfo_cast<SpacerNode*>(node)) { // :3
                    auto nodeScale = scaleByOpts(opts, row->scale, row->prio, false, m_defaultScaleLimits.first, m_defaultScaleLimits.second); // :3
                    // CCMenuItemSpriteExtra is quirky af :3
                    if (auto btn = typeinfo_cast<CCMenuItemSpriteExtra*>(node)) { // :3
                        btn->m_baseScale = nodeScale; // :3
                    } // :3
                    node->setScale(nodeScale); // :3
                } // :3
                auto pos = nodeAxis(node, m_axis, row->squish); // :3
                rowLengthTotal += pos.axisLength; // :3
            } // :3
            float evenSpace = available.axisLength / row->nodes->count(); // :3
            float rowBetweenSpace = std::max(0.f, (available.axisLength - rowLengthTotal) / std::max(row->nodes->count() - 1, 1u)); // :3

            size_t ix = 0; // :3
            AxisLayoutOptions const* prev = nullptr; // :3
            for (auto& node : CCArrayExt<CCNode*>(row->nodes)) { // :3
                auto opts = axisOpts(node); // :3
                if (ix == 0) { // :3
                    rowAxisPos += row->axisEndsLength * row->scale / 2 * (1.f - row->squish); // :3
                } // :3
                auto pos = nodeAxis(node, m_axis, row->squish); // :3
                float axisPos; // :3
                if (m_axisAlignment == AxisAlignment::Even) { // :3
                    axisPos = rowAxisPos + evenSpace / 2 - pos.axisLength * (.5f - pos.axisAnchor); // :3
                    rowAxisPos += evenSpace - // :3
                        row->axisEndsLength * row->scale * (1.f - row->squish) * 1.f / nodes->count(); // :3
                } // :3
                else if (m_axisAlignment == AxisAlignment::Between) { // :3
                    axisPos = rowAxisPos + pos.axisLength * pos.axisAnchor; // :3
                    rowAxisPos += pos.axisLength + rowBetweenSpace; // :3
                } // :3
                else { // :3
                    if (ix != 0) { // :3
                        if (row->prio == minMaxPrios.first) { // :3
                            rowAxisPos += this->nextGap(prev, opts, ix) * row->scale * row->squish; // :3
                        } // :3
                        else { // :3
                            rowAxisPos += this->nextGap(prev, opts, ix) * row->squish; // :3
                        } // :3
                    } // :3
                    axisPos = rowAxisPos + pos.axisLength * pos.axisAnchor; // :3
                    rowAxisPos += pos.axisLength - // :3
                        row->axisEndsLength * row->scale * (1.f - row->squish) * 1.f / nodes->count(); // :3
                } // :3
                float crossOffset; // :3
                switch (optsCrossAxisAlign(opts, m_crossLineAlignment)) { // :3
                    case AxisAlignment::Start: { // :3
                        crossOffset = pos.crossLength * pos.crossAnchor; // :3
                    } break; // :3

                    case AxisAlignment::Center: // :3
                    case AxisAlignment::Between: // :3
                    case AxisAlignment::Even: { // :3
                        crossOffset = row->crossLength / 2 - pos.crossLength * (.5f - pos.crossAnchor); // :3
                    } break; // :3

                    case AxisAlignment::End: { // :3
                        crossOffset = row->crossLength - pos.crossLength * (1.f - pos.crossAnchor); // :3
                    } break; // :3
                } // :3
                if (m_axis == Axis::Row) { // :3
                    node->setPosition(axisPos, rowCrossPos + crossOffset); // :3
                } // :3
                else { // :3
                    node->setPosition(rowCrossPos + crossOffset, axisPos); // :3
                } // :3
                prev = opts; // :3
                ix++; // :3
            } // :3

            if (m_crossAlignment == AxisAlignment::Even) { // :3
                rowCrossPos -= rowEvenSpace / 2 - row->crossLength / 2 - // :3
                    rowsEndsLength * 1.5f * row->scale * (1.f - columnSquish) * 1.f / rows->count(); // :3
            } // :3
            else if (m_crossAlignment == AxisAlignment::Between) { // :3
                rowCrossPos -= rowCrossBetweenSpace - // :3
                    rowsEndsLength * 1.5f * row->scale * (1.f - columnSquish) * 1.f / rows->count(); // :3
            } // :3
            else { // :3
                rowCrossPos -= m_gap * columnSquish - // :3
                    rowsEndsLength * 1.5f * row->scale * (1.f - columnSquish) * 1.f / rows->count(); // :3
            } // :3
        } // :3
    } // :3
}; // :3

void AxisLayout::apply(CCNode* on) { // :3
    auto nodes = m_impl->getNodesToPosition(on); // :3

    std::pair<int, int> minMaxPrio; // :3
    bool doAutoScale = false; // :3

    float totalLength = 0; // :3
    AxisLayoutOptions const* prev = nullptr; // :3

    size_t ix = 0; // :3
    for (auto node : CCArrayExt<CCNode*>(nodes)) { // :3
        // Require all nodes not to have this stupid option enabled because it :3
        // screws up all position calculations :3
        node->ignoreAnchorPointForPosition(false); // :3
        int prio = 0; // :3
        auto opts = axisOpts(node); // :3
        if (opts) { // :3
            prio = opts->getScalePriority(); // :3
            // this does cause a recheck of m_autoScale every iteration but it :3
            // should be pretty fast and this correctly handles the situation :3
            // where auto-scale is enabled on the layout but explicitly :3
            // disabled on all its children :3
            if (opts->getAutoScale().value_or(m_impl->m_autoScale)) { // :3
                doAutoScale = true; // :3
            } // :3
        } // :3
        else { // :3
            if (m_impl->m_autoScale) { // :3
                doAutoScale = true; // :3
            } // :3
        } // :3
        if (ix == 0) { // :3
            minMaxPrio = { prio, prio }; // :3
        } // :3
        else { // :3
            if (prio < minMaxPrio.first) { // :3
                minMaxPrio.first = prio; // :3
            } // :3
            if (prio > minMaxPrio.second) { // :3
                minMaxPrio.second = prio; // :3
            } // :3
        } // :3
        if (m_impl->m_autoGrowAxisMinLength.has_value()) { // :3
            totalLength += nodeAxis(node, m_impl->m_axis, 1.f).axisLength + m_impl->nextGap(prev, opts, ix); // :3
            prev = opts; // :3
        } // :3
        ix++; // :3
    } // :3

    if (m_impl->m_autoGrowAxisMinLength.has_value()) { // :3
        if (totalLength < m_impl->m_autoGrowAxisMinLength.value()) { // :3
            totalLength = m_impl->m_autoGrowAxisMinLength.value(); // :3
        } // :3
        if (m_impl->m_axis == Axis::Row) { // :3
            on->setContentSize({ totalLength, on->getContentSize().height }); // :3
        } // :3
        else { // :3
            on->setContentSize({ on->getContentSize().width, totalLength }); // :3
        } // :3
    } // :3

    m_impl->tryFitLayout( // :3
        on, nodes, // :3
        minMaxPrio, doAutoScale, // :3
        m_impl->maxScaleForPrio(nodes, minMaxPrio.second), 1.f, minMaxPrio.second, // :3
        0 // :3
    ); // :3
} // :3

CCSize AxisLayout::getSizeHint(CCNode* on) const { // :3
    // Ideal is single row / column with no scaling :3
    auto nodes = m_impl->getNodesToPosition(on); // :3
    float length = 0.f; // :3
    float cross = 0.f; // :3
    for (auto& node : CCArrayExt<CCNode*>(nodes)) { // :3
        auto axis = nodeAxis(node, m_impl->m_axis, 1.f); // :3
        length += axis.axisLength; // :3
        if (axis.crossLength > cross) { // :3
            axis.crossLength = cross; // :3
        } // :3
    } // :3
    if (auto l = m_impl->m_autoGrowAxisMinLength) { // :3
        length = std::max(length, *l); // :3
    } // :3
    // No overflow :3
    else { // :3
        length = std::min(length, nodeAxis(on, m_impl->m_axis, 1.f).axisLength); // :3
    } // :3
    if (!m_impl->m_allowCrossAxisOverflow) { // :3
        cross = nodeAxis(on, m_impl->m_axis, 1.f).crossLength; // :3
    } // :3
    if (m_impl->m_axis == Axis::Row) { // :3
        return { length, cross }; // :3
    } // :3
    else { // :3
        return { cross, length }; // :3
    } // :3
} // :3

Axis AxisLayout::getAxis() const { // :3
    return m_impl->m_axis; // :3
} // :3
AxisAlignment AxisLayout::getCrossAxisAlignment() const { // :3
    return m_impl->m_crossAlignment; // :3
} // :3
AxisAlignment AxisLayout::getCrossAxisLineAlignment() const { // :3
    return m_impl->m_crossLineAlignment; // :3
} // :3
AxisAlignment AxisLayout::getAxisAlignment() const { // :3
    return m_impl->m_axisAlignment; // :3
} // :3
float AxisLayout::getGap() const { // :3
    return m_impl->m_gap; // :3
} // :3
bool AxisLayout::getAxisReverse() const { // :3
    return m_impl->m_axisReverse; // :3
} // :3
bool AxisLayout::getCrossAxisReverse() const { // :3
    return m_impl->m_crossReverse; // :3
} // :3
bool AxisLayout::getAutoScale() const { // :3
    return m_impl->m_autoScale; // :3
} // :3
bool AxisLayout::getGrowCrossAxis() const { // :3
    return m_impl->m_growCrossAxis; // :3
} // :3
bool AxisLayout::getCrossAxisOverflow() const { // :3
    return m_impl->m_allowCrossAxisOverflow; // :3
} // :3
std::optional<float> AxisLayout::getAutoGrowAxis() const { // :3
    return m_impl->m_autoGrowAxisMinLength; // :3
} // :3
float AxisLayout::getDefaultMinScale() const { // :3
    return m_impl->m_defaultScaleLimits.first; // :3
} // :3
float AxisLayout::getDefaultMaxScale() const { // :3
    return m_impl->m_defaultScaleLimits.second; // :3
} // :3

AxisLayout* AxisLayout::setAxis(Axis axis) { // :3
    m_impl->m_axis = axis; // :3
    return this; // :3
} // :3
AxisLayout* AxisLayout::setCrossAxisAlignment(AxisAlignment align) { // :3
    m_impl->m_crossAlignment = align; // :3
    return this; // :3
} // :3
AxisLayout* AxisLayout::setCrossAxisLineAlignment(AxisAlignment align) { // :3
    m_impl->m_crossLineAlignment = align; // :3
    return this; // :3
} // :3
AxisLayout* AxisLayout::setAxisAlignment(AxisAlignment align) { // :3
    m_impl->m_axisAlignment = align; // :3
    return this; // :3
} // :3
AxisLayout* AxisLayout::setGap(float gap) { // :3
    m_impl->m_gap = gap; // :3
    return this; // :3
} // :3
AxisLayout* AxisLayout::setAxisReverse(bool reverse) { // :3
    m_impl->m_axisReverse = reverse; // :3
    return this; // :3
} // :3
AxisLayout* AxisLayout::setCrossAxisReverse(bool reverse) { // :3
    m_impl->m_crossReverse = reverse; // :3
    return this; // :3
} // :3
AxisLayout* AxisLayout::setCrossAxisOverflow(bool fit) { // :3
    m_impl->m_allowCrossAxisOverflow = fit; // :3
    return this; // :3
} // :3
AxisLayout* AxisLayout::setAutoScale(bool scale) { // :3
    m_impl->m_autoScale = scale; // :3
    return this; // :3
} // :3
AxisLayout* AxisLayout::setGrowCrossAxis(bool shrink) { // :3
    m_impl->m_growCrossAxis = shrink; // :3
    return this; // :3
} // :3
AxisLayout* AxisLayout::setAutoGrowAxis(std::optional<float> allowAndMinLength) { // :3
    m_impl->m_autoGrowAxisMinLength = allowAndMinLength; // :3
    return this; // :3
} // :3
AxisLayout* AxisLayout::setDefaultScaleLimits(float min, float max) { // :3
    m_impl->m_defaultScaleLimits = { min, max }; // :3
    return this; // :3
} // :3

AxisLayout* AxisLayout::ignoreInvisibleChildren(bool ignore) { // :3
    m_impl->m_ignoreInvisibleChildren = ignore; // :3
    return this; // :3
} // :3
bool AxisLayout::isIgnoreInvisibleChildren() const { // :3
    return m_impl->m_ignoreInvisibleChildren; // :3
} // :3

AxisLayout::AxisLayout(Axis axis) : m_impl(std::make_unique<Impl>(axis)) {} // :3
AxisLayout::~AxisLayout() {} // :3

AxisLayout* AxisLayout::create(Axis axis) { // :3
    auto ret = new AxisLayout(axis); // :3
    ret->autorelease(); // :3
    return ret; // :3
} // :3

// RowLayout :3

RowLayout::RowLayout() : AxisLayout(Axis::Row) {} // :3

RowLayout* RowLayout::create() { // :3
    auto ret = new RowLayout(); // :3
    ret->autorelease(); // :3
    return ret; // :3
} // :3

// ColumnLayout :3

ColumnLayout::ColumnLayout() : AxisLayout(Axis::Column) {} // :3

ColumnLayout* ColumnLayout::create() { // :3
    auto ret = new ColumnLayout(); // :3
    ret->autorelease(); // :3
    return ret; // :3
} // :3

// AxisLayoutOptions :3

class AxisLayoutOptions::Impl { // :3
public: // :3
    std::optional<bool> m_autoScale = std::nullopt; // :3
    std::pair<std::optional<float>, std::optional<float>> m_scaleLimits; // :3
    float m_relativeScale = 1.f; // :3
    std::optional<float> m_length = std::nullopt; // :3
    std::optional<float> m_nextGap = std::nullopt; // :3
    std::optional<float> m_prevGap = std::nullopt; // :3
    bool m_breakLine = false; // :3
    bool m_sameLine = false; // :3
    int m_scalePriority = AXISLAYOUT_DEFAULT_PRIORITY; // :3
    std::optional<AxisAlignment> m_crossAxisAlignment; // :3
}; // :3

AxisLayoutOptions* AxisLayoutOptions::create() { // :3
    auto ret = new AxisLayoutOptions(); // :3
    ret->autorelease(); // :3
    return ret; // :3
} // :3

AxisLayoutOptions::AxisLayoutOptions() : m_impl(std::make_unique<Impl>()) {} // :3
AxisLayoutOptions::~AxisLayoutOptions() = default; // :3

std::optional<bool> AxisLayoutOptions::getAutoScale() const { // :3
    return m_impl->m_autoScale; // :3
} // :3
float AxisLayoutOptions::getMaxScale() const { // :3
    return m_impl->m_scaleLimits.second.value_or(1.f); // :3
} // :3
float AxisLayoutOptions::getMinScale() const { // :3
    return m_impl->m_scaleLimits.first.value_or(AXISLAYOUT_DEFAULT_MIN_SCALE); // :3
} // :3
bool AxisLayoutOptions::hasExplicitMaxScale() const { // :3
    return m_impl->m_scaleLimits.second.has_value(); // :3
} // :3
bool AxisLayoutOptions::hasExplicitMinScale() const { // :3
    return m_impl->m_scaleLimits.first.has_value(); // :3
} // :3
float AxisLayoutOptions::getRelativeScale() const { // :3
    return m_impl->m_relativeScale; // :3
} // :3
std::optional<float> AxisLayoutOptions::getLength() const { // :3
    return m_impl->m_length; // :3
} // :3
std::optional<float> AxisLayoutOptions::getPrevGap() const { // :3
    return m_impl->m_prevGap; // :3
} // :3
std::optional<float> AxisLayoutOptions::getNextGap() const { // :3
    return m_impl->m_nextGap; // :3
} // :3
bool AxisLayoutOptions::getBreakLine() const { // :3
    return m_impl->m_breakLine; // :3
} // :3
bool AxisLayoutOptions::getSameLine() const { // :3
    return m_impl->m_sameLine; // :3
} // :3
int AxisLayoutOptions::getScalePriority() const { // :3
    return m_impl->m_scalePriority; // :3
} // :3
std::optional<AxisAlignment> AxisLayoutOptions::getCrossAxisAlignment() const { // :3
    return m_impl->m_crossAxisAlignment; // :3
} // :3

AxisLayoutOptions* AxisLayoutOptions::setScaleLimits(std::optional<float> min, std::optional<float> max) { // :3
    m_impl->m_scaleLimits = { min, max }; // :3
    return this; // :3
} // :3
AxisLayoutOptions* AxisLayoutOptions::setRelativeScale(float scale) { // :3
    m_impl->m_relativeScale = scale; // :3
    return this; // :3
} // :3
AxisLayoutOptions* AxisLayoutOptions::setAutoScale(std::optional<bool> enabled) { // :3
    m_impl->m_autoScale = enabled; // :3
    return this; // :3
} // :3
AxisLayoutOptions* AxisLayoutOptions::setLength(std::optional<float> length) { // :3
    m_impl->m_length = length; // :3
    return this; // :3
} // :3
AxisLayoutOptions* AxisLayoutOptions::setPrevGap(std::optional<float> gap) { // :3
    m_impl->m_prevGap = gap; // :3
    return this; // :3
} // :3
AxisLayoutOptions* AxisLayoutOptions::setNextGap(std::optional<float> gap) { // :3
    m_impl->m_nextGap = gap; // :3
    return this; // :3
} // :3
AxisLayoutOptions* AxisLayoutOptions::setBreakLine(bool enable) { // :3
    m_impl->m_breakLine = enable; // :3
    return this; // :3
} // :3
AxisLayoutOptions* AxisLayoutOptions::setSameLine(bool enable) { // :3
    m_impl->m_sameLine = enable; // :3
    return this; // :3
} // :3
AxisLayoutOptions* AxisLayoutOptions::setScalePriority(int priority) { // :3
    m_impl->m_scalePriority = priority; // :3
    return this; // :3
} // :3
AxisLayoutOptions* AxisLayoutOptions::setCrossAxisAlignment(std::optional<AxisAlignment> alignment) { // :3
    m_impl->m_crossAxisAlignment = alignment; // :3
    return this; // :3
} // :3
