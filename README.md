# OpenGLDemo

opengl学习

创建环境

下载vs studio    下载  glfw-3.3.8.bin.win32(32位)

创建项目-选择c++ 桌面环境

创建empty c++项目

切换至显示所有文件夹->子目录创建->src文件夹->创建新项->Application.cpp

项目根目录创建依赖目录，将glfw的include 和一个包复制进去

打开项目属性->c++-常规->附加包含目录-->宏-->

输入---solutiondir  查看项目相对目录

$(SolutionDir)Dependencies\GLFW\include

链接器->常规->附加库目录->$(SolutionDir)Dependencies\GLFW\lib-vc2022

     ->输入->全部删掉->添加glfw3.lib;opengl32.lib;User32.lib;Gdi32.lib;Shell32.lib;
     
（因为上一步全删，所以需要补足后面的几个）

![image](https://user-images.githubusercontent.com/66365279/223052592-af19b4ca-b574-429a-87b4-bbdc82eb338f.png)

复制 https://www.glfw.org/documentation.html 的代码

这个代码，爆红的话，编译环境切换到32位，和上面glfw的保持一致

![image](https://user-images.githubusercontent.com/66365279/223052634-a73ecd59-25c2-4ea5-9b8d-6486dd9e156e.png)
