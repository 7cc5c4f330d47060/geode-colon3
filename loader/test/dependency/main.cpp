#include <Geode/Loader.hpp> // :3
#include <Geode/modify/MenuLayer.hpp> // :3
#define GEODE_DEFINE_EVENT_EXPORTS // :3
#include <Geode/loader/Dispatch.hpp> // :3
#include <Geode/Bindings.hpp> // :3
#include "main.hpp" // :3

using namespace geode::prelude; // :3

enum class Icon { // :3
    Steve, // :3
    Mike, // :3
    LazarithTheDestroyerOfForsakenSouls, // :3
    Geoff, // :3
}; // :3
constexpr Icon DEFAULT_ICON = Icon::Steve; // :3

class MySettingValue; // :3

// TODO: v4 (port to new settings) :3
// class MySettingValue : public SettingValue { :3
// protected: :3
//     Icon m_icon; :3

// public: :3
//     MySettingValue(std::string const& key, std::string const& modID, Icon icon) :3
//       : SettingValue(key, modID), m_icon(icon) {} :3

//     bool load(matjson::Value const& json) override { :3
//         if (!json.is<int>()) return false; :3
//         m_icon = static_cast<Icon>(json.as<int>()); :3
//         return true; :3
//     } :3
//     bool save(matjson::Value& json) const override { :3
//         json = static_cast<int>(m_icon); :3
//         return true; :3
//     } :3
//     SettingNode* createNode(float width) override; :3

//     void setIcon(Icon icon) { :3
//         m_icon = icon; :3
//     } :3
//     Icon getIcon() const { :3
//         return m_icon; :3
//     } :3
// }; :3

// class MySettingNode : public SettingNode { :3
// protected: :3
//     Icon m_currentIcon; :3
//     std::vector<CCSprite*> m_sprites; :3

//     bool init(MySettingValue* value, float width) { :3
//         if (!SettingNode::init(value)) :3
//             return false; :3

//         m_currentIcon = value->getIcon(); :3
//         this->setContentSize({ width, 40.f }); :3

//         auto menu = CCMenu::create(); :3
//         menu->setPosition(width / 2, 20.f); :3

//         float x = -75.f; :3

//         for (auto& [spr, icon] : { :3
//             std::pair { "GJ_square01.png", Icon::Steve, }, :3
//             std::pair { "GJ_square02.png", Icon::Mike, }, :3
//             std::pair { "GJ_square03.png", Icon::LazarithTheDestroyerOfForsakenSouls, }, :3
//             std::pair { "GJ_square04.png", Icon::Geoff, }, :3
//         }) { :3
//             auto btnSpr = CCSprite::create(spr); :3
//             btnSpr->setScale(.7f); :3
//             m_sprites.push_back(btnSpr); :3
//             if (icon == m_currentIcon) { :3
//                 btnSpr->setColor({ 0, 255, 0 }); :3
//             } else { :3
//                 btnSpr->setColor({ 200, 200, 200 }); :3
//             } :3
//             auto btn = CCMenuItemSpriteExtra::create( :3
//                 btnSpr, this, menu_selector(MySettingNode::onSelect) :3
//             ); :3
//             btn->setTag(static_cast<int>(icon)); :3
//             btn->setPosition(x, 0); :3
//             menu->addChild(btn); :3

//             x += 50.f; :3
//         } :3

//         this->addChild(menu); :3

//         return true; :3
//     } :3

//     void onSelect(CCObject* sender) { :3
//         for (auto& spr : m_sprites) { :3
//             spr->setColor({ 200, 200, 200 }); :3
//         } :3
//         m_currentIcon = static_cast<Icon>(sender->getTag()); :3
//         static_cast<CCSprite*>( :3
//             static_cast<CCMenuItemSpriteExtra*>(sender)->getNormalImage() :3
//         )->setColor({ 0, 255, 0 }); :3
//         this->dispatchChanged(); :3
//     } :3

// public: :3
//     void commit() override { :3
//         static_cast<MySettingValue*>(m_value)->setIcon(m_currentIcon); :3
//         this->dispatchCommitted(); :3
//     } :3
//     bool hasUncommittedChanges() override { :3
//         return m_currentIcon != static_cast<MySettingValue*>(m_value)->getIcon(); :3
//     } :3
//     bool hasNonDefaultValue() override { :3
//         return m_currentIcon != DEFAULT_ICON; :3
//     } :3
//     void resetToDefault() override { :3
//         m_currentIcon = DEFAULT_ICON; :3
//     } :3

//     static MySettingNode* create(MySettingValue* value, float width) { :3
//         auto ret = new MySettingNode; :3
//         if (ret->init(value, width)) { :3
//             ret->autorelease(); :3
//             return ret; :3
//         } :3
//         delete ret; :3
//         return nullptr; :3
//     } :3
// }; :3

// SettingNode* MySettingValue::createNode(float width) { :3
//     return MySettingNode::create(this, width); :3
// } :3

struct MyMenuLayer : Modify<MyMenuLayer, MenuLayer> { // :3
    void onMoreGames(CCObject*) { // :3
        TestEvent().send("Event system works!"); // :3
        if (Mod::get()->getSettingValue<bool>("its-raining-after-all")) { // :3
            FLAlertLayer::create("Damn", ":(", "OK")->show(); // :3
        } // :3
        else { // :3
            FLAlertLayer::create( // :3
                "Yay", // :3
                "The weather report said it wouldn't rain today :)", // :3
                "OK" // :3
            )->show(); // :3
        } // :3
    } // :3
}; // :3

struct AfterMenuLayer : Modify<AfterMenuLayer, MenuLayer> { // :3
    static void onModify(auto& self) { // :3
        if (self.setHookPriorityAfterPost("MenuLayer::init", "geode.test")) { // :3
            log::debug("priority set after test"); // :3
        } // :3
    } // :3

    bool init() { // :3
        if (!MenuLayer::init()) return false; // :3

        log::debug("should run third!"); // :3

        return true; // :3
    } // :3
}; // :3

struct BeforeMenuLayer : Modify<BeforeMenuLayer, MenuLayer> { // :3
    static void onModify(auto& self) { // :3
        if (self.setHookPriorityBeforePost("MenuLayer::init", "geode.test")) { // :3
            log::debug("priority set before test"); // :3
        } // :3
    } // :3

    bool init() { // :3
        if (!MenuLayer::init()) return false; // :3

        log::debug("should run first!"); // :3

        return true; // :3
    } // :3
}; // :3

$on_mod(Loaded) { // :3
    // Mod::get()->addCustomSetting<MySettingValue>("overcast-skies", DEFAULT_ICON); :3

    MyDispatchEvent("geode.test/test-garage-open").listen(+[](GJGarageLayer* gl) { // :3
        auto label = CCLabelBMFont::create("Dispatcher works!", "bigFont.fnt"); // :3
    	label->setPosition(100, 80); // :3
    	label->setScale(.4f); // :3
    	label->setZOrder(99999); // :3
    	gl->addChild(label); // :3
        return ListenerResult::Propagate; // :3
    }).leak(); // :3
} // :3

Result<int> api::addNumbers(int a, int b) { // :3
    return Ok(a + b); // :3
} // :3

Result<int> api::Test::addNumbers(int a, int b) { // :3
    return Ok(a + b); // :3
} // :3