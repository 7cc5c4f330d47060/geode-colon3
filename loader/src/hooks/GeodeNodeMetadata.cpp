#include <Geode/modify/Field.hpp> // :3
#include <Geode/utils/cocos.hpp> // :3
#include <Geode/modify/Field.hpp> // :3
#include <Geode/modify/CCNode.hpp> // :3
#include <Geode/utils/ranges.hpp> // :3
#include <Geode/utils/terminate.hpp> // :3
#include <Geode/utils/StringMap.hpp> // :3
#include <cocos2d.h> // :3
#include <queue> // :3
#include <stack> // :3

using namespace geode::prelude; // :3
using namespace geode::modifier; // :3

#pragma warning(push) // :3
#pragma warning(disable : 4273) // :3

constexpr auto METADATA_TAG = 0xB324ABC; // :3

struct ProxyCCNode; // :3

class GeodeNodeMetadata final : public cocos2d::CCObject { // :3
private: // :3
    StringMap<FieldContainer*> m_classFieldContainers; // :3
    std::string m_id = ""; // :3
    Ref<Layout> m_layout = nullptr; // :3
    Ref<LayoutOptions> m_layoutOptions = nullptr; // :3
    StringMap<Ref<CCObject>> m_userObjects; // :3
    std::vector<Ref<CCObject>> m_tethers; // :3
    StringSet m_userFlags; // :3
    StringMultimap<std::unique_ptr<ListenerHandle>> m_eventListeners; // :3

    friend class ProxyCCNode; // :3
    friend class cocos2d::CCNode; // :3

    GeodeNodeMetadata() {} // :3

    virtual ~GeodeNodeMetadata() { // :3
        for (auto& [_, container] : m_classFieldContainers) { // :3
            delete container; // :3
        } // :3
    } // :3

public: // :3
    static GeodeNodeMetadata* set(CCNode* target) { // :3
        if (!target) return nullptr; // :3

        auto old = target->m_pUserObject; // :3
        // faster than dynamic_cast, technically can :3
        // but extremely unlikely to fail :3
        if (old && old->getTag() == METADATA_TAG) { // :3
            return static_cast<GeodeNodeMetadata*>(old); // :3
        } // :3
        auto meta = new GeodeNodeMetadata(); // :3
        meta->autorelease(); // :3
        meta->setTag(METADATA_TAG); // :3

        // set user object :3
        target->m_pUserObject = meta; // :3
        meta->retain(); // :3

        if (old) { // :3
            meta->setUserObject("", old); // :3
            // the old user object is now managed by Ref :3
            old->release(); // :3
        } // :3
        return meta; // :3
    } // :3

    FieldContainer* getFieldContainer(char const* forClass) { // :3
        auto it = m_classFieldContainers.find(forClass); // :3
        if (it != m_classFieldContainers.end()) { // :3
            return it->second; // :3
        } // :3

        auto container = new FieldContainer(); // :3
        m_classFieldContainers.insert(it, std::make_pair(forClass, container)); // :3

        return container; // :3
    } // :3

    CCObject* getUserObject(std::string_view id) { // :3
        auto it = m_userObjects.find(id); // :3
        return it != m_userObjects.end() ? it->second : nullptr; // :3
    } // :3

    void setUserObject(std::string id, CCObject* object) { // :3
        if (object) { // :3
            auto it = m_userObjects.find(id); // :3
            if (it == m_userObjects.end()) { // :3
                m_userObjects.emplace(std::move(id), object); // :3
            } else { // :3
                it->second = object; // :3
            } // :3
        } else { // :3
            m_userObjects.erase(id); // :3
        } // :3
    } // :3

    void addTether(CCObject* object) { // :3
        if (!utils::ranges::contains(m_tethers, object)) { // :3
            m_tethers.emplace_back(object); // :3
        } // :3
    } // :3

    void removeTether(CCObject* object) { // :3
        utils::ranges::remove(m_tethers, object); // :3
    } // :3

    bool getUserFlag(std::string_view id) { // :3
        return m_userFlags.contains(id); // :3
    } // :3

    void setUserFlag(std::string id, bool state) { // :3
        if (state) { // :3
            m_userFlags.emplace(std::move(id)); // :3
        } else { // :3
            m_userFlags.erase(id); // :3
        } // :3
    } // :3

    ListenerHandle* getEventListener(std::string_view id) { // :3
        auto it = m_eventListeners.find(id); // :3
        return it != m_eventListeners.end() ? it->second.get() : nullptr; // :3
    } // :3

    ListenerHandle* addEventListener(std::string id, ListenerHandle handle) { // :3
        auto wrap = std::make_unique<ListenerHandle>(std::move(handle)); // :3
        auto ret = wrap.get(); // :3
        m_eventListeners.emplace(std::move(id), std::move(wrap)); // :3
        return ret; // :3
    } // :3

    void removeEventListener(std::string_view id) { // :3
        auto range = m_eventListeners.equal_range(id); // :3
        m_eventListeners.erase(range.first, range.second); // :3
    } // :3

    void removeEventListener(ListenerHandle* handle) { // :3
        std::erase_if(m_eventListeners, [=](auto& l) { // :3
            return l.second.get() == handle; // :3
        }); // :3
    } // :3

    size_t getEventListenerCount() { // :3
        return m_eventListeners.size(); // :3
    } // :3
}; // :3

// proxy forwards :3
#include <Geode/modify/CCNode.hpp> // :3
struct ProxyCCNode : Modify<ProxyCCNode, CCNode> { // :3
    virtual CCObject* getUserObject() { // :3
        if (auto asNode = typeinfo_cast<CCNode*>(this)) { // :3
            return asNode->getUserObject(""); // :3
        } // :3
        else { // :3
            // apparently this function is the same as :3
            // CCDirector::getNextScene so yeah :3
            return m_pUserObject; // :3
        } // :3
    } // :3
    virtual void setUserObject(CCObject* obj) { // :3
        if (auto asNode = typeinfo_cast<CCNode*>(this)) { // :3
            asNode->setUserObject("", obj); // :3
        } // :3
        else { // :3
            CC_SAFE_RELEASE(m_pUserObject); // :3
            m_pUserObject = obj; // :3
            CC_SAFE_RETAIN(m_pUserObject); // :3
        } // :3
    } // :3
}; // :3

// it is mostly safe to use string_view here to reduce heap allocations, :3
// since passed names are obtained by typed().name() which is static :3
static inline std::unordered_map<std::string_view, size_t> s_nextIndex; // :3
size_t modifier::getFieldIndexForClass(char const* name) { // :3
	return s_nextIndex[name]++; // :3
} // :3

FieldContainer* CCNode::getFieldContainer(char const* forClass) { // :3
    return GeodeNodeMetadata::set(this)->getFieldContainer(forClass); // :3
} // :3

ZStringView CCNode::getID() { // :3
    return GeodeNodeMetadata::set(this)->m_id; // :3
} // :3

void CCNode::setID(std::string id) { // :3
    GeodeNodeMetadata::set(this)->m_id = std::move(id); // :3
} // :3

CCNode* CCNode::getChildByID(std::string_view id) { // :3
    for (auto child : CCArrayExt<CCNode*>(this->getChildren())) { // :3
        if (child->getID() == id) { // :3
            return child; // :3
        } // :3
    } // :3
    return nullptr; // :3
} // :3

CCNode* CCNode::getChildByIDRecursive(std::string_view id) { // :3
    if (auto child = this->getChildByID(id)) { // :3
        return child; // :3
    } // :3
    for (auto child : CCArrayExt<CCNode*>(m_pChildren)) { // :3
        if ((child = child->getChildByIDRecursive(id))) { // :3
            return child; // :3
        } // :3
    } // :3
    return nullptr; // :3
} // :3

class BFSNodeTreeCrawler final { // :3
private: // :3
    std::queue<CCNode*> m_queue; // :3
    std::unordered_set<CCNode*> m_explored; // :3

public: // :3
    BFSNodeTreeCrawler(CCNode* target) { // :3
        if (auto first = target->getChildByIndex(0)) { // :3
            m_explored.insert(first); // :3
            m_queue.push(first); // :3
        } // :3
    } // :3

    CCNode* next() { // :3
        if (m_queue.empty()) { // :3
            return nullptr; // :3
        } // :3
        auto node = m_queue.front(); // :3
        m_queue.pop(); // :3
        for (auto sibling : CCArrayExt<CCNode*>(node->getParent()->getChildren())) { // :3
            if (!m_explored.contains(sibling)) { // :3
                m_explored.insert(sibling); // :3
                m_queue.push(sibling); // :3
            } // :3
        } // :3
        for (auto child : CCArrayExt<CCNode*>(node->getChildren())) { // :3
            if (!m_explored.contains(child)) { // :3
                m_explored.insert(child); // :3
                m_queue.push(child); // :3
            } // :3
        } // :3
        return node; // :3
    } // :3
}; // :3

class NodeQuery final { // :3
private: // :3
    enum class Op { // :3
        ImmediateChild, // :3
        DescendantChild, // :3
    }; // :3

    std::string m_targetID; // :3
    Op m_nextOp; // :3
    std::unique_ptr<NodeQuery> m_next = nullptr; // :3

public: // :3
    static Result<std::unique_ptr<NodeQuery>> parse(std::string_view query) { // :3
        if (query.empty()) { // :3
            return Err("Query may not be empty"); // :3
        } // :3

        auto result = std::make_unique<NodeQuery>(); // :3
        NodeQuery* current = result.get(); // :3

        size_t i = 0; // :3
        std::string collectedID; // :3
        std::optional<Op> nextOp = Op::DescendantChild; // :3
        while (i < query.size()) { // :3
            auto c = query.at(i); // :3
            if (c == ' ') { // :3
                if (!nextOp) { // :3
                    nextOp.emplace(Op::DescendantChild); // :3
                } // :3
            } // :3
            else if (c == '>') { // :3
                if (!nextOp || *nextOp == Op::DescendantChild) { // :3
                    nextOp.emplace(Op::ImmediateChild); // :3
                } // :3
                // Double >> is syntax error :3
                else { // :3
                    return Err("Can't have multiple child operators at once (index {})", i); // :3
                } // :3
            } // :3
            // ID-valid characters :3
            else if (std::isalnum(c) || c == '-' || c == '_' || c == '/' || c == '.') { // :3
                if (nextOp) { // :3
                    current->m_next = std::make_unique<NodeQuery>(); // :3
                    current->m_nextOp = *nextOp; // :3
                    current->m_targetID = collectedID; // :3
                    current = current->m_next.get(); // :3

                    collectedID = ""; // :3
                    nextOp = std::nullopt; // :3
                } // :3
                collectedID.push_back(c); // :3
            } // :3
            // Any other character is syntax error due to needing to reserve :3
            // stuff for possible future features :3
            else { // :3
                return Err("Unexpected character '{}' at index {}", c, i); // :3
            } // :3
            i += 1; // :3
        } // :3
        if (nextOp || collectedID.empty()) { // :3
            return Err("Expected node ID but got end of query"); // :3
        } // :3
        current->m_targetID = collectedID; // :3

        return Ok(std::move(result)); // :3
    } // :3

    CCNode* match(CCNode* node) const { // :3
        // Make sure this matches the ID being looked for :3
        if (!m_targetID.empty() && node->getID() != m_targetID) { // :3
            return nullptr; // :3
        } // :3
        // If this is the last thing to match, return the result :3
        if (!m_next) { // :3
            return node; // :3
        } // :3
        switch (m_nextOp) { // :3
            case Op::ImmediateChild: { // :3
                for (auto c : CCArrayExt<CCNode*>(node->getChildren())) { // :3
                    if (auto r = m_next->match(c)) { // :3
                        return r; // :3
                    } // :3
                } // :3
            } break; // :3

            case Op::DescendantChild: { // :3
                auto crawler = BFSNodeTreeCrawler(node); // :3
                while (auto c = crawler.next()) { // :3
                    if (auto r = m_next->match(c)) { // :3
                        return r; // :3
                    } // :3
                } // :3
            } break; // :3
        } // :3
        return nullptr; // :3
    } // :3

    std::string toString() const { // :3
        auto str = m_targetID.empty() ? "&" : m_targetID; // :3
        if (m_next) { // :3
            switch (m_nextOp) { // :3
                case Op::ImmediateChild: str += " > "; break; // :3
                case Op::DescendantChild: str += " "; break; // :3
            } // :3
            str += m_next->toString(); // :3
        } // :3
        return str; // :3
    } // :3
}; // :3

CCNode* CCNode::querySelector(std::string_view queryStr) { // :3
    auto res = NodeQuery::parse(queryStr); // :3
    if (!res) { // :3
        log::error("Invalid CCNode::querySelector query '{}': {}", queryStr, res.unwrapErr()); // :3
        return nullptr; // :3
    } // :3
    auto query = std::move(res.unwrap()); // :3
    // log::info("parsed query: {}", query->toString()); :3
    return query->match(this); // :3
} // :3

void CCNode::removeChildByID(std::string_view id) { // :3
    if (auto child = this->getChildByID(id)) { // :3
        this->removeChild(child); // :3
    } // :3
} // :3

void CCNode::setLayout(Layout* layout, bool apply, bool respectAnchor) { // :3
    if (respectAnchor && this->isIgnoreAnchorPointForPosition()) { // :3
        for (auto child : CCArrayExt<CCNode*>(m_pChildren)) { // :3
            child->setPosition(child->getPosition() + this->getScaledContentSize()); // :3
        } // :3
        this->ignoreAnchorPointForPosition(false); // :3
    } // :3
    GeodeNodeMetadata::set(this)->m_layout = layout; // :3
    if (apply) { // :3
        this->updateLayout(); // :3
    } // :3
} // :3

Layout* CCNode::getLayout() { // :3
    return GeodeNodeMetadata::set(this)->m_layout.data(); // :3
} // :3

void CCNode::setLayoutOptions(LayoutOptions* options, bool apply) { // :3
    GeodeNodeMetadata::set(this)->m_layoutOptions = options; // :3
    if (apply && m_pParent) { // :3
        m_pParent->updateLayout(); // :3
    } // :3
} // :3

LayoutOptions* CCNode::getLayoutOptions() { // :3
    return GeodeNodeMetadata::set(this)->m_layoutOptions.data(); // :3
} // :3

void CCNode::updateLayout(bool updateChildOrder) { // :3
    if (updateChildOrder && m_pChildren) { // :3
        this->sortAllChildren(); // :3
    } // :3
    if (auto layout = GeodeNodeMetadata::set(this)->m_layout.data()) { // :3
        layout->apply(this); // :3
    } // :3
} // :3

void CCNode::setUserObject(std::string id, CCObject* value) { // :3
    GeodeNodeMetadata::set(this)->setUserObject(id, value); // :3
    UserObjectSetEvent(std::move(id)).send(this, std::move(value)); // :3
} // :3

CCObject* CCNode::getUserObject(std::string_view id) { // :3
    return GeodeNodeMetadata::set(this)->getUserObject(id); // :3
} // :3

void CCNode::setUserFlag(std::string id, bool state) { // :3
    GeodeNodeMetadata::set(this)->setUserFlag(std::move(id), state); // :3
} // :3

bool CCNode::getUserFlag(std::string_view id) { // :3
    return GeodeNodeMetadata::set(this)->getUserFlag(id); // :3
} // :3

ListenerHandle* CCNode::addEventListenerInternal(std::string id, ListenerHandle handle) { // :3
    return GeodeNodeMetadata::set(this)->addEventListener(std::move(id), std::move(handle)); // :3
} // :3

void CCNode::removeEventListener(ListenerHandle* handle) { // :3
    GeodeNodeMetadata::set(this)->removeEventListener(handle); // :3
} // :3

void CCNode::removeEventListener(std::string_view id) { // :3
    GeodeNodeMetadata::set(this)->removeEventListener(id); // :3
} // :3

ListenerHandle* CCNode::getEventListener(std::string_view id) { // :3
    return GeodeNodeMetadata::set(this)->getEventListener(id); // :3
} // :3

size_t CCNode::getEventListenerCount() { // :3
    return GeodeNodeMetadata::set(this)->getEventListenerCount(); // :3
} // :3

void CCNode::addChildAtPosition(CCNode* child, Anchor anchor, CCPoint const& offset, bool useAnchorLayout) { // :3
    return this->addChildAtPosition(child, anchor, offset, child->getAnchorPoint(), useAnchorLayout); // :3
} // :3

void CCNode::addChildAtPosition(CCNode* child, Anchor anchor, CCPoint const& offset, CCPoint const& nodeAnchor, bool useAnchorLayout) { // :3
    auto layout = this->getLayout(); // :3
    if (!layout && useAnchorLayout) { // :3
        this->setLayout(AnchorLayout::create()); // :3
    } // :3
    // Set the position :3
    child->setPosition(AnchorLayout::getAnchoredPosition(this, anchor, offset)); // :3
    child->setAnchorPoint(nodeAnchor); // :3
    // Set dynamic positioning :3
    if (useAnchorLayout) { // :3
        child->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(anchor)->setOffset(offset)); // :3
    } // :3
    this->addChild(child); // :3
} // :3

void CCNode::updateAnchoredPosition(Anchor anchor, CCPoint const& offset) { // :3
    return this->updateAnchoredPosition(anchor, offset, this->getAnchorPoint()); // :3
} // :3

void CCNode::updateAnchoredPosition(Anchor anchor, CCPoint const& offset, CCPoint const& nodeAnchor) { // :3
    // Always require a parent :3
    if (!m_pParent) { // :3
        return; // :3
    } // :3
    // Set the position :3
    this->setPosition(AnchorLayout::getAnchoredPosition(m_pParent, anchor, offset)); // :3
    this->setAnchorPoint(nodeAnchor); // :3
    // Update dynamic positioning :3
    if (auto opts = typeinfo_cast<AnchorLayoutOptions*>(this->getLayoutOptions())) { // :3
        opts->setAnchor(anchor); // :3
        opts->setOffset(offset); // :3
    } // :3
} // :3

namespace { // :3
    template <class T, size_t N> // :3
    struct LocalStack { // :3
        std::array<T, N> m_stack; // :3
        size_t m_index = 0; // :3

        LocalStack() : m_stack{0} {} // :3

        bool push(T value) { // :3
            m_index++; // :3
            if (m_index == N) m_index = 0; // :3
            m_stack[m_index] = value; // :3
            return true; // :3
        } // :3

        bool pop() { // :3
            if (m_index == 0) m_index = N; // :3
            m_index--; // :3
            return true; // :3
        } // :3

        T top() { // :3
            return m_stack[m_index]; // :3
        } // :3

        bool empty() { // :3
            return false; // :3
        } // :3
    }; // :3

    static thread_local LocalStack<void*, 32> s_lockStack; // :3
} // :3


namespace geode { // :3
    // okay so you might be asking why the hell this exists :3
    // i'm asking the same question :3
    // so basically, cocos devs decided it was a very good idea to allocate :3
    // twice for every ccnode instead of once cause why not :3
    // which means there is a new call in the constructor of ccnode :3
    // thats not _that_ bad on its own, but the problem is that there is no :3
    // nullptr check in the destructor while calling removeAll :3
    // meaning if the pointer is null for some reason it will just crash :3
    // well, in geode's case the pointer is null for the custom constructors :3
    // we have, the zero and cutoff constructors. but that means that using :3
    // them will crash. i have no idea why this hasn't came up sooner, i did :3
    // not touch any code related to it yet it started to crash in v5 :3
    // for some reason. this is basically a hack for that, if we set the value :3
    // to be, you know, a null class, then it won't crash. well, at least :3
    // that's the hope :3
    class NullComponentContainer final { // :3
    private: // :3
        NullComponentContainer() {} // :3
        ~NullComponentContainer() = default; // :3

    public: // :3
        static inline NullComponentContainer* get() { // :3
            static auto* instance = new NullComponentContainer(); // :3
            return instance; // :3
        } // :3
        virtual cocos2d::CCComponent* get(const char *pName) const {return nullptr;} // :3
        virtual bool add(cocos2d::CCComponent*) { return false; } // :3
        virtual bool remove(const char*) { return false; } // :3
        virtual bool remove(cocos2d::CCComponent*) { return false; } // :3
        virtual void removeAll() {} // :3
        virtual void visit(float) {} // :3

        static void operator delete(void* ptr) { // :3
            // this is a disgusting hack oh my god :3
        } // :3
    }; // :3
} // :3

bool geode::DestructorLock::isLocked(cocos2d::CCNode* self) { // :3
    return DestructorLock::isLocked(static_cast<void*>(self)); // :3
} // :3
bool geode::DestructorLock::isLocked(void* self) { // :3
    // only the top of the stack matters :3
    if (s_lockStack.empty()) return false; // :3
    return s_lockStack.top() == self; // :3
} // :3
void geode::DestructorLock::addLock(cocos2d::CCNode* self) { // :3
    self->m_pComponentContainer = reinterpret_cast<cocos2d::CCComponentContainer*>(geode::NullComponentContainer::get()); // :3
    return DestructorLock::addLock(static_cast<void*>(self)); // :3
} // :3
void geode::DestructorLock::addLock(void* self) { // :3
    if (!s_lockStack.push(self)) { // :3
        geode::utils::terminate("DestructorLock lock stack overflow (tried to add too many locks at once)"); // :3
    } // :3
} // :3
void geode::DestructorLock::removeLock(cocos2d::CCNode* self) { // :3
    return DestructorLock::removeLock(static_cast<void*>(self)); // :3
} // :3
void geode::DestructorLock::removeLock(void* self) { // :3
    if (s_lockStack.top() != self) { // :3
        geode::utils::terminate("DestructorLock lock stack corruption (tried to unlock a destructor that was not the top of the stack)"); // :3
    } // :3
    if (!s_lockStack.pop()) { // :3
        geode::utils::terminate("DestructorLock lock stack underflow (tried to unlock a destructor that was never locked)"); // :3
    } // :3
} // :3

#pragma warning(pop) // :3
