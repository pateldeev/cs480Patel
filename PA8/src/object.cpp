#include "object.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>

#include <Magick++.h>

#include <algorithm>

//does this need to keep constructor and destructor?
Object::Object(const std::string & objFile) :
		m_model(1.0), m_translation(glm::vec3(0.0, 0.0, 0.0)), m_scale(glm::vec3(1.0, 1.0, 1.0)), m_rotationAngles(glm::vec3(0.0, 0.0, 0.0)), VB(0) {
	//vertex attributes: vec3 position, vec3 color, vec2 uv, vec3 normal
	if (!loadObjAssimp(objFile)) {
		printf("Object not properly loaded \n");
		return;
	}

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);

	for (int i = 0; i < IB.size(); ++i) {
		glGenBuffers(1, &IB[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indices[i].size(), &m_indices[i][0], GL_STATIC_DRAW);
	}

}

Object::~Object(void) {
	m_vertices.clear();

	for (std::vector<unsigned int> & temp : m_indices)
		temp.clear();
	m_indices.clear();

	m_textures.clear();
}

glm::mat4 Object::GetModel(void) {
	return m_model;
}

void Object::SetCurrentLocation(const glm::vec3 & location) {
	m_translation = location;
}

glm::vec3 Object::GetCurrentLocation(void) const {
	return m_translation;
}

void Object::SetScale(const glm::vec3 & scale) {
	m_scale = scale;
}

glm::vec3 Object::GetScale(void) const {
	return m_scale;
}

void Object::SetRotationAngles(const glm::vec3 & rotationAngles) {
	m_rotationAngles = rotationAngles;
}

glm::vec3 Object::GetRotationAngles(void) const {
	return m_rotationAngles;
}

void Object::SetName(const std::string & name) {
	m_name = name;
}

std::string Object::GetName(void) const {
	return m_name;
}

float Object::GetDistanceFromPoint(glm::vec3 point) {
	return glm::distance(m_translation, point);
}

bool Object::loadObjAssimp(const std::string & objFile) {
	Assimp::Importer importer;
	const aiScene * scene = importer.ReadFile(objFile, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
	const aiMesh * currMesh;

	glm::vec3 tempVertex;

	glm::vec2 tempUV;
	const aiVector3D * uv;
	const aiMaterial * tempMat;
	aiString path;
	std::vector < aiString > textureFiles;

	if (!scene) {
		printf("Error loading object: %s \n", importer.GetErrorString());
	} else {

		for (unsigned int meshNum = 0; meshNum < scene->mNumMeshes; ++meshNum) {
			currMesh = scene->mMeshes[meshNum];

			if (!currMesh->HasTextureCoords(0)) {
				printf("Object has no texture coordinates. Cannot load object \n");
				return false;
			}

			for (unsigned int faceNum = 0; faceNum < currMesh->mNumFaces; ++faceNum) {

				//iterate through each index of face - should be 3 for triangles
				for (unsigned int indexNum = 0; indexNum < currMesh->mFaces[faceNum].mNumIndices; ++indexNum) {
					tempVertex = {currMesh->mVertices[currMesh->mFaces[faceNum].mIndices[indexNum]].x, currMesh->mVertices[currMesh->mFaces[faceNum].mIndices[indexNum]].y, currMesh->mVertices[currMesh->mFaces[faceNum].mIndices[indexNum]].z};

					uv = &currMesh->mTextureCoords[0][currMesh->mFaces[faceNum].mIndices[indexNum]];
					tempUV = glm::vec2(uv->x,uv->y);

					//get texture file name
					tempMat = scene->mMaterials[currMesh->mMaterialIndex];
					tempMat->GetTexture(aiTextureType_DIFFUSE, 0, &path);

					//check if texture file has already been encourned and add new one if needed
					const std::vector<aiString>::const_iterator nameLoc = std::find(textureFiles.begin(), textureFiles.end(), path);
					std::size_t textureIndex = nameLoc - textureFiles.begin();
					if(textureIndex == textureFiles.size()) {
						textureFiles.push_back(path);
						std::vector<unsigned int> newIndicyArr;
						m_indices.push_back(newIndicyArr);
					}

					//add vertex and index to correct texture and vertex
					m_vertices.push_back(Vertex(tempVertex, tempUV));
					m_indices[textureIndex].push_back(m_vertices.size()-1);
				}
			}

			IB.resize(textureFiles.size());
		}
	}

	loadTextures(objFile, textureFiles);

	return true;
}

void Object::loadTextures(const std::string & objFile, const std::vector<aiString> & textureFiles) {
	GLuint tempTexture;
	Magick::Image * img;
	std::string fileNameStart = "";

	//get leading information on filename
	std::size_t tempPos = objFile.find_last_of('/');
	if (tempPos != std::string::npos)
		fileNameStart = objFile.substr(0, tempPos + 1);

	for (const aiString & path : textureFiles) {
		Magick::Blob blob;
		img = new Magick::Image(std::string(fileNameStart + path.C_Str()));
		img->flip();
		img->write(&blob, "RGBA");

		//store textures on GPU
		glGenTextures(1, &tempTexture);
		glBindTexture(GL_TEXTURE_2D, tempTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->columns(), img->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		delete img;

		m_textures.push_back(tempTexture);
	}
}
