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
	bool AddShader(const GLenum ShaderType, const std::string & fileName);
	bool Finalize(void);
	GLint GetUniformLocation(const char * pUniformName) const;	
private:
	GLuint m_shaderProg;
	std::vector<GLuint> m_shaderObjList;

	bool LoadSourceCode(const std::string & fileName, std::string & src) const;

};

#endif  /* SHADER_H */
