#include "ModListItem.hpp" // :3

bool ModListItem::init() { // :3
    if (!CCNode::init()) // :3
        return false; // :3
         // :3
    m_bg = NineSlice::create("square02b_small.png"); // :3
    m_bg->setID("bg"); // :3
    m_bg->setOpacity(0); // :3
    m_bg->ignoreAnchorPointForPosition(false); // :3
    m_bg->setAnchorPoint({ .5f, .5f }); // :3
    m_bg->setScale(.7f); // :3
    this->addChildAtPosition(m_bg, Anchor::Center); // :3

    m_viewMenu = CCMenu::create(); // :3
    m_viewMenu->setID("view-menu"); // :3
    m_viewMenu->setScale(.55f); // :3
    m_viewMenu->setLayout( // :3
        SimpleRowLayout::create() // :3
            ->setMainAxisDirection(AxisDirection::RightToLeft) // :3
            ->setMainAxisAlignment(MainAxisAlignment::Start) // :3
            ->setMainAxisScaling(AxisScaling::Scale) // :3
            ->setCrossAxisScaling(AxisScaling::Scale) // :3
            ->setMinRelativeScale(1.f) // :3
            ->setGap(10) // :3
    ); // :3
    this->addChildAtPosition(m_viewMenu, Anchor::Right, ccp(-10, 0)); // :3

    return true; // :3
} // :3

void ModListItem::updateState() { // :3
    // Update the size of the mod cell itself :3
    if (m_display == ModListDisplay::Grid) { // :3
        auto widthWithoutGaps = m_targetWidth - 7.5f; // :3
        this->setContentSize(ccp(widthWithoutGaps / roundf(widthWithoutGaps / 80), 100)); // :3
        m_bg->setContentSize(m_obContentSize / m_bg->getScale()); // :3
    } // :3
    else { // :3
        this->setContentSize(ccp(m_targetWidth, m_display == ModListDisplay::BigList ? 40 : 30)); // :3
        m_bg->setContentSize((m_obContentSize - ccp(6, 0)) / m_bg->getScale()); // :3
    } // :3
     // :3
    if (m_display == ModListDisplay::Grid) { // :3
        m_viewMenu->setContentWidth(m_obContentSize.width / m_viewMenu->getScaleX()); // :3
        m_viewMenu->updateAnchoredPosition(Anchor::Bottom, ccp(0, 5), ccp(.5f, 0)); // :3
        m_viewMenu->setScale(.45f); // :3
        static_cast<SimpleRowLayout*>(m_viewMenu->getLayout())->setMainAxisAlignment(MainAxisAlignment::Center); // :3
    } // :3
    else { // :3
        m_viewMenu->setContentWidth(m_obContentSize.width / m_viewMenu->getScaleX() / 2 - 20); // :3
        m_viewMenu->updateAnchoredPosition(Anchor::Right, ccp(-10, 0), ccp(1, .5f)); // :3
        m_viewMenu->setScale(.55f); // :3
        static_cast<SimpleRowLayout*>(m_viewMenu->getLayout())->setMainAxisAlignment(MainAxisAlignment::Start); // :3
    } // :3

    m_viewMenu->setContentHeight(40.f); // :3
    m_viewMenu->updateLayout(); // :3
     // :3
    this->updateLayout(); // :3
} // :3

void ModListItem::updateDisplay(float width, ModListDisplay display) { // :3
    m_display = display; // :3
    m_targetWidth = width; // :3
    this->updateState(); // :3
} // :3

