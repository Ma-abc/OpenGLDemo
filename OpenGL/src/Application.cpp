/*fatal  error C1189: #error:  gl.h included before glew.h   ��Ҫ�ŵ�ǰ��*/
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


//����
#define ASSERT(x) if(!(x)) __debugbreak();
//��Ӻ������ļ����ʹ�������Ϣ
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall2(#x, __FILE__, __LINE__))



/* ������� */
static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}
/* ������  ��������Ҫת����ʮ������ */
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
/* ��ȡ��ɫ�� */
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
* ������ɫ��
*/
static int CompileShader(unsigned int type, const std::string& source)
{
    unsigned id = glCreateShader(type);
    const char* src = source.c_str();
    /* ָ����ɫ�� */
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

    /* ������ɫ�� */
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    /* ���ӳ��� */
    glLinkProgram(program);
    /* ���� */
    glValidateProgram(program);

    /* �Ѿ����ӵ����򣬿���ɾ��shader,�����Ǻܱ�Ҫ�������ڵ��� */
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



    /*��ʼ�� glew  ��Ҫ���� glfwMakeContextCurrent  ����������֮��*/
    if (glewInit() != GLEW_OK)
        std::cout << "Error " << std::endl;

    /*��ȡ�汾*/
    std::cout << glGetString(GL_VERSION) << std::endl;


    /**
    * Opengl�����ɵ����ж������ᱻ����һ��Ψһ�ı�ʶ��(����) 
    */
    //6����������
    float positions[8] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f,
    };

    /* ���������� */
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    /* ѡ�񻺳��� */
    glBindBuffer(GL_ARRAY_BUFFER,buffer);//���飬������
    /*
        STREAM : The data store contents will be modified once and used at most a few times.(���ݴ洢���ݽ����޸�һ�Σ����ʹ�ü���)
        STATIC : The data store contents will be modified once and used many times.(���ݴ洢���ݽ����޸�һ�β�ʹ�ö��)
        DYNAMIC : The data store contents will be modified repeatedly and used many times.(���ݴ洢���ݽ����ظ��޸ĺͶ��ʹ��)
        GL_STATIC_DRAW �����ݲ���򼸺�����ı䡣
        GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶࡣ
        GL_STREAM_DRAW ������ÿ�λ���ʱ����ı䡣
    */
    /* ָ������ :��С���ֽ�Ϊ��λ , ���ݿ��Դ���null*/
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions) * sizeof(float), positions, GL_STATIC_DRAW);


    /* ���������� */
    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);//���飬������
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * sizeof(int), indices, GL_STATIC_DRAW);


    /* ���ö������� */
    glEnableVertexAttribArray(0);
    /* ���Ӷ������� */
    /* (TERPROCGLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) */
    /* (TERPROCGLuint ������GLint ��С��GLenum ���ͣ�GLboolean ��һ����GLsizei stride, const void *ָ��) */
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);


    ShaderPogramSource source = ParseShader("res/shaders/Basic.shader");


    std::cout << "Vertex " << std::endl;
    std::cout << source.VertexSource << std::endl;
    std::cout << "Fragment " << std::endl;
    std::cout << source.FragmentSource << std::endl;

    unsigned int program = CreateShader(source.VertexSource, source.FragmentSource);
    /* ����ɫ�� */
    glUseProgram(program);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


        /* ���ƾ��� : glDrawArrays() û������������ʱ�õķ���      glDrawElements()��������������ʱ�õķ��� */
        //glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT,nullptr);//1-�κλ����������������޷��ŵ��������,2-�Ѿ���glBindBuffer���ã������nullptr
        //glDrawArrays(GL_TRIANGLES,0, sizeof(positions) / 2); //0-firstƫ��   6-����
        //GL_POINTS��GL_TRIANGLES��GL_LINE_STRIP��


        /*  ������� ��ʾ  */
        //GLClearError(); 
        //glDrawElements(GL_TRIANGLES, sizeof(indices), GL_INT, nullptr);//1-�κλ����������������޷��ŵ��������,2-�Ѿ���glBindBuffer���ã������nullptr
        //GLCheckError();//��һ�֣�ֱ�ӵ��Դ�����
        //ASSERT(GLLogCall());//�ڶ��֣����ִ����жϵ���

        /* ������ */
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