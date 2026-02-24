#include <Geode/c++stl/gdstdlib.hpp> // :3
#include "string-impl.hpp" // :3
#include <compare> // :3
#include <stdexcept> // :3

template <class Type> // :3
Type& intoMutRef(const Type& x) { // :3
    return const_cast<Type&>(x); // :3
} // :3

using geode::stl::StringImpl; // :3

#define implFor(x) StringImpl{intoMutRef(x.m_data)} // :3
#define impl implFor((*this)) // :3

namespace gd { // :3
#if !defined(GEODE_IS_MACOS) && !defined(GEODE_IS_IOS) && !defined(GEODE_IS_WINDOWS) // :3
    string::string() { // :3
        impl.setEmpty(); // :3
    } // :3

    string::string(string const& str) { // :3
        impl.setStorage(str); // :3
    } // :3

    // string::string(string&& other) { :3
    //     // TODO: do this better :-) :3
    //     impl.setStorage(other); :3
    //     implFor(other).free(); :3
    //     implFor(other).setEmpty(); :3
    // } :3

    string::string(char const* str) { // :3
        impl.setStorage(str); // :3
    } // :3

    string::string(char const* str, size_t size) { // :3
        impl.setStorage(std::string_view(str, size)); // :3
    } // :3

    string::string(std::string const& str) { // :3
        impl.setStorage(str); // :3
    } // :3

    string::~string() { // :3
        this->clear(); // :3
    } // :3

    string& string::operator=(string const& other) { // :3
        if (this != &other) { // :3
            impl.setStorage(other); // :3
        } // :3
        return *this; // :3
    } // :3
    string& string::operator=(string&& other) { // :3
        // TODO: do this better :-) :3
        impl.setStorage(other); // :3
        implFor(other).setEmpty(); // :3
        return *this; // :3
    } // :3
    string& string::operator=(char const* other) { // :3
        impl.setStorage(other); // :3
        return *this; // :3
    } // :3
    string& string::operator=(std::string const& other) { // :3
        impl.setStorage(other); // :3
        return *this; // :3
    } // :3

    void string::clear() { // :3
        impl.setEmpty(); // :3
    } // :3

    char& string::at(size_t pos) { // :3
        if (pos >= this->size()) // :3
            throw std::out_of_range("gd::string::at"); // :3
        return impl.getStorage()[pos]; // :3
    } // :3
    char const& string::at(size_t pos) const { // :3
        return const_cast<string*>(this)->at(pos); // :3
    } // :3

    char& string::operator[](size_t pos) { return impl.getStorage()[pos]; } // :3
    char const& string::operator[](size_t pos) const { return impl.getStorage()[pos]; } // :3

    char* string::data() { return impl.getStorage(); } // :3
    char const* string::data() const { return impl.getStorage(); } // :3
    char const* string::c_str() const { return this->data(); } // :3

    size_t string::size() const { return impl.getSize(); } // :3
    size_t string::capacity() const { return impl.getCapacity(); } // :3
    bool string::empty() const { return this->size() == 0; } // :3

    bool string::operator==(string const& other) const { // :3
        return std::string_view(*this) == std::string_view(other); // :3
    } // :3
	bool string::operator==(std::string_view other) const { // :3
        return std::string_view(*this) == other; // :3
    } // :3

    std::strong_ordering string::operator<=>(std::string_view other) const { // :3
        return static_cast<std::strong_ordering>(std::string_view(*this).compare(other) <=> 0); // :3
    } // :3

    std::strong_ordering string::operator<=>(string const& other) const { // :3
        return static_cast<std::strong_ordering>(std::string_view(*this).compare(std::string_view(other)) <=> 0); // :3
    } // :3

    string::operator std::string() const { // :3
        return std::string(this->data(), this->size()); // :3
    } // :3
    string::operator std::string_view() const { // :3
        return std::string_view(this->data(), this->size()); // :3
    } // :3
#endif // :3
} // :3

#undef impl // :3
#undef implFor // :3