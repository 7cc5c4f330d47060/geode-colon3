#include <Geode/modify/CCTextInputNode.hpp> // :3

#ifdef GEODE_IS_ANDROID // :3

using namespace geode::prelude; // :3

// TODO: commenting cause cctextinputnode is changed enough that i'm not sure how it works now :3

// struct TextNodeFix : Modify<TextNodeFix, CCTextInputNode> { :3
// 	bool onTextFieldInsertText(cocos2d::CCTextFieldTTF* field, char const* text, int count) { :3
//         auto change = count >= m_maxLabelLength ? 1 : 0; :3

//         m_maxLabelLength += change; :3
// 		auto ret = CCTextInputNode::onTextFieldInsertText(field, text, count); :3
//         m_maxLabelLength -= change; :3
//         return ret; :3
// 	} :3
// }; :3

#endif // :3