#include "shader.h"

#include <fstream>
#include <sstream>

std::string Shader::vertexFile = "";
std::string Shader::fragmentFile = "";

Shader::Shader(void) :
		m_shaderProg(0) {
}

Shader::~Shader(void) {
	for (std::vector<GLuint>::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); ++it)
		glDeleteShader(*it);

	if (m_shaderProg != 0)
		glDeleteProgram (m_shaderProg);
}

bool Shader::Initialize(void) {
	m_shaderProg = glCreateProgram();

	if (m_shaderProg == 0) {
		printf("Error creating shader program \n");
		return false;
	}

	return true;
}

// Use this method to add shaders to the program. When finished - call finalize()
bool Shader::AddShader(const GLenum ShaderType) {
	std::string source = Shader::LoadSourceCode(ShaderType);
	GLuint shaderObj = glCreateShader(ShaderType);

	if (shaderObj == 0) {
		printf("Error creating shader type: %d \n", ShaderType);
		return false;
	}

	// Save the shader object - will be deleted in the destructor
	m_shaderObjList.push_back(shaderObj);

	const GLchar * p[1] = { source.c_str() };
	GLint Lengths[1] = { (GLint) source.size() };

	glShaderSource(shaderObj, 1, p, Lengths);

	glCompileShader(shaderObj);

	GLint success;
	glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);

	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(shaderObj, 1024, NULL, InfoLog);
		printf("Error compiling shader: %s \n", InfoLog);
		return false;
	}

	glAttachShader(m_shaderProg, shaderObj);
	return true;
}

// After all the shaders have been added to the program call this function
// to link and validate the program.
bool Shader::Finalize(void) {
	GLint success = 0;
	GLchar errorLog[1024] = { 0 };

	glLinkProgram (m_shaderProg);

	glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_shaderProg, sizeof(errorLog), NULL, errorLog);
		printf("Error linking shader program: %s \n", errorLog);
		return false;
	}

	glValidateProgram(m_shaderProg);
	glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_shaderProg, sizeof(errorLog), NULL, errorLog);
		printf("Invalid shader program: %s \n", errorLog);
		return false;
	}

	// Delete the intermediate shader objects that have been added to the program
	for (std::vector<GLuint>::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); ++it)
		glDeleteShader(*it);

	m_shaderObjList.clear();

	return true;
}

void Shader::Enable(void) {
	glUseProgram (m_shaderProg);
}

GLint Shader::GetUniformLocation(const char * pUniformName) const {
	GLuint Location = glGetUniformLocation(m_shaderProg, pUniformName);

	if (Location == INVALID_UNIFORM_LOCATION)
		fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);

	return Location;
}

void Shader::SetVertexFile(const std::string & fileName) {
	vertexFile = fileName;
}

void Shader::SetFragmentFile(const std::string & fileName) {
	fragmentFile = fileName;
}

std::string Shader::LoadSourceCode(const GLenum ShaderType) {
	std::stringstream ss;
	std::string data = "";
	std::ifstream inputFile;

	if (ShaderType == GL_VERTEX_SHADER)
		inputFile.open(vertexFile);
	else if (ShaderType == GL_FRAGMENT_SHADER)
		inputFile.open(fragmentFile);

	if (!inputFile.is_open()) {
		printf("Could not open shader file for type: %d \n", ShaderType);
		return data;
	}

	ss << inputFile.rdbuf();
	inputFile.close();

	data = ss.str();
	return data;
}
