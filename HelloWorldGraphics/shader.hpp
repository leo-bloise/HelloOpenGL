#ifndef CUSTOM_SHADER_H
#define CUSTOM_SHADER_H

class Shader {
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);

	void use();

};

#endif // !CUSTOM_SHADER_H
