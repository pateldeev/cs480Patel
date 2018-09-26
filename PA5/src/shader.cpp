#include "shader.h"

std::string Shader::vertexFile = "";
std::string Shader::fragmentFile = "";

void Shader::SetVertexFile(const std::string & fileName) {
	vertexFile = fileName;
}

void Shader::SetFragmentFile(const std::string & fileName) {
	fragmentFile = fileName;
}

std::string Shader::ShaderLoader(GLenum ShaderType) {
	std::stringstream ss;
	std::string data = "";
	std::ifstream inputFile;

	if (ShaderType == GL_VERTEX_SHADER)
		inputFile.open(vertexFile);
	else if (ShaderType == GL_FRAGMENT_SHADER)
		inputFile.open(fragmentFile);

	if (!inputFile.is_open()) {
		std::cerr << "Could not open shader file for type: " << ShaderType << std::endl;
		return data;
	}

	ss << inputFile.rdbuf();
	inputFile.close();
	data = ss.str();
	return data;
}

Shader::Shader(void) {
	m_shaderProg = 0;
}

Shader::~Shader(void) {
	for (std::vector<GLuint>::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++) {
		glDeleteShader(*it);
	}

	if (m_shaderProg != 0) {
		glDeleteProgram (m_shaderProg);
		m_shaderProg = 0;
	}
}

bool Shader::Initialize(void) {
	m_shaderProg = glCreateProgram();

	if (m_shaderProg == 0) {
		std::cerr << "Error creating shader program\n";
		return false;
	}

	return true;
}

// Use this method to add shaders to the program. When finished - call finalize()
bool Shader::AddShader(GLenum ShaderType) {
	std::string s = Shader::ShaderLoader(ShaderType);

	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		std::cerr << "Error creating shader type " << ShaderType << std::endl;
		return false;
	}

	// Save the shader object - will be deleted in the destructor
	m_shaderObjList.push_back(ShaderObj);

	const GLchar* p[1];
	p[0] = s.c_str();
	GLint Lengths[1] = { (GLint) s.size() };

	glShaderSource(ShaderObj, 1, p, Lengths);

	glCompileShader(ShaderObj);

	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		std::cerr << "Error compiling: " << InfoLog << std::endl;
		return false;
	}

	glAttachShader(m_shaderProg, ShaderObj);
	return true;
}

// After all the shaders have been added to the program call this function
// to link and validate the program.
bool Shader::Finalize(void) {
	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram (m_shaderProg);

	glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
		std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
		return false;
	}

	glValidateProgram(m_shaderProg);
	glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
		std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
		return false;
	}

	// Delete the intermediate shader objects that have been added to the program
	for (std::vector<GLuint>::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++) {
		glDeleteShader(*it);
	}

	m_shaderObjList.clear();

	return true;
}

void Shader::Enable(void) {
	glUseProgram (m_shaderProg);
}

GLint Shader::GetUniformLocation(const char * pUniformName) {
	GLuint Location = glGetUniformLocation(m_shaderProg, pUniformName);

	if (Location == INVALID_UNIFORM_LOCATION) {
		fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
	}

	return Location;
}
