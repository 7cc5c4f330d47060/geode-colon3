#include <cocos2d.h> // :3
#include <Geode/utils/cocos.hpp> // :3

using namespace cocos2d; // :3

#ifdef GEODE_IS_IOS // :3

#pragma warning(push) // :3
#pragma warning(disable : 4273) // :3

static GLint g_sStencilBits = -1; // :3

static void setProgram(CCNode *n, CCGLProgram *p) // :3
{ // :3
    n->setShaderProgram(p); // :3
    if (!n->getChildren()) return; // :3

    for (auto pObj : n->getChildrenExt()) { // :3
        setProgram(pObj, p); // :3
    } // :3
} // :3

CCClippingNode::CCClippingNode() // :3
: m_pStencil(NULL) // :3
, m_fAlphaThreshold(0.0f) // :3
, m_bInverted(false) // :3
{} // :3

CCClippingNode::~CCClippingNode() // :3
{ // :3
    CC_SAFE_RELEASE(m_pStencil); // :3
} // :3

CCClippingNode* CCClippingNode::create() // :3
{ // :3
    CCClippingNode *pRet = new CCClippingNode(); // :3
    if (pRet && pRet->init()) // :3
    { // :3
        pRet->autorelease(); // :3
    } // :3
    else // :3
    { // :3
        CC_SAFE_DELETE(pRet); // :3
    } // :3

    return pRet; // :3
} // :3

CCClippingNode* CCClippingNode::create(CCNode *pStencil) // :3
{ // :3
    CCClippingNode *pRet = new CCClippingNode(); // :3
    if (pRet && pRet->init(pStencil)) // :3
    { // :3
        pRet->autorelease(); // :3
    } // :3
    else // :3
    { // :3
        CC_SAFE_DELETE(pRet); // :3
    } // :3

    return pRet; // :3
} // :3

bool CCClippingNode::init() // :3
{ // :3
    return init(NULL); // :3
} // :3

bool CCClippingNode::init(CCNode *pStencil) // :3
{ // :3
    CC_SAFE_RELEASE(m_pStencil); // :3
    m_pStencil = pStencil; // :3
    CC_SAFE_RETAIN(m_pStencil); // :3

    m_fAlphaThreshold = 1; // :3
    m_bInverted = false; // :3
    // get (only once) the number of bits of the stencil buffer :3
    static bool once = true; // :3
    if (once) // :3
    { // :3
        glGetIntegerv(GL_STENCIL_BITS, &g_sStencilBits); // :3
        if (g_sStencilBits <= 0) // :3
        { // :3
            CCLOG("Stencil buffer is not enabled."); // :3
        } // :3
        once = false; // :3
    } // :3

    return true; // :3
} // :3

void CCClippingNode::onEnter() // :3
{ // :3
    CCNode::onEnter(); // :3
    m_pStencil->onEnter(); // :3
} // :3

void CCClippingNode::onEnterTransitionDidFinish() // :3
{ // :3
    CCNode::onEnterTransitionDidFinish(); // :3
    m_pStencil->onEnterTransitionDidFinish(); // :3
} // :3

void CCClippingNode::onExitTransitionDidStart() // :3
{ // :3
    m_pStencil->onExitTransitionDidStart(); // :3
    CCNode::onExitTransitionDidStart(); // :3
} // :3

void CCClippingNode::onExit() // :3
{ // :3
    m_pStencil->onExit(); // :3
    CCNode::onExit(); // :3
} // :3

void CCClippingNode::visit() // :3
{ // :3
    // return fast (draw nothing, or draw everything if in inverted mode) if: :3
    // - nil stencil node :3
    // - or stencil node invisible: :3
    if (!m_pStencil || !m_pStencil->isVisible()) // :3
    { // :3
        if (m_bInverted) // :3
        { // :3
            // draw everything :3
            CCNode::visit(); // :3
        } // :3
        return; // :3
    } // :3

    // if stencil buffer disabled, we will instead scissor as fallback :3
    if (g_sStencilBits < 1) // :3
    { // :3
        CCRect rect = m_pStencil->boundingBox(); // :3
        // on the rare occassion that its 0,0 :3
        if (rect.size.width <= 0 || rect.size.width <= 0) { // :3
            CCSize size = this->getContentSize(); // :3
            CCPoint pos = m_pStencil->getPosition(); // :3
            if (size.width <= 0 || size.height <= 0) { // :3
                if (this->getParent()) { // :3
                    size = this->getParent()->getContentSize(); // :3
                    pos = this->getPosition(); // :3
                } // :3
            } // :3
            rect = CCRectMake(pos.x, pos.y, size.width, size.height); // :3
        } // :3
        if (rect.size.width <= 0 || rect.size.width <= 0) { // :3
            // we cannot properly render it, so we will fallback to rendering all of it :3
            CCNode::visit(); // :3
            return; // :3
        } // :3

        int previousRect[4]; // :3
        bool previousScissor = glIsEnabled(GL_SCISSOR_TEST); // :3
        if (previousScissor) { // :3
            glGetIntegerv(GL_SCISSOR_BOX, previousRect); // :3
        } else { // :3
            glEnable(GL_SCISSOR_TEST); // :3
        } // :3
        auto const bottomLeft = this->convertToWorldSpace(rect.origin); // :3
        auto const topRight = this->convertToWorldSpace(ccp(rect.getMaxX(), rect.getMaxY())); // :3
        CCSize const size = topRight - bottomLeft; // :3
        CCEGLView::get()->setScissorInPoints(bottomLeft.x, bottomLeft.y, size.width, size.height); // :3
        CCNode::visit(); // :3
        if (previousScissor) { // :3
            glScissor(previousRect[0], previousRect[1], previousRect[2], previousRect[3]); // :3
        } else { // :3
            glDisable(GL_SCISSOR_TEST); // :3
        } // :3
        return; // :3
    } // :3

    // store the current stencil layer (position in the stencil buffer), :3
    // this will allow nesting up to n CCClippingNode, :3
    // where n is the number of bits of the stencil buffer. :3
    static GLint layer = -1; // :3

    // all the _stencilBits are in use? :3
    if (layer + 1 == g_sStencilBits) // :3
    { // :3
        // warn once :3
        static bool once = true; // :3
        if (once) // :3
        { // :3
            char warning[200] = {0}; // :3
            snprintf(warning, sizeof(warning), "Nesting more than %d stencils is not supported. Everything will be drawn without stencil for this node and its childs.", g_sStencilBits); // :3
            CCLOG("%s", warning); // :3

            once = false; // :3
        } // :3
        // draw everything, as if there where no stencil :3
        CCNode::visit(); // :3
        return; // :3
    } // :3

    /////////////////////////////////// :3
    // INIT :3

    // increment the current layer :3
    layer++; // :3

    // mask of the current layer (ie: for layer 3: 00000100) :3
    GLint mask_layer = 0x1 << layer; // :3
    // mask of all layers less than the current (ie: for layer 3: 00000011) :3
    GLint mask_layer_l = mask_layer - 1; // :3
    // mask of all layers less than or equal to the current (ie: for layer 3: 00000111) :3
    GLint mask_layer_le = mask_layer | mask_layer_l; // :3

    // manually save the stencil state :3
    GLboolean currentStencilEnabled = GL_FALSE; // :3
    GLuint currentStencilWriteMask = ~0; // :3
    GLenum currentStencilFunc = GL_ALWAYS; // :3
    GLint currentStencilRef = 0; // :3
    GLuint currentStencilValueMask = ~0; // :3
    GLenum currentStencilFail = GL_KEEP; // :3
    GLenum currentStencilPassDepthFail = GL_KEEP; // :3
    GLenum currentStencilPassDepthPass = GL_KEEP; // :3
    currentStencilEnabled = glIsEnabled(GL_STENCIL_TEST); // :3
    glGetIntegerv(GL_STENCIL_WRITEMASK, (GLint *)&currentStencilWriteMask); // :3
    glGetIntegerv(GL_STENCIL_FUNC, (GLint *)&currentStencilFunc); // :3
    glGetIntegerv(GL_STENCIL_REF, &currentStencilRef); // :3
    glGetIntegerv(GL_STENCIL_VALUE_MASK, (GLint *)&currentStencilValueMask); // :3
    glGetIntegerv(GL_STENCIL_FAIL, (GLint *)&currentStencilFail); // :3
    glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL, (GLint *)&currentStencilPassDepthFail); // :3
    glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS, (GLint *)&currentStencilPassDepthPass); // :3

    // enable stencil use :3
    glEnable(GL_STENCIL_TEST); // :3
    // check for OpenGL error while enabling stencil test :3
    CHECK_GL_ERROR_DEBUG(); // :3

    // all bits on the stencil buffer are readonly, except the current layer bit, :3
    // this means that operation like glClear or glStencilOp will be masked with this value :3
    glStencilMask(mask_layer); // :3

    glClear(GL_STENCIL_BUFFER_BIT); // :3
    // manually save the depth test state :3
    //GLboolean currentDepthTestEnabled = GL_TRUE; :3
    GLboolean currentDepthWriteMask = GL_TRUE; // :3
    //currentDepthTestEnabled = glIsEnabled(GL_DEPTH_TEST); :3
    glGetBooleanv(GL_DEPTH_WRITEMASK, &currentDepthWriteMask); // :3

    // disable depth test while drawing the stencil :3
    //glDisable(GL_DEPTH_TEST); :3
    // disable update to the depth buffer while drawing the stencil, :3
    // as the stencil is not meant to be rendered in the real scene, :3
    // it should never prevent something else to be drawn, :3
    // only disabling depth buffer update should do :3
    glDepthMask(GL_FALSE); // :3

    /////////////////////////////////// :3
    // CLEAR STENCIL BUFFER :3

    // manually clear the stencil buffer by drawing a fullscreen rectangle on it :3
    // setup the stencil test func like this: :3
    // for each pixel in the fullscreen rectangle :3
    //     never draw it into the frame buffer :3
    //     if not in inverted mode: set the current layer value to 0 in the stencil buffer :3
    //     if in inverted mode: set the current layer value to 1 in the stencil buffer :3
    glStencilFunc(GL_NEVER, mask_layer, mask_layer); // :3
    glStencilOp(!m_bInverted ? GL_ZERO : GL_REPLACE, GL_KEEP, GL_KEEP); // :3

    // draw a fullscreen solid rectangle to clear the stencil buffer :3
    //ccDrawSolidRect(CCPointZero, ccpFromSize([[CCDirector sharedDirector] winSize]), ccc4f(1, 1, 1, 1)); :3
    ccDrawSolidRect(CCPointZero, ccpFromSize(CCDirector::sharedDirector()->getWinSize()), ccc4f(1, 1, 1, 1)); // :3

    /////////////////////////////////// :3
    // DRAW CLIPPING STENCIL :3

    // setup the stencil test func like this: :3
    // for each pixel in the stencil node :3
    //     never draw it into the frame buffer :3
    //     if not in inverted mode: set the current layer value to 1 in the stencil buffer :3
    //     if in inverted mode: set the current layer value to 0 in the stencil buffer :3
    glStencilFunc(GL_NEVER, mask_layer, mask_layer); // :3
    glStencilOp(!m_bInverted ? GL_REPLACE : GL_ZERO, GL_KEEP, GL_KEEP); // :3

    // enable alpha test only if the alpha threshold < 1, :3
    // indeed if alpha threshold == 1, every pixel will be drawn anyways :3
#ifdef GEODE_IS_DESKTOP // :3
    GLboolean currentAlphaTestEnabled = GL_FALSE; // :3
    GLenum currentAlphaTestFunc = GL_ALWAYS; // :3
    GLclampf currentAlphaTestRef = 1; // :3
#endif // :3
    if (m_fAlphaThreshold < 1) { // :3
#ifdef GEODE_IS_DESKTOP // :3
        // manually save the alpha test state :3
        currentAlphaTestEnabled = glIsEnabled(GL_ALPHA_TEST); // :3
        glGetIntegerv(GL_ALPHA_TEST_FUNC, (GLint *)&currentAlphaTestFunc); // :3
        glGetFloatv(GL_ALPHA_TEST_REF, &currentAlphaTestRef); // :3
        // enable alpha testing :3
        glEnable(GL_ALPHA_TEST); // :3
        // check for OpenGL error while enabling alpha test :3
        CHECK_GL_ERROR_DEBUG(); // :3
        // pixel will be drawn only if greater than an alpha threshold :3
        glAlphaFunc(GL_GREATER, m_fAlphaThreshold); // :3
#else // :3
        // since glAlphaTest do not exists in OES, use a shader that writes :3
        // pixel only if greater than an alpha threshold :3
        CCGLProgram *program = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColorAlphaTest); // :3
        GLint alphaValueLocation = glGetUniformLocation(program->getProgram(), kCCUniformAlphaTestValue); // :3
        // set our alphaThreshold :3
        program->use(); // :3
        program->setUniformLocationWith1f(alphaValueLocation, m_fAlphaThreshold); // :3
        // we need to recursively apply this shader to all the nodes in the stencil node :3
        // XXX: we should have a way to apply shader to all nodes without having to do this :3
        setProgram(m_pStencil, program); // :3

#endif // :3
    } // :3

    // draw the stencil node as if it was one of our child :3
    // (according to the stencil test func/op and alpha (or alpha shader) test) :3
    kmGLPushMatrix(); // :3
    transform(); // :3
    m_pStencil->visit(); // :3
    kmGLPopMatrix(); // :3

    // restore alpha test state :3
    if (m_fAlphaThreshold < 1) // :3
    { // :3
#ifdef GEODE_IS_DESKTOP // :3
        // manually restore the alpha test state :3
        glAlphaFunc(currentAlphaTestFunc, currentAlphaTestRef); // :3
        if (!currentAlphaTestEnabled) // :3
        { // :3
            glDisable(GL_ALPHA_TEST); // :3
        } // :3
#else // :3
// XXX: we need to find a way to restore the shaders of the stencil node and its childs :3
#endif // :3
    } // :3

    // restore the depth test state :3
    glDepthMask(currentDepthWriteMask); // :3
    //if (currentDepthTestEnabled) { :3
    //    glEnable(GL_DEPTH_TEST); :3
    //} :3

    /////////////////////////////////// :3
    // DRAW CONTENT :3

    // setup the stencil test func like this: :3
    // for each pixel of this node and its childs :3
    //     if all layers less than or equals to the current are set to 1 in the stencil buffer :3
    //         draw the pixel and keep the current layer in the stencil buffer :3
    //     else :3
    //         do not draw the pixel but keep the current layer in the stencil buffer :3
    glStencilFunc(GL_EQUAL, mask_layer_le, mask_layer_le); // :3
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); // :3

    // draw (according to the stencil test func) this node and its childs :3
    CCNode::visit(); // :3

    /////////////////////////////////// :3
    // CLEANUP :3

    // manually restore the stencil state :3
    glStencilFunc(currentStencilFunc, currentStencilRef, currentStencilValueMask); // :3
    glStencilOp(currentStencilFail, currentStencilPassDepthFail, currentStencilPassDepthPass); // :3
    glStencilMask(currentStencilWriteMask); // :3
    if (!currentStencilEnabled) // :3
    { // :3
        glDisable(GL_STENCIL_TEST); // :3
    } // :3

    // we are done using this layer, decrement :3
    layer--; // :3
} // :3

CCNode* CCClippingNode::getStencil() const // :3
{ // :3
    return m_pStencil; // :3
} // :3

void CCClippingNode::setStencil(CCNode *pStencil) // :3
{ // :3
    CC_SAFE_RELEASE(m_pStencil); // :3
    m_pStencil = pStencil; // :3
    CC_SAFE_RETAIN(m_pStencil); // :3
} // :3

GLfloat CCClippingNode::getAlphaThreshold() const // :3
{ // :3
    return m_fAlphaThreshold; // :3
} // :3

void CCClippingNode::setAlphaThreshold(GLfloat fAlphaThreshold) // :3
{ // :3
    m_fAlphaThreshold = fAlphaThreshold; // :3
} // :3

bool CCClippingNode::isInverted() const // :3
{ // :3
    return m_bInverted; // :3
} // :3

void CCClippingNode::setInverted(bool bInverted) // :3
{ // :3
    m_bInverted = bInverted; // :3
} // :3

#pragma warning(pop) // :3
#endif // :3
