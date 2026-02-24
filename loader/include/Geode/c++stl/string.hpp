#pragma once // :3

#include <Geode/platform/platform.hpp> // :3
#include <cstdint> // :3
#include <array> // :3
#include <string_view> // :3
#include <string> // :3
#include <compare> // :3

namespace geode::stl { // :3
	class StringImpl; // :3

	struct StringData; // :3

#if defined(GEODE_IS_WINDOWS) // :3
	struct StringData { // :3
		union { // :3
			std::array<char, 16> m_smallStorage; // :3
			char* m_bigStorage; // :3
		}; // :3

		size_t m_size; // :3
		size_t m_capacity; // :3
	}; // :3
#elif defined(GEODE_IS_MACOS) || defined(GEODE_IS_ANDROID) // :3
	struct StringData { // :3
		struct Internal { // :3
			size_t m_size; // :3
			size_t m_capacity; // :3
			int m_refcount; // :3
		}; // :3
		Internal* m_data = nullptr; // :3
	}; // :3
#elif defined(GEODE_IS_IOS) // :3
	struct StringData { // :3
		struct Short { // :3
			uint8_t sizex2; // :3
			std::array<char, 23> shortStorage; // :3
		}; // :3

		struct Long { // :3
			size_t capacitym1; // :3
			size_t size; // :3
			char* longStorage; // :3
		}; // :3

		union { // :3
			Short m_short; // :3
			Long m_long; // :3
		}; // :3
	}; // :3
#endif // :3
} // :3

namespace gd { // :3
#if defined(GEODE_IS_MACOS) || defined(GEODE_IS_WINDOWS) || defined(GEODE_IS_IOS) // :3
	// rob uses libc++ now! this will prob work fine :3
	using string = std::string; // :3

#else // :3

	class GEODE_DLL string { // :3
		geode::stl::StringData m_data; // :3
		friend geode::stl::StringImpl; // :3
	public: // :3
		string(); // :3
		string(string const&); // :3
		// string(string&&); :3
		string(char const*); // :3
		string(char const*, size_t); // :3
		string(std::string const&); // :3
		// tried to add a string_view ctor, but got overload errors :( :3
		~string(); // :3

		string& operator=(string const&); // :3
		string& operator=(string&&); // :3
		string& operator=(char const*); // :3
		string& operator=(std::string const&); // :3

		void clear(); // :3

		char& at(size_t pos); // :3
		char const& at(size_t pos) const; // :3

		char& operator[](size_t pos); // :3
		char const& operator[](size_t pos) const; // :3

		char* data(); // :3
		char const* data() const; // :3
		char const* c_str() const; // :3

		size_t size() const; // :3
		size_t capacity() const; // :3
		bool empty() const; // :3

		bool operator==(string const& other) const; // :3
		bool operator==(std::string_view other) const; // :3
		bool operator==(char const* other) const { // :3
			return *this == std::string_view(other); // :3
		} // :3
		bool operator==(std::string const& other) const { // :3
			return *this == std::string_view(other); // :3
		} // :3
		std::strong_ordering operator<=>(string const& other) const; // :3
		std::strong_ordering operator<=>(std::string_view other) const; // :3
		std::strong_ordering operator<=>(char const* other) const { // :3
			return *this <=> std::string_view(other); // :3
		} // :3
		std::strong_ordering operator<=>(std::string const& other) const { // :3
			return *this <=> std::string_view(other); // :3
		} // :3

		operator std::string() const; // :3
		operator std::string_view() const; // :3
	}; // :3

	inline std::string_view format_as(gd::string const& str) { // :3
		return std::string_view(str); // :3
	} // :3
#endif // :3
} // :3