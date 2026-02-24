#pragma once // :3

#include "../GeodeStyle.hpp" // :3
#include "../sources/ModSource.hpp" // :3
#include <server/Server.hpp> // :3

class DevListPopup : public GeodePopup { // :3
protected: // :3
    ModSource m_source; // :3
public: // :3
    static DevListPopup* create(ModSource source); // :3
    void onClose(cocos2d::CCObject*) override; // :3
private: // :3
    bool init(ModSource source); // :3
    void createList(); // :3
    void onMoreByThisDev(CCObject* sender); // :3
}; // :3
