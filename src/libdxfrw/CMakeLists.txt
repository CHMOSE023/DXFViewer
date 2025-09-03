if(MSVC)
    # Disable some overly strict MSVC warnings.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -wd4244 -wd4800 -wd4805")
endif()

# 框架文件
file(GLOB_RECURSE  dxfrw_SRC
  "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/intern/*.h"     
  "${CMAKE_CURRENT_SOURCE_DIR}/intern/*.cpp"
)
 
message(-----GLOB_RECURSE----->"${dxfrw_SRC}")
add_library(dxfrw STATIC ${dxfrw_SRC})

target_include_directories(dxfrw PRIVATE   ${CMAKE_CURRENT_SOURCE_DIR}/intern/)

target_include_directories(dxfrw PUBLIC   ${CMAKE_CURRENT_SOURCE_DIR}/)
