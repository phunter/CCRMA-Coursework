#include "MultiSampleRenderTarget.h"
#include <stdexcept>

MultiSampleRenderTarget::MultiSampleRenderTarget(unsigned int width, unsigned int height) {
    width_ = width;
    height_ = height;

    glPushAttrib(GL_ALL_ATTRIB_BITS );
    
    // Initialize the texture, including filtering options
    glGenTextures(1, &textureID_);
    glBindTexture(GL_TEXTURE_2D, textureID_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    // Generate a framebuffer
    glGenFramebuffersEXT(1, &frameBufferID_);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferID_);

    // Attach the texture to the frame buffer
    glFramebufferTexture2DEXT(
        GL_FRAMEBUFFER_EXT,
        GL_COLOR_ATTACHMENT0_EXT,
        GL_TEXTURE_2D,
        textureID_,
        0);

    // Check the status of the FBO
    glDrawBuffer(GL_NONE);
    if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)) {
        throw std::runtime_error("Invalid framebuffer configuration");
    }
}

MultiSampleRenderTarget::~MultiSampleRenderTarget() {
    glDeleteFramebuffersEXT(1, &frameBufferID_);
    glDeleteRenderbuffersEXT(1, &depthBufferID_);
    glDeleteTextures(1, &textureID_);
}

GLuint MultiSampleRenderTarget::textureID() const {
    return textureID_;
}

void MultiSampleRenderTarget::bind() {
    glPushAttrib(GL_VIEWPORT_BIT);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferID_);
    glViewport(0, 0, width_, height_);
    glDrawBuffer(GL_NONE);
}

void MultiSampleRenderTarget::unbind() {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    glPopAttrib();
}
