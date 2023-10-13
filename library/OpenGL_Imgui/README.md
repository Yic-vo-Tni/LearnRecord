### GLEW链接未定义的问题（未解决）
glew仍有未定义的问题，静态库动态库全链接了一遍还是未定义，真不知道为啥，属实气人o(╥﹏╥)o，等以后哪天再收拾你。

还是glad好用，啥都不用配(*^▽^*)。

### Imgui初始化问题
也是一堆未定义，同时需要删除不需要的文件，未定义解决办法：

```cmake
file(GLOB_RECURSE IMGUI_SRCS ../.../OpenGL_Imgui/*.cpp)
add_executable(LearnImgui main.cpp glad.c  ${IMGUI_SRCS} )
```
