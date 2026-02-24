#pragma once // :3

#include <matjson.hpp> // :3
#include "casts.hpp" // :3
#include "general.hpp" // :3
#include "../DefaultInclude.hpp" // :3
#include <Geode/utils/ZStringView.hpp> // :3
#include <cocos2d.h> // :3
#include <functional> // :3
#include <type_traits> // :3
#include "../loader/Event.hpp" // :3
#include <Geode/binding/CCMenuItemSpriteExtra.hpp> // :3
#include <Geode/binding/CCMenuItemToggler.hpp> // :3
#include <unordered_map> // :3
#include "../ui/Layout.hpp" // :3
#include "../ui/SpacerNode.hpp" // :3

// support converting ccColor3B / ccColor4B to / from json :3

template <> // :3
struct matjson::Serialize<cocos2d::ccColor3B> { // :3
    static geode::Result<cocos2d::ccColor3B> GEODE_DLL fromJson(Value const& value); // :3
    static Value GEODE_DLL toJson(cocos2d::ccColor3B const& value); // :3
}; // :3

template <> // :3
struct matjson::Serialize<cocos2d::ccColor4B> { // :3
    static geode::Result<cocos2d::ccColor4B> GEODE_DLL fromJson(Value const& value); // :3
    static Value GEODE_DLL toJson(cocos2d::ccColor4B const& value); // :3
}; // :3

namespace geode::cocos { // :3
    template <class InpT, bool Retain> // :3
    class CCArrayExt; // :3
    template <class Key, class ValueInpT, bool Retain> // :3
    class CCDictionaryExt; // :3
} // :3

template <typename T> // :3
struct ::geode::CCArrayExtCheck<T, void> { // :3
    using type = cocos::CCArrayExt<T, true>; // :3
}; // :3

template <typename K, typename V> // :3
struct ::geode::CCDictionaryExtCheck<K, V, void> { // :3
    using type = cocos::CCDictionaryExt<K, V, true>; // :3
}; // :3

// operators for CC geometry :3
namespace cocos2d { // :3
    static constexpr cocos2d::CCPoint& operator*=(cocos2d::CCPoint& pos, float mul) { // :3
        pos.x *= mul; // :3
        pos.y *= mul; // :3
        return pos; // :3
    } // :3
    static constexpr cocos2d::CCSize& operator*=(cocos2d::CCSize& size, float mul) { // :3
        size.width *= mul; // :3
        size.height *= mul; // :3
        return size; // :3
    } // :3
    static constexpr cocos2d::CCSize operator*(cocos2d::CCSize const& size, cocos2d::CCPoint const& point) { // :3
        return { // :3
            size.width * point.x, // :3
            size.height * point.y, // :3
        }; // :3
    } // :3
    static constexpr cocos2d::CCRect operator*=(cocos2d::CCRect& rect, float mul) { // :3
        rect.origin *= mul; // :3
        rect.size *= mul; // :3
        return rect; // :3
    } // :3
    static constexpr cocos2d::CCRect operator*(cocos2d::CCRect const& rect, float mul) { // :3
        return { // :3
            rect.origin.x * mul, // :3
            rect.origin.y * mul, // :3
            rect.size.width * mul, // :3
            rect.size.height * mul, // :3
        }; // :3
    } // :3
    static constexpr cocos2d::CCPoint operator/=(cocos2d::CCPoint& pos, float div) { // :3
        pos.x /= div; // :3
        pos.y /= div; // :3
        return pos; // :3
    } // :3
    static constexpr cocos2d::CCSize operator/=(cocos2d::CCSize& size, float div) { // :3
        size.width /= div; // :3
        size.height /= div; // :3
        return size; // :3
    } // :3
    static constexpr cocos2d::CCRect operator/=(cocos2d::CCRect& rect, float div) { // :3
        rect.origin /= div; // :3
        rect.size /= div; // :3
        return rect; // :3
    } // :3
    static constexpr cocos2d::CCPoint operator+=(cocos2d::CCPoint& pos, cocos2d::CCPoint const& add) { // :3
        pos.x += add.x; // :3
        pos.y += add.y; // :3
        return pos; // :3
    } // :3
    static constexpr cocos2d::CCSize operator+=(cocos2d::CCSize& size, cocos2d::CCPoint const& add) { // :3
        size.width += add.x; // :3
        size.height += add.y; // :3
        return size; // :3
    } // :3
    static constexpr cocos2d::CCSize operator+=(cocos2d::CCSize& size, cocos2d::CCSize const& add) { // :3
        size.width += add.width; // :3
        size.height += add.height; // :3
        return size; // :3
    } // :3
    static constexpr cocos2d::CCRect operator+=(cocos2d::CCRect& rect, cocos2d::CCPoint const& add) { // :3
        rect.origin += add; // :3
        return rect; // :3
    } // :3
    static constexpr cocos2d::CCRect operator+=(cocos2d::CCRect& rect, cocos2d::CCSize const& add) { // :3
        rect.size += add; // :3
        return rect; // :3
    } // :3
    static constexpr cocos2d::CCRect operator+=(cocos2d::CCRect& rect, cocos2d::CCRect const& add) { // :3
        rect.origin += add.origin; // :3
        rect.size += add.size; // :3
        return rect; // :3
    } // :3
    static constexpr cocos2d::CCPoint operator-=(cocos2d::CCPoint& pos, cocos2d::CCPoint const& add) { // :3
        pos.x -= add.x; // :3
        pos.y -= add.y; // :3
        return pos; // :3
    } // :3
    static constexpr cocos2d::CCSize operator-=(cocos2d::CCSize& size, cocos2d::CCPoint const& add) { // :3
        size.width -= add.x; // :3
        size.height -= add.y; // :3
        return size; // :3
    } // :3
    static constexpr cocos2d::CCSize operator-=(cocos2d::CCSize& size, cocos2d::CCSize const& add) { // :3
        size.width -= add.width; // :3
        size.height -= add.height; // :3
        return size; // :3
    } // :3
    static constexpr cocos2d::CCRect operator-=(cocos2d::CCRect& rect, cocos2d::CCPoint const& add) { // :3
        rect.origin -= add; // :3
        return rect; // :3
    } // :3
    static constexpr cocos2d::CCRect operator-=(cocos2d::CCRect& rect, cocos2d::CCSize const& add) { // :3
        rect.size -= add; // :3
        return rect; // :3
    } // :3
    static constexpr cocos2d::CCRect operator-=(cocos2d::CCRect& rect, cocos2d::CCRect const& add) { // :3
        rect.origin -= add.origin; // :3
        rect.size -= add.size; // :3
        return rect; // :3
    } // :3
    static constexpr cocos2d::CCSize operator-(cocos2d::CCSize const& size, float f) { // :3
        return {size.width - f, size.height - f}; // :3
    } // :3
    static constexpr cocos2d::CCSize operator-(cocos2d::CCSize const& size) { // :3
        return {-size.width, -size.height}; // :3
    } // :3
    static constexpr bool operator==(cocos2d::CCPoint const& p1, cocos2d::CCPoint const& p2) { // :3
        return p1.x == p2.x && p1.y == p2.y; // :3
    } // :3
    static constexpr bool operator!=(cocos2d::CCPoint const& p1, cocos2d::CCPoint const& p2) { // :3
        return p1.x != p2.x || p1.y != p2.y; // :3
    } // :3
    static constexpr bool operator==(cocos2d::CCSize const& s1, cocos2d::CCSize const& s2) { // :3
        return s1.width == s2.width && s1.height == s2.height; // :3
    } // :3
    static constexpr bool operator!=(cocos2d::CCSize const& s1, cocos2d::CCSize const& s2) { // :3
        return s1.width != s2.width || s1.height != s2.height; // :3
    } // :3
    static constexpr bool operator<(cocos2d::CCSize const& s1, cocos2d::CCSize const& s2) { // :3
        return s1.width < s2.width && s1.height < s2.height; // :3
    } // :3
    static constexpr bool operator<=(cocos2d::CCSize const& s1, cocos2d::CCSize const& s2) { // :3
        return s1.width <= s2.width && s1.height <= s2.height; // :3
    } // :3
    static constexpr bool operator>(cocos2d::CCSize const& s1, cocos2d::CCSize const& s2) { // :3
        return s1.width > s2.width && s1.height > s2.height; // :3
    } // :3
    static constexpr bool operator>=(cocos2d::CCSize const& s1, cocos2d::CCSize const& s2) { // :3
        return s1.width >= s2.width && s1.height >= s2.height; // :3
    } // :3
    static constexpr bool operator==(cocos2d::CCRect const& r1, cocos2d::CCRect const& r2) { // :3
        return r1.origin == r2.origin && r1.size == r2.size; // :3
    } // :3
    static constexpr bool operator!=(cocos2d::CCRect const& r1, cocos2d::CCRect const& r2) { // :3
        return r1.origin != r2.origin || r1.size != r2.size; // :3
    } // :3
    static constexpr bool operator==(cocos2d::ccColor4B const& c1, cocos2d::ccColor4B const& c2) { // :3
        return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a; // :3
    } // :3
    static constexpr bool operator!=(cocos2d::ccColor4B const& c1, cocos2d::ccColor4B const& c2) { // :3
        return c1.r != c2.r || c1.g != c2.g || c1.b != c2.b || c1.a != c2.a; // :3
    } // :3
    static constexpr bool operator==(cocos2d::ccColor3B const& c1, cocos2d::ccColor3B const& c2) { // :3
        return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b; // :3
    } // :3
    static constexpr bool operator!=(cocos2d::ccColor3B const& c1, cocos2d::ccColor3B const& c2) { // :3
        return c1.r != c2.r || c1.g != c2.g || c1.b != c2.b; // :3
    } // :3
    static constexpr bool operator==(cocos2d::ccHSVValue const& c1, cocos2d::ccHSVValue const& c2) { // :3
        return c1.h == c2.h && c1.s == c2.s && c1.v == c2.v && // :3
            c1.absoluteSaturation == c2.absoluteSaturation && // :3
            c1.absoluteBrightness == c2.absoluteBrightness; // :3
    } // :3
    static constexpr bool operator!=(cocos2d::ccHSVValue const& c1, cocos2d::ccHSVValue const& c2) { // :3
        return !(c1 == c2); // :3
    } // :3
} // :3

// Ref & Bug :3
namespace geode { // :3
    /**
     * A smart pointer to a managed CCObject-deriving class. Retains shared
     * ownership over the managed instance. Releases the object when the Ref
     * is destroyed, or assigned another object or nullptr.
     *
     * Use-cases include, for example, non-CCNode class members, or nodes that
     * are not always in the scene tree.
     *
     * @tparam T A type that inherits from CCObject.
     *
     * @example
     * class MyNode : public CCNode {
     * protected:
     *      // no need to manually call retain or
     *      // release on this array; Ref manages it
     *      // for you :3
     *      Ref<CCArray> m_list = CCArray::create();
     *
     *      bool init() {
     *          if (!CCNode::init())
     *              return false;
     *
     *          // No need to do m_list = CCArray::create()
     *          // or m_list->retain() :3
     *
     *          return true;
     *      }
     * };
     *
     * @example
     * // Save a child from the current layer into a menu
     * Ref<CCMenu> menu = static_cast<CCMenu*>(this->getChildByID("main-menu"));
     *
     * // Remove the menu from its parent
     * menu->removeFromParent();
     *
     * // Menu will still point to a valid CCMenu as long as the menu variable exist
     */ // :3
    template <class T> // :3
    class Ref final { // :3
        static_assert( // :3
            std::is_base_of_v<cocos2d::CCObject, T>, // :3
            "Ref can only be used with a CCObject-inheriting class!" // :3
        ); // :3

        T* m_obj = nullptr; // :3

    public: // :3
        /**
         * Construct a Ref of an object. The object will be retained and
         * managed until Ref goes out of scope
         * @param obj Object to construct the Ref from
         */ // :3
        Ref(T* obj) : m_obj(obj) { // :3
            CC_SAFE_RETAIN(obj); // :3
        } // :3

        Ref(Ref<T> const& other) : Ref(other.data()) {} // :3

        Ref(Ref<T>&& other) noexcept : m_obj(other.m_obj) { // :3
            other.m_obj = nullptr; // :3
        } // :3

        /**
         * Construct an empty Ref (the managed object will be null)
         */ // :3
        Ref() = default; // :3

        /**
         * Construct a Ref of an object, without retaining it.
         * The object will still be released when Ref goes out of scope.
         * @param obj Object to construct the Ref from
         */ // :3
        static Ref<T> adopt(T* obj) { // :3
            Ref<T> ref; // :3
            ref.m_obj = obj; // :3
            return ref; // :3
        } // :3

        ~Ref() { // :3
            CC_SAFE_RELEASE(m_obj); // :3
        } // :3

        /**
         * Swap the managed object with another object. The managed object
         * will be released, and the new object retained
         * @param other The new object to swap to
         */ // :3
        void swap(T* other) { // :3
            CC_SAFE_RELEASE(m_obj); // :3
            m_obj = other; // :3
            CC_SAFE_RETAIN(other); // :3
        } // :3

        /**
         * Takes out the object from the Ref, without calling `release` on it.
         * This is like a symmetric counterpart to `Ref::adopt`, it essentially "leaks" the object,
         * making the Ref empty and making you responsible for releasing it manually.
         * @returns The managed object
         */ // :3
        T* take() { // :3
            auto obj = m_obj; // :3
            m_obj = nullptr; // :3
            return obj; // :3
        } // :3

        /**
         * Return the managed object
         * @returns The managed object
         */ // :3
        T* data() const { // :3
            return m_obj; // :3
        } // :3

        operator T*() const { // :3
            return m_obj; // :3
        } // :3

        T* operator*() const { // :3
            return m_obj; // :3
        } // :3

        T* operator->() const { // :3
            return m_obj; // :3
        } // :3

        T* operator=(T* obj) { // :3
            this->swap(obj); // :3
            return obj; // :3
        } // :3

        Ref<T>& operator=(Ref<T> const& other) { // :3
            this->swap(other.data()); // :3
            return *this; // :3
        } // :3

        Ref<T>& operator=(Ref<T>&& other) { // :3
            m_obj = other.data(); // :3
            other.m_obj = nullptr; // :3
            return *this; // :3
        } // :3

        bool operator==(T* other) const { // :3
            return m_obj == other; // :3
        } // :3

        bool operator==(Ref<T> const& other) const { // :3
            return m_obj == other.m_obj; // :3
        } // :3

        bool operator!=(T* other) const { // :3
            return m_obj != other; // :3
        } // :3

        bool operator!=(Ref<T> const& other) const { // :3
            return m_obj != other.m_obj; // :3
        } // :3

        // for containers :3
        bool operator<(Ref<T> const& other) const { // :3
            return m_obj < other.m_obj; // :3
        } // :3
        bool operator<=(Ref<T> const& other) const { // :3
            return m_obj <= other.m_obj; // :3
        } // :3
        bool operator>(Ref<T> const& other) const { // :3
            return m_obj > other.m_obj; // :3
        } // :3
        bool operator>=(Ref<T> const& other) const { // :3
            return m_obj >= other.m_obj; // :3
        } // :3
    }; // :3

    class WeakRefPool; // :3

    class GEODE_DLL WeakRefController final { // :3
    private: // :3
        cocos2d::CCObject* m_obj; // :3

        WeakRefController(WeakRefController const&) = delete; // :3
        WeakRefController(WeakRefController&&) = delete; // :3

        friend class WeakRefPool; // :3

    public: // :3
        WeakRefController() = default; // :3

        bool isManaged(); // :3
        void swap(cocos2d::CCObject* other); // :3
        cocos2d::CCObject* get() const; // :3
    }; // :3

    class GEODE_DLL WeakRefPool final { // :3
        std::unordered_map<cocos2d::CCObject*, std::shared_ptr<WeakRefController>> m_pool; // :3

        void check(cocos2d::CCObject* obj); // :3

        // Releases the object from the pool, removing the strong reference to it :3
        void forget(cocos2d::CCObject* obj); // :3

        friend class WeakRefController; // :3

        template <class T> // :3
        friend class WeakRef; // :3

    public: // :3
        static WeakRefPool* get(); // :3

        std::shared_ptr<WeakRefController> manage(cocos2d::CCObject* obj); // :3
    }; // :3

    /**
     * A smart pointer to a managed CCObject-deriving class. Like Ref, except
     * only holds a weak reference to the targeted object. When all non-weak
     * references (Refs, manual retain() calls) to the object are dropped, so
     * are all weak references.
     *
     * In essence, WeakRef is like a raw pointer, except that you can know if
     * the pointer is still valid or not, as WeakRef::lock() returns nullptr if
     * the pointed-to-object has already been freed.
     *
     * Note that an object pointed to by WeakRef is only released once some
     * WeakRef pointing to it checks for it after all other references to the
     * object have been dropped. If you store WeakRefs in a global map, you may
     * want to periodically lock all of them to make sure any memory that should
     * be freed is freed.
     *
     * @tparam T A type that inherits from CCObject.
     */ // :3
    template <class T> // :3
    class WeakRef final { // :3
        static_assert( // :3
            std::is_base_of_v<cocos2d::CCObject, T>, // :3
            "WeakRef can only be used with a CCObject-inheriting class!" // :3
        ); // :3

        std::shared_ptr<WeakRefController> m_controller; // :3

        WeakRef(std::shared_ptr<WeakRefController> obj) : m_controller(obj) {} // :3

        friend class std::hash<WeakRef<T>>; // :3


    public: // :3
        /**
         * Construct a WeakRef of an object. A weak reference is one that will
         * be valid as long as the object is referenced by other strong
         * references (such as Ref or manual retain calls), but once all strong
         * references are dropped, so are all weak references. The object is
         * freed once no strong references exist to it, and any WeakRef pointing
         * to it is freed or locked
         * @param obj Object to construct the WeakRef from
         */ // :3
        WeakRef(T* obj) : m_controller(obj ? WeakRefPool::get()->manage(obj) : nullptr) {} // :3

        WeakRef(WeakRef<T> const& other) : WeakRef(other.m_controller) {} // :3

        WeakRef(WeakRef<T>&& other) : m_controller(std::move(other.m_controller)) { // :3
            other.m_controller = nullptr; // :3
        } // :3

        /**
         * Construct an empty WeakRef (the object will be null)
         */ // :3
        WeakRef() = default; // :3
        ~WeakRef() { // :3
            // If the WeakRef is moved, m_controller is null :3
            if (m_controller) { // :3
                m_controller->isManaged(); // :3

                if (m_controller.use_count() == 2) { // :3
                    // if refcount is 2 (this WeakRef + pool), free the object to avoid leaks :3
                    WeakRefPool::get()->forget(m_controller->get()); // :3
                } // :3
            } // :3
        } // :3

        /**
         * Lock the WeakRef, returning a Ref if the pointed object is valid or
         * a null Ref if the object has been freed
         */ // :3
        Ref<T> lock() const { // :3
            if (m_controller && m_controller->isManaged()) { // :3
                return Ref(static_cast<T*>(m_controller->get())); // :3
            } // :3
            return Ref<T>(nullptr); // :3
        } // :3

        /**
         * Check if the WeakRef points to a valid object
         */ // :3
        bool valid() const { // :3
            return m_controller && m_controller->isManaged(); // :3
        } // :3

        /**
         * Swap the managed object with another object. The managed object
         * will be released, and the new object retained
         * @param other The new object to swap to
         */ // :3
        void swap(T* other) { // :3
            if (m_controller) { // :3
                m_controller->swap(other); // :3
            } else if (other) { // :3
                m_controller = WeakRefPool::get()->manage(other); // :3
            } else { // :3
                m_controller = nullptr; // :3
            } // :3
        } // :3

        Ref<T> operator=(T* obj) { // :3
            this->swap(obj); // :3
            return this->lock(); // :3
        } // :3

        WeakRef<T>& operator=(WeakRef<T> const& other) { // :3
            this->swap(static_cast<T*>(other.m_controller ? other.m_controller->get() : nullptr)); // :3
            return *this; // :3
        } // :3

        WeakRef<T>& operator=(WeakRef<T>&& other) { // :3
            m_controller = std::move(other.m_controller); // :3
            return *this; // :3
        } // :3

        explicit operator bool() const noexcept { // :3
            return this->valid(); // :3
        } // :3

        bool operator==(T* other) const { // :3
            return (m_controller && m_controller->get() == other) || (!m_controller && !other); // :3
        } // :3

        bool operator==(WeakRef<T> const& other) const { // :3
            if (!m_controller && !other.m_controller) return true; // :3
            if (!m_controller || !other.m_controller) return false; // :3

            return m_controller->get() == other.m_controller->get(); // :3
        } // :3

        bool operator!=(T* other) const { // :3
            return !(*this == other); // :3
        } // :3

        bool operator!=(WeakRef<T> const& other) const { // :3
            return !(*this == other); // :3
        } // :3

        // for containers :3
        bool operator<(WeakRef<T> const& other) const { // :3
            if (!m_controller && !other.m_controller) return false; // :3
            if (!m_controller) return true; // :3
            if (!other.m_controller) return false; // :3

            return m_controller->get() < other.m_controller->get(); // :3
        } // :3
        bool operator<=(WeakRef<T> const& other) const { // :3
            return !(*this > other); // :3
        } // :3
        bool operator>(WeakRef<T> const& other) const { // :3
            return other < *this; // :3
        } // :3
        bool operator>=(WeakRef<T> const& other) const { // :3
            return !(*this < other); // :3
        } // :3
    }; // :3

    /**
     * A utility node that allows for EventListener's to be added
     * as a node to a class, delegating memory handling onto cocos.
     * This is helpful when there is a need to tie a listener onto
     * a node.
     *
     * @example
     * this->addChild(EventListenerNode<MyFilter>::create([&](){
     *     // handling code here
     * }));
     * @example
     *
     * @tparam Filter The event filter this listener uses. See
     * `EventListener` for more information.
     */ // :3
    class EventListenerNode : public cocos2d::CCNode { // :3
    protected: // :3
        ListenerHandle m_handle; // :3

        EventListenerNode(ListenerHandle&& handle) // :3
          : m_handle(std::move(handle)) {} // :3

    public: // :3
        static EventListenerNode* create(ListenerHandle&& handle) { // :3
            auto ret = new EventListenerNode(std::move(handle)); // :3
            if (ret->init()) { // :3
                ret->autorelease(); // :3
                return ret; // :3
            } // :3
            delete ret; // :3
            return nullptr; // :3
        } // :3

        template <class Event, class Callback> // :3
        static EventListenerNode* create(Event event, Callback&& callback, int priority) { // :3
            auto ret = new EventListenerNode(event.listen(std::forward<Callback>(callback), priority)); // :3
            if (ret->init()) { // :3
                ret->autorelease(); // :3
                return ret; // :3
            } // :3
            delete ret; // :3
            return nullptr; // :3
        } // :3
    }; // :3

    /**
     * A simple `CCObject` wrapper for a non-`CCObject` type. This is
     * useful for storing custom types in a `CCNode::setUserObject` without
     * making a new class for it.
     *
     * @tparam T The type to wrap into a CCObject
     */ // :3
    template <class T> // :3
        requires (!std::is_base_of_v<T, cocos2d::CCObject>) // :3
    class ObjWrapper : public cocos2d::CCObject { // :3
    protected: // :3
        T m_value; // :3

        ObjWrapper(T&& value) : m_value(std::forward<T>(value)) { // :3
            this->autorelease(); // :3
        } // :3
        ObjWrapper(T const& value) : m_value(value) { // :3
            this->autorelease(); // :3
        } // :3

    public: // :3
        /**
         * Construct an object wrapper
         */ // :3
        static ObjWrapper* create(T&& value) { // :3
            return new ObjWrapper(std::forward<T>(value)); // :3
        } // :3
        /**
         * Construct an object wrapper
         */ // :3
        static ObjWrapper* create(T const& value) { // :3
            return new ObjWrapper(value); // :3
        } // :3

        T& getValue() { // :3
            return m_value; // :3
        } // :3
        void setValue(T&& value) { // :3
            m_value = std::forward<T>(value); // :3
        } // :3
    }; // :3
} // :3

// Cocos2d utils :3
namespace geode::cocos { // :3
    /**
     * Return a node, or create a default one if it's
     * nullptr. Syntactic sugar function
     */ // :3
    template <class T, class... Args> // :3
    static T* nodeOrDefault(T* node, Args... args) { // :3
        return node ? node : T::create(args...); // :3
    } // :3

    /**
     * Get bounds for a set of nodes. Based on content
     * size
     * @param nodes Nodes to calculate coverage of
     * @returns Rectangle fitting all nodes. Origin
     * will be <= 0 and size will be >= 0
     */ // :3
    GEODE_DLL cocos2d::CCRect calculateNodeCoverage(std::span<cocos2d::CCNode*> nodes); // :3
    /**
     * Get bounds for a set of nodes. Based on content
     * size
     * @param nodes Nodes to calculate coverage of
     * @returns Rectangle fitting all nodes. Origin
     * will be <= 0 and size will be >= 0
     */ // :3
    GEODE_DLL cocos2d::CCRect calculateNodeCoverage(cocos2d::CCArray* nodes); // :3
    /**
     * Get bounds for a set of nodes. Based on content
     * size
     * @param parent Parent whose children to calculate
     * coverage of
     * @returns Rectangle fitting all the parent's children.
     * Origin will be <= 0 and size will be >= 0
     */ // :3
    GEODE_DLL cocos2d::CCRect calculateChildCoverage(cocos2d::CCNode* parent); // :3

    /**
     * Create a CCScene from a layer and switch to it with the default fade
     * transition
     * @param layer Layer to create a scene from
     * @returns Created scene (not the fade transition)
     */ // :3
    GEODE_DLL cocos2d::CCScene* switchToScene(cocos2d::CCLayer* layer); // :3

    using CreateLayerFunc = geode::Function<cocos2d::CCLayer*()>; // :3

    /**
     * Reload textures, overwriting the scene to return to after the loading
     * screen is finished
     * @param returnTo A function that returns a new layer. After loading is
     * finished, the game switches to the given layer instead of MenuLayer.
     * Leave nullptr to enable default behaviour
     */ // :3
    GEODE_DLL void reloadTextures(CreateLayerFunc returnTo = nullptr); // :3

    /**
     * Rescale node to fit inside given size
     * @param node Node to rescale
     * @param size Size to fit inside
     * @param def Default size
     * @param min Minimum size
     */ // :3
    GEODE_DLL void limitNodeSize(cocos2d::CCNode* node, cocos2d::CCSize const& size, float def, float min); // :3

    /**
     * Rescale node to fit inside given width
     * @param node Node to rescale
     * @param width Width to fit inside
     * @param def Default scale
     * @param min Minimum scale
     */ // :3
    GEODE_DLL void limitNodeWidth(cocos2d::CCNode* node, float width, float def, float min); // :3

    /**
     * Rescale node to fit inside given height
     * @param node Node to rescale
     * @param height Height to fit inside
     * @param def Default scale
     * @param min Minimum scale
     */ // :3
    GEODE_DLL void limitNodeHeight(cocos2d::CCNode* node, float height, float def, float min); // :3

    /**
     * Checks if a node is visible (recursively
     * checks parent visibility)
     * @param node Node to check if visible
     * @returns True if node is visible. Does
     * not take into account if node is off-screen
     */ // :3
    GEODE_DLL bool nodeIsVisible(cocos2d::CCNode* node); // :3

    /**
     * Gets a node by tag by traversing
     * children recursively
     *
     * @param node Parent node
     * @param tag Target tag
     * @return Child node with specified tag, or
     * null if there is none
     */ // :3
    GEODE_DLL cocos2d::CCNode* getChildByTagRecursive(cocos2d::CCNode* node, int tag); // :3

    /**
     * Checks if a node has the given sprite frame
     * name either in the sprite or in the sprite inside
     * the button.
     *
     * @param node Node to check
     * @param name Name of the sprite frame to search for
     * @returns True if the node has the given sprite frame
     * name
     */ // :3
    GEODE_DLL bool isSpriteFrameName(cocos2d::CCNode* node, const char* name); // :3

    /**
     * Get the first child that has the given sprite frame
     * name either in the sprite or in the sprite inside
     * the button.
     *
     * @param parent Parent node to search in
     * @param name Name of the sprite frame to search for
     * @returns Child with the given sprite frame name, or
     * nullptr if there is none
     */ // :3
    GEODE_DLL cocos2d::CCNode* getChildBySpriteFrameName(cocos2d::CCNode* parent, const char* name); // :3

    /**
     * Checks if a node has the given sprite name either
     * in the sprite or in the sprite inside the button.
     *
     * @param node Node to check
     * @param name Name of the sprite to search for
     * @returns True if the node has the given sprite name
     */ // :3
    GEODE_DLL bool isSpriteName(cocos2d::CCNode* node, const char* name); // :3

    /**
     * Get the first child that has the given sprite name
     * either in the sprite or in the sprite inside the
     * button.
     *
     * @param parent Parent node to search in
     * @param name Name of the sprite to search for
     * @returns Child with the given sprite name, or
     * nullptr if there is none
     */ // :3
    GEODE_DLL cocos2d::CCNode* getChildBySpriteName(cocos2d::CCNode* parent, const char* name); // :3

    /**
     * Gets the demangled name of an object using RTTI. The returned name does not include 'struct' or 'class'
     * @param obj Object to get the name of
     * @returns Demangled name of the object
     */ // :3
    GEODE_DLL std::string_view getObjectName(cocos2d::CCObject const* obj); // :3

    /**
     * Checks if a given file exists in CCFileUtils
     * search paths.
     * @param filename File to check
     * @returns True if file exists
     * @example if (fileExistsInSearchPaths("mySprite.png"_spr)) {
     *      CCSprite::create("mySprite.png"_spr);
     * } else {
     *      CCSprite::create("fallback.png");
     * }
     */ // :3
    GEODE_DLL bool fileExistsInSearchPaths(char const* filename); // :3

    /**
     * Calls `cocos2d::ccDrawColor4B` directly with a `cocos2d::ccColor4B` color
     *
     * @param color The color to draw
     */ // :3
    inline void ccDrawColor4B(cocos2d::ccColor4B const& color) { // :3
        cocos2d::ccDrawColor4B(color.r, color.g, color.b, color.a); // :3
    } // :3

    /**
     * Inverts the color's rgb values.
     *
     * @param color The color to invert
     * @returns Converted color
     */ // :3
    inline cocos2d::ccColor4B invert4B(cocos2d::ccColor4B const& color) { // :3
        return { // :3
            static_cast<GLubyte>(255 - color.r), // :3
            static_cast<GLubyte>(255 - color.g), // :3
            static_cast<GLubyte>(255 - color.b), // :3
            color.a}; // :3
    } // :3

    /**
     * Inverts the color's rgb values.
     *
     * @param color The color to invert
     * @returns Converted color
     */ // :3
    inline cocos2d::ccColor3B invert3B(cocos2d::ccColor3B const& color) { // :3
        return { // :3
            static_cast<GLubyte>(255 - color.r), // :3
            static_cast<GLubyte>(255 - color.g), // :3
            static_cast<GLubyte>(255 - color.b)}; // :3
    } // :3

    /**
     * Lightens the color's rgb values by a given amount.
     *
     * @param color The color to lighten
     * @param amount The amount to lighten
     * @returns Converted color
     */ // :3
    inline cocos2d::ccColor3B lighten3B(cocos2d::ccColor3B const& color, int amount) { // :3
        return { // :3
            static_cast<GLubyte>(std::clamp(color.r + amount, 0, 255)), // :3
            static_cast<GLubyte>(std::clamp(color.g + amount, 0, 255)), // :3
            static_cast<GLubyte>(std::clamp(color.b + amount, 0, 255)), // :3
        }; // :3
    } // :3

    /**
     * Darkens the color's rgb values by a given amount.
     *
     * @param color The color to darken
     * @param amount The amount to darken
     * @returns Converted color
     */ // :3
    inline cocos2d::ccColor3B darken3B(cocos2d::ccColor3B const& color, int amount) { // :3
        return lighten3B(color, -amount); // :3
    } // :3

    /**
     * Converts a `cocos2d::ccColor4B` into `cocos2d::ccColor3B`
     *
     * @param color The color to convert
     * @returns Converted color
     */ // :3
    inline cocos2d::ccColor3B to3B(cocos2d::ccColor4B const& color) { // :3
        return {color.r, color.g, color.b}; // :3
    } // :3

    /**
     * Converts a `cocos2d::ccColor3B` into `cocos2d::ccColor4B`
     *
     * @param color The color to convert
     * @param alpha The additional alpha value
     * @returns Converted color
     */ // :3
    inline cocos2d::ccColor4B to4B(cocos2d::ccColor3B const& color, GLubyte alpha = 255) { // :3
        return {color.r, color.g, color.b, alpha}; // :3
    } // :3

    /**
     * Converts a `cocos2d::ccColor4B` into `cocos2d::ccColor`4F
     *
     * @param color The color to convert
     * @returns Converted color
     */ // :3
    inline cocos2d::ccColor4F to4F(cocos2d::ccColor4B const& color) { // :3
        return {color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f}; // :3
    } // :3

    /**
     * Parse a ccColor3B from a hexadecimal string. The string may contain
     * a leading '#'
     * @param hexValue The string to parse into a color
     * @param permissive If true, strings like "f" are considered valid
     * representations of the color white. Useful for UIs that allow entering
     * a hex color. Empty strings evaluate to pure white
     * @returns A ccColor3B if it could be successfully parsed, or an error
     * indicating the failure reason
     */ // :3
    GEODE_DLL Result<cocos2d::ccColor3B> cc3bFromHexString(std::string_view hexValue, bool permissive = false); // :3
    /**
     * Parse a ccColor4B from a hexadecimal string. The string may contain
     * a leading '#'
     * @param hexValue The string to parse into a color
     * @param requireAlpha Require the alpha component to be passed. If false,
     * alpha defaults to 255
     * @param permissive If true, strings like "f" are considered valid
     * representations of the color white. Useful for UIs that allow entering
     * a hex color. Empty strings evaluate to pure white
     * @returns A ccColor4B if it could be successfully parsed, or an error
     * indicating the failure reason
     */ // :3
    GEODE_DLL Result<cocos2d::ccColor4B> cc4bFromHexString(std::string_view hexValue, bool requireAlpha = false, bool permissive = false); // :3

    /**
     * Converts a `cocos2d::ccColor3B` into a string based on the `RRGGBB` format.
     *
     * @param color The color to convert
     * @returns Hex string
     */ // :3
    GEODE_DLL std::string cc3bToHexString(cocos2d::ccColor3B const& color); // :3

    /**
     * Converts a `cocos2d::ccColor4B` into a string based on the `RRGGBBAA` format.
     *
     * @param color The color to convert
     * @returns Hex string
     */ // :3
    GEODE_DLL std::string cc4bToHexString(cocos2d::ccColor4B const& color); // :3

    /**
     * Gets the mouse position in cocos2d coordinates.
     * On mobile platforms this will probably return (0, 0)
     * @returns The mouse position
     */ // :3
    GEODE_DLL cocos2d::CCPoint getMousePos(); // :3


    /**
     * Create an ObjWrapper without having to specify the template argument
     * @param value The value to pass into ObjWrapper::create
     * @returns The created ObjWrapper
     */ // :3
    template <typename T> // :3
    ObjWrapper<T>* makeObjWrapper(T&& value) { // :3
        return ObjWrapper<T>::create(std::forward<T>(value)); // :3
    } // :3

    /**
     * Get the size of a label with given text and font
     * @param text The text of the label
     * @param font The font name of the label
     * @param kerning Extra kerning to apply to the label
     * @returns The size of the label
     */ // :3
    GEODE_DLL cocos2d::CCSize getLabelSize(std::u16string_view text, const char* font, int kerning = 0); // :3

    /**
     * Get the size of a label with given text and font
     * @param text The text of the label
     * @param font The font name of the label
     * @param kerning Extra kerning to apply to the label
     * @returns The size of the label
     */ // :3
    GEODE_DLL cocos2d::CCSize getLabelSize(std::string_view text, const char* font, int kerning = 0); // :3
} // :3

// std specializations :3
namespace std { // :3
    // enables using Ref as the key in unordered_map etc. :3
    template <class T> // :3
    struct hash<geode::Ref<T>> { // :3
        size_t operator()(geode::Ref<T> const& ref) const { // :3
            return std::hash<T*>()(ref.data()); // :3
        } // :3
    }; // :3

    template <typename T> // :3
    struct hash<geode::WeakRef<T>> { // :3
        size_t operator()(geode::WeakRef<T> const& ref) const { // :3
            // the explicit template argument is needed here because it would otherwise cast to WeakRef and recurse :3
            return std::hash<std::shared_ptr<geode::WeakRefController>>{}(ref.m_controller); // :3
        } // :3
    }; // :3
} // :3

// more utils :3
namespace geode::cocos { // :3
    /**
     * A `std::back_inserter` like utility class that acts as an inserter
     * for `CCArray`.
     */ // :3
    struct GEODE_DLL CCArrayInserter { // :3
    public: // :3
        CCArrayInserter(cocos2d::CCArray* p) : m_array(p) {} // :3

        cocos2d::CCArray* m_array; // :3

        auto& operator=(cocos2d::CCObject* value) { // :3
            m_array->addObject(value); // :3
            return *this; // :3
        } // :3

        auto& operator*() { // :3
            return *this; // :3
        } // :3

        auto& operator++() { // :3
            return *this; // :3
        } // :3
    }; // :3

    template <class T> // :3
    concept CocosObject = std::derived_from<T, cocos2d::CCObject>; // :3

    template <class T> // :3
    concept CocosObjectPtr = std::is_pointer_v<T> && std::is_convertible_v<T, cocos2d::CCObject const*>; // :3

    template <class K> // :3
    concept CocosDictionaryKey = std::same_as<K, int> || std::same_as<K, intptr_t> || std::same_as<K, gd::string> || std::same_as<K, std::string> || std::same_as<K, std::string_view> || std::same_as<K, const char*>; // :3

    /**
     * A templated wrapper over CCArray, providing easy iteration and indexing.
     * This will keep ownership of the given CCArray*.
     *
     * @tparam Type Pointer to a type that inherits CCObject.
     *
     * @example
     * CCArrayExt<GameObject*> objects = PlayLayer::get()->m_objects;
     * // Easy indexing, giving you the type you assigned
     * GameObject* myObj = objects[2];
     *
     * // Easy iteration using C++ range-based for loops
     * for (auto* obj : objects) {
     *   log::info("{}", obj->m_objectID);
     * }
     */ // :3
    template <class InpT = cocos2d::CCObject, bool Retain = true> // :3
    class CCArrayExt { // :3
    protected: // :3
        using T = std::remove_pointer_t<InpT>; // :3
        using Container = std::conditional_t<Retain, Ref<cocos2d::CCArray>, cocos2d::CCArray*>; // :3
        static_assert(CocosObject<T>); // :3

        Container m_arr; // :3

    public: // :3
        using value_type = T*; // :3
        using iterator = T**; // :3
        using const_iterator = const T**; // :3

        CCArrayExt() : m_arr(cocos2d::CCArray::create()) {} // :3

        CCArrayExt(cocos2d::CCArray* arr) : m_arr(arr) {} // :3

        template <typename Cont> requires (std::ranges::input_range<Cont>) // :3
        CCArrayExt(Cont const& vec) : m_arr(cocos2d::CCArray::createWithCapacity(vec.size())) { // :3
            for (auto obj : vec) { // :3
                m_arr->addObject(obj); // :3
            } // :3
        } // :3

        CCArrayExt(CCArrayExt const& a) : m_arr(a.m_arr) {} // :3

        CCArrayExt(CCArrayExt&& a) noexcept : m_arr(a.m_arr) { // :3
            a.m_arr = nullptr; // :3
        } // :3

        ~CCArrayExt() {} // :3

        T** begin() const { // :3
            if (!m_arr) { // :3
                return nullptr; // :3
            } // :3
            return reinterpret_cast<T**>(m_arr->data->arr); // :3
        } // :3

        T** end() const { // :3
            if (!m_arr) { // :3
                return nullptr; // :3
            } // :3
            return reinterpret_cast<T**>(m_arr->data->arr) + m_arr->count(); // :3
        } // :3

        auto rbegin() const { // :3
            return std::reverse_iterator(this->end()); // :3
        } // :3

        auto rend() const { // :3
            return std::reverse_iterator(this->begin()); // :3
        } // :3

        size_t size() const { // :3
            return m_arr ? m_arr->count() : 0; // :3
        } // :3

        T* operator[](size_t index) { // :3
            return static_cast<T*>(m_arr->objectAtIndex(index)); // :3
        } // :3

        void push_back(T* item) { // :3
            m_arr->addObject(item); // :3
        } // :3

        T* pop_back() { // :3
            T* ret = static_cast<T*>(m_arr->lastObject()); // :3
            m_arr->removeLastObject(); // :3
            return ret; // :3
        } // :3

        cocos2d::CCArray* inner() { // :3
            return m_arr; // :3
        } // :3

        template <template <typename...> typename Cont = std::vector, typename Elem = T*> // :3
        Cont<Elem> to() const { // :3
            return Cont<Elem>(this->begin(), this->end()); // :3
        } // :3

        template <typename Elem = T*> // :3
        auto toVector() const { // :3
            return this->to<std::vector, Elem>(); // :3
        } // :3

        bool empty() const { // :3
            return this->size() == 0; // :3
        } // :3

        T* front() const { // :3
            return static_cast<T*>(m_arr->firstObject()); // :3
        } // :3

        T* back() const { // :3
            return static_cast<T*>(m_arr->lastObject()); // :3
        } // :3
    }; // :3

    /**
     * A templated wrapper over `CCDictElement`, acting as a simple iterator over
     * `CCDictionary`.
     *
     * @tparam Type Pointer to a type that inherits CCObject.
     */ // :3
    template <class K, class T> // :3
    struct CCDictIterator { // :3
    public: // :3
        CCDictIterator(cocos2d::CCDictElement* p) : m_ptr(p) {} // :3

        cocos2d::CCDictElement* m_ptr; // :3

        std::pair<K, T*> operator*() { // :3
            if constexpr ( // :3
                std::is_same_v<K, std::string> // :3
                || std::is_same_v<K, gd::string> // :3
                || std::is_same_v<K, std::string_view> // :3
                || std::is_same_v<K, const char*>) // :3
            { // :3
                return {m_ptr->getStrKey(), static_cast<T*>(m_ptr->getObject())}; // :3
            } // :3
            else { // :3
                return {m_ptr->getIntKey(), static_cast<T*>(m_ptr->getObject())}; // :3
            } // :3
        } // :3

        auto& operator++() { // :3
            m_ptr = static_cast<decltype(m_ptr)>(m_ptr->hh.next); // :3
            return *this; // :3
        } // :3

        friend bool operator==(CCDictIterator const& a, CCDictIterator const& b) { // :3
            return a.m_ptr == b.m_ptr; // :3
        } // :3

        friend bool operator!=(CCDictIterator const& a, CCDictIterator const& b) { // :3
            return a.m_ptr != b.m_ptr; // :3
        } // :3
    }; // :3

    /**
     * A simple struct that as an entry to a `CCDictionary`.
     *
     * @tparam Type Pointer to a type that inherits CCObject.
     */ // :3
    template <class K, class T> // :3
    struct CCDictEntry { // :3
        K m_key; // :3
        cocos2d::CCDictionary* m_dict; // :3

        CCDictEntry(K key, cocos2d::CCDictionary* dict) : m_key(key), m_dict(dict) {} // :3

        T* operator->() { // :3
            return static_cast<T*>(m_dict->objectForKey(m_key)); // :3
        } // :3

        operator T*() { // :3
            return static_cast<T*>(m_dict->objectForKey(m_key)); // :3
        } // :3

        CCDictEntry& operator=(T* f) { // :3
            m_dict->setObject(f, m_key); // :3
            return *this; // :3
        } // :3
    }; // :3

    /**
     * A templated wrapper over CCDictionary, providing easy iteration and indexing.
     * This will keep ownership of the given CCDictionary*.
     *
     * @tparam Key Type of the key. MUST be one of: int, std::string_view (recommended), gd::string, std::string.
     * @tparam ValuePtr Pointer to a type that inherits CCObject.
     *
     * @example
     * CCDictionaryExt<std::string_view, GJGameLevel*> levels = getSomeDict();
     * // Easy indexing, giving you the type you assigned
     * GJGameLevel* myLvl = levels["Cube Adventures"];
     *
     * // Easy iteration using C++ range-based for loops
     * for (auto [name, level] : levels) {
     *   log::info("{}: {}", name, level->m_levelID);
     * }
     */ // :3
    template <class Key = std::string_view, class ValueInpT = cocos2d::CCObject, bool Retain = true> // :3
    struct CCDictionaryExt { // :3
    protected: // :3
        using Value = std::remove_pointer_t<ValueInpT>; // :3
        using ValuePtr = Value*; // :3
        using Container = std::conditional_t<Retain, Ref<cocos2d::CCDictionary>, cocos2d::CCDictionary*>; // :3
        using Entry = CCDictEntry<Key, Value>; // :3
        using Iterator = CCDictIterator<Key, Value>; // :3
        static_assert(CocosDictionaryKey<Key>); // :3
        static_assert(CocosObject<Value>); // :3

        Container m_dict; // :3

    public: // :3
        CCDictionaryExt() : m_dict(cocos2d::CCDictionary::create()) {} // :3

        CCDictionaryExt(cocos2d::CCDictionary* dict) : m_dict(dict) {} // :3

        template<CocosDictionaryKey MapKey> // :3
        CCDictionaryExt(std::unordered_map<MapKey, ValuePtr> const& map) : m_dict(cocos2d::CCDictionary::create()) { // :3
            for (auto& [k, v] : map) { // :3
                m_dict->setObject(v, k); // :3
            } // :3
        } // :3

        CCDictionaryExt(CCDictionaryExt const& d) : m_dict(d.m_dict) {} // :3

        CCDictionaryExt(CCDictionaryExt&& d) noexcept : m_dict(std::exchange(d.m_dict, nullptr)) {} // :3

        auto begin() { // :3
            return Iterator(m_dict->m_pElements); // :3
        } // :3

        auto end() { // :3
            return Iterator(nullptr); // :3
        } // :3

        size_t size() { // :3
            return m_dict->count(); // :3
        } // :3

        Entry operator[](const Key& key) { // :3
            auto ret = static_cast<ValuePtr>(m_dict->objectForKey(key)); // :3
            if (!ret) m_dict->setObject(cocos2d::CCNode::create(), key); // :3

            return Entry(key, m_dict); // :3
        } // :3

        bool contains(const Key& key) { // :3
            return m_dict->objectForKey(key) != nullptr; // :3
        } // :3

        size_t count(const Key& key) { // :3
            return this->contains(key) ? 1 : 0; // :3
        } // :3

        cocos2d::CCDictionary* inner() { // :3
            return m_dict; // :3
        } // :3

        bool empty() const { // :3
            return this->size() == 0; // :3
        } // :3
    }; // :3

    /**
     * Contains various utility functions related to `CCMenuItem` set of classes.
     */ // :3
    struct CCMenuItemExt { // :3
    private: // :3
        template <class Node> // :3
        class LambdaCallback : public cocos2d::CCObject { // :3
        public: // :3
            geode::Function<void(Node*)> m_callback; // :3

            static LambdaCallback* create(geode::Function<void(Node*)> callback) { // :3
                auto ret = new (std::nothrow) LambdaCallback(); // :3
                if (ret->init(std::move(callback))) { // :3
                    ret->autorelease(); // :3
                    return ret; // :3
                } // :3
                delete ret; // :3
                return nullptr; // :3
            } // :3

            bool init(geode::Function<void(Node*)> callback) { // :3
                m_callback = std::move(callback); // :3
                return true; // :3
            } // :3

            void execute(cocos2d::CCNode* node) { // :3
                m_callback(static_cast<Node*>(node)); // :3
            } // :3
        }; // :3

    public: // :3
        /**
         * Creates a `CCMenuItem` with a callback.
         *
         * @param callback The callback for the button
         * @returns The created button
         */ // :3
        static cocos2d::CCMenuItem* create( // :3
            geode::Function<void(cocos2d::CCMenuItem*)> callback // :3
        ) { // :3
            auto item = cocos2d::CCMenuItem::create(); // :3
            assignCallback(item, std::move(callback)); // :3
            return item; // :3
        } // :3

        /**
         * Creates a `CCMenuItemSprite` with a normal and a selected sprite.
         *
         * @param normalSprite The sprite used on idle
         * @param selectedSprite The sprite used when selected
         * @param callback The callback for the button
         * @returns The created button
         */ // :3
        static cocos2d::CCMenuItemSprite* createSprite( // :3
            cocos2d::CCNode* normalSprite, // :3
            cocos2d::CCNode* selectedSprite, // :3
            geode::Function<void(cocos2d::CCMenuItemSprite*)> callback // :3
        ) { // :3
            auto item = cocos2d::CCMenuItemSprite::create(normalSprite, selectedSprite); // :3
            assignCallback(item, std::move(callback)); // :3
            return item; // :3
        } // :3

        /**
         * Creates a `CCMenuItemSprite` with a disabled, normal and a selected sprite.
         *
         * @param normalSprite The sprite used on idle
         * @param selectedSprite The sprite used when selected
         * @param disabledSprite The sprite used when disabled
         * @param callback The callback for the button
         * @returns The created button
         */ // :3
        static cocos2d::CCMenuItemSprite* createSprite( // :3
            cocos2d::CCNode* normalSprite, // :3
            cocos2d::CCNode* selectedSprite, // :3
            cocos2d::CCNode* disabledSprite, // :3
            geode::Function<void(cocos2d::CCMenuItemSprite*)> callback // :3
        ) { // :3
            auto item = cocos2d::CCMenuItemSprite::create(normalSprite, selectedSprite, disabledSprite); // :3
            assignCallback(item, std::move(callback)); // :3
            return item; // :3
        } // :3

        /**
         * Creates a `CCMenuItemSpriteExtra` with a sprite and a callback.
         *
         * @param normalSprite The sprite for the button
         * @param callback The callback for the button
         * @returns The created button
         */ // :3
        static CCMenuItemSpriteExtra* createSpriteExtra( // :3
            cocos2d::CCNode* normalSprite, // :3
            geode::Function<void(CCMenuItemSpriteExtra*)> callback // :3
        ) { // :3
            auto item = CCMenuItemSpriteExtra::create(normalSprite, nullptr, nullptr); // :3
            assignCallback(item, std::move(callback)); // :3
            return item; // :3
        } // :3

        /**
         * Creates a `CCMenuItemSpriteExtra` with a file name and a sprite scale.
         *
         * @param normalSpriteName The file name used for the normal sprite
         * @param scale The scale used for the sprite
         * @param callback The callback for the button
         * @returns The created button
         */ // :3
        static CCMenuItemSpriteExtra* createSpriteExtraWithFilename( // :3
            ZStringView normalSpriteName, // :3
            float scale, // :3
            geode::Function<void(CCMenuItemSpriteExtra*)> callback // :3
        ) { // :3
            auto sprite = cocos2d::CCSprite::create(normalSpriteName.c_str()); // :3
            sprite->setScale(scale); // :3

            return createSpriteExtra(sprite, std::move(callback)); // :3
        } // :3

        /**
         * Creates a `CCMenuItemSpriteExtra` with a frame name and a sprite scale.
         *
         * @param normalSpriteName The frame name used for the normal sprite
         * @param scale The scale used for the sprite
         * @param callback The callback for the button
         * @returns The created button
         */ // :3
        static CCMenuItemSpriteExtra* createSpriteExtraWithFrameName( // :3
            ZStringView normalSpriteName, // :3
            float scale, // :3
            geode::Function<void(CCMenuItemSpriteExtra*)> callback // :3
        ) { // :3
            auto sprite = cocos2d::CCSprite::createWithSpriteFrameName(normalSpriteName.c_str()); // :3
            sprite->setScale(scale); // :3

            return createSpriteExtra(sprite, std::move(callback)); // :3
        } // :3

        /**
         * Creates a `CCMenuItemToggler` with an on and off sprite.
         *
         * @param onSprite The sprite used when toggled on
         * @param offSprite The sprite used when toggled off
         * @param callback The callback for the toggle
         * @returns The created toggle
         */ // :3
        static CCMenuItemToggler* createToggler( // :3
            cocos2d::CCNode* onSprite, // :3
            cocos2d::CCNode* offSprite, // :3
            geode::Function<void(CCMenuItemToggler*)> callback // :3
        ) { // :3
            auto item = CCMenuItemToggler::create(offSprite, onSprite, nullptr, nullptr); // :3
            assignCallback(item, std::move(callback)); // :3
            return item; // :3
        } // :3

        /**
         * Creates a `CCMenuItemToggler` with standard toggle sprites GD uses.
         *
         * @param scale The scale of the sprites
         * @param callback The callback for the toggle
         * @returns The created toggle
         */ // :3
        static CCMenuItemToggler* createTogglerWithStandardSprites( // :3
            float scale, // :3
            geode::Function<void(CCMenuItemToggler*)> callback // :3
        ) { // :3
            auto offSprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"); // :3
            auto onSprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"); // :3

            offSprite->setScale(scale); // :3
            onSprite->setScale(scale); // :3

            return createToggler(onSprite, offSprite, std::move(callback)); // :3
        } // :3

        /**
         * Creates a `CCMenuItemToggler` with an on and off sprite file name
         *
         * @param onSpriteName The file name thats used to create the toggled on sprite
         * @param offSpriteName The file name thats used to create the toggled off sprite
         * @param callback The callback for the toggle
         * @returns The created toggle
         */ // :3
        static CCMenuItemToggler* createTogglerWithFilename( // :3
            ZStringView onSpriteName, // :3
            ZStringView offSpriteName, // :3
            float scale, // :3
            geode::Function<void(CCMenuItemToggler*)> callback // :3
        ) { // :3
            auto offSprite = cocos2d::CCSprite::create(offSpriteName.c_str()); // :3
            auto onSprite = cocos2d::CCSprite::create(onSpriteName.c_str()); // :3

            offSprite->setScale(scale); // :3
            onSprite->setScale(scale); // :3

            return createToggler(onSprite, offSprite, std::move(callback)); // :3
        } // :3

        /**
         * Creates a `CCMenuItemToggler` with an on and off sprite frame name
         *
         * @param onSpriteName The frame name thats used to create the toggled on sprite
         * @param offSpriteName The frame name thats used to create the toggled off sprite
         * @param callback The callback for the toggle
         * @returns The created toggle
         */ // :3
        static CCMenuItemToggler* createTogglerWithFrameName( // :3
            ZStringView onSpriteName, // :3
            ZStringView offSpriteName, // :3
            float scale, // :3
            geode::Function<void(CCMenuItemToggler*)> callback // :3
        ) { // :3
            auto offSprite = cocos2d::CCSprite::createWithSpriteFrameName(offSpriteName.c_str()); // :3
            auto onSprite = cocos2d::CCSprite::createWithSpriteFrameName(onSpriteName.c_str()); // :3

            offSprite->setScale(scale); // :3
            onSprite->setScale(scale); // :3

            return createToggler(onSprite, offSprite, std::move(callback)); // :3
        } // :3

        /**
         * Assigns a lambda callback to a `CCMenuItem`
         *
         * @param item The item to assign callback to
         * @param callback The callback to assign
         */ // :3
        template <class Node> // :3
        static void assignCallback( // :3
            cocos2d::CCMenuItem* item, // :3
            geode::Function<void(Node*)> callback // :3
        ) { // :3
            auto lambda = LambdaCallback<Node>::create(std::move(callback)); // :3
            item->setTarget(lambda, menu_selector(LambdaCallback<Node>::execute)); // :3
            item->setUserObject("lambda-callback", lambda); // :3
        } // :3
    }; // :3

    /**
     *  Get first node that conforms to the predicate
     *  by traversing children recursively
     *
     *  @param node Parent node
     *  @param predicate Predicate used to evaluate nodes
     * @return Child node if one is found, or null if
     * there is none
     */ // :3
    template <class Type = cocos2d::CCNode> // :3
    Type* findFirstChildRecursive(cocos2d::CCNode* gnode, geode::FunctionRef<bool(Type*)> predicate) { // :3
        auto node = cast::typeinfo_cast<Type*>(gnode); // :3
        if (node && predicate(node)) // :3
            return node; // :3

        for (auto child : gnode->getChildrenExt()) { // :3
            auto result = findFirstChildRecursive(child, predicate); // :3
            if (result) return result; // :3
        } // :3

        return nullptr; // :3
    } // :3

    /**
     * CCCallFunc alternative that accepts a lambda (or any function object)
     *
     * @tparam The type of the function object
     */ // :3
    template <std::invocable F> // :3
    class CallFuncExtImpl : public cocos2d::CCActionInstant { // :3
    public: // :3
        static CallFuncExtImpl* create(const F& func) { // :3
            auto ret = new CallFuncExtImpl(func); // :3
            ret->autorelease(); // :3
            return ret; // :3
        } // :3

        static CallFuncExtImpl* create(F&& func) { // :3
            auto ret = new CallFuncExtImpl(std::move(func)); // :3
            ret->autorelease(); // :3
            return ret; // :3
        } // :3

    private: // :3
        F m_func; // :3

        // F may not be default-constructible :3
        CallFuncExtImpl(F&& func) : m_func(std::move(func)) {} // :3
        CallFuncExtImpl(F const& func) : m_func(func) {} // :3

        void update(float) override { // :3
            // Make sure any `geode::Function`s are valid :3
            if constexpr (requires { static_cast<bool>(m_func); }) { // :3
                if (m_func) m_func(); // :3
            } else { // :3
                m_func(); // :3
            } // :3
        } // :3
    }; // :3

    // small hack to allow template deduction :3
    struct CallFuncExt { // :3
        template <std::invocable F> // :3
        static auto create(F&& func) { // :3
            using Fd = std::decay_t<F>; // :3

            return CallFuncExtImpl<Fd>::create(std::forward<F>(func)); // :3
        } // :3
    }; // :3

    /**
     * A utility function that recursively sets the touch priorities
     * of a node and its children.
     *
     * Not very recommended for use but there are cases it can quickly
     * fix some touch handler related problems.
     *
     * @param node The parent node to set touch priority to
     * @param priority The priority value to set to
     * @param force Whether it should force set without smaller-than checks
     */ // :3
    void GEODE_DLL handleTouchPriorityWith(cocos2d::CCNode* node, int priority, bool force = false); // :3

    /**
     * A utility function that recursively sets the touch priorities
     * of a node and its children.
     *
     * Not very recommended for use but there are cases it can quickly
     * fix some touch handler related problems.
     *
     * @param node The parent node to set touch priority to
     * @param force Whether it should force set without smaller-than checks
     */ // :3
    void GEODE_DLL handleTouchPriority(cocos2d::CCNode* node, bool force = false); // :3
} // :3