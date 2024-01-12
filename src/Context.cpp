#include "context.h"
#include "image.h"


ContextUPtr Context::Create() {
  auto context = ContextUPtr(new Context());
  if (!context->Init())
    return nullptr;
  return std::move(context);
}

bool Context::Init() 
{
    float vertices[] = {
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    };

    uint32_t indices[] = { // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };
 
    m_vertexLayout = VertexLayout::Create();

    m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 32);

    uint32_t Offset = 0;
    const uint32_t Stride = sizeof(float) * 8;
    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, Stride, Offset);    Offset = sizeof(float) * 3;
    m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, Stride, Offset);    Offset = sizeof(float) * 6;
    m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, Stride, Offset);

    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 6);

    ShaderPtr vertShader = Shader::CreateFromFile("./shader/texture.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/texture.fs", GL_FRAGMENT_SHADER);
    if (!vertShader || !fragShader)
        return false;

    SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragShader->Get());
    m_program = Program::Create({fragShader, vertShader});
    if (!m_program)
        return false;

    SPDLOG_INFO("program id: {}", m_program->Get());

    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);

    // ... end of Context::Init()
    // cpu에 image 데이터 할당
#if 1
    auto image = Image::Load("./image/container.jpg");
    if (!image) 
        return false;

    SPDLOG_INFO("image: {}x{}, {} channels", image->GetWidth(), image->GetHeight(), image->GetChannelCount());
#else
    auto image = Image::Create(512, 512);
    image->SetCheckImage(16, 16);
#endif

    // Gpu의 Texture를 담을 메모리 공간 설정.

    m_texture = Texture::CreateFromImage(image.get());

    auto image2 = Image::Load("./image/awesomeface.png");
    m_texture2 = Texture::CreateFromImage(image2.get());

    glActiveTexture(GL_TEXTURE0);       // 0 번째 texture slot 선택 (max slot 은 32개)
    glBindTexture(GL_TEXTURE_2D, m_texture->Get()); // 현재 texture slot에 texture instance를 전달(바인딩).
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texture2->Get());


    m_program->Use();
    glUniform1i(glGetUniformLocation(m_program->Get(), "tex"), 0);      // glGetUniformLocation  sampler2D  unifor 핸들 얻어옴.
    glUniform1i(glGetUniformLocation(m_program->Get(), "tex2"), 1);     // texture slot index 입력.

    return true;
}

void Context::Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    m_program->Use();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}