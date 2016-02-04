#ifndef _SCENE_H
#define _SCENE_H

#include "Model.h"
#include "Shader.h"

// Rory Clark
// https://rorywebdev.com
// 2016

class Scene
{
public:

	Scene(int _w, int _h);
	~Scene();

	void LoadShaderFiles(std::string _vname, std::string _fname);
	bool LoadTexture(std::string _filename);

	void CreateShader();

	void SetTextureEnabled(bool _value) { m_textureEnabled = _value; if (m_textureEnabled) { m_isTexture = 0; }; };
	void SetCameraRotX(int _value) { m_cameraRotX = _value; };
	void SetCameraRotY(int _value) { m_cameraRotY = _value; };
	void SetCameraZoom(int _value) { m_cameraZoom = _value; };
	void SetSceneLight(int _value) { m_sceneLight = _value; };
	void SetRColour(float _value) { m_rColour = _value; };
	void SetGColour(float _value) { m_gColour = _value; };
	void SetBColour(float _value) { m_bColour = _value; };
	void SwapTexture();

	float GetLightPower() { return m_lightPower; };
	bool GetIsTexture() { if (m_isTexture == 0) { return true; } return false; };
	float GetRedColour() { return m_diffuseColour.r; };
	float GetGreenColour() { return m_diffuseColour.g; };
	float GetBlueColour() { return m_diffuseColour.b; };

	void Update(float _delta);

	void Draw();

	Model* OBJModel;

private:

	Shader* m_shader;

	int m_screenW, m_screenH;

	float m_cameraRotX, m_cameraRotY, m_cameraZoom, m_sceneLight,m_rColour,m_gColour,m_bColour;

	float m_lightPower;

	GLuint m_viewMatLoc, m_projMatLoc, m_lightVecLoc, m_modelMatLoc,m_lightPowerLoc,m_textureLoc,m_isTextureLoc,m_diffuseColourLoc;

	glm::mat4 m_mvpMatrix, m_modelMatrix, m_viewMatrix, m_projMatrix;

	glm::vec3 m_lightPos,m_diffuseColour;

	bool m_textureEnabled;
	int m_isTexture;

};

#endif