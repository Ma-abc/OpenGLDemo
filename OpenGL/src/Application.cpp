/*fatal  error C1189: #error:  gl.h included before glew.h   ��Ҫ�ŵ�ǰ��*/
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <iostream>

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
    //3����������
    float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
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
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);



    /* ���ö������� */
    glEnableVertexAttribArray(0);
    /* ���Ӷ������� */
    /* (TERPROCGLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) */
    /* (TERPROCGLuint ������GLint ��С��GLenum ���ͣ�GLboolean ��һ����GLsizei stride, const void *ָ��) */
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
    /* ����ɫ�� */
    glUseProgram(program);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


        /* ���������� : glDrawArrays() û������������ʱ�õķ���      glDrawElements()��������������ʱ�õķ��� */
        glDrawArrays(GL_TRIANGLES,0,3); //0-firstƫ��   3-����
        //GL_POINTS��GL_TRIANGLES��GL_LINE_STRIP��


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}