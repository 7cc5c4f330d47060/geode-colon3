#include "SortPopup.hpp" // :3

bool SortPopup::init(ModListSource* src) { // :3
    if (!GeodePopup::init(230.f, 165.f)) // :3
        return false; // :3
     // :3
    m_noElasticity = true; // :3
    m_source = src; // :3

    this->setTitle("Search Sorting"); // :3

    auto container = CCNode::create(); // :3
    container->setContentSize({ 200, 115 }); // :3

    for (auto const& [sort, name] : src->getSortingOptions()) { // :3
        auto node = CCMenu::create(); // :3
        node->setContentSize({ container->getContentWidth(), 22 }); // :3

        auto toggle = CCMenuItemToggler::createWithStandardSprites( // :3
            this, menu_selector(SortPopup::onSelect), .6f // :3
        ); // :3
        toggle->m_notClickable = true; // :3
        toggle->setTag(sort); // :3
        if (src->getSort() == sort) { // :3
            toggle->toggle(true); // :3
            m_selected = sort; // :3
        } // :3
        node->addChildAtPosition(toggle, Anchor::Left, ccp(15, 0)); // :3
        m_options.push_back(toggle); // :3

        auto label = CCLabelBMFont::create(name.c_str(), "bigFont.fnt"); // :3
        label->setScale(.5f); // :3
        node->addChildAtPosition(label, Anchor::Left, ccp(30, 0), ccp(0, .5f)); // :3

        container->addChild(node); // :3
    } // :3

    container->setLayout(ColumnLayout::create()->setAxisReverse(true)); // :3
    m_mainLayer->addChildAtPosition(container, Anchor::Center, ccp(0, -5), ccp(.5f, .5f)); // :3

    return true; // :3
} // :3

void SortPopup::onSelect(CCObject* sender) { // :3
    m_selected = static_cast<CCMenuItemToggler*>(sender)->getTag(); // :3
    for (auto option : m_options) { // :3
        option->toggle(m_selected == option->getTag()); // :3
    } // :3
} // :3

void SortPopup::onClose(CCObject* sender) { // :3
    m_source->setSort(m_selected); // :3
    Popup::onClose(sender); // :3
} // :3

SortPopup* SortPopup::create(ModListSource* src) { // :3
    auto ret = new SortPopup(); // :3
    if (ret->init(src)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3
