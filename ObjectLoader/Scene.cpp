#include "Scene.h"

// Rory Clark
// https://rorywebdev.com
// 2016

Scene::Scene(int _w, int _h)
{
	// This matrix is responsible for the size, position and rotation of our model directly
	m_modelMatrix = glm::mat4(1.0f);

	// Set up the viewing matrix
	// This is the matrix that views the object
	m_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3.5f));

	// Our light position vector, minus values are used because we are applying it to the projection matrix in the shader
	// The project matrix is looking at 0,0,0 so minus values are closer and positive are further away
	m_lightPos = glm::vec3(-4, 4, -5);

	// Our default diffuse colour vector for when we don't want to use textures
	m_diffuseColour = glm::vec3(0.0f, 0.5f, 1.0f);

	// Set up a projection matrix
	m_screenW = _w;
	m_screenH = _h;
	// We create our projection matrix and use the screen sizes to correctly produce our aspect ratio
	m_projMatrix = glm::perspective(45.0f, (float)_w/_h, 0.1f, 100.0f);

	// Light power value
	m_lightPower = 75.0f;

	// Default is to expect a texture but this changes if necessary later
	m_isTexture = 1;

	// Create the shader
	m_shader = new Shader();
}

Scene::~Scene()
{
	delete m_shader;
}

void Scene::LoadShaderFiles(std::string _vname, std::string _fname)
{
	// Load the shader file names
	m_shader->AddFileNames(_vname, _fname);
}

void Scene::CreateShader()
{
	// Compile the shaders and get all the memory locations for our uniform values within the shaders
	m_shader->CompileShaders();
	glUseProgram(m_shader->GetShaderProg());
	m_viewMatLoc = m_shader->GetULocation("viewMat");
	m_modelMatLoc = m_shader->GetULocation("modelMat");
	m_projMatLoc = m_shader->GetULocation("projMat");
	m_lightVecLoc = m_shader->GetULocation("lightVec");
	m_lightPowerLoc = m_shader->GetULocation("lightPower");
	m_textureLoc = m_shader->GetULocation("modelTexture");
	m_isTextureLoc = m_shader->GetULocation("isTexture");
	m_diffuseColourLoc = m_shader->GetULocation("diffuseColour");
	glUseProgram(0);
}

void Scene::SwapTexture()
{
	// Enable/disable the texture if we have it loaded into memory
	if (m_textureEnabled)
	{
		if (m_isTexture == 0)
		{
			m_isTexture = 1;
		}
		else
		{ 
			m_isTexture = 0;
		}
	} 
}

void Scene::Update(float _delta)
{
	if (m_cameraRotX != 0)
	{
		// Rotate the model matrix on the X axis
		m_modelMatrix = glm::rotate(m_modelMatrix,m_cameraRotX*_delta, glm::vec3(0, 1, 0));
	}
	if (m_cameraRotY != 0)
	{
		// Rotate the view matrix on the Y axis
		m_viewMatrix = glm::rotate(m_viewMatrix, m_cameraRotY*_delta, glm::vec3(1, 0, 0));		 
	}
	if (m_cameraZoom != 0)
	{
		// Scale the model in general when "zooming"
		// There are a number of ways to do this however this will mean lighting can always be applied even if the model is initially outside of the lighting position
		m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(1 + ((m_cameraZoom*.75f)*_delta), 1 + ((m_cameraZoom*.75f)*_delta), 1 + ((m_cameraZoom*.75f)*_delta)));
	}
	if (m_sceneLight != 0)
	{
		// Change the light power
		m_lightPower += m_sceneLight * _delta;
	}
	if (m_isTexture == 1)
	{
		// Change the scene colours if we're currently not using the texture values
		// Clamped to 0 and 1 so that we don't have errors in the shader
		if (m_rColour != 0)
		{
			m_diffuseColour.r += m_rColour *_delta;
			if (m_diffuseColour.r > 1)
			{
				m_diffuseColour.r = 1;
			}
			if (m_diffuseColour.r < 0)
			{
				m_diffuseColour.r = 0;
			}
		}
		if (m_gColour != 0)
		{
			m_diffuseColour.g += m_gColour *_delta;
			if (m_diffuseColour.g > 1)
			{
				m_diffuseColour.g = 1;
			}
			if (m_diffuseColour.g < 0)
			{
				m_diffuseColour.g = 0;
			}
		}
		if (m_bColour != 0)
		{
			m_diffuseColour.b += m_bColour *_delta;
			if (m_diffuseColour.b > 1)
			{
				m_diffuseColour.b = 1;
			}
			if (m_diffuseColour.b < 0)
			{
				m_diffuseColour.b = 0;
			}
		}
	}
	
}

void Scene::Draw()
{
	// Need to use the program so we can access the memory
	glUseProgram(m_shader->GetShaderProg());

	// Pass through our values to the memory locations
	// 4fv = mat4
	// 3f = vec3
	// 1f = float
	// 1i = int

	glUniformMatrix4fv(m_modelMatLoc, 1, GL_FALSE, glm::value_ptr(m_modelMatrix));
	glUniformMatrix4fv(m_viewMatLoc, 1, GL_FALSE, glm::value_ptr(m_viewMatrix));
	glUniformMatrix4fv(m_projMatLoc, 1, GL_FALSE, glm::value_ptr(m_projMatrix));
	glUniform1f(m_lightPowerLoc, m_lightPower);
	glUniform3f(m_lightVecLoc, m_lightPos.x, m_lightPos.y, m_lightPos.z);
	if (m_isTexture == 0)
	{
		glUniform1i(m_isTextureLoc, 0);
	}
	else
	{
		glUniform1i(m_isTextureLoc, 1);
	}
	
	glUniform3f(m_diffuseColourLoc, m_diffuseColour.r, m_diffuseColour.g, m_diffuseColour.b);

	// Draw the model
	OBJModel->Draw();

	// Stop using the program
	glUseProgram(0);
}