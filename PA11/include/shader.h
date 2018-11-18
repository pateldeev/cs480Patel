#ifndef SHADER_H
#define SHADER_H

#include "graphics_headers.h"

#include <string>
#include <vector> 

class Shader {
public:
	Shader(void);
	~Shader(void);

	//Shader is not meant to be copied or moved
	Shader(const Shader &) = delete;
	Shader(Shader &&) = delete;
	Shader& operator=(const Shader &) = delete;
	Shader& operator=(Shader&&) = delete;

	// Use this method to add shaders to the program. When finished - call finalize()
	void AddShader(const GLenum shaderType, const std::string & fileName);
	// After all the shaders have been added to the program call this function to link and validate the program.
	void Finalize(void);

	//Use the shaders
	void Enable(void);

	GLint GetUniformLocation(const std::string & pUniformName) const;

private:
	GLuint m_shaderProg;
	std::vector<GLuint> m_shaderObjList;

	//internal helper fucntion to load source code
	void LoadSourceCode(const std::string & fileName, std::string & src) const;
};

#endif  /* SHADER_H */
