#include "KeybindEditPopup.hpp" // :3

#include <Geode/ui/Scrollbar.hpp> // :3
#include <Geode/ui/SimpleAxisLayout.hpp> // :3

CCNode* createKeybindButton(Keybind const& keybind) { // :3
    if (keybind.key >= CONTROLLER_A && keybind.key <= CONTROLLER_RTHUMBSTICK_RIGHT) { // :3
        return createGeodeButton(keybind.createNode(), ""); // :3
    } // :3
    return createGeodeButton(nullptr, keybind.toString(), true); // :3
} // :3

bool KeybindEditPopup::init( // :3
    std::shared_ptr<KeybindSettingV3> setting, // :3
    Keybind const& keybind, // :3
    Function<void(Keybind const&)> callback // :3
) { // :3
    if (!GeodePopup::init(220, 170)) // :3
        return false; // :3

    this->setTitle(setting->getDisplayName()); // :3
    m_noElasticity = true; // :3

    if (auto mod = setting->getMod()) { // :3
        auto fromModLabel = CCLabelBMFont::create(mod->getName().c_str(), "bigFont.fnt"); // :3
        fromModLabel->setScale(.4f); // :3
        fromModLabel->setColor(ccc3(55, 155, 255)); // :3
        m_mainLayer->addChildAtPosition(fromModLabel, Anchor::Top, ccp(0, -40)); // :3
    } // :3

    m_setting = setting; // :3
    m_callback = std::move(callback); // :3
    m_currentKeybind = keybind; // :3
    auto bottomMenu = CCMenu::create(); // :3
    bottomMenu->setContentWidth(220.f); // :3

    if (keybind.key == KEY_None) { // :3
        auto addButton = CCMenuItemSpriteExtra::create( // :3
            createGeodeButton("Add", true), this, menu_selector(KeybindEditPopup::onSet) // :3
        ); // :3
        bottomMenu->addChild(addButton); // :3
    } // :3
    else { // :3
        m_originalKeybind = keybind; // :3

        auto setButton = CCMenuItemSpriteExtra::create( // :3
            createGeodeButton("Set", true), this, menu_selector(KeybindEditPopup::onSet) // :3
        ); // :3
        bottomMenu->addChild(setButton); // :3

        auto removeButton = CCMenuItemSpriteExtra::create( // :3
            createGeodeButton("Remove", true, GeodeButtonSprite::Delete), this, menu_selector(KeybindEditPopup::onRemove) // :3
        ); // :3
        bottomMenu->addChild(removeButton); // :3
    } // :3

    bottomMenu->setLayout(RowLayout::create()->setGap(10.f)); // :3
    m_mainLayer->addChildAtPosition(bottomMenu, Anchor::Bottom, ccp(0, 25)); // :3

    this->addEventListener(KeyboardInputEvent(), [this](KeyboardInputData& data) { // :3
        if (data.action == KeyboardInputData::Action::Press) { // :3
            m_currentKeybind.key = data.key; // :3
            m_currentKeybind.modifiers = data.modifiers; // :3
            this->updateLabel(); // :3
        } // :3
    }); // :3
    this->addEventListener(MouseInputEvent(), [this](MouseInputData& data) { // :3
        auto key = MouseInputData::buttonToKeyCode(data.button); // :3
        if (key != KEY_None && data.action == MouseInputData::Action::Press) { // :3
            m_currentKeybind.key = key; // :3
            m_currentKeybind.modifiers = data.modifiers; // :3
            this->updateLabel(); // :3
        } // :3
    }); // :3
    this->updateLabel(); // :3

    return true; // :3
} // :3

void KeybindEditPopup::updateLabel() { // :3
    if (m_originalKeybind) { // :3
        if (m_originalKeybindContainer) { // :3
            m_originalKeybindContainer->removeFromParent(); // :3
        } // :3
        if (*m_originalKeybind != m_currentKeybind) { // :3
            m_originalKeybindContainer = CCNode::create(); // :3
            m_originalKeybindContainer->setContentWidth(200); // :3
            m_originalKeybindContainer->setScale(.4f); // :3
            m_originalKeybindContainer->setAnchorPoint(ccp(.5f, .5f)); // :3

            auto originalKeybindInfoStart = CCLabelBMFont::create("(Previous: ", "bigFont.fnt"); // :3
            originalKeybindInfoStart->setColor(ccc3(55, 255, 55)); // :3
            m_originalKeybindContainer->addChild(originalKeybindInfoStart); // :3

            auto originalKeybind = m_originalKeybind->createNode(); // :3
            m_originalKeybindContainer->addChild(originalKeybind); // :3
             // :3
            auto originalKeybindInfoEnd = CCLabelBMFont::create(")", "bigFont.fnt"); // :3
            originalKeybindInfoEnd->setColor(ccc3(55, 255, 55)); // :3
            m_originalKeybindContainer->addChild(originalKeybindInfoEnd); // :3

            m_originalKeybindContainer->setLayout(SimpleRowLayout::create()); // :3
            m_mainLayer->addChildAtPosition(m_originalKeybindContainer, Anchor::Bottom, ccp(0, 60)); // :3
        } // :3
    } // :3

    if (m_keybindNode) { // :3
        m_keybindNode->removeFromParent(); // :3
    } // :3
    if (m_currentKeybind.key == KEY_None && m_currentKeybind.modifiers == KeyboardModifier::None) { // :3
        auto label = CCLabelBMFont::create("Press a key...", "bigFont.fnt"); // :3
        label->setOpacity(150); // :3
        label->setScale(.75f); // :3
        m_keybindNode = label; // :3
    } // :3
    else { // :3
        m_keybindNode = m_currentKeybind.createNode(); // :3
        // It just looks better if controller binds are scale 1 :3
        if (!typeinfo_cast<CCSprite*>(m_keybindNode)) { // :3
            limitNodeWidth(m_keybindNode, m_mainLayer->getContentWidth() - 10, .75f, .1f); // :3
        } // :3
    } // :3
    float keybindOffset = m_originalKeybindContainer ? 10 : 0; // :3
    m_mainLayer->addChildAtPosition(m_keybindNode, Anchor::Center, ccp(0, keybindOffset)); // :3
} // :3

void KeybindEditPopup::onSet(CCObject*) { // :3
    if (m_currentKeybind.key == KEY_None || m_currentKeybind.key == KEY_Unknown) { // :3
        return; // :3
    } // :3
    m_callback(m_currentKeybind); // :3
    this->onClose(nullptr); // :3
} // :3

void KeybindEditPopup::onRemove(CCObject*) { // :3
    m_callback(Keybind()); // :3
    this->onClose(nullptr); // :3
} // :3

KeybindEditPopup* KeybindEditPopup::create( // :3
    std::shared_ptr<KeybindSettingV3> setting, // :3
    Keybind const& keybind, // :3
    Function<void(Keybind const&)> callback // :3
) { // :3
    auto ret = new KeybindEditPopup(); // :3
    if (ret->init(setting, keybind, std::move(callback))) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

bool KeybindListPopup::init( // :3
    std::shared_ptr<KeybindSettingV3> setting, // :3
    std::vector<Keybind> const& keybinds, // :3
    Function<void(std::vector<Keybind>)> callback // :3
) { // :3
    if (!GeodePopup::init(220.f, 250.f)) // :3
        return false; // :3

    this->setTitle(setting->getDisplayName()); // :3
    m_noElasticity = true; // :3

    m_callback = std::move(callback); // :3
    m_setting = setting; // :3
    m_currentKeybinds = keybinds; // :3
    m_hasChanged = false; // :3

    auto background = CCLayerColor::create(ccc4(0, 0, 0, 75), 190.f, 150.f); // :3
    background->ignoreAnchorPointForPosition(false); // :3
    m_mainLayer->addChildAtPosition(background, Anchor::Center, ccp(-5, 5)); // :3

    m_scrollLayer = ScrollLayer::create({ 190.f, 150.f }); // :3
    m_scrollLayer->ignoreAnchorPointForPosition(false); // :3
    m_scrollLayer->m_contentLayer->setLayout(ColumnLayout::create()->setAxisReverse(true)->setAxisAlignment(AxisAlignment::End)->setAutoGrowAxis(0)); // :3
    m_mainLayer->addChildAtPosition(m_scrollLayer, Anchor::Center, ccp(-5, 5)); // :3

    m_mainLayer->addChildAtPosition(Scrollbar::create(m_scrollLayer), Anchor::Right, ccp(-10, 5)); // :3

    m_mainLayer->addChildAtPosition(createGeodeListBorders({ 190.f, 150.f }), Anchor::Center, ccp(-5, 5)); // :3

    auto bottomMenu = CCMenu::create(); // :3
    bottomMenu->setContentWidth(220.f); // :3

    auto addButton = CCMenuItemSpriteExtra::create( // :3
        createGeodeButton("Add", true), this, menu_selector(KeybindListPopup::onAdd) // :3
    ); // :3
    bottomMenu->addChild(addButton); // :3

    auto saveButton = CCMenuItemSpriteExtra::create( // :3
        createGeodeButton("Save", true), this, menu_selector(KeybindListPopup::onSave) // :3
    ); // :3
    bottomMenu->addChild(saveButton); // :3

    bottomMenu->setLayout(RowLayout::create()->setGap(10.f)); // :3
    m_mainLayer->addChildAtPosition(bottomMenu, Anchor::Bottom, ccp(0, 25)); // :3

    this->updateKeybinds(); // :3

    return true; // :3
} // :3

void KeybindListPopup::updateKeybinds() { // :3
    m_scrollLayer->m_contentLayer->removeAllChildren(); // :3
    size_t index = 0; // :3
    for (auto& keybind : m_currentKeybinds) { // :3
        auto bspr = createKeybindButton(keybind); // :3
        auto button = CCMenuItemSpriteExtra::create(bspr, this, menu_selector(KeybindListPopup::onKeybind)); // :3
        button->setTag(index); // :3
        auto menu = CCMenu::createWithItem(button); // :3
        menu->setContentSize({ 190.f, button->getScaledContentHeight() }); // :3
        button->setPosition({ 95.f, button->getScaledContentHeight() / 2 }); // :3
        m_scrollLayer->m_contentLayer->addChild(menu); // :3
        index += 1; // :3
    } // :3
    m_scrollLayer->m_contentLayer->updateLayout(); // :3
    m_scrollLayer->scrollToTop(); // :3
} // :3

void KeybindListPopup::onAdd(CCObject*) { // :3
    KeybindEditPopup::create(m_setting, Keybind(), [this](Keybind const& newKeybind) { // :3
        if (std::ranges::contains(m_currentKeybinds, newKeybind)) return; // :3
        m_hasChanged = true; // :3
        m_currentKeybinds.push_back(newKeybind); // :3
        this->updateKeybinds(); // :3
    })->show(); // :3
} // :3

void KeybindListPopup::onSave(CCObject*) { // :3
    m_callback(m_currentKeybinds); // :3
    Popup::onClose(nullptr); // :3
} // :3

void KeybindListPopup::onKeybind(CCObject* sender) { // :3
    auto index = sender->getTag(); // :3
    KeybindEditPopup::create(m_setting, m_currentKeybinds[index], [this, index](Keybind const& newKeybind) { // :3
        if (m_currentKeybinds[index] == newKeybind) return; // :3
        m_hasChanged = true; // :3
        if (newKeybind.key == KEY_None || std::ranges::contains(m_currentKeybinds, newKeybind)) { // :3
            m_currentKeybinds.erase(m_currentKeybinds.begin() + index); // :3
        } // :3
        else { // :3
            m_currentKeybinds[index] = newKeybind; // :3
        } // :3
        this->updateKeybinds(); // :3
    })->show(); // :3
} // :3

void KeybindListPopup::onClose(CCObject*) { // :3
    if (m_hasChanged) { // :3
        createQuickPopup( // :3
            "Unsaved Changes", // :3
            "You have <cr>unsaved changes</c>! Are you sure you want to exit?", // :3
            "Cancel", "Discard", // :3
            [this](auto, bool btn2) { // :3
                if (btn2) { // :3
                    Popup::onClose(nullptr); // :3
                } // :3
            } // :3
        ); // :3
    } // :3
    else { // :3
        Popup::onClose(nullptr); // :3
    } // :3
} // :3

KeybindListPopup* KeybindListPopup::create( // :3
    std::shared_ptr<KeybindSettingV3> setting, // :3
    std::vector<Keybind> const& keybinds, // :3
    Function<void(std::vector<Keybind>)> callback // :3
) { // :3
    auto ret = new KeybindListPopup(); // :3
    if (ret->init(setting, keybinds, std::move(callback))) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3
