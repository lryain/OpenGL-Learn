#include <iostream>
#define GLEW_STATIC

#include<GL\glew.h>
#include <GLFW\glfw3.h>

//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//	glViewport(0, 0, width, height);
//}

// 着色器源码
const char* vertexShaderSource =
"#version 330 core                                           \n"
// 从 location 0 位置去挖数据，然后塞给 aPos 这个变量
"layout(location = 6) in vec3 aPos;                          \n"
"void main() {												\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);}		\n";

const char* fragmentShaderSource =
"#version 330 core                                \n"
"out vec4 FragColor;                              \n"
"void main() {\n									   "
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);} 	 \n";

// 三角形顶点

float vertices0[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f,

	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f,
	 0.8f,  0.5f, 0.0f,
};

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f,
	 0.8f,  0.5f, 0.0f,
};

// 使用EBO
unsigned int indices[]{
	0,1,2,
	2,3,1 // 正序：2,1,3 逆序：2,3,1
};

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {
	//printf("Hello OpenGL!");
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 接下来我们来创造一个GLFW窗口
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello OpenGL", NULL, NULL);
	if (window == NULL) {
		printf("Init GLEW failed!");
		//std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// 使用这个窗口 作为当前窗口
	glfwMakeContextCurrent(window);

	//初始化 GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		printf("Init GLEW failed!");
		glfwTerminate();
		return -1;
	}

	// 视口
	glViewport(0, 0, 800, 600);

	// 启用剔除 此处的开启要配合三角形索引正序和逆序
	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);*/
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	unsigned int VAO[10];
	glGenVertexArrays(1, VAO); // 可以一次造多个VAO
	glBindVertexArray(VAO[0]); // 现在我们有了一个VAO，接下俩要绑定它

	// 0. 复制顶点数组到缓冲中供OpenGL使用
	unsigned int VBO[10];
	glGenBuffers(1, VBO); // 可以一次造多个VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]); // 绑定
	// 塞进GPU缓冲区 把用户定义的数据复制到目前绑定的buffer上去
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 声明EBO
	unsigned int EBO[10];
	glGenBuffers(1, EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//载入和编译 Shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // 创建 Shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // 载入源码
	glCompileShader(vertexShader); // 编译

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // 创建 Shader
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // 载入源码
	glCompileShader(fragmentShader); // 编译

	// 组装 Shader Program 附着，链接
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// 设置数据格式 0 对应 顶点着色器中使用layout(location = 0)
	// 第二个参数指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3
	// 第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔
	// 1. 设置顶点属性指针 
	// 这里相当于设置了 location = 0 的数据位 可以给着色器使用
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(6);

	// 窗口调整 函数回调

	// 渲染引擎
	// 如果用户没有关闭窗口就不要关闭窗口
	// glfwWindowShouldClose函数在我们每次循环的开始前检查一次GLFW是否被要求退出，如果是的话该函数返回true然后渲染循环便结束了，之后为我们就可以关闭应用程序了。
	while (!glfwWindowShouldClose(window))
	{
		// 按ESC键退出窗口
		processInput(window);

		//每次循环之前都要清屏，否则就会看到前一帧的内容
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f); // 设置要清屏的颜色
		glClear(GL_COLOR_BUFFER_BIT); // 只清除颜色

		// 在这里绘制物体
		glBindVertexArray(VAO[0]); // 绘制第一个VAO, 为什么又绑定一遍？
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
		glUseProgram(shaderProgram);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// 一直切换 DisplayBuffer 会交换两个色彩缓冲区：一个包含屏幕上所有像素颜色信息的缓冲区
		// 双缓冲：前缓冲保存着最终输出的图像，它会在屏幕上显示；而所有的的渲染指令都会在后缓冲上绘制。当所有的渲染指令执行完毕后，我们交换(Swap)前缓冲和后缓冲，这样图像就立即呈显出来，之前提到的不真实感就消除了。
		// glfwSwapBuffers函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上
		glfwSwapBuffers(window);
		// glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）。
		glfwPollEvents();
	}

	// 适当的删除我们分配的资源
	glfwTerminate();
	return 0;
}