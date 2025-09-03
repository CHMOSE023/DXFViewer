# CMake配置文件
cmake_minimum_required (VERSION 3.8)
project ("DXFViewer")

# 设置 C++17 标准
set(CMAKE_CXX_STANDARD 17)            # 指定 C++17 标准
set(CMAKE_CXX_STANDARD_REQUIRED YES)  # 确保 C++17 标准是必须的
set(CMAKE_CXX_EXTENSIONS OFF)         # 禁用编译器特有的扩展
 
add_definitions("-DUNICODE" "-D_UNICODE") # 定义UNICODE宏
 

# 搜索目录
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/deps/include/")            # 包含文件搜索目录 
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/extlib/glm/")              # 包含文件搜索目录 
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/extlib/glfw/include")      # 包含文件搜索目录
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/extlib/libdxfrw")          # 包含文件搜索目录 

# 添加编译宏
add_compile_definitions(
  _CRT_SECURE_NO_WARNINGS  #fopen sscanf 警告
)

# src内的文件夹
set( EXAMPLES                   
	 "Viewer"             
	 "ReadDXF"             
)

# glad文件
set(GLAD_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/deps/include/glad/glad.h" 
    "${CMAKE_CURRENT_SOURCE_DIR}/deps/include/glad/glad.c"
)

# 框架文件
file(GLOB CORE_SRC
  "${CMAKE_CURRENT_SOURCE_DIR}/src/App/*"   
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Core/*"    
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Shader/*"  
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Entity/*"   
)

# 复制textures编译目录
file(GLOB Resources "${CMAKE_CURRENT_SOURCE_DIR}/deps/resources/*" )  

file(COPY ${Resources} DESTINATION  ${CMAKE_CURRENT_BINARY_DIR} )

message(---------->"${CORE_SRC}")

# 添加glad库
add_library(glad ${GLAD_SRC})
add_library(core ${CORE_SRC})
 
add_subdirectory("extlib/libdxfrw") # 添加dxfrw库
add_subdirectory("extlib/glfw") # 添加dxfrw库

# 遍历src文件夹内文件，添加exe程序
foreach(Example ${EXAMPLES} )	
    message(${Example})
    file(GLOB SrcFile  "examples/${Example}/*.h"  "examples/${Example}/*.hpp"  "examples/${Example}/*.cpp" "examples/${Example}/*.c" )
    add_executable (${Example}  ${SrcFile} ) 
    target_link_libraries(${Example} openGL32 glad core dxfrw  glfw)
endforeach()
 