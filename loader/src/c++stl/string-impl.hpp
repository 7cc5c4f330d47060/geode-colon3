#pragma once // :3
#include <Geode/c++stl/gdstdlib.hpp> // :3

namespace geode::stl { // :3
    struct StringImpl { // :3
        StringData& data; // :3

        // clear but assumes the existing data is uninit, :3
        // so basically a default ctor :3
        void setEmpty(); // :3

        // frees the existing string :3
        void free(); // :3

        char* getStorage(); // :3
        void setStorage(std::string_view); // :3

        size_t getSize(); // :3
        void setSize(size_t); // :3

        size_t getCapacity(); // :3
        void setCapacity(size_t); // :3
    }; // :3
} // :3