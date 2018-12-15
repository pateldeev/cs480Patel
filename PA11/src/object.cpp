#include "object.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>

#include <Magick++.h>

Object::Object(const std::string & objFile, const glm::uvec2 & size, const glm::vec3 & changeRow, const glm::vec3 & changeCol,
		const glm::vec3 & traslation, const glm::vec3 & rotation, const glm::vec3 & scale) :
		m_model(1.0), m_translation(traslation), m_rotation(rotation), m_scale(scale), m_normal(glm::normalize(glm::cross(changeRow, changeCol))), m_numInstances(
				size), m_changeRow(changeRow), m_changeCol(changeCol), VB(0), IB(0) {

	//vertex attributes: vec3 position, vec3 color, vec2 uv, vec3 normal
	LoadObjAssimp(objFile);

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW);

	//load dead texture by default
	m_types.resize(m_numInstances.x * m_numInstances.y, DEAD);

	//calculate instance positions
	m_instancePositions.resize(m_numInstances.x * m_numInstances.y);
	for (unsigned int r = 0; r < m_numInstances.x; ++r) //row
		for (unsigned int c = 0; c < m_numInstances.y; ++c) //col
			m_instancePositions[r * m_numInstances.y + c] = (m_translation + (m_changeRow * float(r)) + (m_changeCol * float(c)));
}

Object::~Object(void) {
}

void Object::Update(void) {
	glm::mat4 translate = glm::translate(glm::mat4(1.0), m_translation);
	glm::mat4 rotate = glm::mat4_cast(glm::quat(m_rotation));
	glm::mat4 scale = glm::scale(glm::mat4(1.0), m_scale);

	m_model = translate * rotate * scale;
}

void Object::Render(void) {
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, m_vertex));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, m_normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, m_texture));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	glDrawElementsInstanced(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0, m_numInstances.x * m_numInstances.y);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

void Object::BindTextures(void) {
	//bind textures
	glActiveTexture (GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textures[DEAD]);
	glActiveTexture (GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_textures[P1_ALIVE_FUTURE]);
	glActiveTexture (GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_textures[P2_ALIVE_FUTURE]);
	glActiveTexture (GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_textures[P1_ALIVE]);
	glActiveTexture (GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_textures[P2_ALIVE]);
	glActiveTexture (GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_textures[P1_DEAD_FUTURE]);
	glActiveTexture (GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, m_textures[P2_DEAD_FUTURE]);
	glActiveTexture (GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, m_textures[P1_ALIVE_MARKED]);
	glActiveTexture (GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, m_textures[P2_ALIVE_MARKED]);
	glActiveTexture (GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, m_textures[P1_DEAD_MARKED]);
	glActiveTexture (GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, m_textures[P2_DEAD_MARKED]);
}

glm::mat4 Object::GetModel(void) {
	return m_model;
}

glm::vec3 Object::GetTranslation(void) const {
	return m_translation;
}

void Object::SetTranslation(const glm::vec3 & translation) {
	m_translation = translation;
}

glm::vec3 Object::GetRotation(void) const {
	return m_rotation;
}

void Object::SetRotation(const glm::vec3 & rotation) {
	m_rotation = rotation;
}

glm::vec3 Object::GetScale(void) const {
	return m_scale;
}

glm::vec3 Object::GetNormal(void) const {
	return m_normal;
}

void Object::SetScale(const glm::vec3 & scale) {
	m_scale = scale;
}

ObjType Object::GetType(unsigned int r, unsigned int c) const {
	if (r >= m_numInstances.x || c >= m_numInstances.y) {
		std::string err = "Location: ";
		err += std::to_string(r);
		err += ",";
		err += std::to_string(c);
		err += " is not a valid instance";
		throw err;
	}
	return m_types[r * m_numInstances.y + c];
}

const std::vector<ObjType>& Object::GetTypesList(void) const {
	return m_types;
}

void Object::SetType(unsigned int r, unsigned int c, const ObjType type) {
	if (r >= m_numInstances.x || c >= m_numInstances.y) {
		std::string err = "Location: ";
		err += std::to_string(r);
		err += ",";
		err += std::to_string(c);
		err += " is not a valid instance";
		throw err;
	}
	m_types[r * m_numInstances.y + c] = type;
}

const std::vector<glm::vec3>& Object::GetInstancePositions(void) const {
	return m_instancePositions;
}

//use to find cube at position - return {r,c}
glm::uvec2 Object::GetCubeByPosition(const glm::vec3 & position) const {
	for (unsigned int r = 0; r < m_numInstances.x; ++r) { //row
		for (unsigned int c = 0; c < m_numInstances.y; ++c) { //col
			if (m_instancePositions[r * m_numInstances.y + c] == position)
				return glm::uvec2(r, c);
		}
	}
	throw std::string("NOT FOUND");
}

glm::uvec2 Object::GetSize(void) const {
	return m_numInstances;
}

//checks if given element is valid in regards to bounds of instance sizes
bool Object::IsValidElement(const glm::uvec2 & pos) const {
	return (pos.x < m_numInstances.x && pos.y < m_numInstances.y);
}

glm::vec3 Object::GetChangeRow(void) const {
	return m_changeRow;
}

glm::vec3 Object::GetChangeCol(void) const {
	return m_changeCol;
}

void Object::LoadTexture(const std::string & textureFile, const ObjType type) {
	Magick::Image * img;

	try {
		img = new Magick::Image(textureFile);
		img->flip();

		Magick::Blob blob;
		img->write(&blob, "RGBA");

		//store textures on GPU
		glGenTextures(1, &m_textures[type]);
		glBindTexture(GL_TEXTURE_2D, m_textures[type]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->columns(), img->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		delete img;
	} catch (...) {
		printf("Could not load texture: %s\n", textureFile.c_str());
		throw;
	}
}

void Object::LoadObjAssimp(const std::string & objFile) {
	Assimp::Importer importer;
	const aiScene * scene = importer.ReadFile(objFile, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
	const aiMesh * currMesh;

	const aiVector3D *tempVertex, *tempNormal, *tempUV;

	if (!scene)
		throw std::string("Error loading object: " + std::string(importer.GetErrorString()));

	for (unsigned int meshNum = 0; meshNum < scene->mNumMeshes; ++meshNum) {
		currMesh = scene->mMeshes[meshNum];

		if (!currMesh->HasTextureCoords(0))
			throw std::string("Object has no texture coordinates. Cannot load object");

		for (unsigned int faceNum = 0; faceNum < currMesh->mNumFaces; ++faceNum) {

			//iterate through each index of face - should be 3 for triangles
			for (unsigned int indexNum = 0; indexNum < currMesh->mFaces[faceNum].mNumIndices; ++indexNum) {
				tempVertex = &currMesh->mVertices[currMesh->mFaces[faceNum].mIndices[indexNum]];
				tempNormal = &currMesh->mNormals[currMesh->mFaces[faceNum].mIndices[indexNum]];
				tempUV = &currMesh->mTextureCoords[0][currMesh->mFaces[faceNum].mIndices[indexNum]];

				//add vertex and index to internal array
				m_vertices.emplace_back(glm::vec3(tempVertex->x, tempVertex->y, tempVertex->z),
						glm::vec3(tempNormal->x, tempNormal->y, tempNormal->z), glm::vec2(tempUV->x, tempUV->y));
				m_indices.push_back(m_vertices.size() - 1);
			}
		}
	}
}
