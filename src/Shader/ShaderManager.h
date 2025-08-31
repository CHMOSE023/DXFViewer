#pragma once
#include <glm/glm.hpp>
#include <map> 
#include "Shader.h"
 
class ShaderManager
{
public:
    void AddShader(const std::string& name, std::shared_ptr<Shader> shader)
    {
        shaders[name] = shader; 
    }

    std::shared_ptr<Shader> GetShader(const std::string& name)
    {
        return shaders.at(name); 
    } 

private:
    std::map<std::string, std::shared_ptr<Shader>> shaders;
 };
