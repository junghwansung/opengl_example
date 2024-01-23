#include "framebuffer.h"

FramebufferUPtr Framebuffer::Create(const TexturePtr colorAttachment) 
{
    auto framebuffer = FramebufferUPtr(new Framebuffer());
    if (!framebuffer->InitWithColorAttachment(colorAttachment))
        return nullptr;
    return std::move(framebuffer);
}

Framebuffer::~Framebuffer() 
{
  if (m_depthStencilBuffer) 
  {
    glDeleteRenderbuffers(1, &m_depthStencilBuffer);
  }

  if (m_framebuffer) 
  {
    glDeleteFramebuffers(1, &m_framebuffer);
  }
}

void Framebuffer::BindToDefault() 
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Bind() const 
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
}

bool Framebuffer::InitWithColorAttachment(const TexturePtr colorAttachment) 
{
    m_colorAttachment = colorAttachment;

    // glfw는 기본적으로 rendering buffer를 기본 제공해주는데, 그걸 안쓰고
    // opengl에서는 다른 rendering buffer로 하고 싶을 경우 FrameBuffer라는걸 사용하라고 제공해준다.
    
    // Frame Buffer 생성. (기본 화면에 그리는게 아니라 새로운 m_framebuffer에 그릴거다.)
    glGenFramebuffers(1, &m_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

    // Frame Buffer에 Texture를 붙인다. (color Buffer를 붙인다.)
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 
        colorAttachment->Get(), 0);

    // Render Buffer 생성 
    glGenRenderbuffers(1, &m_depthStencilBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilBuffer);

    // Render Buffer의 타입 지정 (size와 pixel format을 지정.)
    glRenderbufferStorage(
        GL_RENDERBUFFER, 
        GL_DEPTH24_STENCIL8,        // depth size 24-bit, stencil size 8-bit
        colorAttachment->GetWidth(), colorAttachment->GetHeight());
    glBindRenderbuffer(GL_RENDERBUFFER, 0); // default render buffer로 돌린다.

    // frame buffer에 위에서 만든 render buffer(depth,stencil buffer)를 연결.
    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER, m_depthStencilBuffer);

    auto result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (result != GL_FRAMEBUFFER_COMPLETE) 
    {
        SPDLOG_ERROR("failed to create framebuffer: {}", result);
        return false;
    }

    BindToDefault();

    return true;
}