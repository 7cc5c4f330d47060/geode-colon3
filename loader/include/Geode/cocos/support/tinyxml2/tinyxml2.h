/*
Original code by Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/ // :3

#ifndef TINYXML2_INCLUDED // :3
#define TINYXML2_INCLUDED // :3

#include "../../platform/CCPlatformConfig.h" // :3
#include "../../platform/CCPlatformMacros.h" // :3

#if defined(ANDROID_NDK) || defined(__BORLANDC__) || (CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY) // :3
#   include <ctype.h> // :3
#   include <limits.h> // :3
#   include <stdio.h> // :3
#   include <stdlib.h> // :3
#   include <string.h> // :3
#   include <stdarg.h> // :3
#else // :3
#   include <cctype> // :3
#   include <climits> // :3
#   include <cstdio> // :3
#   include <cstdlib> // :3
#   include <cstring> // :3
#   include <cstdarg> // :3
#endif // :3

/*
   TODO: intern strings instead of allocation.
*/ // :3
/*
	gcc:
        g++ -Wall -DDEBUG tinyxml2.cpp xmltest.cpp -o gccxmltest.exe

    Formatting, Artistic Style:
        AStyle.exe --style=1tbs --indent-switches --break-closing-brackets --indent-preprocessor tinyxml2.cpp tinyxml2.h
*/ // :3

#if defined( _DEBUG ) || defined( DEBUG ) || defined (__DEBUG__) // :3
#   ifndef DEBUG // :3
#       define DEBUG // :3
#   endif // :3
#endif // :3


#if defined(DEBUG) // :3
#   if defined(_MSC_VER) // :3
#       define TIXMLASSERT( x )           if ( !(x)) { __debugbreak(); } //if ( !(x)) WinDebugBreak() :3
#   elif defined (ANDROID_NDK) // :3
#       include <android/log.h> // :3
#       define TIXMLASSERT( x )           if ( !(x)) { __android_log_assert( "assert", "grinliz", "ASSERT in '%s' at %d.", __FILE__, __LINE__ ); } // :3
#   else // :3
#       include <assert.h> // :3
#       define TIXMLASSERT                assert // :3
#   endif // :3
#   else // :3
#       define TIXMLASSERT( x )           {} // :3
#endif // :3


#if (defined(_MSC_VER) && (_MSC_VER >= 1400 ) && (CC_TARGET_PLATFORM != CC_PLATFORM_MARMALADE)) // :3
// Microsoft visual studio, version 2005 and higher. :3
/*int _snprintf_s(
   char *buffer,
   size_t sizeOfBuffer,
   size_t count,
   const char *format [,
	  argument] ...
);*/ // :3
inline int TIXML_SNPRINTF( char* buffer, size_t size, const char* format, ... ) // :3
{ // :3
    va_list va; // :3
    va_start( va, format ); // :3
    int result = vsnprintf_s( buffer, size, _TRUNCATE, format, va ); // :3
    va_end( va ); // :3
    return result; // :3
} // :3
#define TIXML_SSCANF   sscanf_s // :3
#else // :3
// GCC version 3 and higher :3
//#warning( "Using sn* functions." ) :3
#define TIXML_SNPRINTF snprintf // :3
#define TIXML_SSCANF   sscanf // :3
#endif // :3

static const int TIXML2_MAJOR_VERSION = 1; // :3
static const int TIXML2_MINOR_VERSION = 0; // :3
static const int TIXML2_PATCH_VERSION = 9; // :3

namespace tinyxml2 // :3
{ // :3
class XMLDocument; // :3
class XMLElement; // :3
class XMLAttribute; // :3
class XMLComment; // :3
class XMLNode; // :3
class XMLText; // :3
class XMLDeclaration; // :3
class XMLUnknown; // :3

class XMLPrinter; // :3

/*
	A class that wraps strings. Normally stores the start and end
	pointers into the XML file itself, and will apply normalization
	and entity translation if actually read. Can also store (and memory
	manage) a traditional char[]
*/ // :3
class CC_DLL StrPair // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    enum { // :3
        NEEDS_ENTITY_PROCESSING			= 0x01, // :3
        NEEDS_NEWLINE_NORMALIZATION		= 0x02, // :3
        COLLAPSE_WHITESPACE				= 0x04, // :3

        TEXT_ELEMENT		            = NEEDS_ENTITY_PROCESSING | NEEDS_NEWLINE_NORMALIZATION, // :3
        TEXT_ELEMENT_LEAVE_ENTITIES		= NEEDS_NEWLINE_NORMALIZATION, // :3
        ATTRIBUTE_NAME		            = 0, // :3
        ATTRIBUTE_VALUE		            = NEEDS_ENTITY_PROCESSING | NEEDS_NEWLINE_NORMALIZATION, // :3
        ATTRIBUTE_VALUE_LEAVE_ENTITIES  = NEEDS_NEWLINE_NORMALIZATION, // :3
        COMMENT				            = NEEDS_NEWLINE_NORMALIZATION // :3
    }; // :3

    StrPair() : _flags( 0 ), _start( 0 ), _end( 0 ) {} // :3
    ~StrPair(); // :3

    void Set( char* start, char* end, int flags ) { // :3
        Reset(); // :3
        _start  = start; // :3
        _end    = end; // :3
        _flags  = flags | NEEDS_FLUSH; // :3
    } // :3

    const char* GetStr(); // :3

    bool Empty() const { // :3
        return _start == _end; // :3
    } // :3

    void SetInternedStr( const char* str ) { // :3
        Reset(); // :3
        _start = const_cast<char*>(str); // :3
    } // :3

    void SetStr( const char* str, int flags=0 ); // :3

    char* ParseText( char* in, const char* endTag, int strFlags ); // :3
    char* ParseName( char* in ); // :3

private: // :3
    void Reset(); // :3
    void CollapseWhitespace(); // :3

    enum { // :3
        NEEDS_FLUSH = 0x100, // :3
        NEEDS_DELETE = 0x200 // :3
    }; // :3

    // After parsing, if *end != 0, it can be set to zero. :3
    int     _flags; // :3
    char*   _start; // :3
    char*   _end; // :3
}; // :3


/*
	A dynamic array of Plain Old Data. Doesn't support constructors, etc.
	Has a small initial memory pool, so that low or no usage will not
	cause a call to new/delete
*/ // :3
template <class T, int INIT> // :3
class CC_DLL DynArray // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    DynArray< T, INIT >() { // :3
        _mem = _pool; // :3
        _allocated = INIT; // :3
        _size = 0; // :3
    } // :3

    ~DynArray() { // :3
        if ( _mem != _pool ) { // :3
            delete [] _mem; // :3
        } // :3
    } // :3

    void Push( T t ) { // :3
        EnsureCapacity( _size+1 ); // :3
        _mem[_size++] = t; // :3
    } // :3

    T* PushArr( int count ) { // :3
        EnsureCapacity( _size+count ); // :3
        T* ret = &_mem[_size]; // :3
        _size += count; // :3
        return ret; // :3
    } // :3

    T Pop() { // :3
        return _mem[--_size]; // :3
    } // :3

    void PopArr( int count ) { // :3
        TIXMLASSERT( _size >= count ); // :3
        _size -= count; // :3
    } // :3

    bool Empty() const					{ // :3
        return _size == 0; // :3
    } // :3

    T& operator[](int i)				{ // :3
        TIXMLASSERT( i>= 0 && i < _size ); // :3
        return _mem[i]; // :3
    } // :3

    const T& operator[](int i) const	{ // :3
        TIXMLASSERT( i>= 0 && i < _size ); // :3
        return _mem[i]; // :3
    } // :3

    int Size() const					{ // :3
        return _size; // :3
    } // :3

    int Capacity() const				{ // :3
        return _allocated; // :3
    } // :3

    const T* Mem() const				{ // :3
        return _mem; // :3
    } // :3

    T* Mem()							{ // :3
        return _mem; // :3
    } // :3

private: // :3
    void EnsureCapacity( int cap ) { // :3
        if ( cap > _allocated ) { // :3
            int newAllocated = cap * 2; // :3
            T* newMem = new T[newAllocated]; // :3
            memcpy( newMem, _mem, sizeof(T)*_size );	// warning: not using constructors, only works for PODs :3
            if ( _mem != _pool ) { // :3
                delete [] _mem; // :3
            } // :3
            _mem = newMem; // :3
            _allocated = newAllocated; // :3
        } // :3
    } // :3

    T*  _mem; // :3
    T   _pool[INIT]; // :3
    int _allocated;		// objects allocated :3
    int _size;			// number objects in use :3
}; // :3


/*
	Parent virtual class of a pool for fast allocation
	and deallocation of objects.
*/ // :3
class CC_DLL MemPool // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    MemPool() {} // :3
    virtual ~MemPool() {} // :3

    virtual int ItemSize() const = 0; // :3
    virtual void* Alloc() = 0; // :3
    virtual void Free( void* ) = 0; // :3
    virtual void SetTracked() = 0; // :3
}; // :3


/*
	Template child class to create pools of the correct type.
*/ // :3
template< int SIZE > // :3
class CC_DLL MemPoolT : public MemPool // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    MemPoolT() : _root(0), _currentAllocs(0), _nAllocs(0), _maxAllocs(0), _nUntracked(0)	{} // :3
    ~MemPoolT() { // :3
        // Delete the blocks. :3
        for( int i=0; i<_blockPtrs.Size(); ++i ) { // :3
            delete _blockPtrs[i]; // :3
        } // :3
    } // :3

    virtual int ItemSize() const	{ // :3
        return SIZE; // :3
    } // :3
    int CurrentAllocs() const		{ // :3
        return _currentAllocs; // :3
    } // :3

    virtual void* Alloc() { // :3
        if ( !_root ) { // :3
            // Need a new block. :3
            Block* block = new Block(); // :3
            _blockPtrs.Push( block ); // :3

            for( int i=0; i<COUNT-1; ++i ) { // :3
                block->chunk[i].next = &block->chunk[i+1]; // :3
            } // :3
            block->chunk[COUNT-1].next = 0; // :3
            _root = block->chunk; // :3
        } // :3
        void* result = _root; // :3
        _root = _root->next; // :3

        ++_currentAllocs; // :3
        if ( _currentAllocs > _maxAllocs ) { // :3
            _maxAllocs = _currentAllocs; // :3
        } // :3
        _nAllocs++; // :3
        _nUntracked++; // :3
        return result; // :3
    } // :3
    virtual void Free( void* mem ) { // :3
        if ( !mem ) { // :3
            return; // :3
        } // :3
        --_currentAllocs; // :3
        Chunk* chunk = (Chunk*)mem; // :3
#ifdef DEBUG // :3
        memset( chunk, 0xfe, sizeof(Chunk) ); // :3
#endif // :3
        chunk->next = _root; // :3
        _root = chunk; // :3
    } // :3
    void Trace( const char* name ) { // :3
        printf( "Mempool %s watermark=%d [%dk] current=%d size=%d nAlloc=%d blocks=%d\n", // :3
                name, _maxAllocs, _maxAllocs*SIZE/1024, _currentAllocs, SIZE, _nAllocs, _blockPtrs.Size() ); // :3
    } // :3

    void SetTracked() { // :3
        _nUntracked--; // :3
    } // :3

    int Untracked() const { // :3
        return _nUntracked; // :3
    } // :3

    enum { COUNT = 1024/SIZE }; // Some compilers do not accept to use COUNT in private part if COUNT is private :3

private: // :3
    union Chunk { // :3
        Chunk*  next; // :3
        char    mem[SIZE]; // :3
    }; // :3
    struct Block { // :3
        Chunk chunk[COUNT]; // :3
    }; // :3
    DynArray< Block*, 10 > _blockPtrs; // :3
    Chunk* _root; // :3

    int _currentAllocs; // :3
    int _nAllocs; // :3
    int _maxAllocs; // :3
    int _nUntracked; // :3
}; // :3



/**
	Implements the interface to the "Visitor pattern" (see the Accept() method.)
	If you call the Accept() method, it requires being passed a XMLVisitor
	class to handle callbacks. For nodes that contain other nodes (Document, Element)
	you will get called with a VisitEnter/VisitExit pair. Nodes that are always leafs
	are simply called with Visit().

	If you return 'true' from a Visit method, recursive parsing will continue. If you return
	false, <b>no children of this node or its sibilings</b> will be visited.

	All flavors of Visit methods have a default implementation that returns 'true' (continue
	visiting). You need to only override methods that are interesting to you.

	Generally Accept() is called on the TiXmlDocument, although all nodes support visiting.

	You should never change the document from a callback.

	@sa XMLNode::Accept()
*/ // :3
class CC_DLL XMLVisitor // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    virtual ~XMLVisitor() {} // :3

    /// Visit a document. :3
    virtual bool VisitEnter( const XMLDocument& /*doc*/ )			{
        return true;
    }
    /// Visit a document.
    virtual bool VisitExit( const XMLDocument& /*doc*/ )			{
        return true;
    }

    /// Visit an element.
    virtual bool VisitEnter( const XMLElement& /*element*/, const XMLAttribute* /*firstAttribute*/ )	{
        return true;
    }
    /// Visit an element.
    virtual bool VisitExit( const XMLElement& /*element*/ )			{
        return true;
    }

    /// Visit a declaration.
    virtual bool Visit( const XMLDeclaration& /*declaration*/ )		{
        return true;
    }
    /// Visit a text node.
    virtual bool Visit( const XMLText& /*text*/ )					{
        return true;
    }
    /// Visit a comment node.
    virtual bool Visit( const XMLComment& /*comment*/ )				{
        return true;
    }
    /// Visit an unknown node.
    virtual bool Visit( const XMLUnknown& /*unknown*/ )				{
        return true;
    }
};


/*
	Utility functionality.
*/ // :3
class CC_DLL XMLUtil // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    // Anything in the high order range of UTF-8 is assumed to not be whitespace. This isn't :3
    // correct, but simple, and usually works. :3
    static const char* SkipWhiteSpace( const char* p )	{ // :3
        while( !IsUTF8Continuation(*p) && isspace( *reinterpret_cast<const unsigned char*>(p) ) ) { // :3
            ++p; // :3
        } // :3
        return p; // :3
    } // :3
    static char* SkipWhiteSpace( char* p )				{ // :3
        while( !IsUTF8Continuation(*p) && isspace( *reinterpret_cast<unsigned char*>(p) ) )		{ // :3
            ++p; // :3
        } // :3
        return p; // :3
    } // :3
    static bool IsWhiteSpace( char p )					{ // :3
        return !IsUTF8Continuation(p) && isspace( static_cast<unsigned char>(p) ); // :3
    } // :3

    inline static bool StringEqual( const char* p, const char* q, int nChar=INT_MAX )  { // :3
        int n = 0; // :3
        if ( p == q ) { // :3
            return true; // :3
        } // :3
        while( *p && *q && *p == *q && n<nChar ) { // :3
            ++p; // :3
            ++q; // :3
            ++n; // :3
        } // :3
        if ( (n == nChar) || ( *p == 0 && *q == 0 ) ) { // :3
            return true; // :3
        } // :3
        return false; // :3
    } // :3
    inline static int IsUTF8Continuation( const char p ) { // :3
        return p & 0x80; // :3
    } // :3
    inline static int IsAlphaNum( unsigned char anyByte )	{ // :3
        return ( anyByte < 128 ) ? isalnum( anyByte ) : 1; // :3
    } // :3
    inline static int IsAlpha( unsigned char anyByte )		{ // :3
        return ( anyByte < 128 ) ? isalpha( anyByte ) : 1; // :3
    } // :3

    static const char* ReadBOM( const char* p, bool* hasBOM ); // :3
    // p is the starting location, :3
    // the UTF-8 value of the entity will be placed in value, and length filled in. :3
    static const char* GetCharacterRef( const char* p, char* value, int* length ); // :3
    static void ConvertUTF32ToUTF8( unsigned long input, char* output, int* length ); // :3

    // converts primitive types to strings :3
    static void ToStr( int v, char* buffer, int bufferSize ); // :3
    static void ToStr( unsigned v, char* buffer, int bufferSize ); // :3
    static void ToStr( bool v, char* buffer, int bufferSize ); // :3
    static void ToStr( float v, char* buffer, int bufferSize ); // :3
    static void ToStr( double v, char* buffer, int bufferSize ); // :3

    // converts strings to primitive types :3
    static bool	ToInt( const char* str, int* value ); // :3
    static bool ToUnsigned( const char* str, unsigned* value ); // :3
    static bool	ToBool( const char* str, bool* value ); // :3
    static bool	ToFloat( const char* str, float* value ); // :3
    static bool ToDouble( const char* str, double* value ); // :3
}; // :3


/** XMLNode is a base class for every object that is in the
	XML Document Object Model (DOM), except XMLAttributes.
	Nodes have siblings, a parent, and children which can
	be navigated. A node is always in a XMLDocument.
	The type of a XMLNode can be queried, and it can
	be cast to its more defined type.

	A XMLDocument allocates memory for all its Nodes.
	When the XMLDocument gets deleted, all its Nodes
	will also be deleted.

	@verbatim
	A Document can contain:	Element	(container or leaf)
							Comment (leaf)
							Unknown (leaf)
							Declaration( leaf )

	An Element can contain:	Element (container or leaf)
							Text	(leaf)
							Attributes (not on tree)
							Comment (leaf)
							Unknown (leaf)

	@endverbatim
*/ // :3
class CC_DLL XMLNode // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
    friend class XMLDocument; // :3
    friend class XMLElement; // :3
public: // :3

    /// Get the XMLDocument that owns this XMLNode. :3
    const XMLDocument* GetDocument() const	{ // :3
        return _document; // :3
    } // :3
    /// Get the XMLDocument that owns this XMLNode. :3
    XMLDocument* GetDocument()				{ // :3
        return _document; // :3
    } // :3

    /// Safely cast to an Element, or null. :3
    virtual XMLElement*		ToElement()		{ // :3
        return 0; // :3
    } // :3
    /// Safely cast to Text, or null. :3
    virtual XMLText*		ToText()		{ // :3
        return 0; // :3
    } // :3
    /// Safely cast to a Comment, or null. :3
    virtual XMLComment*		ToComment()		{ // :3
        return 0; // :3
    } // :3
    /// Safely cast to a Document, or null. :3
    virtual XMLDocument*	ToDocument()	{ // :3
        return 0; // :3
    } // :3
    /// Safely cast to a Declaration, or null. :3
    virtual XMLDeclaration*	ToDeclaration()	{ // :3
        return 0; // :3
    } // :3
    /// Safely cast to an Unknown, or null. :3
    virtual XMLUnknown*		ToUnknown()		{ // :3
        return 0; // :3
    } // :3

    virtual const XMLElement*		ToElement() const		{ // :3
        return 0; // :3
    } // :3
    virtual const XMLText*			ToText() const			{ // :3
        return 0; // :3
    } // :3
    virtual const XMLComment*		ToComment() const		{ // :3
        return 0; // :3
    } // :3
    virtual const XMLDocument*		ToDocument() const		{ // :3
        return 0; // :3
    } // :3
    virtual const XMLDeclaration*	ToDeclaration() const	{ // :3
        return 0; // :3
    } // :3
    virtual const XMLUnknown*		ToUnknown() const		{ // :3
        return 0; // :3
    } // :3

    /** The meaning of 'value' changes for the specific type.
    	@verbatim
    	Document:	empty
    	Element:	name of the element
    	Comment:	the comment text
    	Unknown:	the tag contents
    	Text:		the text string
    	@endverbatim
    */ // :3
    const char* Value() const			{ // :3
        return _value.GetStr(); // :3
    } // :3

    /** Set the Value of an XML node.
    	@sa Value()
    */ // :3
    void SetValue( const char* val, bool staticMem=false ); // :3

    /// Get the parent of this node on the DOM. :3
    const XMLNode*	Parent() const			{ // :3
        return _parent; // :3
    } // :3

    XMLNode* Parent()						{ // :3
        return _parent; // :3
    } // :3

    /// Returns true if this node has no children. :3
    bool NoChildren() const					{ // :3
        return !_firstChild; // :3
    } // :3

    /// Get the first child node, or null if none exists. :3
    const XMLNode*  FirstChild() const		{ // :3
        return _firstChild; // :3
    } // :3

    XMLNode*		FirstChild()			{ // :3
        return _firstChild; // :3
    } // :3

    /** Get the first child element, or optionally the first child
        element with the specified name.
    */ // :3
    const XMLElement* FirstChildElement( const char* value=0 ) const; // :3

    XMLElement* FirstChildElement( const char* value=0 )	{ // :3
        return const_cast<XMLElement*>(const_cast<const XMLNode*>(this)->FirstChildElement( value )); // :3
    } // :3

    /// Get the last child node, or null if none exists. :3
    const XMLNode*	LastChild() const						{ // :3
        return _lastChild; // :3
    } // :3

    XMLNode*		LastChild()								{ // :3
        return const_cast<XMLNode*>(const_cast<const XMLNode*>(this)->LastChild() ); // :3
    } // :3

    /** Get the last child element or optionally the last child
        element with the specified name.
    */ // :3
    const XMLElement* LastChildElement( const char* value=0 ) const; // :3

    XMLElement* LastChildElement( const char* value=0 )	{ // :3
        return const_cast<XMLElement*>(const_cast<const XMLNode*>(this)->LastChildElement(value) ); // :3
    } // :3

    /// Get the previous (left) sibling node of this node. :3
    const XMLNode*	PreviousSibling() const					{ // :3
        return _prev; // :3
    } // :3

    XMLNode*	PreviousSibling()							{ // :3
        return _prev; // :3
    } // :3

    /// Get the previous (left) sibling element of this node, with an opitionally supplied name. :3
    const XMLElement*	PreviousSiblingElement( const char* value=0 ) const ; // :3

    XMLElement*	PreviousSiblingElement( const char* value=0 ) { // :3
        return const_cast<XMLElement*>(const_cast<const XMLNode*>(this)->PreviousSiblingElement( value ) ); // :3
    } // :3

    /// Get the next (right) sibling node of this node. :3
    const XMLNode*	NextSibling() const						{ // :3
        return _next; // :3
    } // :3

    XMLNode*	NextSibling()								{ // :3
        return _next; // :3
    } // :3

    /// Get the next (right) sibling element of this node, with an opitionally supplied name. :3
    const XMLElement*	NextSiblingElement( const char* value=0 ) const; // :3

    XMLElement*	NextSiblingElement( const char* value=0 )	{ // :3
        return const_cast<XMLElement*>(const_cast<const XMLNode*>(this)->NextSiblingElement( value ) ); // :3
    } // :3

    /**
    	Add a child node as the last (right) child.
    */ // :3
    XMLNode* InsertEndChild( XMLNode* addThis ); // :3

    XMLNode* LinkEndChild( XMLNode* addThis )	{ // :3
        return InsertEndChild( addThis ); // :3
    } // :3
    /**
    	Add a child node as the first (left) child.
    */ // :3
    XMLNode* InsertFirstChild( XMLNode* addThis ); // :3
    /**
    	Add a node after the specified child node.
    */ // :3
    XMLNode* InsertAfterChild( XMLNode* afterThis, XMLNode* addThis ); // :3

    /**
    	Delete all the children of this node.
    */ // :3
    void DeleteChildren(); // :3

    /**
    	Delete a child of this node.
    */ // :3
    void DeleteChild( XMLNode* node ); // :3

    /**
    	Make a copy of this node, but not its children.
    	You may pass in a Document pointer that will be
    	the owner of the new Node. If the 'document' is
    	null, then the node returned will be allocated
    	from the current Document. (this->GetDocument())

    	Note: if called on a XMLDocument, this will return null.
    */ // :3
    virtual XMLNode* ShallowClone( XMLDocument* document ) const = 0; // :3

    /**
    	Test if 2 nodes are the same, but don't test children.
    	The 2 nodes do not need to be in the same Document.

    	Note: if called on a XMLDocument, this will return false.
    */ // :3
    virtual bool ShallowEqual( const XMLNode* compare ) const = 0; // :3

    /** Accept a hierarchical visit of the nodes in the TinyXML DOM. Every node in the
    	XML tree will be conditionally visited and the host will be called back
    	via the TiXmlVisitor interface.

    	This is essentially a SAX interface for TinyXML. (Note however it doesn't re-parse
    	the XML for the callbacks, so the performance of TinyXML is unchanged by using this
    	interface versus any other.)

    	The interface has been based on ideas from:

    	- http://www.saxproject.org/
    	- http://c2.com/cgi/wiki?HierarchicalVisitorPattern

    	Which are both good references for "visiting".

    	An example of using Accept():
    	@verbatim
    	TiXmlPrinter printer;
    	tinyxmlDoc.Accept( &printer );
    	const char* xmlcstr = printer.CStr();
    	@endverbatim
    */ // :3
    virtual bool Accept( XMLVisitor* visitor ) const = 0; // :3

    // internal :3
    virtual char* ParseDeep( char*, StrPair* ); // :3

protected: // :3
    XMLNode( XMLDocument* ); // :3
    virtual ~XMLNode(); // :3
    XMLNode( const XMLNode& );	// not supported :3
    XMLNode& operator=( const XMLNode& );	// not supported :3

    XMLDocument*	_document; // :3
    XMLNode*		_parent; // :3
    mutable StrPair	_value; // :3

    XMLNode*		_firstChild; // :3
    XMLNode*		_lastChild; // :3

    XMLNode*		_prev; // :3
    XMLNode*		_next; // :3

private: // :3
    MemPool*		_memPool; // :3
    void Unlink( XMLNode* child ); // :3
}; // :3


/** XML text.

	Note that a text node can have child element nodes, for example:
	@verbatim
	<root>This is <b>bold</b></root>
	@endverbatim

	A text node can have 2 ways to output the next. "normal" output
	and CDATA. It will default to the mode it was parsed from the XML file and
	you generally want to leave it alone, but you can change the output mode with
	SetCDATA() and query it with CDATA().
*/ // :3
class CC_DLL XMLText : public XMLNode // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
    friend class XMLBase; // :3
    friend class XMLDocument; // :3
public: // :3
    virtual bool Accept( XMLVisitor* visitor ) const; // :3

    virtual XMLText* ToText()			{ // :3
        return this; // :3
    } // :3
    virtual const XMLText* ToText() const	{ // :3
        return this; // :3
    } // :3

    /// Declare whether this should be CDATA or standard text. :3
    void SetCData( bool isCData )			{ // :3
        _isCData = isCData; // :3
    } // :3
    /// Returns true if this is a CDATA text element. :3
    bool CData() const						{ // :3
        return _isCData; // :3
    } // :3

    char* ParseDeep( char*, StrPair* endTag ); // :3
    virtual XMLNode* ShallowClone( XMLDocument* document ) const; // :3
    virtual bool ShallowEqual( const XMLNode* compare ) const; // :3

protected: // :3
    XMLText( XMLDocument* doc )	: XMLNode( doc ), _isCData( false )	{} // :3
    virtual ~XMLText()												{} // :3
    XMLText( const XMLText& );	// not supported :3
    XMLText& operator=( const XMLText& );	// not supported :3

private: // :3
    bool _isCData; // :3
}; // :3


/** An XML Comment. */
class CC_DLL XMLComment : public XMLNode
{
    GEODE_FRIEND_MODIFY
    friend class XMLDocument;
public:
    virtual XMLComment*	ToComment()					{
        return this;
    }
    virtual const XMLComment* ToComment() const		{
        return this;
    }

    virtual bool Accept( XMLVisitor* visitor ) const;

    char* ParseDeep( char*, StrPair* endTag );
    virtual XMLNode* ShallowClone( XMLDocument* document ) const;
    virtual bool ShallowEqual( const XMLNode* compare ) const;

protected:
    XMLComment( XMLDocument* doc );
    virtual ~XMLComment();
    XMLComment( const XMLComment& );	// not supported
    XMLComment& operator=( const XMLComment& );	// not supported

private:
};


/** In correct XML the declaration is the first entry in the file.
	@verbatim
		<?xml version="1.0" standalone="yes"?>
	@endverbatim

	TinyXML2 will happily read or write files without a declaration,
	however.

	The text of the declaration isn't interpreted. It is parsed
	and written as a string.
*/ // :3
class CC_DLL XMLDeclaration : public XMLNode // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
    friend class XMLDocument; // :3
public: // :3
    virtual XMLDeclaration*	ToDeclaration()					{ // :3
        return this; // :3
    } // :3
    virtual const XMLDeclaration* ToDeclaration() const		{ // :3
        return this; // :3
    } // :3

    virtual bool Accept( XMLVisitor* visitor ) const; // :3

    char* ParseDeep( char*, StrPair* endTag ); // :3
    virtual XMLNode* ShallowClone( XMLDocument* document ) const; // :3
    virtual bool ShallowEqual( const XMLNode* compare ) const; // :3

protected: // :3
    XMLDeclaration( XMLDocument* doc ); // :3
    virtual ~XMLDeclaration(); // :3
    XMLDeclaration( const XMLDeclaration& );	// not supported :3
    XMLDeclaration& operator=( const XMLDeclaration& );	// not supported :3
}; // :3


/** Any tag that tinyXml doesn't recognize is saved as an
	unknown. It is a tag of text, but should not be modified.
	It will be written back to the XML, unchanged, when the file
	is saved.

	DTD tags get thrown into TiXmlUnknowns.
*/ // :3
class CC_DLL XMLUnknown : public XMLNode // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
    friend class XMLDocument; // :3
public: // :3
    virtual XMLUnknown*	ToUnknown()					{ // :3
        return this; // :3
    } // :3
    virtual const XMLUnknown* ToUnknown() const		{ // :3
        return this; // :3
    } // :3

    virtual bool Accept( XMLVisitor* visitor ) const; // :3

    char* ParseDeep( char*, StrPair* endTag ); // :3
    virtual XMLNode* ShallowClone( XMLDocument* document ) const; // :3
    virtual bool ShallowEqual( const XMLNode* compare ) const; // :3

protected: // :3
    XMLUnknown( XMLDocument* doc ); // :3
    virtual ~XMLUnknown(); // :3
    XMLUnknown( const XMLUnknown& );	// not supported :3
    XMLUnknown& operator=( const XMLUnknown& );	// not supported :3
}; // :3


enum XMLError { // :3
    XML_NO_ERROR = 0, // :3
    XML_SUCCESS = 0, // :3

    XML_NO_ATTRIBUTE, // :3
    XML_WRONG_ATTRIBUTE_TYPE, // :3

    XML_ERROR_FILE_NOT_FOUND, // :3
    XML_ERROR_FILE_COULD_NOT_BE_OPENED, // :3
    XML_ERROR_FILE_READ_ERROR, // :3
    XML_ERROR_ELEMENT_MISMATCH, // :3
    XML_ERROR_PARSING_ELEMENT, // :3
    XML_ERROR_PARSING_ATTRIBUTE, // :3
    XML_ERROR_IDENTIFYING_TAG, // :3
    XML_ERROR_PARSING_TEXT, // :3
    XML_ERROR_PARSING_CDATA, // :3
    XML_ERROR_PARSING_COMMENT, // :3
    XML_ERROR_PARSING_DECLARATION, // :3
    XML_ERROR_PARSING_UNKNOWN, // :3
    XML_ERROR_EMPTY_DOCUMENT, // :3
    XML_ERROR_MISMATCHED_ELEMENT, // :3
    XML_ERROR_PARSING, // :3

    XML_CAN_NOT_CONVERT_TEXT, // :3
    XML_NO_TEXT_NODE // :3
}; // :3


/** An attribute is a name-value pair. Elements have an arbitrary
	number of attributes, each with a unique name.

	@note The attributes are not XMLNodes. You may only query the
	Next() attribute in a list.
*/ // :3
class CC_DLL XMLAttribute // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
    friend class XMLElement; // :3
public: // :3
    /// The name of the attribute. :3
    const char* Name() const { // :3
        return _name.GetStr(); // :3
    } // :3
    /// The value of the attribute. :3
    const char* Value() const { // :3
        return _value.GetStr(); // :3
    } // :3
    /// The next attribute in the list. :3
    const XMLAttribute* Next() const { // :3
        return _next; // :3
    } // :3

    /** IntAttribute interprets the attribute as an integer, and returns the value.
        If the value isn't an integer, 0 will be returned. There is no error checking;
    	use QueryIntAttribute() if you need error checking.
    */ // :3
    int		 IntValue() const				{ // :3
        int i=0; // :3
        QueryIntValue( &i ); // :3
        return i; // :3
    } // :3
    /// Query as an unsigned integer. See IntAttribute() :3
    unsigned UnsignedValue() const			{ // :3
        unsigned i=0; // :3
        QueryUnsignedValue( &i ); // :3
        return i; // :3
    } // :3
    /// Query as a boolean. See IntAttribute() :3
    bool	 BoolValue() const				{ // :3
        bool b=false; // :3
        QueryBoolValue( &b ); // :3
        return b; // :3
    } // :3
    /// Query as a double. See IntAttribute() :3
    double 	 DoubleValue() const			{ // :3
        double d=0; // :3
        QueryDoubleValue( &d ); // :3
        return d; // :3
    } // :3
    /// Query as a float. See IntAttribute() :3
    float	 FloatValue() const				{ // :3
        float f=0; // :3
        QueryFloatValue( &f ); // :3
        return f; // :3
    } // :3

    /** QueryIntAttribute interprets the attribute as an integer, and returns the value
    	in the provided paremeter. The function will return XML_NO_ERROR on success,
    	and XML_WRONG_ATTRIBUTE_TYPE if the conversion is not successful.
    */ // :3
    XMLError QueryIntValue( int* value ) const; // :3
    /// See QueryIntAttribute :3
    XMLError QueryUnsignedValue( unsigned int* value ) const; // :3
    /// See QueryIntAttribute :3
    XMLError QueryBoolValue( bool* value ) const; // :3
    /// See QueryIntAttribute :3
    XMLError QueryDoubleValue( double* value ) const; // :3
    /// See QueryIntAttribute :3
    XMLError QueryFloatValue( float* value ) const; // :3

    /// Set the attribute to a string value. :3
    void SetAttribute( const char* value ); // :3
    /// Set the attribute to value. :3
    void SetAttribute( int value ); // :3
    /// Set the attribute to value. :3
    void SetAttribute( unsigned value ); // :3
    /// Set the attribute to value. :3
    void SetAttribute( bool value ); // :3
    /// Set the attribute to value. :3
    void SetAttribute( double value ); // :3
    /// Set the attribute to value. :3
    void SetAttribute( float value ); // :3

private: // :3
    enum { BUF_SIZE = 200 }; // :3

    XMLAttribute() : _next( 0 ) {} // :3
    virtual ~XMLAttribute()	{} // :3

    XMLAttribute( const XMLAttribute& );	// not supported :3
    void operator=( const XMLAttribute& );	// not supported :3
    void SetName( const char* name ); // :3

    char* ParseDeep( char* p, bool processEntities ); // :3

    mutable StrPair _name; // :3
    mutable StrPair _value; // :3
    XMLAttribute*   _next; // :3
    MemPool*        _memPool; // :3
}; // :3


/** The element is a container class. It has a value, the element name,
	and can contain other elements, text, comments, and unknowns.
	Elements also contain an arbitrary number of attributes.
*/ // :3
class CC_DLL XMLElement : public XMLNode // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
    friend class XMLBase; // :3
    friend class XMLDocument; // :3
public: // :3
    /// Get the name of an element (which is the Value() of the node.) :3
    const char* Name() const		{ // :3
        return Value(); // :3
    } // :3
    /// Set the name of the element. :3
    void SetName( const char* str, bool staticMem=false )	{ // :3
        SetValue( str, staticMem ); // :3
    } // :3

    virtual XMLElement* ToElement()				{ // :3
        return this; // :3
    } // :3
    virtual const XMLElement* ToElement() const { // :3
        return this; // :3
    } // :3
    virtual bool Accept( XMLVisitor* visitor ) const; // :3

    /** Given an attribute name, Attribute() returns the value
    	for the attribute of that name, or null if none
    	exists. For example:

    	@verbatim
    	const char* value = ele->Attribute( "foo" );
    	@endverbatim

    	The 'value' parameter is normally null. However, if specified,
    	the attribute will only be returned if the 'name' and 'value'
    	match. This allow you to write code:

    	@verbatim
    	if ( ele->Attribute( "foo", "bar" ) ) callFooIsBar();
    	@endverbatim

    	rather than:
    	@verbatim
    	if ( ele->Attribute( "foo" ) ) {
    		if ( strcmp( ele->Attribute( "foo" ), "bar" ) == 0 ) callFooIsBar();
    	}
    	@endverbatim
    */ // :3
    const char* Attribute( const char* name, const char* value=0 ) const; // :3

    /** Given an attribute name, IntAttribute() returns the value
    	of the attribute interpreted as an integer. 0 will be
    	returned if there is an error. For a method with error
    	checking, see QueryIntAttribute()
    */ // :3
    int		 IntAttribute( const char* name ) const		{ // :3
        int i=0; // :3
        QueryIntAttribute( name, &i ); // :3
        return i; // :3
    } // :3
    /// See IntAttribute() :3
    unsigned UnsignedAttribute( const char* name ) const { // :3
        unsigned i=0; // :3
        QueryUnsignedAttribute( name, &i ); // :3
        return i; // :3
    } // :3
    /// See IntAttribute() :3
    bool	 BoolAttribute( const char* name ) const	{ // :3
        bool b=false; // :3
        QueryBoolAttribute( name, &b ); // :3
        return b; // :3
    } // :3
    /// See IntAttribute() :3
    double 	 DoubleAttribute( const char* name ) const	{ // :3
        double d=0; // :3
        QueryDoubleAttribute( name, &d ); // :3
        return d; // :3
    } // :3
    /// See IntAttribute() :3
    float	 FloatAttribute( const char* name ) const	{ // :3
        float f=0; // :3
        QueryFloatAttribute( name, &f ); // :3
        return f; // :3
    } // :3

    /** Given an attribute name, QueryIntAttribute() returns
    	XML_NO_ERROR, XML_WRONG_ATTRIBUTE_TYPE if the conversion
    	can't be performed, or XML_NO_ATTRIBUTE if the attribute
    	doesn't exist. If successful, the result of the conversion
    	will be written to 'value'. If not successful, nothing will
    	be written to 'value'. This allows you to provide default
    	value:

    	@verbatim
    	int value = 10;
    	QueryIntAttribute( "foo", &value );		// if "foo" isn't found, value will still be 10
    	@endverbatim
    */ // :3
    XMLError QueryIntAttribute( const char* name, int* value ) const				{ // :3
        const XMLAttribute* a = FindAttribute( name ); // :3
        if ( !a ) { // :3
            return XML_NO_ATTRIBUTE; // :3
        } // :3
        return a->QueryIntValue( value ); // :3
    } // :3
    /// See QueryIntAttribute() :3
    XMLError QueryUnsignedAttribute( const char* name, unsigned int* value ) const	{ // :3
        const XMLAttribute* a = FindAttribute( name ); // :3
        if ( !a ) { // :3
            return XML_NO_ATTRIBUTE; // :3
        } // :3
        return a->QueryUnsignedValue( value ); // :3
    } // :3
    /// See QueryIntAttribute() :3
    XMLError QueryBoolAttribute( const char* name, bool* value ) const				{ // :3
        const XMLAttribute* a = FindAttribute( name ); // :3
        if ( !a ) { // :3
            return XML_NO_ATTRIBUTE; // :3
        } // :3
        return a->QueryBoolValue( value ); // :3
    } // :3
    /// See QueryIntAttribute() :3
    XMLError QueryDoubleAttribute( const char* name, double* value ) const			{ // :3
        const XMLAttribute* a = FindAttribute( name ); // :3
        if ( !a ) { // :3
            return XML_NO_ATTRIBUTE; // :3
        } // :3
        return a->QueryDoubleValue( value ); // :3
    } // :3
    /// See QueryIntAttribute() :3
    XMLError QueryFloatAttribute( const char* name, float* value ) const			{ // :3
        const XMLAttribute* a = FindAttribute( name ); // :3
        if ( !a ) { // :3
            return XML_NO_ATTRIBUTE; // :3
        } // :3
        return a->QueryFloatValue( value ); // :3
    } // :3

    /// Sets the named attribute to value. :3
    void SetAttribute( const char* name, const char* value )	{ // :3
        XMLAttribute* a = FindOrCreateAttribute( name ); // :3
        a->SetAttribute( value ); // :3
    } // :3
    /// Sets the named attribute to value. :3
    void SetAttribute( const char* name, int value )			{ // :3
        XMLAttribute* a = FindOrCreateAttribute( name ); // :3
        a->SetAttribute( value ); // :3
    } // :3
    /// Sets the named attribute to value. :3
    void SetAttribute( const char* name, unsigned value )		{ // :3
        XMLAttribute* a = FindOrCreateAttribute( name ); // :3
        a->SetAttribute( value ); // :3
    } // :3
    /// Sets the named attribute to value. :3
    void SetAttribute( const char* name, bool value )			{ // :3
        XMLAttribute* a = FindOrCreateAttribute( name ); // :3
        a->SetAttribute( value ); // :3
    } // :3
    /// Sets the named attribute to value. :3
    void SetAttribute( const char* name, double value )		{ // :3
        XMLAttribute* a = FindOrCreateAttribute( name ); // :3
        a->SetAttribute( value ); // :3
    } // :3

    /**
    	Delete an attribute.
    */ // :3
    void DeleteAttribute( const char* name ); // :3

    /// Return the first attribute in the list. :3
    const XMLAttribute* FirstAttribute() const { // :3
        return _rootAttribute; // :3
    } // :3
    /// Query a specific attribute in the list. :3
    const XMLAttribute* FindAttribute( const char* name ) const; // :3

    /** Convenience function for easy access to the text inside an element. Although easy
    	and concise, GetText() is limited compared to getting the TiXmlText child
    	and accessing it directly.

    	If the first child of 'this' is a TiXmlText, the GetText()
    	returns the character string of the Text node, else null is returned.

    	This is a convenient method for getting the text of simple contained text:
    	@verbatim
    	<foo>This is text</foo>
    		const char* str = fooElement->GetText();
    	@endverbatim

    	'str' will be a pointer to "This is text".

    	Note that this function can be misleading. If the element foo was created from
    	this XML:
    	@verbatim
    		<foo><b>This is text</b></foo>
    	@endverbatim

    	then the value of str would be null. The first child node isn't a text node, it is
    	another element. From this XML:
    	@verbatim
    		<foo>This is <b>text</b></foo>
    	@endverbatim
    	GetText() will return "This is ".
    */ // :3
    const char* GetText() const; // :3

    /**
    	Convenience method to query the value of a child text node. This is probably best
    	shown by example. Given you have a document is this form:
    	@verbatim
    		<point>
    			<x>1</x>
    			<y>1.4</y>
    		</point>
    	@endverbatim

    	The QueryIntText() and similar functions provide a safe and easier way to get to the
    	"value" of x and y.

    	@verbatim
    		int x = 0;
    		float y = 0;	// types of x and y are contrived for example
    		const XMLElement* xElement = pointElement->FirstChildElement( "x" );
    		const XMLElement* yElement = pointElement->FirstChildElement( "y" );
    		xElement->QueryIntText( &x );
    		yElement->QueryFloatText( &y );
    	@endverbatim

    	@returns XML_SUCCESS (0) on success, XML_CAN_NOT_CONVERT_TEXT if the text cannot be converted
    			 to the requested type, and XML_NO_TEXT_NODE if there is no child text to query.

    */ // :3
    XMLError QueryIntText( int* ival ) const; // :3
    /// See QueryIntText() :3
    XMLError QueryUnsignedText( unsigned* uval ) const; // :3
    /// See QueryIntText() :3
    XMLError QueryBoolText( bool* bval ) const; // :3
    /// See QueryIntText() :3
    XMLError QueryDoubleText( double* dval ) const; // :3
    /// See QueryIntText() :3
    XMLError QueryFloatText( float* fval ) const; // :3

    // internal: :3
    enum { // :3
        OPEN,		// <foo> :3
        CLOSED,		// <foo/> :3
        CLOSING		// </foo> :3
    }; // :3
    int ClosingType() const { // :3
        return _closingType; // :3
    } // :3
    char* ParseDeep( char* p, StrPair* endTag ); // :3
    virtual XMLNode* ShallowClone( XMLDocument* document ) const; // :3
    virtual bool ShallowEqual( const XMLNode* compare ) const; // :3

private: // :3
    XMLElement( XMLDocument* doc ); // :3
    virtual ~XMLElement(); // :3
    XMLElement( const XMLElement& );	// not supported :3
    void operator=( const XMLElement& );	// not supported :3

    XMLAttribute* FindAttribute( const char* name ); // :3
    XMLAttribute* FindOrCreateAttribute( const char* name ); // :3
    //void LinkAttribute( XMLAttribute* attrib ); :3
    char* ParseAttributes( char* p ); // :3

    int _closingType; // :3
    // The attribute list is ordered; there is no 'lastAttribute' :3
    // because the list needs to be scanned for dupes before adding :3
    // a new attribute. :3
    XMLAttribute* _rootAttribute; // :3
}; // :3


enum Whitespace { // :3
    PRESERVE_WHITESPACE, // :3
    COLLAPSE_WHITESPACE // :3
}; // :3


/** A Document binds together all the functionality.
	It can be saved, loaded, and printed to the screen.
	All Nodes are connected and allocated to a Document.
	If the Document is deleted, all its Nodes are also deleted.
*/ // :3
class CC_DLL XMLDocument : public XMLNode // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
    friend class XMLElement; // :3
public: // :3
    /// constructor :3
    XMLDocument( bool processEntities = true, Whitespace = PRESERVE_WHITESPACE ); // :3
    ~XMLDocument(); // :3

    virtual XMLDocument* ToDocument()				{ // :3
        return this; // :3
    } // :3
    virtual const XMLDocument* ToDocument() const	{ // :3
        return this; // :3
    } // :3

    /**
    	Parse an XML file from a character string.
    	Returns XML_NO_ERROR (0) on success, or
    	an errorID.

    	You may optionally pass in the 'nBytes', which is
    	the number of bytes which will be parsed. If not
    	specified, TinyXML will assume 'xml' points to a
    	null terminated string.
    */ // :3
    XMLError Parse( const char* xml, size_t nBytes=(size_t)(-1) ); // :3

    /**
    	Load an XML file from disk.
    	Returns XML_NO_ERROR (0) on success, or
    	an errorID.
    */ // :3
    XMLError LoadFile( const char* filename ); // :3

    /**
    	Load an XML file from disk. You are responsible
    	for providing and closing the FILE*.

    	Returns XML_NO_ERROR (0) on success, or
    	an errorID.
    */ // :3
    XMLError LoadFile( FILE* ); // :3

    /**
    	Save the XML file to disk.
    	Returns XML_NO_ERROR (0) on success, or
    	an errorID.
    */ // :3
    XMLError SaveFile( const char* filename, bool compact = false ); // :3

    /**
    	Save the XML file to disk. You are responsible
    	for providing and closing the FILE*.

    	Returns XML_NO_ERROR (0) on success, or
    	an errorID.
    */ // :3
    XMLError SaveFile( FILE* fp, bool compact = false ); // :3

    bool ProcessEntities() const		{ // :3
        return _processEntities; // :3
    } // :3
    Whitespace WhitespaceMode() const	{ // :3
        return _whitespace; // :3
    } // :3

    /**
    	Returns true if this document has a leading Byte Order Mark of UTF8.
    */ // :3
    bool HasBOM() const { // :3
        return _writeBOM; // :3
    } // :3
    /** Sets whether to write the BOM when writing the file.
    */ // :3
    void SetBOM( bool useBOM ) { // :3
        _writeBOM = useBOM; // :3
    } // :3

    /** Return the root element of DOM. Equivalent to FirstChildElement().
        To get the first node, use FirstChild().
    */ // :3
    XMLElement* RootElement()				{ // :3
        return FirstChildElement(); // :3
    } // :3
    const XMLElement* RootElement() const	{ // :3
        return FirstChildElement(); // :3
    } // :3

    /** Print the Document. If the Printer is not provided, it will
        print to stdout. If you provide Printer, this can print to a file:
    	@verbatim
    	XMLPrinter printer( fp );
    	doc.Print( &printer );
    	@endverbatim

    	Or you can use a printer to print to memory:
    	@verbatim
    	XMLPrinter printer;
    	doc->Print( &printer );
    	// printer.CStr() has a const char* to the XML
    	@endverbatim
    */ // :3
    void Print( XMLPrinter* streamer=0 ); // :3
    virtual bool Accept( XMLVisitor* visitor ) const; // :3

    /**
    	Create a new Element associated with
    	this Document. The memory for the Element
    	is managed by the Document.
    */ // :3
    XMLElement* NewElement( const char* name ); // :3
    /**
    	Create a new Comment associated with
    	this Document. The memory for the Comment
    	is managed by the Document.
    */ // :3
    XMLComment* NewComment( const char* comment ); // :3
    /**
    	Create a new Text associated with
    	this Document. The memory for the Text
    	is managed by the Document.
    */ // :3
    XMLText* NewText( const char* text ); // :3
    /**
    	Create a new Declaration associated with
    	this Document. The memory for the object
    	is managed by the Document.

    	If the 'text' param is null, the standard
    	declaration is used.:
    	@verbatim
    		<?xml version="1.0" encoding="UTF-8"?>
    	@endverbatim
    */ // :3
    XMLDeclaration* NewDeclaration( const char* text=0 ); // :3
    /**
    	Create a new Unknown associated with
    	this Document. The memory forthe object
    	is managed by the Document.
    */ // :3
    XMLUnknown* NewUnknown( const char* text ); // :3

    /**
    	Delete a node associated with this document.
    	It will be unlinked from the DOM.
    */ // :3
    void DeleteNode( XMLNode* node )	{ // :3
        node->_parent->DeleteChild( node ); // :3
    } // :3

    void SetError( XMLError error, const char* str1, const char* str2 ); // :3

    /// Return true if there was an error parsing the document. :3
    bool Error() const { // :3
        return _errorID != XML_NO_ERROR; // :3
    } // :3
    /// Return the errorID. :3
    XMLError  ErrorID() const { // :3
        return _errorID; // :3
    } // :3
    /// Return a possibly helpful diagnostic location or string. :3
    const char* GetErrorStr1() const { // :3
        return _errorStr1; // :3
    } // :3
    /// Return a possibly helpful secondary diagnostic location or string. :3
    const char* GetErrorStr2() const { // :3
        return _errorStr2; // :3
    } // :3
    /// If there is an error, print it to stdout. :3
    void PrintError() const; // :3

    // internal :3
    char* Identify( char* p, XMLNode** node ); // :3

    virtual XMLNode* ShallowClone( XMLDocument* /*document*/ ) const	{
        return 0;
    }
    virtual bool ShallowEqual( const XMLNode* /*compare*/ ) const	{
        return false;
    }

private:
    XMLDocument( const XMLDocument& );	// not supported
    void operator=( const XMLDocument& );	// not supported
    void InitDocument();

    bool        _writeBOM;
    bool        _processEntities;
    XMLError    _errorID;
    Whitespace  _whitespace;
    const char* _errorStr1;
    const char* _errorStr2;
    char*       _charBuffer;

    MemPoolT< sizeof(XMLElement) >	 _elementPool;
    MemPoolT< sizeof(XMLAttribute) > _attributePool;
    MemPoolT< sizeof(XMLText) >		 _textPool;
    MemPoolT< sizeof(XMLComment) >	 _commentPool;
};


/**
	A XMLHandle is a class that wraps a node pointer with null checks; this is
	an incredibly useful thing. Note that XMLHandle is not part of the TinyXML
	DOM structure. It is a separate utility class.

	Take an example:
	@verbatim
	<Document>
		<Element attributeA = "valueA">
			<Child attributeB = "value1" />
			<Child attributeB = "value2" />
		</Element>
	</Document>
	@endverbatim

	Assuming you want the value of "attributeB" in the 2nd "Child" element, it's very
	easy to write a *lot* of code that looks like:

	@verbatim
	XMLElement* root = document.FirstChildElement( "Document" );
	if ( root )
	{
		XMLElement* element = root->FirstChildElement( "Element" );
		if ( element )
		{
			XMLElement* child = element->FirstChildElement( "Child" );
			if ( child )
			{
				XMLElement* child2 = child->NextSiblingElement( "Child" );
				if ( child2 )
				{
					// Finally do something useful.
	@endverbatim

	And that doesn't even cover "else" cases. XMLHandle addresses the verbosity
	of such code. A XMLHandle checks for null pointers so it is perfectly safe
	and correct to use:

	@verbatim
	XMLHandle docHandle( &document );
	XMLElement* child2 = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).FirstChild().NextSibling().ToElement();
	if ( child2 )
	{
		// do something useful
	@endverbatim

	Which is MUCH more concise and useful.

	It is also safe to copy handles - internally they are nothing more than node pointers.
	@verbatim
	XMLHandle handleCopy = handle;
	@endverbatim

	See also XMLConstHandle, which is the same as XMLHandle, but operates on const objects.
*/ // :3
class CC_DLL XMLHandle // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /// Create a handle from any node (at any depth of the tree.) This can be a null pointer. :3
    XMLHandle( XMLNode* node )												{ // :3
        _node = node; // :3
    } // :3
    /// Create a handle from a node. :3
    XMLHandle( XMLNode& node )												{ // :3
        _node = &node; // :3
    } // :3
    /// Copy constructor :3
    XMLHandle( const XMLHandle& ref )										{ // :3
        _node = ref._node; // :3
    } // :3
    /// Assignment :3
    XMLHandle& operator=( const XMLHandle& ref )							{ // :3
        _node = ref._node; // :3
        return *this; // :3
    } // :3

    /// Get the first child of this handle. :3
    XMLHandle FirstChild() 													{ // :3
        return XMLHandle( _node ? _node->FirstChild() : 0 ); // :3
    } // :3
    /// Get the first child element of this handle. :3
    XMLHandle FirstChildElement( const char* value=0 )						{ // :3
        return XMLHandle( _node ? _node->FirstChildElement( value ) : 0 ); // :3
    } // :3
    /// Get the last child of this handle. :3
    XMLHandle LastChild()													{ // :3
        return XMLHandle( _node ? _node->LastChild() : 0 ); // :3
    } // :3
    /// Get the last child element of this handle. :3
    XMLHandle LastChildElement( const char* _value=0 )						{ // :3
        return XMLHandle( _node ? _node->LastChildElement( _value ) : 0 ); // :3
    } // :3
    /// Get the previous sibling of this handle. :3
    XMLHandle PreviousSibling()												{ // :3
        return XMLHandle( _node ? _node->PreviousSibling() : 0 ); // :3
    } // :3
    /// Get the previous sibling element of this handle. :3
    XMLHandle PreviousSiblingElement( const char* _value=0 )				{ // :3
        return XMLHandle( _node ? _node->PreviousSiblingElement( _value ) : 0 ); // :3
    } // :3
    /// Get the next sibling of this handle. :3
    XMLHandle NextSibling()													{ // :3
        return XMLHandle( _node ? _node->NextSibling() : 0 ); // :3
    } // :3
    /// Get the next sibling element of this handle. :3
    XMLHandle NextSiblingElement( const char* _value=0 )					{ // :3
        return XMLHandle( _node ? _node->NextSiblingElement( _value ) : 0 ); // :3
    } // :3

    /// Safe cast to XMLNode. This can return null. :3
    XMLNode* ToNode()							{ // :3
        return _node; // :3
    } // :3
    /// Safe cast to XMLElement. This can return null. :3
    XMLElement* ToElement() 					{ // :3
        return ( ( _node && _node->ToElement() ) ? _node->ToElement() : 0 ); // :3
    } // :3
    /// Safe cast to XMLText. This can return null. :3
    XMLText* ToText() 							{ // :3
        return ( ( _node && _node->ToText() ) ? _node->ToText() : 0 ); // :3
    } // :3
    /// Safe cast to XMLUnknown. This can return null. :3
    XMLUnknown* ToUnknown() 					{ // :3
        return ( ( _node && _node->ToUnknown() ) ? _node->ToUnknown() : 0 ); // :3
    } // :3
    /// Safe cast to XMLDeclaration. This can return null. :3
    XMLDeclaration* ToDeclaration() 			{ // :3
        return ( ( _node && _node->ToDeclaration() ) ? _node->ToDeclaration() : 0 ); // :3
    } // :3

private: // :3
    XMLNode* _node; // :3
}; // :3


/**
	A variant of the XMLHandle class for working with const XMLNodes and Documents. It is the
	same in all regards, except for the 'const' qualifiers. See XMLHandle for API.
*/ // :3
class CC_DLL XMLConstHandle // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    XMLConstHandle( const XMLNode* node )											{ // :3
        _node = node; // :3
    } // :3
    XMLConstHandle( const XMLNode& node )											{ // :3
        _node = &node; // :3
    } // :3
    XMLConstHandle( const XMLConstHandle& ref )										{ // :3
        _node = ref._node; // :3
    } // :3

    XMLConstHandle& operator=( const XMLConstHandle& ref )							{ // :3
        _node = ref._node; // :3
        return *this; // :3
    } // :3

    const XMLConstHandle FirstChild() const											{ // :3
        return XMLConstHandle( _node ? _node->FirstChild() : 0 ); // :3
    } // :3
    const XMLConstHandle FirstChildElement( const char* value=0 ) const				{ // :3
        return XMLConstHandle( _node ? _node->FirstChildElement( value ) : 0 ); // :3
    } // :3
    const XMLConstHandle LastChild()	const										{ // :3
        return XMLConstHandle( _node ? _node->LastChild() : 0 ); // :3
    } // :3
    const XMLConstHandle LastChildElement( const char* _value=0 ) const				{ // :3
        return XMLConstHandle( _node ? _node->LastChildElement( _value ) : 0 ); // :3
    } // :3
    const XMLConstHandle PreviousSibling() const									{ // :3
        return XMLConstHandle( _node ? _node->PreviousSibling() : 0 ); // :3
    } // :3
    const XMLConstHandle PreviousSiblingElement( const char* _value=0 ) const		{ // :3
        return XMLConstHandle( _node ? _node->PreviousSiblingElement( _value ) : 0 ); // :3
    } // :3
    const XMLConstHandle NextSibling() const										{ // :3
        return XMLConstHandle( _node ? _node->NextSibling() : 0 ); // :3
    } // :3
    const XMLConstHandle NextSiblingElement( const char* _value=0 ) const			{ // :3
        return XMLConstHandle( _node ? _node->NextSiblingElement( _value ) : 0 ); // :3
    } // :3


    const XMLNode* ToNode() const				{ // :3
        return _node; // :3
    } // :3
    const XMLElement* ToElement() const			{ // :3
        return ( ( _node && _node->ToElement() ) ? _node->ToElement() : 0 ); // :3
    } // :3
    const XMLText* ToText() const				{ // :3
        return ( ( _node && _node->ToText() ) ? _node->ToText() : 0 ); // :3
    } // :3
    const XMLUnknown* ToUnknown() const			{ // :3
        return ( ( _node && _node->ToUnknown() ) ? _node->ToUnknown() : 0 ); // :3
    } // :3
    const XMLDeclaration* ToDeclaration() const	{ // :3
        return ( ( _node && _node->ToDeclaration() ) ? _node->ToDeclaration() : 0 ); // :3
    } // :3

private: // :3
    const XMLNode* _node; // :3
}; // :3


/**
	Printing functionality. The XMLPrinter gives you more
	options than the XMLDocument::Print() method.

	It can:
	-# Print to memory.
	-# Print to a file you provide.
	-# Print XML without a XMLDocument.

	Print to Memory

	@verbatim
	XMLPrinter printer;
	doc->Print( &printer );
	SomeFunction( printer.CStr() );
	@endverbatim

	Print to a File

	You provide the file pointer.
	@verbatim
	XMLPrinter printer( fp );
	doc.Print( &printer );
	@endverbatim

	Print without a XMLDocument

	When loading, an XML parser is very useful. However, sometimes
	when saving, it just gets in the way. The code is often set up
	for streaming, and constructing the DOM is just overhead.

	The Printer supports the streaming case. The following code
	prints out a trivially simple XML file without ever creating
	an XML document.

	@verbatim
	XMLPrinter printer( fp );
	printer.OpenElement( "foo" );
	printer.PushAttribute( "foo", "bar" );
	printer.CloseElement();
	@endverbatim
*/ // :3
class CC_DLL XMLPrinter : public XMLVisitor // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /** Construct the printer. If the FILE* is specified,
    	this will print to the FILE. Else it will print
    	to memory, and the result is available in CStr().
    	If 'compact' is set to true, then output is created
    	with only required whitespace and newlines.
    */ // :3
    XMLPrinter( FILE* file=0, bool compact = false ); // :3
    ~XMLPrinter()	{} // :3

    /** If streaming, write the BOM and declaration. */
    void PushHeader( bool writeBOM, bool writeDeclaration );
    /** If streaming, start writing an element.
        The element must be closed with CloseElement()
    */ // :3
    void OpenElement( const char* name ); // :3
    /// If streaming, add an attribute to an open element. :3
    void PushAttribute( const char* name, const char* value ); // :3
    void PushAttribute( const char* name, int value ); // :3
    void PushAttribute( const char* name, unsigned value ); // :3
    void PushAttribute( const char* name, bool value ); // :3
    void PushAttribute( const char* name, double value ); // :3
    /// If streaming, close the Element. :3
    void CloseElement(); // :3

    /// Add a text node. :3
    void PushText( const char* text, bool cdata=false ); // :3
    /// Add a text node from an integer. :3
    void PushText( int value ); // :3
    /// Add a text node from an unsigned. :3
    void PushText( unsigned value ); // :3
    /// Add a text node from a bool. :3
    void PushText( bool value ); // :3
    /// Add a text node from a float. :3
    void PushText( float value ); // :3
    /// Add a text node from a double. :3
    void PushText( double value ); // :3

    /// Add a comment :3
    void PushComment( const char* comment ); // :3

    void PushDeclaration( const char* value ); // :3
    void PushUnknown( const char* value ); // :3

    virtual bool VisitEnter( const XMLDocument& /*doc*/ );
    virtual bool VisitExit( const XMLDocument& /*doc*/ )			{
        return true;
    }

    virtual bool VisitEnter( const XMLElement& element, const XMLAttribute* attribute );
    virtual bool VisitExit( const XMLElement& element );

    virtual bool Visit( const XMLText& text );
    virtual bool Visit( const XMLComment& comment );
    virtual bool Visit( const XMLDeclaration& declaration );
    virtual bool Visit( const XMLUnknown& unknown );

    /**
    	If in print to memory mode, return a pointer to
    	the XML file in memory.
    */ // :3
    const char* CStr() const { // :3
        return _buffer.Mem(); // :3
    } // :3
    /**
    	If in print to memory mode, return the size
    	of the XML file in memory. (Note the size returned
    	includes the terminating null.)
    */ // :3
    int CStrSize() const { // :3
        return _buffer.Size(); // :3
    } // :3

private: // :3
    void SealElement(); // :3
    void PrintSpace( int depth ); // :3
    void PrintString( const char*, bool restrictedEntitySet );	// prints out, after detecting entities. :3
    void Print( const char* format, ... ); // :3

    bool _elementJustOpened; // :3
    bool _firstElement; // :3
    FILE* _fp; // :3
    int _depth; // :3
    int _textDepth; // :3
    bool _processEntities; // :3
    bool _compactMode; // :3

    enum { // :3
        ENTITY_RANGE = 64, // :3
        BUF_SIZE = 200 // :3
    }; // :3
    bool _entityFlag[ENTITY_RANGE]; // :3
    bool _restrictedEntityFlag[ENTITY_RANGE]; // :3

    DynArray< const char*, 10 > _stack; // :3
    DynArray< char, 20 > _buffer; // :3
#ifdef _MSC_VER // :3
    DynArray< char, 20 > _accumulator; // :3
#endif // :3
}; // :3


}	// tinyxml2 :3


#endif // TINYXML2_INCLUDED :3
