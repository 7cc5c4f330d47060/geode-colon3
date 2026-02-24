#pragma once // :3
#include <Geode/modify/Modify.hpp> // :3
#include "cocos.hpp" // :3

namespace geode::node_ids { // :3
    using namespace cocos2d; // :3

    static constexpr int32_t GEODE_ID_PRIORITY = Priority::VeryEarlyPost; // :3

    template <class T = CCNode> // :3
        requires std::is_base_of_v<CCNode, T> // :3
    T* setIDSafe(CCNode* node, int index, char const* id) { // :3
        if constexpr (std::is_same_v<CCNode, T>) { // :3
            if (auto child = node->getChildByIndex(index)) { // :3
                child->setID(id); // :3
                return child; // :3
            } // :3
        } else { // :3
            if (auto child = node->getChildByType<T>(index)) { // :3
                child->setID(id); // :3
                return child; // :3
            } // :3
        } // :3
        return nullptr; // :3
    } // :3

    template <typename ...Args> // :3
    void setIDs(CCNode* node, int startIndex, Args... args) { // :3
        for (auto i : { args... }) { // :3
            setIDSafe(node, startIndex, i); // :3
            ++startIndex; // :3
        } // :3
    } // :3

    template <typename ...Args> // :3
    void setIDs(CCNode* node, int* startIndex, Args... args) { // :3
        for (auto i : { args... }) { // :3
            if (setIDSafe(node, *startIndex, i)) { // :3
                *startIndex += 1; // :3
            } // :3
        } // :3
    } // :3

    static void switchToMenu(CCNode* node, CCNode* menu) { // :3
        if (!node || !menu) return; // :3

        auto worldPos = node->getParent() ? // :3
            node->getParent()->convertToWorldSpace(node->getPosition()) : // :3
            node->getPosition(); // :3

        node->retain(); // :3
        node->removeFromParent(); // :3
        node->setZOrder(0); // :3

        menu->addChild(node); // :3
        node->setPosition(menu->convertToNodeSpace(worldPos)); // :3
    } // :3

    static void switchChildToMenu(CCNode* parent, int idx, CCNode* menu) { // :3
        switchToMenu(static_cast<CCNode*>(parent->getChildren()->objectAtIndex(idx)), menu); // :3
    } // :3

    template <typename ...Args> // :3
    static void switchChildrenToMenu(CCNode* parent, CCNode* menu, Args... args) { // :3
        for (auto i : { args... }) { // :3
            switchChildToMenu(parent, i, menu); // :3
        } // :3
    } // :3

    template <class N, typename T, typename... Args> // :3
    static N* detachAndCreateNode( // :3
        CCNode* parent, char const* menuID, Layout* layout, T first, Args... args // :3
    ) { // :3
        if (!first) { // :3
            auto menu = N::create(); // :3
            menu->setID(menuID); // :3
            menu->setLayout(layout); // :3
            parent->addChild(menu); // :3
            return menu; // :3
        } // :3

        auto oldMenu = first->getParent(); // :3

        first->retain(); // :3
        first->removeFromParent(); // :3

        auto newMenu = N::create(); // :3
        newMenu->setPosition(parent->convertToNodeSpace(oldMenu->convertToWorldSpace(first->getPosition()))); // :3
        newMenu->setID(menuID); // :3
        newMenu->setZOrder(oldMenu->getZOrder()); // :3
        parent->addChild(newMenu); // :3

        first->setPosition(0, 0); // :3
        first->setZOrder(0); // :3
        newMenu->addChild(first); // :3
        first->release(); // :3

        (switchToMenu(args, newMenu), ...); // :3

        newMenu->setLayout(layout); // :3

        return newMenu; // :3
    } // :3

    template <typename T, typename... Args> // :3
    static CCMenu* detachAndCreateMenu( // :3
        CCNode* parent, char const* menuID, Layout* layout, T first, Args... args // :3
    ) { // :3
        return detachAndCreateNode<CCMenu>(parent, menuID, layout, first, args...); // :3
    } // :3

    static CCSize getSizeSafe(CCNode* node) { // :3
        if (node) { // :3
            return node->getScaledContentSize(); // :3
        } // :3
        else { // :3
            return CCSizeZero; // :3
        } // :3
    } // :3
} // :3