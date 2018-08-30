#ifndef SHADER_H
#define SHADER_H

#include <vector>
#include <fstream>
#include <sstream> 

#include "graphics_headers.h"

class Shader {
public:
	Shader();
	~Shader();
	bool Initialize();
	void Enable();
	bool AddShader(GLenum ShaderType, const std::string fileName = "");
	bool Finalize();
	GLint GetUniformLocation(const char* pUniformName);

private:
	GLuint m_shaderProg;
	std::vector<GLuint> m_shaderObjList;
};

#endif  /* SHADER_H */
