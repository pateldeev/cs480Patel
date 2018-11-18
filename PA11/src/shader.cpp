#include "shader.h"

#include <fstream>
#include <sstream>

Shader::Shader(void) :
		m_shaderProg(0) {
	m_shaderProg = glCreateProgram();
	if (m_shaderProg == 0)
		throw std::string("Error creating shader program. Shader could not be created!");
}

Shader::~Shader(void) {
	for (std::vector<GLuint>::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); ++it)
		glDeleteShader(*it);

	if (m_shaderProg != 0)
		glDeleteProgram (m_shaderProg);
}

// Use this method to add shaders to the program. When finished - call finalize()
void Shader::AddShader(const GLenum shaderType, const std::string & fileName) {
	//load source code
	std::string source;
	LoadSourceCode(fileName, source);

	GLuint shaderObj = glCreateShader(shaderType);
	if (shaderObj == 0)
		throw std::string("Error creating shader type: " + std::to_string(shaderType));

	// Save the shader object - will be deleted in the destructor
	m_shaderObjList.push_back(shaderObj);

	//add source and compile
	const GLchar * src[1] = { source.c_str() };
	glShaderSource(shaderObj, 1, src, NULL);
	glCompileShader(shaderObj);

	GLint success;
	glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(shaderObj, 1024, NULL, InfoLog);
		std::string errMsg = "Error compiling shader: ";
		errMsg += InfoLog;
		throw errMsg;
	}

	glAttachShader(m_shaderProg, shaderObj);
}

// After all the shaders have been added to the program call this function to link and validate the program.
void Shader::Finalize(void) {
	GLint success = 0;
	GLchar errorLog[1024] = { 0 };

	glLinkProgram (m_shaderProg);

	glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_shaderProg, sizeof(errorLog), NULL, errorLog);
		std::string errMsg = "Error linking shader program: ";
		errMsg += errorLog;
		throw errMsg;
	}

	glValidateProgram(m_shaderProg);
	glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_shaderProg, sizeof(errorLog), NULL, errorLog);
		std::string errMsg = "Invalid shader program: ";
		errMsg += errorLog;
		throw errMsg;
	}

	// Delete the intermediate shader objects that have been added to the program
	for (std::vector<GLuint>::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); ++it)
		glDeleteShader(*it);

	m_shaderObjList.clear();
}

void Shader::Enable(void) {
	glUseProgram (m_shaderProg);
}

GLint Shader::GetUniformLocation(const std::string & pUniformName) const {
	GLint Location = glGetUniformLocation(m_shaderProg, pUniformName.c_str());

	if (Location == INVALID_UNIFORM_LOCATION)
		printf("Warning! Unable to get the location of uniform %s", pUniformName.c_str());

	return Location;
}

//internal helper fucntion to load source code
void Shader::LoadSourceCode(const std::string & fileName, std::string & src) const {
	std::ifstream inputFile(fileName);

	if (!inputFile.is_open())
		throw std::string("Could not open shader source file: " + fileName);

	std::stringstream ss;
	ss << inputFile.rdbuf();
	inputFile.close();

	src = ss.str(); //save source code
}
