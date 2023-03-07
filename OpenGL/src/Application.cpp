/*fatal  error C1189: #error:  gl.h included before glew.h   需要放到前面*/
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <iostream>

/**
* 创建着色器
*/
static int CompileShader(unsigned int type, const std::string& source)
{
    unsigned id = glCreateShader(type);
    const char* src = source.c_str();
    /* 指定着色器 */
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //TODO Eroor Handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message); 
        std::cout << "Failed to compile " << 
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
        std::cout << message << std::endl;


        glDeleteShader(id);
        return 0;
    }

    return id;
}


static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER,vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER,fragmentShader);

    /* 链接着色器 */
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    /* 链接程序 */
    glLinkProgram(program);
    /* 检索 */
    glValidateProgram(program);

    /* 已经链接到程序，可以删除shader,但不是很必要，不利于调试 */
    //glDeleteShader(vs);
    //glDeleteShader(fs);

    return program;
}


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
  

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);



    /*初始化 glew  需要放在 glfwMakeContextCurrent  创建上下文之后*/
    if (glewInit() != GLEW_OK)
        std::cout << "Error " << std::endl;

    /*获取版本*/
    std::cout << glGetString(GL_VERSION) << std::endl;


    /**
    * Opengl中生成的所有东西都会被分配一个唯一的标识符(整数) 
    */
    //3个顶点坐标
    float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    /* 创建缓冲区 */
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    /* 选择缓冲区 */
    glBindBuffer(GL_ARRAY_BUFFER,buffer);//数组，缓冲区
    /*
        STREAM : The data store contents will be modified once and used at most a few times.(数据存储内容将被修改一次，最多使用几次)
        STATIC : The data store contents will be modified once and used many times.(数据存储内容将被修改一次并使用多次)
        DYNAMIC : The data store contents will be modified repeatedly and used many times.(数据存储内容将被重复修改和多次使用)
        GL_STATIC_DRAW ：数据不会或几乎不会改变。
        GL_DYNAMIC_DRAW：数据会被改变很多。
        GL_STREAM_DRAW ：数据每次绘制时都会改变。
    */
    /* 指定数据 :大小以字节为单位 , 数据可以传入null*/
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);



    /* 启用顶点属性 */
    glEnableVertexAttribArray(0);
    /* 链接顶点属性 */
    /* (TERPROCGLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) */
    /* (TERPROCGLuint 索引，GLint 大小，GLenum 类型，GLboolean 归一化，GLsizei stride, const void *指针) */
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);



    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";

    unsigned int program = CreateShader(vertexShader,fragmentShader);
    /* 绑定着色器 */
    glUseProgram(program);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


        /* 绘制三角形 : glDrawArrays() 没有索引缓冲区时用的方法      glDrawElements()：有索引缓冲区时用的方法 */
        glDrawArrays(GL_TRIANGLES,0,3); //0-first偏移   3-数量
        //GL_POINTS、GL_TRIANGLES、GL_LINE_STRIP。


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}