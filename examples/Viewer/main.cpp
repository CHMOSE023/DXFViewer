#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "linmath.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

float g_scale = 1.0f;
float g_offset_x = 0.0f;
float g_offset_y = 0.0f;
int g_middle_button_down = 0;
double g_last_cursor_x, g_last_cursor_y;

typedef struct Vertex
{
    vec2 pos;
    vec3 col;
} Vertex;

static const Vertex vertices[3] =
{
    { { -0.6f, -0.4f }, { 1.f, 0.f, 0.f } },
    { {  0.6f, -0.4f }, { 0.f, 1.f, 0.f } },
    { {   0.f,  0.6f }, { 0.f, 0.f, 1.f } }
};
static const char* vertex_shader_text = R"(
    #version 330
    uniform mat4 MVP;
    in vec3 vCol;
    in vec2 vPos;
    out vec3 color;
    void main()
    {
        gl_Position = MVP * vec4(vPos, 0.0, 1.0);
       color = vCol;
    }
)";

static const char* fragment_shader_text = R"(
    #version 330
    in vec3 color;
    out vec4 fragment;
    void main()
    {
        fragment = vec4(color, 1.0);
    }
)";

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

// 键盘输入处理
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{ 
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        else if (key == GLFW_KEY_UP)
			glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
        else if (key == GLFW_KEY_DOWN)
            g_offset_y -= 0.1f;
        else if (key == GLFW_KEY_LEFT)
            g_offset_x -= 0.1f;
        else if (key == GLFW_KEY_RIGHT)
            g_offset_x += 0.1f;
    }
	printf("key:%d, scancode:%d, action:%d, mods:%d\n", key, scancode, action, mods);

}

// 鼠标滚轮缩放
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (yoffset > 0)
        g_scale *= 1.1f;
    else if (yoffset < 0)
        g_scale *= 0.9f;
}

//  鼠标中键拖动：添加光标和平移逻辑
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_MIDDLE)
    {
        if (action == GLFW_PRESS)
        {
            g_middle_button_down = 1;
            glfwGetCursorPos(window, &g_last_cursor_x, &g_last_cursor_y);
        }
        else if (action == GLFW_RELEASE)
        {
            g_middle_button_down = 0;
        }
    }
}
 
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (g_middle_button_down)
    {
        double dx = xpos - g_last_cursor_x;
        double dy = ypos - g_last_cursor_y;
        g_last_cursor_x = xpos;
        g_last_cursor_y = ypos;

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        // 将像素平移转换为视图坐标系
        g_offset_x += (float)(dx / width) * 2.0f;
        g_offset_y -= (float)(dy / height) * 2.0f;
    }
}

static void character_callback(GLFWwindow* window, unsigned int codepoint)
{
   // glfwSetInputMode（窗口，GLFW_CURSOR，GLFW_CURSOR_CAPTURED）；
    //glfwSetInputMode（窗口，GLFW_CURSOR，GLFW_CURSOR_HIDDEN）；
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
	int data = glfwGetTime();
    printf("key:%d;  data:%d\n", codepoint, data);
    return;

	if (codepoint == 'q')
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	else if (codepoint == 'r')
		g_scale = 1.0f;
	else if (codepoint == 'c')
		g_offset_x = g_offset_y = 0.0f;
}

int main(void)
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "DXFViewer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    } 

    //glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetCharCallback(window, character_callback);

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);

    // NOTE: OpenGL error checks have been omitted for brevity

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    const GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    const GLint mvp_location = glGetUniformLocation(program, "MVP");
    const GLint vpos_location = glGetAttribLocation(program, "vPos");
    const GLint vcol_location = glGetAttribLocation(program, "vCol");

    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex), (void*)offsetof(Vertex, col));

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        mat4x4 m, p, mvp;
        mat4x4_identity(m);
        mat4x4_translate(m, g_offset_x, g_offset_y, 0.0f); // 平移
        mat4x4_scale_aniso(m, m, g_scale, g_scale, 1.0f);  // 缩放
       
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);

        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&mvp);
        glBindVertexArray(vertex_array);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwWaitEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
