#pragma once
#include "Shader.h"
class ShaderCircle :public Shader
{ 
public:
    ShaderCircle()
    {
        m_mvp = -1;
        m_center = -1;
        m_radius = -1;
        m_color = -1;      
    };

    ~ShaderCircle()
    {
    };


    virtual bool  Initialize()
    {
        const char* vs = R"(
               #version 330
               uniform mat4  MVP;
               uniform vec3  vCenter;
               uniform float vRadius;
               uniform vec3  vCol; 

               in vec3 vPos;
               out vec3 color;
               void main()
               {
                  // 将单位圆点按半径缩放，然后平移到圆心
                  vec3 transformedPos = vCenter + vPos * vRadius; 
                  gl_Position = MVP * vec4(transformedPos, 1.0);
                  color = vCol;
               }
          )";

        const char* ps = R"(
                #version 330
                in vec3 color;
                out vec4 fragment;
                void main()
                {
                     fragment = vec4(color, 1.0); 
                }
          )";

        bool    res = CreateShader(vs, ps);

        if (res)
        {
            m_position = glGetAttribLocation(m_shaderId, "vPos");
            m_mvp = glGetUniformLocation(m_shaderId, "MVP");
            m_center = glGetUniformLocation(m_shaderId, "vCenter");
            m_radius = glGetUniformLocation(m_shaderId, "vRadius");
            m_color = glGetUniformLocation(m_shaderId, "vCol");;
        }

        return true;
    }


public:
    int m_mvp;
    int m_position;
    int m_center;
    int m_radius;
    int m_color;
};

