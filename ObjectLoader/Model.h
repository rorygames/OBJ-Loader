#ifndef _MODEL_H
#define _MODEL_H

#include "GenericIncludes.h"

// Rory Clark
// https://rorywebdev.com
// 2016

class Model
{
public:

	Model();
	~Model();

	void LoadObj(std::string _fileName);
	void LoadTexture(std::string _fileName);
	void CreateModel();

	void Draw();

	std::vector<GLushort> GetElements() { return m_elements; };
	std::vector<glm::vec3> GetVertices() { return m_vertices; };
	std::vector<glm::vec2> GetUvs() { return m_uvs; };
	std::vector<glm::vec3> GetNormals() { return m_normals; };
	GLuint GetTextureID() { return m_textureID; };

	int GetVerticesCount() { return m_vertices.size(); };
	int GetUVCount() { return m_uvs.size(); };
	int GetNormalCount() { return m_normals.size(); };

private:

	std::vector<GLushort> m_elements;
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec2> m_uvs;
	std::vector<glm::vec3> m_normals;
	GLuint m_textureID;

	SDL_Surface* m_texture;

	std::vector<unsigned int> m_vertexIndex, m_normalIndex, m_textureIndex;

	GLuint m_vao;

};

#endif