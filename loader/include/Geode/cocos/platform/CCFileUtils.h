/****************************************************************************
Copyright (c) 2010-2013 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/ // :3
#ifndef __CC_FILEUTILS_H__ // :3
#define __CC_FILEUTILS_H__ // :3

#include <string> // :3
#include <vector> // :3
#include <map> // :3
#include "CCPlatformMacros.h" // :3
#include "../include/ccTypes.h" // :3
#include "../include/ccTypeInfo.h" // :3

NS_CC_BEGIN // :3

class CCDictionary; // :3
class CCArray; // :3
/**
 * @addtogroup platform
 * @{
 */ // :3

struct CCTexturePack { // :3
    std::string m_id; // :3
    std::vector<std::string> m_paths; // :3
}; // :3

//! @brief  Helper class to handle file operations :3
class CC_DLL CCFileUtils : public TypeInfo // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
    friend class CCArray; // :3
    friend class CCDictionary; // :3
public: // :3

    GEODE_CUSTOM_CONSTRUCTOR_BEGIN(CCFileUtils) // :3
    /**
     *  Returns an unique ID for this class.
     *  @note It's only used for JSBindings now.
     *  @return The unique ID for this class.
     *  @js NA
     *  @lua NA
     */ // :3
    virtual long getClassTypeInfo() { // :3
		static const long id = cocos2d::getHashCodeByString(typeid(cocos2d::CCFileUtils).name()); // :3
		return id; // :3
    } // :3

    /**
     *  Gets the instance of CCFileUtils.
     *  @js getInstance
     */ // :3
    static CCFileUtils* sharedFileUtils(); // :3

    static GEODE_DLL CCFileUtils* get(); // :3

    /**
     *  Destroys the instance of CCFileUtils.
     */ // :3
    static void purgeFileUtils(); // :3

    /**
     *  The destructor of CCFileUtils.
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCFileUtils(); // :3

    /**
     *  Purges the file searching cache.
     *
     *  @note It should be invoked after the resources were updated.
     *        For instance, in the CocosPlayer sample, every time you run application from CocosBuilder,
     *        All the resources will be downloaded to the writable folder, before new js app launchs,
     *        this method should be invoked to clean the file search cache.
     */ // :3
    virtual void purgeCachedEntries(); // :3

    /**
     *  Gets resource file data
     *
     *  @param[in]  pszFileName The resource file name which contains the path.
     *  @param[in]  pszMode The read mode of the file.
     *  @param[out] pSize If the file read operation succeeds, it will be the data size, otherwise 0.
     *  @return Upon success, a pointer to the data is returned, otherwise NULL.
     *  @warning Recall: you are responsible for calling delete[] on any Non-NULL pointer returned.
     *  @js NA
     */ // :3
    virtual unsigned char* getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize); // :3

    /**
     *  Gets resource file data from a zip file.
     *
     *  @param[in]  pszFileName The resource file name which contains the relative path of the zip file.
     *  @param[out] pSize If the file read operation succeeds, it will be the data size, otherwise 0.
     *  @return Upon success, a pointer to the data is returned, otherwise NULL.
     *  @warning Recall: you are responsible for calling delete[] on any Non-NULL pointer returned.
     *  @js NA
     */ // :3
    virtual unsigned char* getFileDataFromZip(const char* pszZipFilePath, const char* pszFileName, unsigned long * pSize); // :3

    /** Returns the fullpath for a given filename.

     First it will try to get a new filename from the "filenameLookup" dictionary.
     If a new filename can't be found on the dictionary, it will use the original filename.
     Then it will try to obtain the full path of the filename using the CCFileUtils search rules: resolutions, and search paths.
     The file search is based on the array element order of search paths and resolution directories.

     For instance:

     	We set two elements("/mnt/sdcard/", "internal_dir/") to search paths vector by setSearchPaths,
     	and set three elements("resources-ipadhd/", "resources-ipad/", "resources-iphonehd")
     	to resolutions vector by setSearchResolutionsOrder. The "internal_dir" is relative to "Resources/".

		If we have a file named 'sprite.png', the mapping in fileLookup dictionary contains `key: sprite.png -> value: sprite.pvr.gz`.
     	Firstly, it will replace 'sprite.png' with 'sprite.pvr.gz', then searching the file sprite.pvr.gz as follows:

     	    /mnt/sdcard/resources-ipadhd/sprite.pvr.gz      (if not found, search next)
     	    /mnt/sdcard/resources-ipad/sprite.pvr.gz        (if not found, search next)
     	    /mnt/sdcard/resources-iphonehd/sprite.pvr.gz    (if not found, search next)
     	    /mnt/sdcard/sprite.pvr.gz                       (if not found, search next)
     	    internal_dir/resources-ipadhd/sprite.pvr.gz     (if not found, search next)
     	    internal_dir/resources-ipad/sprite.pvr.gz       (if not found, search next)
     	    internal_dir/resources-iphonehd/sprite.pvr.gz   (if not found, search next)
     	    internal_dir/sprite.pvr.gz                      (if not found, return "sprite.png")

        If the filename contains relative path like "gamescene/uilayer/sprite.png",
        and the mapping in fileLookup dictionary contains `key: gamescene/uilayer/sprite.png -> value: gamescene/uilayer/sprite.pvr.gz`.
        The file search order will be:

     	    /mnt/sdcard/gamescene/uilayer/resources-ipadhd/sprite.pvr.gz      (if not found, search next)
     	    /mnt/sdcard/gamescene/uilayer/resources-ipad/sprite.pvr.gz        (if not found, search next)
     	    /mnt/sdcard/gamescene/uilayer/resources-iphonehd/sprite.pvr.gz    (if not found, search next)
     	    /mnt/sdcard/gamescene/uilayer/sprite.pvr.gz                       (if not found, search next)
     	    internal_dir/gamescene/uilayer/resources-ipadhd/sprite.pvr.gz     (if not found, search next)
     	    internal_dir/gamescene/uilayer/resources-ipad/sprite.pvr.gz       (if not found, search next)
     	    internal_dir/gamescene/uilayer/resources-iphonehd/sprite.pvr.gz   (if not found, search next)
     	    internal_dir/gamescene/uilayer/sprite.pvr.gz                      (if not found, return "gamescene/uilayer/sprite.png")

     If the new file can't be found on the file system, it will return the parameter pszFileName directly.

     This method was added to simplify multiplatform support. Whether you are using cocos2d-js or any cross-compilation toolchain like StellaSDK or Apportable,
     you might need to load different resources for a given file in the different platforms.

     @since v2.1
     @note Robtop Addition: added a bool parameter
     */ // :3
    virtual gd::string fullPathForFilename(const char* pszFileName, bool skipSuffix); // :3

    // @note RobTop Addition :3
    virtual void removeFullPath(const char* path); // :3

    /**
     * Loads the filenameLookup dictionary from the contents of a filename.
     *
     * @note The plist file name should follow the format below:
     *
     * @code
     * <?xml version="1.0" encoding="UTF-8"?>
     * <!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
     * <plist version="1.0">
     * <dict>
     *     <key>filenames</key>
     *     <dict>
     *         <key>sounds/click.wav</key>
     *         <string>sounds/click.caf</string>
     *         <key>sounds/endgame.wav</key>
     *         <string>sounds/endgame.caf</string>
     *         <key>sounds/gem-0.wav</key>
     *         <string>sounds/gem-0.caf</string>
     *     </dict>
     *     <key>metadata</key>
     *     <dict>
     *         <key>version</key>
     *         <integer>1</integer>
     *     </dict>
     * </dict>
     * </plist>
     * @endcode
     * @param filename The plist file name.
     *
     * @since v2.1
     * @loadFilenameLookup
     */ // :3
    virtual void loadFilenameLookupDictionaryFromFile(const char* filename); // :3

    /**
     *  Sets the filenameLookup dictionary.
     *
     *  @param pFilenameLookupDict The dictionary for replacing filename.
     *  @since v2.1
     *  @lua NA
     */ // :3
    virtual void setFilenameLookupDictionary(CCDictionary* pFilenameLookupDict); // :3

    /**
     *  Gets full path from a file name and the path of the reletive file.
     *  @param pszFilename The file name.
     *  @param pszRelativeFile The path of the relative file.
     *  @return The full path.
     *          e.g. pszFilename: hello.png, pszRelativeFile: /User/path1/path2/hello.plist
     *               Return: /User/path1/path2/hello.pvr (If there a a key(hello.png)-value(hello.pvr) in FilenameLookup dictionary. )
     *
     */ // :3
    virtual const char* fullPathFromRelativeFile(const char *pszFilename, const char *pszRelativeFile); // :3

    /**
     *  Sets the array that contains the search order of the resources.
     *
     *  @param searchResolutionsOrder The source array that contains the search order of the resources.
     *  @see getSearchResolutionsOrder(void), fullPathForFilename(const char*).
     *  @since v2.1
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void setSearchResolutionsOrder(const gd::vector<gd::string>& searchResolutionsOrder); // :3

    /**
      * Append search order of the resources.
      *
      * @see setSearchResolutionsOrder(), fullPathForFilename().
      * @since v2.1
      */ // :3
    virtual void addSearchResolutionsOrder(const char* order); // :3

    /**
     *  Gets the array that contains the search order of the resources.
     *
     *  @see setSearchResolutionsOrder(const gd::vector<gd::string>&), fullPathForFilename(const char*).
     *  @since v2.1
     *  @js NA
     *  @lua NA
     */ // :3
    virtual const gd::vector<gd::string>& getSearchResolutionsOrder(); // :3

    /**
     *  Sets the array of search paths.
     *
     *  You can use this array to modify the search path of the resources.
     *  If you want to use "themes" or search resources in the "cache", you can do it easily by adding new entries in this array.
     *
     *  @note This method could access relative path and absolute path.
     *        If the relative path was passed to the vector, CCFileUtils will add the default resource directory before the relative path.
     *        For instance:
     *        	On Android, the default resource root path is "assets/".
     *        	If "/mnt/sdcard/" and "resources-large" were set to the search paths vector,
     *        	"resources-large" will be converted to "assets/resources-large" since it was a relative path.
     *
     *  @param searchPaths The array contains search paths.
     *  @see fullPathForFilename(const char*)
     *  @since v2.1
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void setSearchPaths(const gd::vector<gd::string>& searchPaths); // :3

    /**
     * Add a texture pack. Texture packs are prioritized over other search
     * paths, so if a texture pack has a replacement for a file, it will be
     * used over others. Contrary to addSearchPath, this function adds the
     * pack to the front of the list. If the pack has already been added,
     * it's moved to the front of the list (equivalent to removing and
     * re-adding the pack)
     * @param pack Pack to add
     * @note Geode addition
     */ // :3
    void GEODE_DLL addTexturePack(CCTexturePack pack); // :3
    /**
     * Remove texture pack by ID
     * @param id ID of the texture pack
     * @note Geode addition
     */ // :3
    void GEODE_DLL removeTexturePack(std::string_view id); // :3
    /**
     * Add a search path to the front of the list
     * @param path Path to add
     * @note Geode addition
     */ // :3
    void GEODE_DLL addPriorityPath(const char* path); // :3
    /**
     * Update search path order; texture packs are added first, then other
     * paths
     * @note Geode addition
     */ // :3
    void GEODE_DLL updatePaths(); // :3

    /**
      * Adds a path to search paths.
	  *
	  * @since v2.2
      */ // :3
    virtual void addSearchPath(const char* path); // :3

    /**
      * Removes a path from search paths.
      *
      * @since v2.2
      * @lua NA
      */ // :3
	 virtual void removeSearchPath(const char *path); // :3

    /**
      * Removes all paths.
      *
      * @since v2.2
      * @lua NA
      */ // :3
	 void removeAllPaths(); // :3

    /**
     *  Gets the array of search paths.
     *
     *  @return The array of search paths.
     *  @see fullPathForFilename(const char*).
     *  @js NA
     *  @lua NA
     */ // :3
    virtual const gd::vector<gd::string>& getSearchPaths(); // :3

    /**
     *  Gets the writable path.
     *  @return  The path that can be write/read a file in
     *  @lua NA
     */ // :3
    virtual gd::string getWritablePath() { return ""; } // :3

    // @note RobTop Addition :3
    virtual gd::string getWritablePath2(); // :3

    /**
     *  Checks whether a file exists.
     *
     *  @note If a relative path was passed in, it will be inserted a default root path at the beginning.
     *  @param strFilePath The path of the file, it could be a relative or absolute path.
     *  @return true if the file exists, otherwise it will return false.
     *  @lua NA
     */ // :3
    virtual bool isFileExist(const gd::string& strFilePath) { return false; } // :3

    /**
     *  Checks whether the path is an absolute path.
     *
     *  @note On Android, if the parameter passed in is relative to "assets/", this method will treat it as an absolute path.
     *        Also on Blackberry, path starts with "app/native/Resources/" is treated as an absolute path.
     *
     *  @param strPath The path that needs to be checked.
     *  @return true if it's an absolute path, otherwise it will return false.
     *  @lua NA
     */ // :3
    virtual bool isAbsolutePath(const gd::string& strPath); // :3


    /**
     *  Sets/Gets whether to pop-up a message box when failed to load an image.
     */ // :3
    virtual void setPopupNotify(bool bNotify); // :3
    virtual bool isPopupNotify(); // :3


protected: // :3
    /**
     *  The default constructor.
     */ // :3
    CCFileUtils(); // :3

    /**
     *  Initializes the instance of CCFileUtils. It will set m_searchPathArray and m_searchResolutionsOrderArray to default values.
     *
     *  @note When you are porting Cocos2d-x to a new platform, you may need to take care of this method.
     *        You could assign a default value to m_strDefaultResRootPath in the subclass of CCFileUtils(e.g. CCFileUtilsAndroid). Then invoke the CCFileUtils::init().
     *  @return true if successed, otherwise it returns false.
     *
     */ // :3
    virtual bool init(); // :3

    /**
     *  Gets the new filename from the filename lookup dictionary.
     *  @param pszFileName The original filename.
     *  @return The new filename after searching in the filename lookup dictionary.
     *          If the original filename wasn't in the dictionary, it will return the original filename.
     */ // :3
    virtual gd::string getNewFilename(const char* pszFileName); // :3

    // @note RobTop Addition :3
    virtual bool shouldUseHD(); // :3
    // @note RobTop Addition :3
    virtual gd::string addSuffix(gd::string, gd::string); // :3

    /**
     *  Gets full path for filename, resolution directory and search path.
     *
     *  @param filename The file name.
     *  @param resolutionDirectory The resolution directory.
     *  @param searchPath The search path.
     *  @return The full path of the file. It will return an empty string if the full path of the file doesn't exist.
     */ // :3
    virtual gd::string getPathForFilename(const gd::string& filename, const gd::string& resolutionDirectory, const gd::string& searchPath); // :3

    /**
     *  Gets full path for the directory and the filename.
     *
     *  @note Only iOS and Mac need to override this method since they are using
     *        `[[NSBundle mainBundle] pathForResource: ofType: inDirectory:]` to make a full path.
     *        Other platforms will use the default implementation of this method.
     *  @param strDirectory The directory contains the file we are looking for.
     *  @param strFilename  The name of the file.
     *  @return The full path of the file, if the file can't be found, it will return an empty string.
     */ // :3
    virtual gd::string getFullPathForDirectoryAndFilename(const gd::string& strDirectory, const gd::string& strFilename); // :3

    /**
     *  Creates a dictionary by the contents of a file.
     *  @note This method is used internally.
     */ // :3
    virtual CCDictionary* createCCDictionaryWithContentsOfFile(const gd::string& filename); // :3

    /**
     *  Write a dictionary to a plist file.
     *  @note This method is used internally.
     */ // :3
    virtual bool writeToFile(CCDictionary *dict, const gd::string& fullPath); // :3

    /**
     *  Creates an array by the contents of a file.
     *  @note This method is used internally.
     */ // :3
    virtual CCArray* createCCArrayWithContentsOfFile(const gd::string& filename); // :3

public: // :3
    /** Dictionary used to lookup filenames based on a key.
     *  It is used internally by the following methods:
     *
     *  gd::string fullPathForFilename(const char*);
     *
     *  @since v2.1
     */ // :3
    CCDictionary* m_pFilenameLookupDict; // :3

    /**
     *  The vector contains resolution folders.
     *  The lower index of the element in this vector, the higher priority for this resolution directory.
     */ // :3
    gd::vector<gd::string> m_searchResolutionsOrderArray; // :3

    /**
     * The vector contains search paths.
     * The lower index of the element in this vector, the higher priority for this search path.
     */ // :3
    gd::vector<gd::string> m_searchPathArray; // :3

    /**
     *  The default root path of resources.
     *  If the default root path of resources needs to be changed, do it in the `init` method of CCFileUtils's subclass.
     *  For instance:
     *  On Android, the default root path of resources will be assigned with "assets/" in CCFileUtilsAndroid::init().
     *  Similarly on Blackberry, we assign "app/native/Resources/" to this variable in CCFileUtilsBlackberry::init().
     */ // :3
    gd::string m_strDefaultResRootPath; // :3

    /**
     *  The full path cache. When a file is found, it will be added into this cache.
     *  This variable is used for improving the performance of file search.
     */ // :3
    gd::map<gd::string, gd::string> m_fullPathCache; // :3

    // @note RobTop Addition :3
    CC_SYNTHESIZE_NV(gd::string, m_strAndroidPath, AndroidPath); // :3

protected: // :3
    /**
     *  The singleton pointer of CCFileUtils.
     */ // :3
    static CCFileUtils* s_sharedFileUtils; // :3

}; // :3

// end of platform group :3
/// @} :3

NS_CC_END // :3

#endif    // __CC_FILEUTILS_H__ :3
