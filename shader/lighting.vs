#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 transform;     // perspective transformation matrix가 포함되어있다.
uniform mat4 modelTransform;    // 이놈은 real world coordinate 상에서 matrix 변환을 하기 위해서이다.

out vec3 normal;
out vec2 texCoord;
out vec3 position;

void main() 
{
    gl_Position = transform * vec4(aPos, 1.0);
    // modelTransform 제대로 뭐지?
    normal = (transpose(inverse(modelTransform)) * vec4(aNormal, 0.0)).xyz;
    texCoord = aTexCoord;
    position = (modelTransform * vec4(aPos, 1.0)).xyz;
}