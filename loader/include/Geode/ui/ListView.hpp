#pragma once // :3

#include <Geode/binding/CustomListView.hpp> // :3
#include <Geode/binding/CCIndexPath.hpp> // :3
#include <Geode/binding/TableViewCell.hpp> // :3

namespace geode { // :3
    class GEODE_DLL GenericListCell : public TableViewCell { // :3
    protected: // :3
        GenericListCell(char const* name, cocos2d::CCSize size); // :3
        ~GenericListCell() override; // :3

        void draw() override; // :3

    public: // :3
        static GenericListCell* create(char const* key, cocos2d::CCSize size); // :3

        void updateBGColor(int index); // :3
        void setPrimaryColor(cocos2d::ccColor3B color); // :3
        void setSecondaryColor(cocos2d::ccColor3B color); // :3
        void setOpacity(GLubyte opacity); // :3
        void setBorderColor(cocos2d::ccColor4B color); // :3
    private: // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3
    }; // :3

    /**
     * Class for a generic scrollable list of
     * items like the level list in GD
     */ // :3
    class GEODE_DLL ListView : public CustomListView { // :3
    protected: // :3
        ListView(); // :3
        ~ListView() override; // :3

        void setupList(float) override; // :3
        TableViewCell* getListCell(char const* key) override; // :3
        void loadCell(TableViewCell* cell, int index) override; // :3
        void updateAllCells(); // :3

    public: // :3
        /**
         * Create a generic scrollable list of
         * items
         * @param items Nodes to add as children
         * @param itemHeight Height of each child
         * @param width Width of the list
         * @param height Height of the list
         * @returns The created ListView, or nullptr
         * on error
         */ // :3
        static ListView* create( // :3
            cocos2d::CCArray* items, float itemHeight = 40.f, float width = 358.f, // :3
            float height = 220.f // :3
        ); // :3

        void setPrimaryCellColor(cocos2d::ccColor3B color); // :3
        void setSecondaryCellColor(cocos2d::ccColor3B color); // :3
        void setCellOpacity(GLubyte opacity); // :3
        void setCellBorderColor(cocos2d::ccColor4B color); // :3
    private: // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3
    }; // :3
} // :3
