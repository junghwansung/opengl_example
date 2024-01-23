#include "texture.h"

TextureUPtr Texture::Create(int width, int height, uint32_t format) 
{
    auto texture = TextureUPtr(new Texture());
    texture->CreateTexture();
    texture->SetTextureFormat(width, height, format);
    texture->SetFilter(GL_LINEAR, GL_LINEAR);
    return std::move(texture);
}

TextureUPtr Texture::CreateFromImage(const Image* image) 
{
    auto texture = TextureUPtr(new Texture());
    texture->CreateTexture();
    texture->SetTextureFromImage(image);
    return std::move(texture);
}

Texture::~Texture() 
{
    if (m_texture) {
        glDeleteTextures(1, &m_texture);
    }
}

void Texture::Bind() const 
{
    glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture::SetFilter(uint32_t minFilter, uint32_t magFilter) const 
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}

void Texture::SetWrap(uint32_t sWrap, uint32_t tWrap) const 
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrap);
}
    
void Texture::CreateTexture() 
{
    glGenTextures(1, &m_texture);
    // bind and set default filter and wrap option
    Bind();
#if 0
    SetFilter(GL_LINEAR, GL_LINEAR);
#else
    SetFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);  // GL_NEAREST_MIPMAP_NEAREST 도 있음.
#endif
    SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
}

void Texture::SetTextureFromImage(const Image* image) 
{
    GLenum format = GL_RGBA;
    switch (image->GetChannelCount()) {
        default: break;
        case 1: format = GL_RED; break;
        case 2: format = GL_RG; break;
        case 3: format = GL_RGB; break;
    }
    

    // gpu에 cpu image 데이터 복사.

    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,             // texture type, level, image type (gpu side), 
    //     image->GetWidth(), image->GetHeight(), 0,       // width, height, boarder (gpu side)
    //     format, GL_UNSIGNED_BYTE, image->GetData());    // image type, 1 channel data type, data (of cpu image) 

    m_width = image->GetWidth();
    m_height = image->GetHeight();
    m_format = format;
  
    glTexImage2D(GL_TEXTURE_2D, 0, m_format,
        m_width, m_height, 0,
        format, GL_UNSIGNED_BYTE,
        image->GetData());

    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::SetTextureFormat(int width, int height, uint32_t format) 
{
    m_width = width;
    m_height = height;
    m_format = format;

    glTexImage2D(GL_TEXTURE_2D, 0, m_format,
        m_width, m_height, 0,
        m_format, GL_UNSIGNED_BYTE,
        nullptr);       // null 이면, 메모리 할당만 한다.
}