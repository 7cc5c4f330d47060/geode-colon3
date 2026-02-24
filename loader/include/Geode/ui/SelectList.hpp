#pragma once // :3

#include <Geode/binding/CCMenuItemSpriteExtra.hpp> // :3
#include <Geode/utils/function.hpp> // :3

namespace geode { // :3

    template <class T> // :3
    T do_nothing(T t) { // :3
        return t; // :3
    } // :3

    template <class T = std::string, auto Stringify = do_nothing<std::string>> // :3
    class SelectList : public cocos2d::CCMenu { // :3
    protected: // :3
        std::vector<T> m_list; // :3
        size_t m_index = 0; // :3
        geode::Function<void(T const&, size_t)> m_onChange; // :3
        cocos2d::CCLabelBMFont* m_label; // :3
        CCMenuItemSpriteExtra* m_prevBtn; // :3
        CCMenuItemSpriteExtra* m_nextBtn; // :3

        bool init( // :3
            float width, std::span<T> list, geode::Function<void(T const&, size_t)> onChange // :3
        ) { // :3
            if (!cocos2d::CCMenu::init()) return false; // :3

            m_list = list; // :3
            m_onChange = std::move(onChange); // :3

            this->setContentSize({ width, 30.f }); // :3

            auto prevSpr = cocos2d::CCSprite::createWithSpriteFrameName("navArrowBtn_001.png"); // :3
            prevSpr->setFlipX(true); // :3
            prevSpr->setScale(.3f); // :3

            m_prevBtn = // :3
                CCMenuItemSpriteExtra::create(prevSpr, this, menu_selector(SelectList<T>::onPrev)); // :3
            m_prevBtn->setPosition(-width / 2 + 10.f, 0.f); // :3
            this->addChild(m_prevBtn); // :3

            auto nextSpr = cocos2d::CCSprite::createWithSpriteFrameName("navArrowBtn_001.png"); // :3
            nextSpr->setScale(.3f); // :3

            m_nextBtn = // :3
                CCMenuItemSpriteExtra::create(nextSpr, this, menu_selector(SelectList<T>::onNext)); // :3
            m_nextBtn->setPosition(width / 2 - 10.f, 0.f); // :3
            this->addChild(m_nextBtn); // :3

            m_label = cocos2d::CCLabelBMFont::create("", "bigFont.fnt"); // :3
            this->addChild(m_label); // :3

            this->updateLabel(); // :3

            this->setTouchEnabled(true); // :3

            return true; // :3
        } // :3

        void updateLabel() { // :3
            if (m_list.size()) { // :3
                m_label->setString(Stringify(m_list.at(m_index)).c_str()); // :3
                m_prevBtn->setEnabled(true); // :3
                m_nextBtn->setEnabled(true); // :3
            } // :3
            else { // :3
                m_label->setString("-"); // :3
                m_prevBtn->setEnabled(false); // :3
                m_nextBtn->setEnabled(false); // :3
            } // :3
            m_label->limitLabelWidth(m_obContentSize.width - 40.f, .6f, .1f); // :3
        } // :3

        void onPrev(CCObject* sender) { // :3
            if (m_index == 0) { // :3
                m_index = m_list.size() - 1; // :3
            } // :3
            else { // :3
                m_index--; // :3
            } // :3
            this->updateLabel(); // :3
            m_onChange(m_list.at(m_index), m_index); // :3
        } // :3

        void onNext(CCObject* sender) { // :3
            if (m_index == m_list.size() - 1) { // :3
                m_index = 0; // :3
            } // :3
            else { // :3
                m_index++; // :3
            } // :3
            this->updateLabel(); // :3
            m_onChange(m_list.at(m_index), m_index); // :3
        } // :3

    public: // :3
        static SelectList* create( // :3
            float width, std::span<T> list, geode::Function<void(T const&, size_t)> onChange // :3
        ) { // :3
            auto ret = new SelectList(); // :3
            if (ret->init(width, list, std::move(onChange))) { // :3
                ret->autorelease(); // :3
                return ret; // :3
            } // :3
            delete ret; // :3
            return nullptr; // :3
        } // :3

        void setItems(std::span<T> list) { // :3
            m_index = 0; // :3
            m_list = list; // :3
            this->updateLabel(); // :3
        } // :3
    }; // :3

} // :3
