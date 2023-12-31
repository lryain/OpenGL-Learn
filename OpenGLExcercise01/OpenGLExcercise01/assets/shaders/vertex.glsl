#version 330 core
layout(location = 6) in vec3 aPos;
layout(location = 7) in vec3 aColor;
out vec4 vertexColor;					// 为fragment shader指定一个颜色输出
void main() {
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	vertexColor = vec4(aColor.x, aColor.y, aColor.z, 1.0);
}
