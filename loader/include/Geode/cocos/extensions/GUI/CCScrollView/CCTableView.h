/****************************************************************************
 Copyright (c) 2012 cocos2d-x.org
 Copyright (c) 2010 Sangwoo Im

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/ // :3

#ifndef __CCTABLEVIEW_H__ // :3
#define __CCTABLEVIEW_H__ // :3

#include "CCScrollView.h" // :3
#include "CCTableViewCell.h" // :3

#include <set> // :3
#include <vector> // :3

NS_CC_EXT_BEGIN // :3

class CC_DLL CCTableView; // :3
class CC_DLL CCArrayForObjectSorting; // :3

typedef enum { // :3
    kCCTableViewFillTopDown, // :3
    kCCTableViewFillBottomUp // :3
} CCTableViewVerticalFillOrder; // :3

/**
 * Sole purpose of this delegate is to single touch event in this version.
 * @js NA
 * @lua NA
 */ // :3
class CC_DLL CCTableViewDelegate : public CCScrollViewDelegate // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     * Delegate to respond touch event
     *
     * @param table table contains the given cell
     * @param cell  cell that is touched
     */ // :3
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell) = 0; // :3

    /**
     * Delegate to respond a table cell press event.
     *
     * @param table table contains the given cell
     * @param cell  cell that is pressed
     */ // :3
    virtual void tableCellHighlight(CCTableView* table, CCTableViewCell* cell){}; // :3

    /**
     * Delegate to respond a table cell release event
     *
     * @param table table contains the given cell
     * @param cell  cell that is pressed
     */ // :3
    virtual void tableCellUnhighlight(CCTableView* table, CCTableViewCell* cell){}; // :3

    /**
     * Delegate called when the cell is about to be recycled. Immediately
     * after this call the cell will be removed from the scene graph and
     * recycled.
     *
     * @param table table contains the given cell
     * @param cell  cell that is pressed
     */ // :3
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell){}; // :3

}; // :3


/**
 * Data source that governs table backend data.
 * @lua NA
 */ // :3
class CC_DLL CCTableViewDataSource // :3
{ // :3
public: // :3
    virtual ~CCTableViewDataSource() {} // :3

    /**
     * cell size for a given index
     *
     * @param idx the index of a cell to get a size
     * @return size of a cell at given index
     */ // :3
    virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx) { // :3
        return cellSizeForTable(table); // :3
    }; // :3
    /**
     * cell height for a given table.
     *
     * @param table table to hold the instances of Class
     * @return cell size
     */ // :3
    virtual CCSize cellSizeForTable(CCTableView *table) { // :3
        return CCSizeZero; // :3
    }; // :3
    /**
     * a cell instance at a given index
     *
     * @param idx index to search for a cell
     * @return cell found at idx
     */ // :3
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx) = 0; // :3
    /**
     * Returns number of cells in a given table view.
     *
     * @return number of cells
     */ // :3
    virtual unsigned int numberOfCellsInTableView(CCTableView *table) = 0; // :3

}; // :3


/**
 * UITableView counterpart for cocos2d for iphone.
 *
 * this is a very basic, minimal implementation to bring UITableView-like component into cocos2d world.
 * @lua NA
 */ // :3
class CC_DLL CCTableView : public CCScrollView, public CCScrollViewDelegate // :3
{ // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTableView, CCScrollView) // :3
    /**
     *  @js ctor
     */ // :3
    CCTableView(); // :3
    /**
     *  @js NA
     */ // :3
    virtual ~CCTableView(); // :3

    /**
     * An intialized table view object
     *
     * @param dataSource data source
     * @param size view size
     * @return table view
     */ // :3
    static CCTableView* create(CCTableViewDataSource* dataSource, CCSize size); // :3
    /**
     * An initialized table view object
     *
     * @param dataSource data source;
     * @param size view size
     * @param container parent object for cells
     * @return table view
     */ // :3
    static CCTableView* create(CCTableViewDataSource* dataSource, CCSize size, CCNode *container); // :3

    /**
     * data source
     * @js NA
     */ // :3
    CCTableViewDataSource* getDataSource() { return m_pDataSource; } // :3
    void setDataSource(CCTableViewDataSource* source) { m_pDataSource = source; } // :3
    /**
     * delegate
     * @js NA
     */ // :3
    CCTableViewDelegate* getDelegate() { return m_pTableViewDelegate; } // :3
    void setDelegate(CCTableViewDelegate* pDelegate) { m_pTableViewDelegate = pDelegate; } // :3

    /**
     * determines how cell is ordered and filled in the view.
     */ // :3
    void setVerticalFillOrder(CCTableViewVerticalFillOrder order); // :3
    CCTableViewVerticalFillOrder getVerticalFillOrder(); // :3


    bool initWithViewSize(CCSize size, CCNode* container = NULL); // :3
    /**
     * Updates the content of the cell at a given index.
     *
     * @param idx index to find a cell
     */ // :3
    void updateCellAtIndex(unsigned int idx); // :3
    /**
     * Inserts a new cell at a given index
     *
     * @param idx location to insert
     */ // :3
    void insertCellAtIndex(unsigned int idx); // :3
    /**
     * Removes a cell at a given index
     *
     * @param idx index to find a cell
     */ // :3
    void removeCellAtIndex(unsigned int idx); // :3
    /**
     * reloads data from data source.  the view will be refreshed.
     */ // :3
    void reloadData(); // :3
    /**
     * Dequeues a free cell if available. nil if not.
     *
     * @return free cell
     */ // :3
    CCTableViewCell *dequeueCell(); // :3

    /**
     * Returns an existing cell at a given index. Returns nil if a cell is nonexistent at the moment of query.
     *
     * @param idx index
     * @return a cell at a given index
     */ // :3
    CCTableViewCell *cellAtIndex(unsigned int idx); // :3


    virtual void scrollViewDidScroll(CCScrollView* view); // :3
    virtual void scrollViewDidZoom(CCScrollView* view) {} // :3

    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent); // :3
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent); // :3
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent); // :3
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent); // :3

public: // :3
    CCTableViewCell *m_pTouchedCell; // :3
    /**
     * vertical direction of cell filling
     */ // :3
    CCTableViewVerticalFillOrder m_eVordering; // :3

    /**
     * index set to query the indexes of the cells used.
     */ // :3
    gd::set<unsigned int>* m_pIndices; // :3

    /**
     * vector with all cell positions
     */ // :3
    gd::vector<float> m_vCellsPositions; // :3
    //NSMutableIndexSet *indices_; :3
    /**
     * cells that are currently in the table
     */ // :3
    CCArrayForObjectSorting* m_pCellsUsed; // :3
    /**
     * free list of cells
     */ // :3
    CCArrayForObjectSorting* m_pCellsFreed; // :3
    /**
     * weak link to the data source object
     */ // :3
    CCTableViewDataSource* m_pDataSource; // :3
    /**
     * weak link to the delegate object
     */ // :3
    CCTableViewDelegate* m_pTableViewDelegate; // :3

	CCScrollViewDirection m_eOldDirection; // :3

protected: // :3
    int __indexFromOffset(CCPoint offset); // :3
    unsigned int _indexFromOffset(CCPoint offset); // :3
    CCPoint __offsetFromIndex(unsigned int index); // :3
    CCPoint _offsetFromIndex(unsigned int index); // :3

    void _moveCellOutOfSight(CCTableViewCell *cell); // :3
    void _setIndexForCell(unsigned int index, CCTableViewCell *cell); // :3
    void _addCellIfNecessary(CCTableViewCell * cell); // :3

    void _updateCellPositions(); // :3
public: // :3
    void _updateContentSize(); // :3

    enum TableViewScriptEventType // :3
    { // :3
        kTableViewScroll   = 0, // :3
        kTableViewZoom, // :3
        kTableCellTouched, // :3
        kTableCellHighLight, // :3
        kTableCellUnhighLight, // :3
        kTableCellWillRecycle, // :3
        kTableCellSizeForIndex, // :3
        kTableCellSizeAtIndex, // :3
        kNumberOfCellsInTableView, // :3
    }; // :3
    void unregisterAllScriptHandler(); // :3
}; // :3


NS_CC_EXT_END // :3

#endif /* __CCTABLEVIEW_H__ */
