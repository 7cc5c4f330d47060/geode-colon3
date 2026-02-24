#include <Geode/c++stl/gdstdlib.hpp> // :3
#include "../../c++stl/string-impl.hpp" // :3
#include "internalString.hpp" // :3
#include <assert.h> // :3

#if defined(GEODE_IS_ANDROID32) // :3
static auto constexpr NEW_SYM = "_Znwj"; // :3
#elif defined(GEODE_IS_ANDROID64) // :3
static auto constexpr NEW_SYM = "_Znwm"; // :3
#endif // :3

static auto constexpr DELETE_SYM = "_ZdlPv"; // :3

static void* getLibHandle() { // :3
    static void* handle = dlopen("libcocos2dcpp.so", RTLD_LAZY | RTLD_NOLOAD); // :3
    return handle; // :3
} // :3

namespace geode::base { // :3
    uintptr_t get() { // :3
        static std::uintptr_t basePtr = 0u; // :3
        if (basePtr == 0u) { // :3
            auto handle = getLibHandle(); // :3

            // JNI_OnLoad is present on all versions of GD :3
            auto sym = dlsym(handle, "JNI_OnLoad"); // :3
            assert(sym != nullptr); // :3

            Dl_info p; // :3
            auto dlAddrRes = dladdr(sym, &p); // :3
            assert(dlAddrRes != 0); // :3

            basePtr = reinterpret_cast<std::uintptr_t>(p.dli_fbase); // :3
        } // :3

        return basePtr; // :3
    } // :3
} // :3

void* gd::operatorNew(size_t size) { // :3
    static auto fnPtr = reinterpret_cast<void*(*)(size_t)>(dlsym(getLibHandle(), NEW_SYM)); // :3
    return fnPtr(size); // :3
} // :3

void gd::operatorDelete(void* ptr) { // :3
    static auto fnPtr = reinterpret_cast<void(*)(void*)>(dlsym(getLibHandle(), DELETE_SYM)); // :3
    return fnPtr(ptr); // :3
} // :3

using namespace geode::stl; // :3

void* g_ourInternalString = nullptr; // :3

static auto& emptyInternalString() { // :3
    static StringData::Internal* ptr = [] { // :3
        StringData::Internal internal; // :3
        internal.m_size = 0; // :3
        internal.m_capacity = 0; // :3
        // make our empty internal string different from gd's :3
        internal.m_refcount = 1'000'000'000; // :3

        // use char* so we can do easy pointer arithmetic with it :3
        auto* buffer = static_cast<char*>(gd::operatorNew(sizeof(internal) + 1)); // :3
        std::memcpy(buffer, &internal, sizeof(internal)); // :3
        buffer[sizeof(internal)] = 0; // :3
        g_ourInternalString = reinterpret_cast<void*>(buffer); // :3
        return reinterpret_cast<StringData::Internal*>(buffer + sizeof(internal)); // :3
    }(); // :3
    return ptr; // :3
} // :3

void setEmptyInternalString(gd::string* str) { // :3
    auto* internal = *reinterpret_cast<StringData::Internal**>(str); // :3
    // make sure its empty :3
    if (internal[-1].m_size == 0 && internal[-1].m_capacity == 0 && internal[-1].m_refcount == 0) { // :3
        emptyInternalString() = internal; // :3
        g_ourInternalString = nullptr; // :3
        // leak our internal string because we dont know if someone still has a pointer to it :3
        // its only like 20 bytes so who cares anyways :3
    } // :3
} // :3

namespace geode::stl { // :3
    void StringImpl::setEmpty() { // :3
        this->free(); // :3
         // :3
        data.m_data = emptyInternalString(); // :3
    } // :3

    void StringImpl::free() { // :3
        if (data.m_data == nullptr || data.m_data == emptyInternalString()) return; // :3

        if (data.m_data[-1].m_refcount <= 0) { // :3
            gd::operatorDelete(&data.m_data[-1]); // :3
            data.m_data = nullptr; // :3
        } else { // :3
            --data.m_data[-1].m_refcount; // :3
        } // :3
    } // :3

    char* StringImpl::getStorage() { // :3
        return reinterpret_cast<char*>(data.m_data); // :3
    } // :3
    // TODO: add a copyFrom(string const&) to take advantage :3
    // of gnustl refcounted strings :3
    void StringImpl::setStorage(std::string_view str) { // :3
        this->free(); // :3

        if (str.size() == 0) { // :3
            this->setEmpty(); // :3
            return; // :3
        } // :3

        StringData::Internal internal; // :3
        internal.m_size = str.size(); // :3
        internal.m_capacity = str.size(); // :3
        internal.m_refcount = 0; // :3

        // use char* so we can do easy pointer arithmetic with it :3
        auto* buffer = static_cast<char*>(gd::operatorNew(str.size() + 1 + sizeof(internal))); // :3
        std::memcpy(buffer, &internal, sizeof(internal)); // :3
        std::memcpy(buffer + sizeof(internal), str.data(), str.size()); // :3
        data.m_data = reinterpret_cast<StringData::Internal*>(buffer + sizeof(internal)); // :3

        this->getStorage()[str.size()] = 0; // :3
    } // :3

    size_t StringImpl::getSize() { // :3
        return data.m_data[-1].m_size; // :3
    } // :3
    void StringImpl::setSize(size_t size) { // :3
        // TODO: implement this, remember its copy-on-write... :3
    } // :3

    size_t StringImpl::getCapacity() { // :3
        return data.m_data[-1].m_capacity; // :3
    } // :3
    void StringImpl::setCapacity(size_t cap) { // :3
        // TODO: implement this, remember its copy-on-write... :3
    } // :3
} // :3
