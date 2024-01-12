#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"
#include "Buffer.h"
#include "VertexLayout.h"
#include "Texture.h"
// ... context 클래스 선언

CLASS_PTR(Context)
class Context {
public:
    static ContextUPtr Create();
    void Render();    
    void ProcessInput(GLFWwindow* window);
    
private:
    Context() {}
    bool Init();

    ProgramUPtr m_program;

    VertexLayoutUPtr m_vertexLayout;
    BufferUPtr m_vertexBuffer;
    BufferUPtr m_indexBuffer;
    TextureUPtr m_texture;
    TextureUPtr m_texture2;

    // camera parameter
    glm::vec3 m_cameraPos { glm::vec3(0.0f, 0.0f, 3.0f) };
    glm::vec3 m_cameraFront { glm::vec3(0.0f, 0.0f, -1.0f) };
    glm::vec3 m_cameraUp { glm::vec3(0.0f, 1.0f, 0.0f) };
};

#endif // __CONTEXT_H__