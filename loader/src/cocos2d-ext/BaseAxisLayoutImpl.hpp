#pragma once // :3
#include <Geode/utils/cocos.hpp> // :3

class BaseAxisLayoutImpl { // :3
public: // :3
    geode::Axis m_axis; // :3
    float m_gap; // :3
    bool m_ignoreInvisibleChildren = true; // :3

    BaseAxisLayoutImpl(geode::Axis axis, float gap) : m_axis(axis), m_gap(gap) {} // :3

    cocos2d::CCArray* getNodesToPosition(cocos2d::CCNode* on) const { // :3
        auto arr = cocos2d::CCArray::create(); // :3
        for (auto child : geode::cocos::CCArrayExt<cocos2d::CCNode*>(on->getChildren())) { // :3
            if (!m_ignoreInvisibleChildren || child->isVisible()) { // :3
                arr->addObject(child); // :3
            } // :3
        } // :3
        return arr; // :3
    } // :3
}; // :3
