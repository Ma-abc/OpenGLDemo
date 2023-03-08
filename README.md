# 着色器

顶点缓冲---去掉vertex--只是一个内存缓冲区   字节数组缓冲区

顶点缓冲区和c++中字符数组缓冲区不一样，顶点缓冲区实际上实在gpu上(显卡vram)

cpu发出DrawCall命令->

着色器----运行在gpu上的程序

上下文会有自己的流，缓冲区会有自己的流

但是Opengl中生成的所有东西都会被分配一个唯一的标识符(整数)

一个完整的顶点(vertex)：不止包含坐标，还包含颜色，纹理其他东西

顶点着色器(Vertex Shader)是几个可编程着色器中的一个。如果我们打算做渲染的话，现代OpenGL需要我们至少设置一个顶点和一个片段着色器。

顶点着色器允许我们指定任何以顶点属性为形式的输入。

stride--每个顶点之间的字节数量--偏移，顶点开始到顶点结束的跨度长度

pointer---数组，顶点里，位置、纹理、法线的偏移量(字节长度)

着色器：shader

在计算机图形中颜色被表示为有4个元素的数组：红色、绿色、蓝色和alpha(透明度)分量，通常缩写为RGBA。

当在OpenGL或GLSL中定义一个颜色的时候，我们把颜色每个分量的强度设置在0.0到1.0之间。

比如说我们设置红为1.0f，绿为1.0f，我们会得到两个颜色的混合色，即黄色。

这三种颜色分量的不同调配可以生成超过1600万种不同的颜色！

顶点着色器：指定顶点的位置（需要能够给他们提供转换），每一个顶点调用一次顶点着色器

片段(像素)着色器：基本目的是决定像素的颜色是什么，会为每个像素运行一次去光栅化(画在屏幕上)   --损耗大

glShaderSource()  --length-> nullptr,以null结尾----每个字符都认定以null结尾

![image](https://user-images.githubusercontent.com/66365279/223400269-c3d1818a-3946-4bc8-93c9-30efebaddf74.png)

![image](https://user-images.githubusercontent.com/66365279/223400309-b2a59bec-8ee7-4404-9dc3-a4ae51e00d29.png)


索引缓冲区  ： 任何缓冲区，都必须由无符号的整数组成

关于错误检查

1、glGetError ---三种不同类型的错误标志，最兼容

（每个函数前清除错误，然后调用函数后使用glGetError获取是否有错误）

2、opengl 4.3中 添加了，glDebugMessageCall

1需要不停主动调用，2发生错误时被动通知

Uniforms

如果shader里的uniform没有使用，会被剥离，就会返回-1

glfwSwapInterval()---设置刷新帧率

关于glsl--错误

![image](https://user-images.githubusercontent.com/66365279/223693142-dfc56096-2c74-4af9-addf-34e079d002e7.png)

