#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"
#include "Buffer.h"
#include "VertexLayout.h"
#include "Texture.h"
#include "Mesh.h"
#include "Model.h"
#include "FrameBuffer.h"
// ... context 클래스 선언

CLASS_PTR(Context)
class Context {
public:
    static ContextUPtr Create();
    void Render();    
    void ProcessInput(GLFWwindow* window);
    void Reshape(int width, int height);
    void MouseMove(double x, double y);
    void MouseButton(int button, int action, double x, double y);

private:
    Context() {}
    bool Init();

    ProgramUPtr m_program;
    ProgramUPtr m_simpleProgram;
    ProgramUPtr m_textureProgram;
    ProgramUPtr m_postProgram;



    MeshUPtr m_box;
    MeshUPtr m_plane;

    MaterialPtr m_planeMaterial;
    MaterialPtr m_box1Material;
    MaterialPtr m_box2Material;
    TexturePtr m_windowTexture;

    

    // animation
    bool m_animation { true };
    bool m_flashLightMode { false };

    // clear color
    glm::vec4 m_clearColor  { glm::vec4(0.0f, 0.1f, 0.2f, 0.0f) };
    
    // light parameter
    struct Light {
        glm::vec3 position  { glm::vec3(1.0f, 4.0f, 4.0f) };
        glm::vec3 direction { glm::vec3(-1.0f, -1.0f, -1.0f) };
        glm::vec2 cutoff    { glm::vec2(120.0f, 5.0f) };
        float distance      { 128.0f };
        glm::vec3 ambient   { glm::vec3(0.1f, 0.1f, 0.1f) };
        glm::vec3 diffuse   { glm::vec3(0.5f, 0.5f, 0.5f) };
        glm::vec3 specular  { glm::vec3(1.0f, 1.0f, 1.0f) };
    };
    Light m_light;

    // camera parameter
    bool m_cameraControl { false };
    glm::vec2 m_prevMousePos { glm::vec2(0.0f) };
    float m_cameraPitch { -20.0f };
    float m_cameraYaw { 0.0f };
    glm::vec3 m_cameraPos { glm::vec3(0.0f, 2.5f, 8.0f) };
    glm::vec3 m_cameraFront { glm::vec3(0.0f, 0.0f, -1.0f) };
    glm::vec3 m_cameraUp    { glm::vec3(0.0f, 1.0f, 0.0f) };

    // framebuffer
    FramebufferUPtr m_framebuffer;

    // Window Size
    int m_width     { 640 };
    int m_height    { 480 };
};

#endif // __CONTEXT_H__