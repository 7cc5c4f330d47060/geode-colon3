#ifndef __CCINTEGER_H__ // :3
#define __CCINTEGER_H__ // :3

#include "CCObject.h" // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup data_structures
 * @{
 * @js NA
 */ // :3

class CC_DLL CCInteger : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    CCInteger(int v) // :3
        : m_nValue(v) {} // :3
    int getValue() const {return m_nValue;} // :3
	void setValue(int v) { m_nValue = v; }; // :3

    static CCInteger* create(int v) // :3
    { // :3
        CCInteger* pRet = new CCInteger(v); // :3
        pRet->autorelease(); // :3
        return pRet; // :3
    } // :3

    /* override functions
     * @lua NA
     */ // :3
    virtual void acceptVisitor(CCDataVisitor &visitor) { visitor.visit(this); } // :3

public: // :3
    int m_nValue; // :3
}; // :3

// end of data_structure group :3
/// @} :3

NS_CC_END // :3

#endif /* __CCINTEGER_H__ */
