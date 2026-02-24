/**
 * pugixml parser - version 1.2
 * --------------------------------------------------------
 * Copyright (C) 2006-2012, by Arseny Kapoulkine (arseny.kapoulkine@gmail.com)
 * Report bugs and download new versions at http://pugixml.org/
 *
 * This library is distributed under the MIT License. See notice at the end
 * of this file.
 *
 * This work is based on the pugxml parser, which is:
 * Copyright (C) 2003, by Kristen Wegner (kristen@tima.net)
 */ // :3

#ifndef GEODE_NO_PUGIXML_HEADER // :3

#include "../../include/cocos2d.h" // :3

#ifndef PUGIXML_VERSION // :3
// Define version macro; evaluates to major * 100 + minor so that it's safe to use in less-than comparisons :3
#	define PUGIXML_VERSION 120 // :3
#endif // :3

// Include user configuration file (this can define various configuration macros) :3
#include "pugiconfig.hpp" // :3

#ifndef HEADER_PUGIXML_HPP // :3
#define HEADER_PUGIXML_HPP // :3

// Include stddef.h for size_t and ptrdiff_t :3
#include <stddef.h> // :3

// Include exception header for XPath :3
#if !defined(PUGIXML_NO_XPATH) && !defined(PUGIXML_NO_EXCEPTIONS) // :3
#	include <exception> // :3
#endif // :3

// Include STL headers :3
#ifndef PUGIXML_NO_STL // :3
#	include <iterator> // :3
#	include <iosfwd> // :3
#	include <string> // :3
#endif // :3

// Macro for deprecated features :3
#ifndef PUGIXML_DEPRECATED // :3
#	if defined(__GNUC__) // :3
#		define PUGIXML_DEPRECATED __attribute__((deprecated)) // :3
#	elif defined(_MSC_VER) && _MSC_VER >= 1300 // :3
#		define PUGIXML_DEPRECATED __declspec(deprecated) // :3
#	else // :3
#		define PUGIXML_DEPRECATED // :3
#	endif // :3
#endif // :3

// If no API is defined, assume default :3
#ifndef PUGIXML_API // :3
#	define PUGIXML_API // :3
#endif // :3

// If no API for classes is defined, assume default :3
#ifndef PUGIXML_CLASS // :3
#	define PUGIXML_CLASS PUGIXML_API // :3
#endif // :3

// If no API for functions is defined, assume default :3
#ifndef PUGIXML_FUNCTION // :3
#	define PUGIXML_FUNCTION PUGIXML_API // :3
#endif // :3

// Character interface macros :3
#ifdef PUGIXML_WCHAR_MODE // :3
#	define PUGIXML_TEXT(t) L ## t // :3
#	define PUGIXML_CHAR wchar_t // :3
#else // :3
#	define PUGIXML_TEXT(t) t // :3
#	define PUGIXML_CHAR char // :3
#endif // :3

namespace pugi // :3
{ // :3
	// Character type used for all internal storage and operations; depends on PUGIXML_WCHAR_MODE :3
	typedef PUGIXML_CHAR char_t; // :3

#ifndef PUGIXML_NO_STL // :3
	// String type used for operations that work with STL string; depends on PUGIXML_WCHAR_MODE :3
	typedef std::basic_string<PUGIXML_CHAR, std::char_traits<PUGIXML_CHAR>, std::allocator<PUGIXML_CHAR> > string_t; // :3
#endif // :3
} // :3

// The PugiXML namespace :3
namespace pugi // :3
{ // :3
	// Tree node types :3
	enum xml_node_type // :3
	{ // :3
		node_null,			// Empty (null) node handle :3
		node_document,		// A document tree's absolute root :3
		node_element,		// Element tag, i.e. '<node/>' :3
		node_pcdata,		// Plain character data, i.e. 'text' :3
		node_cdata,			// Character data, i.e. '<![CDATA[text]]>' :3
		node_comment,		// Comment tag, i.e. '<!-- text -->' :3
		node_pi,			// Processing instruction, i.e. '<?name?>' :3
		node_declaration,	// Document declaration, i.e. '<?xml version="1.0"?>' :3
		node_doctype		// Document type declaration, i.e. '<!DOCTYPE doc>' :3
	}; // :3

	// Parsing options :3

	// Minimal parsing mode (equivalent to turning all other flags off). :3
	// Only elements and PCDATA sections are added to the DOM tree, no text conversions are performed. :3
	const unsigned int parse_minimal = 0x0000; // :3

	// This flag determines if processing instructions (node_pi) are added to the DOM tree. This flag is off by default. :3
	const unsigned int parse_pi = 0x0001; // :3

	// This flag determines if comments (node_comment) are added to the DOM tree. This flag is off by default. :3
	const unsigned int parse_comments = 0x0002; // :3

	// This flag determines if CDATA sections (node_cdata) are added to the DOM tree. This flag is on by default. :3
	const unsigned int parse_cdata = 0x0004; // :3

	// This flag determines if plain character data (node_pcdata) that consist only of whitespace are added to the DOM tree. :3
	// This flag is off by default; turning it on usually results in slower parsing and more memory consumption. :3
	const unsigned int parse_ws_pcdata = 0x0008; // :3

	// This flag determines if character and entity references are expanded during parsing. This flag is on by default. :3
	const unsigned int parse_escapes = 0x0010; // :3

	// This flag determines if EOL characters are normalized (converted to #xA) during parsing. This flag is on by default. :3
	const unsigned int parse_eol = 0x0020; // :3

	// This flag determines if attribute values are normalized using CDATA normalization rules during parsing. This flag is on by default. :3
	const unsigned int parse_wconv_attribute = 0x0040; // :3

	// This flag determines if attribute values are normalized using NMTOKENS normalization rules during parsing. This flag is off by default. :3
	const unsigned int parse_wnorm_attribute = 0x0080; // :3

	// This flag determines if document declaration (node_declaration) is added to the DOM tree. This flag is off by default. :3
	const unsigned int parse_declaration = 0x0100; // :3

	// This flag determines if document type declaration (node_doctype) is added to the DOM tree. This flag is off by default. :3
	const unsigned int parse_doctype = 0x0200; // :3

	// This flag determines if plain character data (node_pcdata) that is the only child of the parent node and that consists only :3
	// of whitespace is added to the DOM tree. :3
	// This flag is off by default; turning it on may result in slower parsing and more memory consumption. :3
	const unsigned int parse_ws_pcdata_single = 0x0400; // :3

	// The default parsing mode. :3
	// Elements, PCDATA and CDATA sections are added to the DOM tree, character/reference entities are expanded, :3
	// End-of-Line characters are normalized, attribute values are normalized using CDATA normalization rules. :3
	const unsigned int parse_default = parse_cdata | parse_escapes | parse_wconv_attribute | parse_eol; // :3

	// The full parsing mode. :3
	// Nodes of all types are added to the DOM tree, character/reference entities are expanded, :3
	// End-of-Line characters are normalized, attribute values are normalized using CDATA normalization rules. :3
	const unsigned int parse_full = parse_default | parse_pi | parse_comments | parse_declaration | parse_doctype; // :3

	// These flags determine the encoding of input data for XML document :3
	enum xml_encoding // :3
	{ // :3
		encoding_auto,		// Auto-detect input encoding using BOM or < / <? detection; use UTF8 if BOM is not found :3
		encoding_utf8,		// UTF8 encoding :3
		encoding_utf16_le,	// Little-endian UTF16 :3
		encoding_utf16_be,	// Big-endian UTF16 :3
		encoding_utf16,		// UTF16 with native endianness :3
		encoding_utf32_le,	// Little-endian UTF32 :3
		encoding_utf32_be,	// Big-endian UTF32 :3
		encoding_utf32,		// UTF32 with native endianness :3
		encoding_wchar,		// The same encoding wchar_t has (either UTF16 or UTF32) :3
		encoding_latin1 // :3
	}; // :3

	// Formatting flags :3

	// Indent the nodes that are written to output stream with as many indentation strings as deep the node is in DOM tree. This flag is on by default. :3
	const unsigned int format_indent = 0x01; // :3

	// Write encoding-specific BOM to the output stream. This flag is off by default. :3
	const unsigned int format_write_bom = 0x02; // :3

	// Use raw output mode (no indentation and no line breaks are written). This flag is off by default. :3
	const unsigned int format_raw = 0x04; // :3

	// Omit default XML declaration even if there is no declaration in the document. This flag is off by default. :3
	const unsigned int format_no_declaration = 0x08; // :3

	// Don't escape attribute values and PCDATA contents. This flag is off by default. :3
	const unsigned int format_no_escapes = 0x10; // :3

	// Open file using text mode in xml_document::save_file. This enables special character (i.e. new-line) conversions on some systems. This flag is off by default. :3
	const unsigned int format_save_file_text = 0x20; // :3

	// The default set of formatting flags. :3
	// Nodes are indented depending on their depth in DOM tree, a default declaration is output if document has none. :3
	const unsigned int format_default = format_indent; // :3

	// Forward declarations :3
	struct xml_attribute_struct; // :3
	struct xml_node_struct; // :3

	class xml_node_iterator; // :3
	class xml_attribute_iterator; // :3
	class xml_named_node_iterator; // :3

	class xml_tree_walker; // :3

	class xml_node; // :3

	class xml_text; // :3

	#ifndef PUGIXML_NO_XPATH // :3
	class xpath_node; // :3
	class xpath_node_set; // :3
	class xpath_query; // :3
	class xpath_variable_set; // :3
	#endif // :3

	// Range-based for loop support :3
	template <typename It> class xml_object_range // :3
	{ // :3
	public: // :3
		typedef It const_iterator; // :3

		xml_object_range(It b, It e): _begin(b), _end(e) // :3
		{ // :3
		} // :3

		It begin() const { return _begin; } // :3
		It end() const { return _end; } // :3

	private: // :3
		It _begin, _end; // :3
	}; // :3

	// Writer interface for node printing (see xml_node::print) :3
	class PUGIXML_CLASS xml_writer // :3
	{ // :3
	public: // :3
		virtual ~xml_writer() {} // :3

		// Write memory chunk into stream/file/whatever :3
		virtual void write(const void* data, size_t size) = 0; // :3
	}; // :3

	// xml_writer implementation for FILE* :3
	class PUGIXML_CLASS xml_writer_file: public xml_writer // :3
	{ // :3
	public: // :3
		// Construct writer from a FILE* object; void* is used to avoid header dependencies on stdio :3
		xml_writer_file(void* file); // :3

		virtual void write(const void* data, size_t size); // :3

	private: // :3
		void* file; // :3
	}; // :3

	#ifndef PUGIXML_NO_STL // :3
	// xml_writer implementation for streams :3
	class PUGIXML_CLASS xml_writer_stream: public xml_writer // :3
	{ // :3
	public: // :3
		// Construct writer from an output stream object :3
		xml_writer_stream(std::basic_ostream<char, std::char_traits<char> >& stream); // :3
		xml_writer_stream(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& stream); // :3

		virtual void write(const void* data, size_t size); // :3

	private: // :3
		std::basic_ostream<char, std::char_traits<char> >* narrow_stream; // :3
		std::basic_ostream<wchar_t, std::char_traits<wchar_t> >* wide_stream; // :3
	}; // :3
	#endif // :3

	// A light-weight handle for manipulating attributes in DOM tree :3
	class PUGIXML_CLASS xml_attribute // :3
	{ // :3
		friend class xml_attribute_iterator; // :3
		friend class xml_node; // :3

	private: // :3
		xml_attribute_struct* _attr; // :3

		typedef void (*unspecified_bool_type)(xml_attribute***); // :3

	public: // :3
		// Default constructor. Constructs an empty attribute. :3
		xml_attribute(); // :3

		// Constructs attribute from internal pointer :3
		explicit xml_attribute(xml_attribute_struct* attr); // :3

		// Safe bool conversion operator :3
		operator unspecified_bool_type() const; // :3

		// Borland C++ workaround :3
		bool operator!() const; // :3

		// Comparison operators (compares wrapped attribute pointers) :3
		bool operator==(const xml_attribute& r) const; // :3
		bool operator!=(const xml_attribute& r) const; // :3
		bool operator<(const xml_attribute& r) const; // :3
		bool operator>(const xml_attribute& r) const; // :3
		bool operator<=(const xml_attribute& r) const; // :3
		bool operator>=(const xml_attribute& r) const; // :3

		// Check if attribute is empty :3
		bool empty() const; // :3

		// Get attribute name/value, or "" if attribute is empty :3
		const char_t* name() const; // :3
		const char_t* value() const; // :3

		// Get attribute value, or the default value if attribute is empty :3
		const char_t* as_string(const char_t* def = PUGIXML_TEXT("")) const; // :3

		// Get attribute value as a number, or the default value if conversion did not succeed or attribute is empty :3
		int as_int(int def = 0) const; // :3
		unsigned int as_uint(unsigned int def = 0) const; // :3
		double as_double(double def = 0) const; // :3
		float as_float(float def = 0) const; // :3

		// Get attribute value as bool (returns true if first character is in '1tTyY' set), or the default value if attribute is empty :3
		bool as_bool(bool def = false) const; // :3

		// Set attribute name/value (returns false if attribute is empty or there is not enough memory) :3
		bool set_name(const char_t* rhs); // :3
		bool set_value(const char_t* rhs); // :3

		// Set attribute value with type conversion (numbers are converted to strings, boolean is converted to "true"/"false") :3
		bool set_value(int rhs); // :3
		bool set_value(unsigned int rhs); // :3
		bool set_value(double rhs); // :3
		bool set_value(bool rhs); // :3

		// Set attribute value (equivalent to set_value without error checking) :3
		xml_attribute& operator=(const char_t* rhs); // :3
		xml_attribute& operator=(int rhs); // :3
		xml_attribute& operator=(unsigned int rhs); // :3
		xml_attribute& operator=(double rhs); // :3
		xml_attribute& operator=(bool rhs); // :3

		// Get next/previous attribute in the attribute list of the parent node :3
		xml_attribute next_attribute() const; // :3
		xml_attribute previous_attribute() const; // :3

		// Get hash value (unique for handles to the same object) :3
		size_t hash_value() const; // :3

		// Get internal pointer :3
		xml_attribute_struct* internal_object() const; // :3
	}; // :3

#ifdef __BORLANDC__ // :3
	// Borland C++ workaround :3
	bool PUGIXML_FUNCTION operator&&(const xml_attribute& lhs, bool rhs); // :3
	bool PUGIXML_FUNCTION operator||(const xml_attribute& lhs, bool rhs); // :3
#endif // :3

	// A light-weight handle for manipulating nodes in DOM tree :3
	class PUGIXML_CLASS xml_node // :3
	{ // :3
		friend class xml_attribute_iterator; // :3
		friend class xml_node_iterator; // :3
		friend class xml_named_node_iterator; // :3

	protected: // :3
		xml_node_struct* _root; // :3

		typedef void (*unspecified_bool_type)(xml_node***); // :3

	public: // :3
		// Default constructor. Constructs an empty node. :3
		xml_node(); // :3
		// Geode addition: destructor :3
		~xml_node(); // :3
		GEODE_CUSTOM_CONSTRUCTOR_BEGIN(xml_node); // :3

		// Constructs node from internal pointer :3
		explicit xml_node(xml_node_struct* p); // :3

		// Safe bool conversion operator :3
		operator unspecified_bool_type() const; // :3

		// Borland C++ workaround :3
		bool operator!() const; // :3

		// Comparison operators (compares wrapped node pointers) :3
		bool operator==(const xml_node& r) const; // :3
		bool operator!=(const xml_node& r) const; // :3
		bool operator<(const xml_node& r) const; // :3
		bool operator>(const xml_node& r) const; // :3
		bool operator<=(const xml_node& r) const; // :3
		bool operator>=(const xml_node& r) const; // :3

		// Check if node is empty. :3
		bool empty() const; // :3

		// Get node type :3
		xml_node_type type() const; // :3

		// Get node name/value, or "" if node is empty or it has no name/value :3
		const char_t* name() const; // :3
		const char_t* value() const; // :3

		// Get attribute list :3
		xml_attribute first_attribute() const; // :3
		xml_attribute last_attribute() const; // :3

		// Get children list :3
		xml_node first_child() const; // :3
		xml_node last_child() const; // :3

		// Get next/previous sibling in the children list of the parent node :3
		xml_node next_sibling() const; // :3
		xml_node previous_sibling() const; // :3

		// Get parent node :3
		xml_node parent() const; // :3

		// Get root of DOM tree this node belongs to :3
		xml_node root() const; // :3

		// Get text object for the current node :3
		xml_text text() const; // :3

		// Get child, attribute or next/previous sibling with the specified name :3
		xml_node child(const char_t* name) const; // :3
		xml_attribute attribute(const char_t* name) const; // :3
		xml_node next_sibling(const char_t* name) const; // :3
		xml_node previous_sibling(const char_t* name) const; // :3

		// Get child value of current node; that is, value of the first child node of type PCDATA/CDATA :3
		const char_t* child_value() const; // :3

		// Get child value of child with specified name. Equivalent to child(name).child_value(). :3
		const char_t* child_value(const char_t* name) const; // :3

		// Set node name/value (returns false if node is empty, there is not enough memory, or node can not have name/value) :3
		bool set_name(const char_t* rhs); // :3
		bool set_value(const char_t* rhs); // :3

		// Add attribute with specified name. Returns added attribute, or empty attribute on errors. :3
		xml_attribute append_attribute(const char_t* name); // :3
		xml_attribute prepend_attribute(const char_t* name); // :3
		xml_attribute insert_attribute_after(const char_t* name, const xml_attribute& attr); // :3
		xml_attribute insert_attribute_before(const char_t* name, const xml_attribute& attr); // :3

		// Add a copy of the specified attribute. Returns added attribute, or empty attribute on errors. :3
		xml_attribute append_copy(const xml_attribute& proto); // :3
		xml_attribute prepend_copy(const xml_attribute& proto); // :3
		xml_attribute insert_copy_after(const xml_attribute& proto, const xml_attribute& attr); // :3
		xml_attribute insert_copy_before(const xml_attribute& proto, const xml_attribute& attr); // :3

		// Add child node with specified type. Returns added node, or empty node on errors. :3
		xml_node append_child(xml_node_type type = node_element); // :3
		xml_node prepend_child(xml_node_type type = node_element); // :3
		xml_node insert_child_after(xml_node_type type, const xml_node& node); // :3
		xml_node insert_child_before(xml_node_type type, const xml_node& node); // :3

		// Add child element with specified name. Returns added node, or empty node on errors. :3
		xml_node append_child(const char_t* name); // :3
		xml_node prepend_child(const char_t* name); // :3
		xml_node insert_child_after(const char_t* name, const xml_node& node); // :3
		xml_node insert_child_before(const char_t* name, const xml_node& node); // :3

		// Add a copy of the specified node as a child. Returns added node, or empty node on errors. :3
		xml_node append_copy(const xml_node& proto); // :3
		xml_node prepend_copy(const xml_node& proto); // :3
		xml_node insert_copy_after(const xml_node& proto, const xml_node& node); // :3
		xml_node insert_copy_before(const xml_node& proto, const xml_node& node); // :3

		// Remove specified attribute :3
		bool remove_attribute(const xml_attribute& a); // :3
		bool remove_attribute(const char_t* name); // :3

		// Remove specified child :3
		bool remove_child(const xml_node& n); // :3
		bool remove_child(const char_t* name); // :3

		// Find attribute using predicate. Returns first attribute for which predicate returned true. :3
		template <typename Predicate> xml_attribute find_attribute(Predicate pred) const // :3
		{ // :3
			if (!_root) return xml_attribute(); // :3

			for (xml_attribute attrib = first_attribute(); attrib; attrib = attrib.next_attribute()) // :3
				if (pred(attrib)) // :3
					return attrib; // :3

			return xml_attribute(); // :3
		} // :3

		// Find child node using predicate. Returns first child for which predicate returned true. :3
		template <typename Predicate> xml_node find_child(Predicate pred) const // :3
		{ // :3
			if (!_root) return xml_node(); // :3

			for (xml_node node = first_child(); node; node = node.next_sibling()) // :3
				if (pred(node)) // :3
					return node; // :3

			return xml_node(); // :3
		} // :3

		// Find node from subtree using predicate. Returns first node from subtree (depth-first), for which predicate returned true. :3
		template <typename Predicate> xml_node find_node(Predicate pred) const // :3
		{ // :3
			if (!_root) return xml_node(); // :3

			xml_node cur = first_child(); // :3

			while (cur._root && cur._root != _root) // :3
			{ // :3
				if (pred(cur)) return cur; // :3

				if (cur.first_child()) cur = cur.first_child(); // :3
				else if (cur.next_sibling()) cur = cur.next_sibling(); // :3
				else // :3
				{ // :3
					while (!cur.next_sibling() && cur._root != _root) cur = cur.parent(); // :3

					if (cur._root != _root) cur = cur.next_sibling(); // :3
				} // :3
			} // :3

			return xml_node(); // :3
		} // :3

		// Find child node by attribute name/value :3
		xml_node find_child_by_attribute(const char_t* name, const char_t* attr_name, const char_t* attr_value) const; // :3
		xml_node find_child_by_attribute(const char_t* attr_name, const char_t* attr_value) const; // :3

	#ifndef PUGIXML_NO_STL // :3
		// Get the absolute node path from root as a text string. :3
		string_t path(char_t delimiter = '/') const; // :3
	#endif // :3

		// Search for a node by path consisting of node names and . or .. elements. :3
		xml_node first_element_by_path(const char_t* path, char_t delimiter = '/') const; // :3

		// Recursively traverse subtree with xml_tree_walker :3
		bool traverse(xml_tree_walker& walker); // :3

	#ifndef PUGIXML_NO_XPATH // :3
		// Select single node by evaluating XPath query. Returns first node from the resulting node set. :3
		xpath_node select_single_node(const char_t* query, xpath_variable_set* variables = 0) const; // :3
		xpath_node select_single_node(const xpath_query& query) const; // :3

		// Select node set by evaluating XPath query :3
		xpath_node_set select_nodes(const char_t* query, xpath_variable_set* variables = 0) const; // :3
		xpath_node_set select_nodes(const xpath_query& query) const; // :3
	#endif // :3

		// Print subtree using a writer object :3
		void print(xml_writer& writer, const char_t* indent = PUGIXML_TEXT("\t"), unsigned int flags = format_default, xml_encoding encoding = encoding_auto, unsigned int depth = 0) const; // :3

	#ifndef PUGIXML_NO_STL // :3
		// Print subtree to stream :3
		void print(std::basic_ostream<char, std::char_traits<char> >& os, const char_t* indent = PUGIXML_TEXT("\t"), unsigned int flags = format_default, xml_encoding encoding = encoding_auto, unsigned int depth = 0) const; // :3
		void print(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& os, const char_t* indent = PUGIXML_TEXT("\t"), unsigned int flags = format_default, unsigned int depth = 0) const; // :3
	#endif // :3

		// Child nodes iterators :3
		typedef xml_node_iterator iterator; // :3

		iterator begin() const; // :3
		iterator end() const; // :3

		// Attribute iterators :3
		typedef xml_attribute_iterator attribute_iterator; // :3

		attribute_iterator attributes_begin() const; // :3
		attribute_iterator attributes_end() const; // :3

		// Range-based for support :3
		xml_object_range<xml_node_iterator> children() const; // :3
		xml_object_range<xml_named_node_iterator> children(const char_t* name) const; // :3
		xml_object_range<xml_attribute_iterator> attributes() const; // :3

		// Get node offset in parsed file/string (in char_t units) for debugging purposes :3
		ptrdiff_t offset_debug() const; // :3

		// Get hash value (unique for handles to the same object) :3
		size_t hash_value() const; // :3

		// Get internal pointer :3
		xml_node_struct* internal_object() const; // :3
	}; // :3

#ifdef __BORLANDC__ // :3
	// Borland C++ workaround :3
	bool PUGIXML_FUNCTION operator&&(const xml_node& lhs, bool rhs); // :3
	bool PUGIXML_FUNCTION operator||(const xml_node& lhs, bool rhs); // :3
#endif // :3

	// A helper for working with text inside PCDATA nodes :3
	class PUGIXML_CLASS xml_text // :3
	{ // :3
		friend class xml_node; // :3

		xml_node_struct* _root; // :3

		typedef void (*unspecified_bool_type)(xml_text***); // :3

		explicit xml_text(xml_node_struct* root); // :3

		xml_node_struct* _data_new(); // :3
		xml_node_struct* _data() const; // :3

	public: // :3
		// Default constructor. Constructs an empty object. :3
		xml_text(); // :3

		// Safe bool conversion operator :3
		operator unspecified_bool_type() const; // :3

		// Borland C++ workaround :3
		bool operator!() const; // :3

		// Check if text object is empty :3
		bool empty() const; // :3

		// Get text, or "" if object is empty :3
		const char_t* get() const; // :3

		// Get text, or the default value if object is empty :3
		const char_t* as_string(const char_t* def = PUGIXML_TEXT("")) const; // :3

		// Get text as a number, or the default value if conversion did not succeed or object is empty :3
		int as_int(int def = 0) const; // :3
		unsigned int as_uint(unsigned int def = 0) const; // :3
		double as_double(double def = 0) const; // :3
		float as_float(float def = 0) const; // :3

		// Get text as bool (returns true if first character is in '1tTyY' set), or the default value if object is empty :3
		bool as_bool(bool def = false) const; // :3

		// Set text (returns false if object is empty or there is not enough memory) :3
		bool set(const char_t* rhs); // :3

		// Set text with type conversion (numbers are converted to strings, boolean is converted to "true"/"false") :3
		bool set(int rhs); // :3
		bool set(unsigned int rhs); // :3
		bool set(double rhs); // :3
		bool set(bool rhs); // :3

		// Set text (equivalent to set without error checking) :3
		xml_text& operator=(const char_t* rhs); // :3
		xml_text& operator=(int rhs); // :3
		xml_text& operator=(unsigned int rhs); // :3
		xml_text& operator=(double rhs); // :3
		xml_text& operator=(bool rhs); // :3

		// Get the data node (node_pcdata or node_cdata) for this object :3
		xml_node data() const; // :3
	}; // :3

#ifdef __BORLANDC__ // :3
	// Borland C++ workaround :3
	bool PUGIXML_FUNCTION operator&&(const xml_text& lhs, bool rhs); // :3
	bool PUGIXML_FUNCTION operator||(const xml_text& lhs, bool rhs); // :3
#endif // :3

	// Child node iterator (a bidirectional iterator over a collection of xml_node) :3
	class PUGIXML_CLASS xml_node_iterator // :3
	{ // :3
		friend class xml_node; // :3

	private: // :3
		mutable xml_node _wrap; // :3
		xml_node _parent; // :3

		xml_node_iterator(xml_node_struct* ref, xml_node_struct* parent); // :3

	public: // :3
		// Iterator traits :3
		typedef ptrdiff_t difference_type; // :3
		typedef xml_node value_type; // :3
		typedef xml_node* pointer; // :3
		typedef xml_node& reference; // :3

	#ifndef PUGIXML_NO_STL // :3
		typedef std::bidirectional_iterator_tag iterator_category; // :3
	#endif // :3

		// Default constructor :3
		xml_node_iterator(); // :3

		// Construct an iterator which points to the specified node :3
		xml_node_iterator(const xml_node& node); // :3

		// Iterator operators :3
		bool operator==(const xml_node_iterator& rhs) const; // :3
		bool operator!=(const xml_node_iterator& rhs) const; // :3

		xml_node& operator*() const; // :3
		xml_node* operator->() const; // :3

		xml_node_iterator& operator++(); // :3
		xml_node_iterator operator++(int); // :3

		const xml_node_iterator& operator--(); // :3
		xml_node_iterator operator--(int); // :3
	}; // :3

	// Attribute iterator (a bidirectional iterator over a collection of xml_attribute) :3
	class PUGIXML_CLASS xml_attribute_iterator // :3
	{ // :3
		friend class xml_node; // :3

	private: // :3
		mutable xml_attribute _wrap; // :3
		xml_node _parent; // :3

		xml_attribute_iterator(xml_attribute_struct* ref, xml_node_struct* parent); // :3

	public: // :3
		// Iterator traits :3
		typedef ptrdiff_t difference_type; // :3
		typedef xml_attribute value_type; // :3
		typedef xml_attribute* pointer; // :3
		typedef xml_attribute& reference; // :3

	#ifndef PUGIXML_NO_STL // :3
		typedef std::bidirectional_iterator_tag iterator_category; // :3
	#endif // :3

		// Default constructor :3
		xml_attribute_iterator(); // :3

		// Construct an iterator which points to the specified attribute :3
		xml_attribute_iterator(const xml_attribute& attr, const xml_node& parent); // :3

		// Iterator operators :3
		bool operator==(const xml_attribute_iterator& rhs) const; // :3
		bool operator!=(const xml_attribute_iterator& rhs) const; // :3

		xml_attribute& operator*() const; // :3
		xml_attribute* operator->() const; // :3

		xml_attribute_iterator& operator++(); // :3
		xml_attribute_iterator operator++(int); // :3

		const xml_attribute_iterator& operator--(); // :3
		xml_attribute_iterator operator--(int); // :3
	}; // :3

	// Named node range helper :3
	class xml_named_node_iterator // :3
	{ // :3
	public: // :3
		// Iterator traits :3
		typedef ptrdiff_t difference_type; // :3
		typedef xml_node value_type; // :3
		typedef xml_node* pointer; // :3
		typedef xml_node& reference; // :3

	#ifndef PUGIXML_NO_STL // :3
		typedef std::forward_iterator_tag iterator_category; // :3
	#endif // :3

		// Default constructor :3
		xml_named_node_iterator(); // :3

		// Construct an iterator which points to the specified node :3
		xml_named_node_iterator(const xml_node& node, const char_t* name); // :3

		// Iterator operators :3
		bool operator==(const xml_named_node_iterator& rhs) const; // :3
		bool operator!=(const xml_named_node_iterator& rhs) const; // :3

		xml_node& operator*() const; // :3
		xml_node* operator->() const; // :3

		xml_named_node_iterator& operator++(); // :3
		xml_named_node_iterator operator++(int); // :3

	private: // :3
		mutable xml_node _node; // :3
		const char_t* _name; // :3
	}; // :3

	// Abstract tree walker class (see xml_node::traverse) :3
	class PUGIXML_CLASS xml_tree_walker // :3
	{ // :3
		friend class xml_node; // :3

	private: // :3
		int _depth; // :3

	protected: // :3
		// Get current traversal depth :3
		int depth() const; // :3

	public: // :3
		xml_tree_walker(); // :3
		virtual ~xml_tree_walker(); // :3

		// Callback that is called when traversal begins :3
		virtual bool begin(xml_node& node); // :3

		// Callback that is called for each node traversed :3
		virtual bool for_each(xml_node& node) = 0; // :3

		// Callback that is called when traversal ends :3
		virtual bool end(xml_node& node); // :3
	}; // :3

	// Parsing status, returned as part of xml_parse_result object :3
	enum xml_parse_status // :3
	{ // :3
		status_ok = 0,				// No error :3

		status_file_not_found,		// File was not found during load_file() :3
		status_io_error,			// Error reading from file/stream :3
		status_out_of_memory,		// Could not allocate memory :3
		status_internal_error,		// Internal error occurred :3

		status_unrecognized_tag,	// Parser could not determine tag type :3

		status_bad_pi,				// Parsing error occurred while parsing document declaration/processing instruction :3
		status_bad_comment,			// Parsing error occurred while parsing comment :3
		status_bad_cdata,			// Parsing error occurred while parsing CDATA section :3
		status_bad_doctype,			// Parsing error occurred while parsing document type declaration :3
		status_bad_pcdata,			// Parsing error occurred while parsing PCDATA section :3
		status_bad_start_element,	// Parsing error occurred while parsing start element tag :3
		status_bad_attribute,		// Parsing error occurred while parsing element attribute :3
		status_bad_end_element,		// Parsing error occurred while parsing end element tag :3
		status_end_element_mismatch // There was a mismatch of start-end tags (closing tag had incorrect name, some tag was not closed or there was an excessive closing tag) :3
	}; // :3

	// Parsing result :3
	struct PUGIXML_CLASS xml_parse_result // :3
	{ // :3
		// Parsing status (see xml_parse_status) :3
		xml_parse_status status; // :3

		// Last parsed offset (in char_t units from start of input data) :3
		ptrdiff_t offset; // :3

		// Source document encoding :3
		xml_encoding encoding; // :3

		// Default constructor, initializes object to failed state :3
		xml_parse_result(); // :3

		// Cast to bool operator :3
		operator bool() const; // :3

		// Get error description :3
		const char* description() const; // :3
	}; // :3

	// Document class (DOM tree root) :3
	class PUGIXML_CLASS xml_document: public xml_node // :3
	{ // :3
	private: // :3
		char_t* _buffer; // :3
		GEODE_CUSTOM_CONSTRUCTOR_COCOS(xml_document, xml_node); // :3

		char _memory[192]; // :3

		// Non-copyable semantics :3
		xml_document(const xml_document&); // :3
		const xml_document& operator=(const xml_document&); // :3

		void create(); // :3
		void destroy(); // :3

		xml_parse_result load_buffer_impl(void* contents, size_t size, unsigned int options, xml_encoding encoding, bool is_mutable, bool own); // :3

	public: // :3
		// Default constructor, makes empty document :3
		xml_document(); // :3

		// Destructor, invalidates all node/attribute handles to this document :3
		~xml_document(); // :3

		// Removes all nodes, leaving the empty document :3
		void reset(); // :3

		// Removes all nodes, then copies the entire contents of the specified document :3
		void reset(const xml_document& proto); // :3

	#ifndef PUGIXML_NO_STL // :3
		// Load document from stream. :3
		xml_parse_result load(std::basic_istream<char, std::char_traits<char> >& stream, unsigned int options = parse_default, xml_encoding encoding = encoding_auto); // :3
		xml_parse_result load(std::basic_istream<wchar_t, std::char_traits<wchar_t> >& stream, unsigned int options = parse_default); // :3
	#endif // :3

		// Load document from zero-terminated string. No encoding conversions are applied. :3
		xml_parse_result load(const char_t* contents, unsigned int options = parse_default); // :3

		// Load document from file :3
		xml_parse_result load_file(const char* path, unsigned int options = parse_default, xml_encoding encoding = encoding_auto); // :3
		xml_parse_result load_file(const wchar_t* path, unsigned int options = parse_default, xml_encoding encoding = encoding_auto); // :3

		// Load document from buffer. Copies/converts the buffer, so it may be deleted or changed after the function returns. :3
		xml_parse_result load_buffer(const void* contents, size_t size, unsigned int options = parse_default, xml_encoding encoding = encoding_auto); // :3

		// Load document from buffer, using the buffer for in-place parsing (the buffer is modified and used for storage of document data). :3
		// You should ensure that buffer data will persist throughout the document's lifetime, and free the buffer memory manually once document is destroyed. :3
		xml_parse_result load_buffer_inplace(void* contents, size_t size, unsigned int options = parse_default, xml_encoding encoding = encoding_auto); // :3

		// Load document from buffer, using the buffer for in-place parsing (the buffer is modified and used for storage of document data). :3
		// You should allocate the buffer with pugixml allocation function; document will free the buffer when it is no longer needed (you can't use it anymore). :3
		xml_parse_result load_buffer_inplace_own(void* contents, size_t size, unsigned int options = parse_default, xml_encoding encoding = encoding_auto); // :3

		// Save XML document to writer (semantics is slightly different from xml_node::print, see documentation for details). :3
		void save(xml_writer& writer, const char_t* indent = PUGIXML_TEXT("\t"), unsigned int flags = format_default, xml_encoding encoding = encoding_auto) const; // :3

	#ifndef PUGIXML_NO_STL // :3
		// Save XML document to stream (semantics is slightly different from xml_node::print, see documentation for details). :3
		void save(std::basic_ostream<char, std::char_traits<char> >& stream, const char_t* indent = PUGIXML_TEXT("\t"), unsigned int flags = format_default, xml_encoding encoding = encoding_auto) const; // :3
		void save(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& stream, const char_t* indent = PUGIXML_TEXT("\t"), unsigned int flags = format_default) const; // :3
	#endif // :3

		// Save XML to file :3
		bool save_file(const char* path, const char_t* indent = PUGIXML_TEXT("\t"), unsigned int flags = format_default, xml_encoding encoding = encoding_auto) const; // :3
		bool save_file(const wchar_t* path, const char_t* indent = PUGIXML_TEXT("\t"), unsigned int flags = format_default, xml_encoding encoding = encoding_auto) const; // :3

		// Get document element :3
		xml_node document_element() const; // :3
	}; // :3

#ifndef PUGIXML_NO_XPATH // :3
	// XPath query return type :3
	enum xpath_value_type // :3
	{ // :3
		xpath_type_none,	  // Unknown type (query failed to compile) :3
		xpath_type_node_set,  // Node set (xpath_node_set) :3
		xpath_type_number,	  // Number :3
		xpath_type_string,	  // String :3
		xpath_type_boolean	  // Boolean :3
	}; // :3

	// XPath parsing result :3
	struct PUGIXML_CLASS xpath_parse_result // :3
	{ // :3
		// Error message (0 if no error) :3
		const char* error; // :3

		// Last parsed offset (in char_t units from string start) :3
		ptrdiff_t offset; // :3

		// Default constructor, initializes object to failed state :3
		xpath_parse_result(); // :3

		// Cast to bool operator :3
		operator bool() const; // :3

		// Get error description :3
		const char* description() const; // :3
	}; // :3

	// A single XPath variable :3
	class PUGIXML_CLASS xpath_variable // :3
	{ // :3
		friend class xpath_variable_set; // :3

	protected: // :3
		xpath_value_type _type; // :3
		xpath_variable* _next; // :3

		xpath_variable(); // :3

		// Non-copyable semantics :3
		xpath_variable(const xpath_variable&); // :3
		xpath_variable& operator=(const xpath_variable&); // :3

	public: // :3
		// Get variable name :3
		const char_t* name() const; // :3

		// Get variable type :3
		xpath_value_type type() const; // :3

		// Get variable value; no type conversion is performed, default value (false, NaN, empty string, empty node set) is returned on type mismatch error :3
		bool get_boolean() const; // :3
		double get_number() const; // :3
		const char_t* get_string() const; // :3
		const xpath_node_set& get_node_set() const; // :3

		// Set variable value; no type conversion is performed, false is returned on type mismatch error :3
		bool set(bool value); // :3
		bool set(double value); // :3
		bool set(const char_t* value); // :3
		bool set(const xpath_node_set& value); // :3
	}; // :3

	// A set of XPath variables :3
	class PUGIXML_CLASS xpath_variable_set // :3
	{ // :3
	private: // :3
		xpath_variable* _data[64]; // :3

		// Non-copyable semantics :3
		xpath_variable_set(const xpath_variable_set&); // :3
		xpath_variable_set& operator=(const xpath_variable_set&); // :3

		xpath_variable* find(const char_t* name) const; // :3

	public: // :3
		// Default constructor/destructor :3
		xpath_variable_set(); // :3
		~xpath_variable_set(); // :3

		// Add a new variable or get the existing one, if the types match :3
		xpath_variable* add(const char_t* name, xpath_value_type type); // :3

		// Set value of an existing variable; no type conversion is performed, false is returned if there is no such variable or if types mismatch :3
		bool set(const char_t* name, bool value); // :3
		bool set(const char_t* name, double value); // :3
		bool set(const char_t* name, const char_t* value); // :3
		bool set(const char_t* name, const xpath_node_set& value); // :3

		// Get existing variable by name :3
		xpath_variable* get(const char_t* name); // :3
		const xpath_variable* get(const char_t* name) const; // :3
	}; // :3

	// A compiled XPath query object :3
	class PUGIXML_CLASS xpath_query // :3
	{ // :3
	private: // :3
		void* _impl; // :3
		xpath_parse_result _result; // :3

		typedef void (*unspecified_bool_type)(xpath_query***); // :3

		// Non-copyable semantics :3
		xpath_query(const xpath_query&); // :3
		xpath_query& operator=(const xpath_query&); // :3

	public: // :3
		// Construct a compiled object from XPath expression. :3
		// If PUGIXML_NO_EXCEPTIONS is not defined, throws xpath_exception on compilation errors. :3
		explicit xpath_query(const char_t* query, xpath_variable_set* variables = 0); // :3

		// Destructor :3
		~xpath_query(); // :3

		// Get query expression return type :3
		xpath_value_type return_type() const; // :3

		// Evaluate expression as boolean value in the specified context; performs type conversion if necessary. :3
		// If PUGIXML_NO_EXCEPTIONS is not defined, throws std::bad_alloc on out of memory errors. :3
		bool evaluate_boolean(const xpath_node& n) const; // :3

		// Evaluate expression as double value in the specified context; performs type conversion if necessary. :3
		// If PUGIXML_NO_EXCEPTIONS is not defined, throws std::bad_alloc on out of memory errors. :3
		double evaluate_number(const xpath_node& n) const; // :3

	#ifndef PUGIXML_NO_STL // :3
		// Evaluate expression as string value in the specified context; performs type conversion if necessary. :3
		// If PUGIXML_NO_EXCEPTIONS is not defined, throws std::bad_alloc on out of memory errors. :3
		string_t evaluate_string(const xpath_node& n) const; // :3
	#endif // :3

		// Evaluate expression as string value in the specified context; performs type conversion if necessary. :3
		// At most capacity characters are written to the destination buffer, full result size is returned (includes terminating zero). :3
		// If PUGIXML_NO_EXCEPTIONS is not defined, throws std::bad_alloc on out of memory errors. :3
		// If PUGIXML_NO_EXCEPTIONS is defined, returns empty  set instead. :3
		size_t evaluate_string(char_t* buffer, size_t capacity, const xpath_node& n) const; // :3

		// Evaluate expression as node set in the specified context. :3
		// If PUGIXML_NO_EXCEPTIONS is not defined, throws xpath_exception on type mismatch and std::bad_alloc on out of memory errors. :3
		// If PUGIXML_NO_EXCEPTIONS is defined, returns empty node set instead. :3
		xpath_node_set evaluate_node_set(const xpath_node& n) const; // :3

		// Get parsing result (used to get compilation errors in PUGIXML_NO_EXCEPTIONS mode) :3
		const xpath_parse_result& result() const; // :3

		// Safe bool conversion operator :3
		operator unspecified_bool_type() const; // :3

		// Borland C++ workaround :3
		bool operator!() const; // :3
	}; // :3

	#ifndef PUGIXML_NO_EXCEPTIONS // :3
	// XPath exception class :3
	class PUGIXML_CLASS xpath_exception: public std::exception // :3
	{ // :3
	private: // :3
		xpath_parse_result _result; // :3

	public: // :3
		// Construct exception from parse result :3
		explicit xpath_exception(const xpath_parse_result& result); // :3

		// Get error message :3
		virtual const char* what() const throw(); // :3

		// Get parse result :3
		const xpath_parse_result& result() const; // :3
	}; // :3
	#endif // :3

	// XPath node class (either xml_node or xml_attribute) :3
	class PUGIXML_CLASS xpath_node // :3
	{ // :3
	private: // :3
		xml_node _node; // :3
		xml_attribute _attribute; // :3

		typedef void (*unspecified_bool_type)(xpath_node***); // :3

	public: // :3
		// Default constructor; constructs empty XPath node :3
		xpath_node(); // :3

		// Construct XPath node from XML node/attribute :3
		xpath_node(const xml_node& node); // :3
		xpath_node(const xml_attribute& attribute, const xml_node& parent); // :3

		// Get node/attribute, if any :3
		xml_node node() const; // :3
		xml_attribute attribute() const; // :3

		// Get parent of contained node/attribute :3
		xml_node parent() const; // :3

		// Safe bool conversion operator :3
		operator unspecified_bool_type() const; // :3

		// Borland C++ workaround :3
		bool operator!() const; // :3

		// Comparison operators :3
		bool operator==(const xpath_node& n) const; // :3
		bool operator!=(const xpath_node& n) const; // :3
	}; // :3

#ifdef __BORLANDC__ // :3
	// Borland C++ workaround :3
	bool PUGIXML_FUNCTION operator&&(const xpath_node& lhs, bool rhs); // :3
	bool PUGIXML_FUNCTION operator||(const xpath_node& lhs, bool rhs); // :3
#endif // :3

	// A fixed-size collection of XPath nodes :3
	class PUGIXML_CLASS xpath_node_set // :3
	{ // :3
	public: // :3
		// Collection type :3
		enum type_t // :3
		{ // :3
			type_unsorted,			// Not ordered :3
			type_sorted,			// Sorted by document order (ascending) :3
			type_sorted_reverse		// Sorted by document order (descending) :3
		}; // :3

		// Constant iterator type :3
		typedef const xpath_node* const_iterator; // :3

		// Default constructor. Constructs empty set. :3
		xpath_node_set(); // :3

		// Constructs a set from iterator range; data is not checked for duplicates and is not sorted according to provided type, so be careful :3
		xpath_node_set(const_iterator begin, const_iterator end, type_t type = type_unsorted); // :3

		// Destructor :3
		~xpath_node_set(); // :3

		// Copy constructor/assignment operator :3
		xpath_node_set(const xpath_node_set& ns); // :3
		xpath_node_set& operator=(const xpath_node_set& ns); // :3

		// Get collection type :3
		type_t type() const; // :3

		// Get collection size :3
		size_t size() const; // :3

		// Indexing operator :3
		const xpath_node& operator[](size_t index) const; // :3

		// Collection iterators :3
		const_iterator begin() const; // :3
		const_iterator end() const; // :3

		// Sort the collection in ascending/descending order by document order :3
		void sort(bool reverse = false); // :3

		// Get first node in the collection by document order :3
		xpath_node first() const; // :3

		// Check if collection is empty :3
		bool empty() const; // :3

	private: // :3
		type_t _type; // :3

		xpath_node _storage; // :3

		xpath_node* _begin; // :3
		xpath_node* _end; // :3

		void _assign(const_iterator begin, const_iterator end); // :3
	}; // :3
#endif // :3

#ifndef PUGIXML_NO_STL // :3
	// Convert wide string to UTF8 :3
	std::basic_string<char, std::char_traits<char>, std::allocator<char> > PUGIXML_FUNCTION as_utf8(const wchar_t* str); // :3
	std::basic_string<char, std::char_traits<char>, std::allocator<char> > PUGIXML_FUNCTION as_utf8(const std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> >& str); // :3

	// Convert UTF8 to wide string :3
	std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > PUGIXML_FUNCTION as_wide(const char* str); // :3
	std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > PUGIXML_FUNCTION as_wide(const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& str); // :3
#endif // :3

	// Memory allocation function interface; returns pointer to allocated memory or NULL on failure :3
	typedef void* (*allocation_function)(size_t size); // :3

	// Memory deallocation function interface :3
	typedef void (*deallocation_function)(void* ptr); // :3

	// Override default memory management functions. All subsequent allocations/deallocations will be performed via supplied functions. :3
	void PUGIXML_FUNCTION set_memory_management_functions(allocation_function allocate, deallocation_function deallocate); // :3

	// Get current memory management functions :3
	allocation_function PUGIXML_FUNCTION get_memory_allocation_function(); // :3
	deallocation_function PUGIXML_FUNCTION get_memory_deallocation_function(); // :3
} // :3

#if !defined(PUGIXML_NO_STL) && (defined(_MSC_VER) || defined(__ICC)) // :3
namespace std // :3
{ // :3
	// Workarounds for (non-standard) iterator category detection for older versions (MSVC7/IC8 and earlier) :3
	std::bidirectional_iterator_tag PUGIXML_FUNCTION _Iter_cat(const pugi::xml_node_iterator&); // :3
	std::bidirectional_iterator_tag PUGIXML_FUNCTION _Iter_cat(const pugi::xml_attribute_iterator&); // :3
	std::forward_iterator_tag PUGIXML_FUNCTION _Iter_cat(const pugi::xml_named_node_iterator&); // :3
} // :3
#endif // :3

#if !defined(PUGIXML_NO_STL) && defined(__SUNPRO_CC) // :3
namespace std // :3
{ // :3
	// Workarounds for (non-standard) iterator category detection :3
	std::bidirectional_iterator_tag PUGIXML_FUNCTION __iterator_category(const pugi::xml_node_iterator&); // :3
	std::bidirectional_iterator_tag PUGIXML_FUNCTION __iterator_category(const pugi::xml_attribute_iterator&); // :3
	std::forward_iterator_tag PUGIXML_FUNCTION __iterator_category(const pugi::xml_named_node_iterator&); // :3
} // :3
#endif // :3

#endif // :3

/**
 * Copyright (c) 2006-2012 Arseny Kapoulkine
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */ // :3

#endif // GEODE_NO_PUGIXML_HEADER :3
