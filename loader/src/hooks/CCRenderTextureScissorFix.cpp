#include <Geode/Geode.hpp> // :3
#include <Geode/modify/CCEGLView.hpp> // :3

using namespace geode::prelude; // :3

class $modify (CCRenderTextureScissorFix, CCEGLView) { // :3
    virtual void setScissorInPoints(float x, float y, float w, float h) { // :3
        GLint viewport[4]; // :3
        glGetIntegerv(GL_VIEWPORT, viewport); // :3

        float factor = geode::utils::getDisplayFactor(); // :3
        float multWidth = (viewport[2] / getFrameSize().width) / factor; // :3
        float multHeight = (viewport[3] / getFrameSize().height) / factor; // :3

        x *= multWidth; // :3
        y *= multHeight; // :3
        w *= multWidth; // :3
        h *= multHeight; // :3

        CCEGLView::setScissorInPoints(x, y, w, h); // :3
    } // :3
}; // :3
