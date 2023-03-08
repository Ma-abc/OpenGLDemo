/*fatal  error C1189: #error:  gl.h included before glew.h   需要放到前面*/
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


//断言
#define ASSERT(x) if(!(x)) __debugbreak();
//添加函数，文件，和错误行信息
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall2(#x, __FILE__, __LINE__))



/* 清除错误 */
static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}
/* 检查错误  错误码需要转换成十六进制 */
static void GLCheckError()
{
    while(GLenum error = glGetError())
    {
        std::cout << "[OpenGL error ] (" << error << " )" << std::endl;
    }
}

static bool GLLogCall2(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL error ] (" << error << " )" << function <<
            " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}
static bool GLLogCall()
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL error ] (" << error << " )" << std::endl;
        return false;
    }
    return true;
}


struct ShaderPogramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};
/* 读取着色器 */
static ShaderPogramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    
    enum class ShaderType
    {
        NONE = -1, VERTEXT = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos) 
            {
                type = ShaderType::VERTEXT;
            }
            else if(line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(),ss[1].str() };
}


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
    //6个顶点坐标
    float positions[8] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f,
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions) * sizeof(float), positions, GL_STATIC_DRAW);


    /* 索引缓冲区 */
    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);//数组，缓冲区
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * sizeof(int), indices, GL_STATIC_DRAW);


    /* 启用顶点属性 */
    glEnableVertexAttribArray(0);
    /* 链接顶点属性 */
    /* (TERPROCGLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) */
    /* (TERPROCGLuint 索引，GLint 大小，GLenum 类型，GLboolean 归一化，GLsizei stride, const void *指针) */
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);


    ShaderPogramSource source = ParseShader("res/shaders/Basic.shader");


    std::cout << "Vertex " << std::endl;
    std::cout << source.VertexSource << std::endl;
    std::cout << "Fragment " << std::endl;
    std::cout << source.FragmentSource << std::endl;

    unsigned int program = CreateShader(source.VertexSource, source.FragmentSource);
    /* 绑定着色器 */
    glUseProgram(program);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


        /* 绘制矩形 : glDrawArrays() 没有索引缓冲区时用的方法      glDrawElements()：有索引缓冲区时用的方法 */
        //glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT,nullptr);//1-任何缓冲区，都必须由无符号的整数组成,2-已经在glBindBuffer设置，这里给nullptr
        //glDrawArrays(GL_TRIANGLES,0, sizeof(positions) / 2); //0-first偏移   6-数量
        //GL_POINTS、GL_TRIANGLES、GL_LINE_STRIP。


        /*  错误操作 演示  */
        //GLClearError(); 
        //glDrawElements(GL_TRIANGLES, sizeof(indices), GL_INT, nullptr);//1-任何缓冲区，都必须由无符号的整数组成,2-已经在glBindBuffer设置，这里给nullptr
        //GLCheckError();//第一种，直接调试错误行
        //ASSERT(GLLogCall());//第二种，出现错误中断调试

        /* 第三种 */
        GLCall(glDrawElements(GL_TRIANGLES, sizeof(indices), GL_INT, nullptr));



        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(program);

    glfwTerminate();
    return 0;
}