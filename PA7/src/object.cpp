#include "object.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>

#include <Magick++.h>

#include <algorithm>

Object::Object(const std::string & objFile, float orbitRadiusX, float orbitRadiusZ, float rotationSpeed, float orbitSpeed) :
		m_model(1.0), angleRotation(0.0f), angleOrbit(0.0f), orbitRadiusX(orbitRadiusX), orbitRadiusZ(orbitRadiusZ), orbitCenter(
				glm::vec3(0.0f, 0.0f, 0.0f)), orbitLoc(glm::vec3(0.0f, 0.0f, 0.0f)), objectScale(glm::vec3(1.0f, 1.0f, 1.0f)), speedRotation(
				rotationSpeed), speedOrbit(orbitSpeed), pauseRotation(false), pauseOrbit(false) {

	//vertex attributes: vec3 position, vec3 color, vec2 uv, vec3 normal
	if (!loadObjAssimp(objFile)) {
		printf("Object not properly loaded \n");
		return;
	}

	loadTextures(objFile);

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	for (int i = 0; i < IB.size(); ++i) {
		glGenBuffers(1, &IB[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices[i].size(), &Indices[i][0], GL_STATIC_DRAW);
	}
}

Object::~Object(void) {
	Vertices.clear();
	for (std::vector<unsigned int> & temp : Indices)
		temp.clear();
	Indices.clear();
}

void Object::Update(unsigned int dt) {

	if (!pauseRotation)
		angleRotation += dt * M_PI * speedRotation;

	if (!pauseOrbit)
		angleOrbit += dt * M_PI * speedOrbit;

	orbitLoc = orbitCenter + glm::vec3(orbitRadiusX * std::cos(angleOrbit), 0.0, orbitRadiusZ * std::sin(angleOrbit));

	//m_model = glm::scale(objectScale); //model = glm::rotate(model, (angleRotation), glm::vec3(0.0, 1.0, 0.0)); //model = glm::translate(model, orbitLoc);
	m_model = glm::translate(orbitLoc) * glm::rotate((angleRotation), glm::vec3(0.0, 1.0, 0.0)) * glm::scale(objectScale);
}

void Object::Render(void) {
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, m_texture));

	for (int i = 0; i < IB.size(); ++i) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB[i]);

		glActiveTexture (GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_textures[i]);

		glDrawElements(GL_TRIANGLES, Indices[i].size(), GL_UNSIGNED_INT, 0);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

glm::mat4 Object::GetModel(void) {
	return m_model;
}

void Object::SetRotationSpeed(float rotationSpeed) {
	speedRotation = rotationSpeed;
}

float Object::GetRotationSpeed(void) const {
	return speedRotation;
}

void Object::SetOrbitSpeed(float orbitSpeed) {
	speedOrbit = orbitSpeed;
}

float Object::GetOrbitSpeed(void) const {
	return speedOrbit;
}

void Object::ToggleRotation(bool rotate) {
	pauseRotation = !rotate;
}

bool Object::IsRotating(void) const {
	return !pauseRotation;
}

void Object::ToggleOrbit(bool orbit) {
	pauseOrbit = !orbit;
}

bool Object::IsOrbiting(void) const {
	return !pauseOrbit;
}

void Object::SetOrbitCenter(const glm::vec3 & center) {
	orbitCenter = center;
}

glm::vec3 Object::GetOrbitCenter(void) const {
	return orbitCenter;
}

glm::vec3 Object::GetOrbitLoc(void) const {
	return orbitLoc;
}

void Object::SetScale(const glm::vec3 & scale) {
	objectScale = scale;
}

glm::vec3 Object::GetScale(void) const {
	return objectScale;
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
					const std::vector<aiString>::const_iterator nameLoc = std::find(m_textureFiles.begin(), m_textureFiles.end(), path);
					std::size_t textureIndex = nameLoc - m_textureFiles.begin();
					if(textureIndex == m_textureFiles.size()) {
						m_textureFiles.push_back(path);
						std::vector<unsigned int> newIndicyArr;
						Indices.push_back(newIndicyArr);
					}

					//add vertex and index to correct texture and vertex
					Vertices.push_back(Vertex(tempVertex, tempUV));
					Indices[textureIndex].push_back(Vertices.size()-1);
				}
			}

			IB.resize(m_textureFiles.size());
		}
	}
	return true;
}

void Object::loadTextures(const std::string & objFile) {
	GLuint tempTexture;
	Magick::Image * img;
	std::string fileNameStart = "";

	//get leading information on filename
	std::size_t tempPos = objFile.find_last_of('/');
	if (tempPos != std::string::npos)
		fileNameStart = objFile.substr(0, tempPos + 1);

	for (const aiString & path : m_textureFiles) {
		Magick::Blob blob;
		img = new Magick::Image(std::string(fileNameStart + path.C_Str()));
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
