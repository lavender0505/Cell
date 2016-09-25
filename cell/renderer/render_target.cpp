#include "render_target.h"

#include <utility/logging/log.h>

namespace Cell
{
    // ------------------------------------------------------------------------
    RenderTarget::RenderTarget(unsigned int width, unsigned int height, GLenum type, unsigned int nrColorAttachments, bool depthAndStencil)
    {
        glGenFramebuffers(1, &m_ID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
        // NOTE(Joey): generate all color attachments
        for (unsigned int i = 0; i < nrColorAttachments; ++i)
        {
            Texture texture;
            texture.FilterMin = GL_LINEAR;
            texture.FilterMax = GL_LINEAR;
            texture.Mipmapping = false;
            texture.Generate(width, height, GL_RGBA, GL_RGBA, type, 0); 

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture.ID, 0);
            m_ColorAttachments.push_back(texture);
        }
        // NOTE(Joey): then generate Depth/Stencil texture if requested
        HasDepthAndStencil = depthAndStencil;
        if (depthAndStencil)
        {
            Texture texture;
            texture.Mipmapping = false;
            texture.Generate(width, height, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture.ID, 0);
            m_DepthStencil = texture;
        }
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            Log::Message("Framebuffer not complete!", LOG_ERROR);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    // ------------------------------------------------------------------------
    Texture* RenderTarget::GetDepthStencilTexture()
    {
        return &m_DepthStencil;
    }
    // ------------------------------------------------------------------------
    Texture* RenderTarget::GetColorTexture(unsigned int index)
    {
        if(m_ColorAttachments.size() < index)
            return &m_ColorAttachments[index];
        else
        {
            Log::Message("RenderTarget color texture requested, but not available: " + std::to_string(index), LOG_WARNING);
            return nullptr;
        }
    }
    // ------------------------------------------------------------------------
    void RenderTarget::SetTarget(GLenum target)
    {
        m_Target = target;
    }
}