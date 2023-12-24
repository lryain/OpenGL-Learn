#pragma once

#include <GL\glew.h>
#include <iostream>
#include <fstream>
#include <sstream>

// 这个类使用起来很简单：只要创建一个着色器对象，从那一点开始我们就可以开始使用了
// 
class Shader
{
public:
	// 程序 ProgramID
	unsigned int ID;
	// 构造方法，读取并构建着色器
	Shader(const char* vertexPath, const char* fragmentPath);
	// 使用、激活程序
	void use();
	// uniform 工具函数
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
};