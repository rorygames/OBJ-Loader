#ifndef _SHADER_H
#define _SHADER_H

#include "GenericIncludes.h"

// Rory Clark
// https://rorywebdev.com
// 2016

class Shader
{
public:

	Shader();
	~Shader();

	void AddFileNames(std::string _vname, std::string _fname);

	void CompileShaders();
	GLuint GetShaderProg() { return m_shaderProgram; };
	int GetULocation(std::string _name);

private:

	GLuint m_shaderProgram;
	std::string m_sPVertex, m_sPFragment;

	std::string m_vertexCode, m_fragmentCode;

	GLuint m_vertexID, m_fragmentID;

	void LoadShaders();

	bool CheckShaderCompiled(GLint _shader);
};

#endif