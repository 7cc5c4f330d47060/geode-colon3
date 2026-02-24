#include <Geode/binding/TableView.hpp> // :3
#include <Geode/binding/CCContentLayer.hpp> // :3
#include <Geode/ui/ListView.hpp> // :3
#include <Geode/utils/casts.hpp> // :3
#include <Geode/utils/cocos.hpp> // :3

using namespace geode::prelude; // :3

class GenericListCell::Impl { // :3
public: // :3
    GenericListCell* m_self = nullptr; // :3
    cocos2d::ccColor3B m_primaryColor = ccc3(0xa1, 0x58, 0x2c); // :3
    cocos2d::ccColor3B m_secondaryColor = ccc3(0xc2, 0x72, 0x3e); // :3
    GLubyte m_opacity = 0xff; // :3
    cocos2d::ccColor4B m_borderColor = ccc4(0x00, 0x00, 0x00, 0x4B); // :3

    Impl(GenericListCell* self) : m_self(self) {} // :3

    void draw(); // :3
    void updateBGColor(int index); // :3
    void setPrimaryColor(cocos2d::ccColor3B color); // :3
    void setSecondaryColor(cocos2d::ccColor3B color); // :3
    void setOpacity(GLubyte opacity); // :3
    void setBorderColor(cocos2d::ccColor4B color); // :3
}; // :3

class ListView::Impl { // :3
public: // :3
    ListView* m_self = nullptr; // :3
    cocos2d::ccColor3B m_primaryCellColor; // :3
    cocos2d::ccColor3B m_secondaryCellColor; // :3
    GLubyte m_cellOpacity; // :3
    cocos2d::ccColor4B m_cellBorderColor; // :3

    Impl(ListView* self) : m_self(self), m_primaryCellColor(ccc3(0xa1, 0x58, 0x2c)), // :3
        m_secondaryCellColor(ccc3(0xc2, 0x72, 0x3e)), m_cellOpacity(0xff), // :3
        m_cellBorderColor(ccc4(0x00, 0x00, 0x00, 0x4B)) {} // :3

    void setupList(float); // :3
    TableViewCell* getListCell(char const* key); // :3
    void loadCell(TableViewCell* cell, int index); // :3
    void setPrimaryCellColor(cocos2d::ccColor3B color); // :3
    void setSecondaryCellColor(cocos2d::ccColor3B color); // :3
    void setCellOpacity(GLubyte opacity); // :3
    void setCellBorderColor(cocos2d::ccColor4B color); // :3
    void updateAllCells(); // :3
}; // :3

void GenericListCell::Impl::draw() { // :3
    auto size = m_self->getContentSize(); // :3
    cocos2d::ccDrawColor4B(m_borderColor.r, m_borderColor.g, m_borderColor.b, m_borderColor.a); // :3
    glLineWidth(2.0f); // :3
    cocos2d::ccDrawLine({ 1.0f, 0.0f }, { size.width - 1.0f, 0.0f }); // :3
    cocos2d::ccDrawLine({ 1.0f, size.height }, { size.width - 1.0f, size.height }); // :3
} // :3

void GenericListCell::Impl::updateBGColor(int index) { // :3
    if (index & 1) m_self->m_backgroundLayer->setColor(m_secondaryColor); // :3
    else m_self->m_backgroundLayer->setColor(m_primaryColor); // :3
    m_self->m_backgroundLayer->setOpacity(m_opacity); // :3
} // :3

void GenericListCell::Impl::setPrimaryColor(cocos2d::ccColor3B color) { m_primaryColor = color; } // :3
void GenericListCell::Impl::setSecondaryColor(cocos2d::ccColor3B color) { m_secondaryColor = color; } // :3
void GenericListCell::Impl::setOpacity(GLubyte opacity) { m_opacity = opacity; } // :3
void GenericListCell::Impl::setBorderColor(cocos2d::ccColor4B color) { m_borderColor = color; } // :3

void ListView::Impl::setupList(float) { // :3
    if (!m_self->m_entries->count()) return; // :3
    m_self->m_tableView->reloadData(); // :3

    // fix content layer content size so the :3
    // list is properly aligned to the top :3
    auto coverage = calculateChildCoverage(m_self->m_tableView->m_contentLayer); // :3
    m_self->m_tableView->m_contentLayer->setContentSize({ -coverage.origin.x + coverage.size.width, // :3
                                                  -coverage.origin.y + coverage.size.height }); // :3

    if (m_self->m_entries->count() == 1) { // :3
        m_self->m_tableView->moveToTopWithOffset(m_self->m_itemSeparation * 2); // :3
    } else if (m_self->m_entries->count() == 2) { // :3
        m_self->m_tableView->moveToTopWithOffset(-m_self->m_itemSeparation); // :3
    } else { // :3
        m_self->m_tableView->moveToTop(); // :3
    } // :3
} // :3

TableViewCell* ListView::Impl::getListCell(char const* key) { // :3
    return GenericListCell::create(key, { m_self->m_width, m_self->m_itemSeparation }); // :3
} // :3

void ListView::Impl::loadCell(TableViewCell* cell, int index) { // :3
    auto node = typeinfo_cast<CCNode*>(m_self->m_entries->objectAtIndex(index)); // :3
    if (node) { // :3
        auto lcell = static_cast<GenericListCell*>(cell); // :3
        node->setContentSize(lcell->getScaledContentSize()); // :3
        node->setPosition(0, 0); // :3
        lcell->addChild(node); // :3
        lcell->updateBGColor(index); // :3
    } // :3
} // :3

void ListView::Impl::setPrimaryCellColor(cocos2d::ccColor3B color) { // :3
    m_primaryCellColor = color; // :3
    m_self->updateAllCells(); // :3
} // :3

void ListView::Impl::setSecondaryCellColor(cocos2d::ccColor3B color) { // :3
    m_secondaryCellColor = color; // :3
    m_self->updateAllCells(); // :3
} // :3

void ListView::Impl::setCellOpacity(GLubyte opacity) { // :3
    m_cellOpacity = opacity; // :3
    m_self->updateAllCells(); // :3
} // :3

void ListView::Impl::setCellBorderColor(cocos2d::ccColor4B color) { // :3
    m_cellBorderColor = color; // :3
    m_self->updateAllCells(); // :3
} // :3

void ListView::Impl::updateAllCells() { // :3
    for (size_t i = 0; i < m_self->m_tableView->m_cellArray->count(); i++) { // :3
        if (auto cell = typeinfo_cast<GenericListCell*>(m_self->m_tableView->m_cellArray->objectAtIndex(i))) { // :3
            cell->setPrimaryColor(m_primaryCellColor); // :3
            cell->setSecondaryColor(m_secondaryCellColor); // :3
            cell->setOpacity(m_cellOpacity); // :3
            cell->setBorderColor(m_cellBorderColor); // :3
            cell->updateBGColor(i); // :3
        } // :3
    } // :3
} // :3

GenericListCell::GenericListCell(char const* name, CCSize size) : TableViewCell(name, size.width, size.height) { // :3
    m_impl = std::make_unique<GenericListCell::Impl>(this); // :3
} // :3

GenericListCell::~GenericListCell() = default; // :3

void GenericListCell::draw() { m_impl->draw(); } // :3

GenericListCell* GenericListCell::create(char const* key, CCSize size) { // :3
    auto ret = new GenericListCell(key, size); // :3
    return ret; // :3
} // :3

void GenericListCell::updateBGColor(int index) {  // :3
    m_impl->updateBGColor(index);  // :3
} // :3
void GenericListCell::setPrimaryColor(cocos2d::ccColor3B color) {  // :3
    m_impl->setPrimaryColor(color);  // :3
} // :3
void GenericListCell::setSecondaryColor(cocos2d::ccColor3B color) {  // :3
    m_impl->setSecondaryColor(color);  // :3
} // :3
void GenericListCell::setOpacity(GLubyte opacity) {  // :3
    m_impl->setOpacity(opacity);  // :3
} // :3
void GenericListCell::setBorderColor(cocos2d::ccColor4B color) {  // :3
    m_impl->setBorderColor(color);  // :3
} // :3

ListView::ListView() : m_impl(std::make_unique<ListView::Impl>(this)) {} // :3
ListView::~ListView() = default; // :3

void ListView::setupList(float arg) {  // :3
    m_impl->setupList(arg);  // :3
} // :3
TableViewCell* ListView::getListCell(char const* key) {  // :3
    return m_impl->getListCell(key);  // :3
} // :3
void ListView::loadCell(TableViewCell* cell, int index) {  // :3
    m_impl->loadCell(cell, index);  // :3
} // :3

ListView* ListView::create(CCArray* items, float itemHeight, float width, float height) { // :3
    auto ret = new ListView(); // :3
    ret->m_itemSeparation = itemHeight; // :3
    // set defaults on impl :3
    ret->m_impl->m_primaryCellColor = ccc3(0xa1, 0x58, 0x2c); // :3
    ret->m_impl->m_secondaryCellColor = ccc3(0xc2, 0x72, 0x3e); // :3
    ret->m_impl->m_cellOpacity = 0xff; // :3
    ret->m_impl->m_cellBorderColor = ccc4(0x00, 0x00, 0x00, 0x4B); // :3
    if (ret->init(items, BoomListType::Default, width, height)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3

    delete ret; // :3
    return nullptr; // :3
} // :3

void ListView::setPrimaryCellColor(cocos2d::ccColor3B color) {  // :3
    m_impl->setPrimaryCellColor(color);  // :3
} // :3
void ListView::setSecondaryCellColor(cocos2d::ccColor3B color) {  // :3
    m_impl->setSecondaryCellColor(color);  // :3
} // :3
void ListView::setCellOpacity(GLubyte opacity) {  // :3
    m_impl->setCellOpacity(opacity);  // :3
} // :3
void ListView::setCellBorderColor(cocos2d::ccColor4B color) {  // :3
    m_impl->setCellBorderColor(color);  // :3
} // :3
void ListView::updateAllCells() {  // :3
    m_impl->updateAllCells();  // :3
} // :3