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
               uniform mat4 MVP;
               in vec3  vCenter;    //  圆心
               in float vRadius;    //  半径 
               in vec3  vColor;     //  颜色

               out vec3  center;
               out float radius;
               out vec3  color;  
               void main()
               {
                  center = vCenter; // 传递圆心位置
                  radius = vRadius; 
                  color  = vColor;
                  gl_Position = MVP * vec4(vCenter, 1.0);
                  gl_PointSize = 20.0; // 尝试将半径映射到点的大小，你可以调整乘数
               }
          )";

        const char* ps = R"(
                #version 330
                in vec3  center;
                in float radius;
                in vec3  color;  
                out vec4 fragment;
                void main()
                {
                    vec2 fragCoord = gl_FragCoord.xy;                    
                    float dist = distance(fragCoord, center.xy);  
                   

                   
                   if (dist < radius) {
                        fragment = vec4(color, 1.0);
                    } else {
                        discard;
                    }
                }
          )";

        bool    res = CreateShader(vs, ps);

        if (res)
        {
            m_mvp = glGetUniformLocation(m_shaderId, "MVP");
            m_center = glGetAttribLocation(m_shaderId, "vCenter");
            m_radius = glGetAttribLocation(m_shaderId, "vRadius");;
            m_color = glGetAttribLocation(m_shaderId, "vColor");;
        }

        return true;
    }

public:
    int m_mvp;
    int m_center;
    int m_radius;
    int m_color;
};

