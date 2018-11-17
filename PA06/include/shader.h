#ifndef SHADER_H
#define SHADER_H

#include "graphics_headers.h"

#include <string>
#include <vector> 

class Shader {
public:
	Shader(void);
	~Shader(void);
	bool Initialize(void);
	void Enable(void);
	bool AddShader(const GLenum ShaderType);
	bool Finalize(void);
	GLint GetUniformLocation(const char * pUniformName) const;

	static void SetVertexFile(const std::string & fileName);
	static void SetFragmentFile(const std::string & fileName);
	static std::string LoadSourceCode(const GLenum ShaderType);
	
private:
	static std::string vertexFile;
	static std::string fragmentFile;

	GLuint m_shaderProg;
	std::vector<GLuint> m_shaderObjList;
};

#endif  /* SHADER_H */
