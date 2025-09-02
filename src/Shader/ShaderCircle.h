#pragma once
#include "Shader.h"

class ShaderCircle : public Shader
{
public:
    ShaderCircle() : m_mvp(-1) {}

    bool Initialize()  {
        const char* vs = R"(
            #version 430

            struct CircleEntity {
                vec3  center;
                float radius;
                vec3  color;
                float pad;  // 用来对齐 (std430 要求 vec3 后要补齐成 16 字节)
            };

            layout(std430, binding = 0) buffer CircleBuffer {
                CircleEntity circles[];  // 动态数组
            };

            layout(location = 0) in vec3 vPos;

            uniform mat4 MVP;

            out vec3 fragColor;

            void main() {
                CircleEntity c = circles[gl_InstanceID];
                vec3 worldPos = c.center + vPos * c.radius;
                gl_Position = MVP * vec4(worldPos, 1.0);
                fragColor = c.color;
            }
        )";

        const char* fs = R"(
            #version 430
            in vec3 fragColor;
            out vec4 color;
            void main() {
                color = vec4(1.0,0.0,1.0,fragColor.x);
            }
        )";

        bool ok = CreateShader(vs, fs);
        if (ok) {
            m_mvp = glGetUniformLocation(m_shaderId, "MVP");
        }
        return ok;
    }

    int m_mvp;
};
