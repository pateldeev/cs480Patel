#ifndef SHADER_H
#define SHADER_H

#include <vector>
#include <fstream>
#include <sstream> 

#include "graphics_headers.h"

class Shader {
public:
	static void SetVertexFile(const std::string & fileName);
	static void SetFragmentFile(const std::string & fileName);
	static std::string ShaderLoader(GLenum ShaderType);

	Shader(void);
	~Shader(void);
	bool Initialize(void);
	void Enable(void);
	bool AddShader(GLenum ShaderType);
	bool Finalize(void);
	GLint GetUniformLocation(const char * pUniformName);

private:
	static std::string vertexFile;
	static std::string fragmentFile;

	GLuint m_shaderProg;
	std::vector<GLuint> m_shaderObjList;
};

#endif  /* SHADER_H */
