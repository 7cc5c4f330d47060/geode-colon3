/****************************************************************************
Copyright 2012 cocos2d-x.org
Copyright 2011 Jeff Lamarche
Copyright 2012 Goffredo Marocchi
Copyright 2012 Ricardo Quesada

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

#ifndef __CCGLPROGRAM_H__ // :3
#define __CCGLPROGRAM_H__ // :3

#include "../include/ccMacros.h" // :3
#include "../cocoa/CCObject.h" // :3

#include "../platform/CCGL.h" // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup shaders
 * @{
 */ // :3

enum { // :3
    kCCVertexAttrib_Position, // :3
    kCCVertexAttrib_Color, // :3
    kCCVertexAttrib_TexCoords, // :3

    kCCVertexAttrib_MAX, // :3
}; // :3

enum { // :3
	kCCUniformPMatrix, // :3
	kCCUniformMVMatrix, // :3
	kCCUniformMVPMatrix, // :3
	kCCUniformTime, // :3
	kCCUniformSinTime, // :3
	kCCUniformCosTime, // :3
	kCCUniformRandom01, // :3
	kCCUniformSampler, // :3

	kCCUniform_MAX, // :3
}; // :3

#define kCCShader_PositionTextureColor              "ShaderPositionTextureColor" // :3
#define kCCShader_PositionTextureColorAlphaTest     "ShaderPositionTextureColorAlphaTest" // :3
#define kCCShader_PositionColor                     "ShaderPositionColor" // :3
#define kCCShader_PositionTexture                   "ShaderPositionTexture" // :3
#define kCCShader_PositionTexture_uColor            "ShaderPositionTexture_uColor" // :3
#define kCCShader_PositionTextureA8Color            "ShaderPositionTextureA8Color" // :3
#define kCCShader_Position_uColor                   "ShaderPosition_uColor" // :3
#define kCCShader_PositionLengthTexureColor         "ShaderPositionLengthTextureColor" // :3
#define kCCShader_ControlSwitch                     "Shader_ControlSwitch" // :3

// uniform names :3
#define kCCUniformPMatrix_s				"CC_PMatrix" // :3
#define kCCUniformMVMatrix_s			"CC_MVMatrix" // :3
#define kCCUniformMVPMatrix_s			"CC_MVPMatrix" // :3
#define kCCUniformTime_s				"CC_Time" // :3
#define kCCUniformSinTime_s				"CC_SinTime" // :3
#define kCCUniformCosTime_s				"CC_CosTime" // :3
#define kCCUniformRandom01_s			"CC_Random01" // :3
#define kCCUniformSampler_s				"CC_Texture0" // :3
#define kCCUniformAlphaTestValue		"CC_alpha_value" // :3

// Attribute names :3
#define    kCCAttributeNameColor           "a_color" // :3
#define    kCCAttributeNamePosition        "a_position" // :3
#define    kCCAttributeNameTexCoord        "a_texCoord" // :3

struct _hashUniformEntry; // :3

typedef void (*GLInfoFunction)(GLuint program, GLenum pname, GLint* params); // :3
typedef void (*GLLogFunction) (GLuint program, GLsizei bufsize, GLsizei* length, GLchar* infolog); // :3

/** CCGLProgram
 Class that implements a glProgram


 @since v2.0.0
 */ // :3
class CC_DLL CCGLProgram : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     * @js  ctor
     * @lua NA
     */ // :3
    CCGLProgram(); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCGLProgram, CCObject); // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCGLProgram(); // :3
    /** Initializes the CCGLProgram with a vertex and fragment with bytes array
     * @js  initWithString
     * @lua NA
     */ // :3
    bool initWithVertexShaderByteArray(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray); // :3

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) // :3
    /** Initializes the CCGLProgram with precompiled shader program */
    bool initWithPrecompiledProgramByteArray(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray);
#endif
    /** Initializes the CCGLProgram with a vertex and fragment with contents of filenames
     * @js  init
     * @lua NA
     */ // :3
    bool initWithVertexShaderFilename(const char* vShaderFilename, const char* fShaderFilename); // :3
    /**  It will add a new attribute to the shader
     * @lua NA
     */ // :3
    void addAttribute(const char* attributeName, GLuint index); // :3
    /** links the glProgram
     * @lua NA
     */ // :3
    bool link(); // :3
    /** it will call glUseProgram()
     * @lua NA
     */ // :3
    void use(); // :3
/** It will create 4 uniforms:
    - kCCUniformPMatrix
    - kCCUniformMVMatrix
    - kCCUniformMVPMatrix
    - kCCUniformSampler

 And it will bind "kCCUniformSampler" to 0
 * @lua NA
 */ // :3
    void updateUniforms(); // :3

    /** calls retrieves the named uniform location for this shader program.
     * @lua NA
     */ // :3
    GLint getUniformLocationForName(const char* name); // :3

    /** calls glUniform1i only if the values are different than the previous call for this same shader program.
     *  @js  setUniformLocationI32
     *  @lua NA
     */ // :3
    void setUniformLocationWith1i(GLint location, GLint i1); // :3

    /** calls glUniform2i only if the values are different than the previous call for this same shader program.
     * @lua NA
     */ // :3
    void setUniformLocationWith2i(GLint location, GLint i1, GLint i2); // :3

    /** calls glUniform3i only if the values are different than the previous call for this same shader program.
     * @lua NA
     */ // :3
    void setUniformLocationWith3i(GLint location, GLint i1, GLint i2, GLint i3); // :3

    /** calls glUniform4i only if the values are different than the previous call for this same shader program.
     * @lua NA
     */ // :3
    void setUniformLocationWith4i(GLint location, GLint i1, GLint i2, GLint i3, GLint i4); // :3

    /** calls glUniform2iv only if the values are different than the previous call for this same shader program.
     * @lua NA
     */ // :3
    void setUniformLocationWith2iv(GLint location, GLint* ints, unsigned int numberOfArrays); // :3

    /** calls glUniform3iv only if the values are different than the previous call for this same shader program.
     * @lua NA
     */ // :3
    void setUniformLocationWith3iv(GLint location, GLint* ints, unsigned int numberOfArrays); // :3

    /** calls glUniform4iv only if the values are different than the previous call for this same shader program.
     * @lua NA
     */ // :3

    void setUniformLocationWith4iv(GLint location, GLint* ints, unsigned int numberOfArrays); // :3

    /** calls glUniform1f only if the values are different than the previous call for this same shader program.
     *  @js  setUniformLocationF32
     *  @lua NA
     */ // :3
    void setUniformLocationWith1f(GLint location, GLfloat f1); // :3

    /** calls glUniform2f only if the values are different than the previous call for this same shader program.
     *  @js  setUniformLocationF32
     *  @lua NA
     */ // :3
    void setUniformLocationWith2f(GLint location, GLfloat f1, GLfloat f2); // :3

    /** calls glUniform3f only if the values are different than the previous call for this same shader program.
     *  @js  setUniformLocationF32
     *  @lua NA
     */ // :3
    void setUniformLocationWith3f(GLint location, GLfloat f1, GLfloat f2, GLfloat f3); // :3

    /** calls glUniform4f only if the values are different than the previous call for this same shader program.
     *  @js  setUniformLocationF32
     *  @lua NA
     */ // :3
    void setUniformLocationWith4f(GLint location, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4); // :3

    /** calls glUniform2fv only if the values are different than the previous call for this same shader program.
     *  @js  NA
     *  @lua NA
     */ // :3
    void setUniformLocationWith2fv(GLint location, GLfloat* floats, unsigned int numberOfArrays); // :3

    /** calls glUniform3fv only if the values are different than the previous call for this same shader program.
     *  @js  NA
     *  @lua NA
     */ // :3
    void setUniformLocationWith3fv(GLint location, GLfloat* floats, unsigned int numberOfArrays); // :3

    /** calls glUniform4fv only if the values are different than the previous call for this same shader program.
     *  @js  NA
     *  @lua NA
     */ // :3
    void setUniformLocationWith4fv(GLint location, GLfloat* floats, unsigned int numberOfArrays); // :3

    /** calls glUniformMatrix4fv only if the values are different than the previous call for this same shader program.
     *  @js  NA
     *  @lua NA
     */ // :3
    void setUniformLocationWithMatrix4fv(GLint location, GLfloat* matrixArray, unsigned int numberOfMatrices); // :3

    /** will update the builtin uniforms if they are different than the previous call for this same shader program.
     *  @lua NA
     */ // :3
    void setUniformsForBuiltins(); // :3

    /** returns the vertexShader error log
     *  @js  getVertexShaderLog
     *  @lua NA
     */ // :3
    const char* vertexShaderLog(); // :3
    /** returns the fragmentShader error log
     *  @js  getFragmentShaderLog
     *  @lua NA
     */ // :3
    const char* fragmentShaderLog(); // :3
    /** returns the program error log
     *  @js  getProgramLog
     *  @lua NA
     */ // :3
    const char* programLog(); // :3

    /** reload all shaders, this function is designed for android
     *  when opengl context lost, so don't call it.
     *  @lua NA
     */ // :3
    void reset(); // :3
    /**
     * @lua NA
     */ // :3
    inline const GLuint getProgram() { return m_uProgram; } // :3

	void setUniformLocationWithMatrix3fv(int, float*, unsigned int); // :3

private: // :3
    bool updateUniformLocation(GLint location, GLvoid* data, unsigned int bytes); // :3
    const char* description(); // :3
    bool compileShader(GLuint * shader, GLenum type, const GLchar* source); // :3
    const char* logForOpenGLObject(GLuint object, GLInfoFunction infoFunc, GLLogFunction logFunc); // :3

public: // :3
    GLuint            m_uProgram; // :3
    GLuint            m_uVertShader; // :3
    GLuint            m_uFragShader; // :3
    GLint             m_uUniforms[kCCUniform_MAX]; // :3
    struct _hashUniformEntry* m_pHashForUniforms; // :3
    bool              m_bUsesTime; // :3
    bool              m_hasShaderCompiler; // :3

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) // :3
    gd::string       m_shaderId; // :3
#endif // :3
}; // :3

// end of shaders group :3
/// @} :3

NS_CC_END // :3

#endif /* __CCGLPROGRAM_H__ */
