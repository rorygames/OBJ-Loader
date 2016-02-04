#include "Shader.h"

// Rory Clark
// https://rorywebdev.com
// 2016

Shader::Shader()
{
}

Shader::~Shader()
{
	m_vertexCode.clear();
	m_fragmentCode.clear();
}

void Shader::AddFileNames(std::string _vname, std::string _fname)
{
	// Get out file names
	m_sPFragment = _fname;
	m_sPVertex = _vname;
}


/// The following section references parts but not whole from :-
/// Opengl-tutorials (2015). Tutorial 8: Basic Shading [online]. [Accessed 2015]. 
/// Available from: <http://www.opengl-tutorial.org/beginners-tutorials/tutorial-8-basic-shading/>.
void Shader::LoadShaders()
{
	// Create the shaders in opengl
	m_vertexID = glCreateShader(GL_VERTEX_SHADER);
	m_fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

	m_vertexCode = "";

	// Read in our vertex shader code into our vertex code string from the vertex shader file
	std::ifstream vertexShaderStream(m_sPVertex, std::ios::in);
	if (vertexShaderStream.is_open())
	{
		std::string line = "";
		while (std::getline(vertexShaderStream, line))
			m_vertexCode += "\n" + line;
		vertexShaderStream.close();
	}
	else
	{
		// Error if we can't read the file
		throw("Impossible to read vertex shader.\nIs the vertex shader file present?\n");
	}

	// Wipe the stream
	vertexShaderStream.clear();

	m_fragmentCode = "";

	// Read in our fragment shader code into our fragment code string from the fragment shader file
	std::ifstream fragmentShaderStream(m_sPFragment, std::ios::in);
	if (fragmentShaderStream.is_open())
	{
		std::string line = "";
		while (std::getline(fragmentShaderStream, line))
			m_fragmentCode += "\n" + line;
		fragmentShaderStream.close();
	}
	else
	{
		throw("Impossible to read fragment shader.\nIs the fragment shader file present?\n");
	}
}

void Shader::CompileShaders()
{
	// Load the files holding the shaders
	this->LoadShaders();

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader from the loaded vertex code
	char const * vPointer = m_vertexCode.c_str();
	glShaderSource(m_vertexID, 1, &vPointer, NULL);
	// Tell OpenGL to compile the shader at the memory location
	glCompileShader(m_vertexID);

	// Check Vertex Shader
	if (!CheckShaderCompiled(m_vertexID))
	{
		throw("Vertex shader compiled incorrectly.\nCheck console for more information.");
	}

	// Compile Fragment Shader
	char const * fPointer = m_fragmentCode.c_str();
	glShaderSource(m_fragmentID, 1, &fPointer, NULL);
	glCompileShader(m_fragmentID);

	// Check Vertex Shader
	if (!CheckShaderCompiled(m_fragmentID))
	{
		throw("Fragment shader compiled incorrectly.\nCheck console for more information.");
	}


	// Link the program
	m_shaderProgram = glCreateProgram();
	// Attach the shaders we just made 
	glAttachShader(m_shaderProgram, m_vertexID);
	glAttachShader(m_shaderProgram, m_fragmentID);
	// Link the program we just made with the attached shaders
	glLinkProgram(m_shaderProgram);

	// Check the program
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &Result);
	glGetProgramiv(m_shaderProgram, GL_INFO_LOG_LENGTH, &InfoLogLength);

	// Certain graphics cards report a "0" as the info log length while others return blank
	if (InfoLogLength > 1)
	{
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(m_shaderProgram, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("Program Error: %s\n", &ProgramErrorMessage[0]);
	}

	// No need to keep hold of the shader connection as it's loaded into the memory
	glDetachShader(m_shaderProgram, m_vertexID);
	glDetachShader(m_shaderProgram, m_fragmentID);

	// Delete the temporary data of the shaders
	glDeleteShader(m_fragmentID);
	glDeleteShader(m_vertexID);

}

// End of citation

bool Shader::CheckShaderCompiled(GLint _shader)
{
	GLint compiled;
	glGetShaderiv(_shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLsizei len;
		glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, &len);

		// OpenGL will store an error message as a string that we can retrieve and print
		GLchar* log = new GLchar[len + 1];
		glGetShaderInfoLog(_shader, len, &len, log);
		std::cerr << "ERROR: Shader compilation failed: " << log << std::endl;
		delete[] log;

		return false;
	}
	return true;
}

int Shader::GetULocation(std::string _name)
{
	// Get the location of our uniform in the shader
	return glGetUniformLocation(m_shaderProgram, _name.c_str());
}