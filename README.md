# OpenGLDemo
opengl学习

在调用glewInit()前需要创建一个有效的渲染上下文

首先需要glewInit()后才能调用opengl的api

环境配置

在选项->c++->常规->追加 $(SolutionDir)Dependencies\GLEW\include

在链接->常规->添加lib目录

          ->输入->最前面添加 glew32s.lib
          
![image](https://user-images.githubusercontent.com/66365279/223065011-6f8f4ec7-075e-466e-8666-d270d80dee2f.png)

/*fatal  error C1189: #error:  gl.h included before glew.h   需要放到GLFW前面*/

#include <GL/glew.h>

#include <GLFW/glfw3.h>

无法解析的外部符号 __imp__glewInit@0，函数 _main 中引用了该符号

需要定义glew static（内部使用dll）

c++->预处理器->预处理器定义->添加GLEW_STATIC;
