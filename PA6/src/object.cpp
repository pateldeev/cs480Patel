#include "object.h"

#include <Magick++.h>

Object::Object(const std::string & objFile) :
		m_model(1.0), m_translation(0.0, 0.0, 0.0), m_scale(1.0, 1.0, 1.0), m_rotationAngles(0.0, 0.0, 0.0), m_scene(nullptr) {

	//vertex attributes: vec3 position, vec3 color, vec2 uv, vec3 normal
	loadObjAssimp(objFile);
	loadTexture("/home/dp/Desktop/CS480_Workspace/cs480Patel/PA6/objFiles/checker.jpg", texture);

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}

Object::~Object(void) {
	Vertices.clear();
	Indices.clear();
}

void Object::Update(void) {
	Update(m_translation, m_scale, m_rotationAngles);
}

void Object::Update(const glm::vec3 & translation, const glm::vec3 & scale, const glm::vec3 & rotationAngles) {
	m_translation = translation;
	m_scale = scale;
	m_rotationAngles = rotationAngles;

	glm::mat4 rotationMat = glm::rotate((rotationAngles.x), glm::vec3(1.0, 0.0, 0.0)) * glm::rotate((rotationAngles.z), glm::vec3(0.0, 0.0, 1.0))
			* glm::rotate((rotationAngles.y), glm::vec3(0.0, 1.0, 0.0));

	m_model = glm::translate(m_translation) * rotationMat * glm::scale(m_scale);
}

void Object::Render(void) {
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, m_texture));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

glm::mat4 Object::GetModel(void) {
	return m_model;
}

glm::vec3 Object::GetTranslation(void) const {
	return m_translation;
}

glm::vec3 Object::GetScale(void) const {
	return m_scale;
}

glm::vec3 Object::GetRotationAngles(void) const {
	return m_rotationAngles;
}

void Object::loadObjAssimp(const std::string & objFile) {

	m_scene = m_importer.ReadFile(objFile, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
	const aiMesh * currMesh;
	glm::vec3 tempVertex;
	glm::vec2 tempUV;

	if (!m_scene) {
		printf("Error loading object: %s \n", m_importer.GetErrorString());
	} else {

		for (unsigned int meshNum = 0; meshNum < m_scene->mNumMeshes; ++meshNum) {
			currMesh = m_scene->mMeshes[meshNum];

			if (!currMesh->HasTextureCoords(0)) {
				printf("Object has no texture coordinates. Cannot load object \n");
				return;
			}

			for (unsigned int faceNum = 0; faceNum < currMesh->mNumFaces; ++faceNum) {

				//iterate through each index of face - should be 3 for triangles
				for (unsigned int indexNum = 0; indexNum < currMesh->mFaces[faceNum].mNumIndices; ++indexNum) {
					tempVertex = {currMesh->mVertices[currMesh->mFaces[faceNum].mIndices[indexNum]].x, currMesh->mVertices[currMesh->mFaces[faceNum].mIndices[indexNum]].y, currMesh->mVertices[currMesh->mFaces[faceNum].mIndices[indexNum]].z};

					const aiVector3D uv = currMesh->mTextureCoords[0][currMesh->mFaces[faceNum].mIndices[indexNum]];
					tempUV = glm::vec2(uv.x,uv.y);

					const aiMaterial * tempMat = m_scene->mMaterials[currMesh->mMaterialIndex];
					aiString path;
					tempMat->GetTexture(aiTextureType_DIFFUSE, 0, &path);

					Vertices.push_back(Vertex(tempVertex, tempUV));
					Indices.push_back(Vertices.size()-1);
				}
			}
		}
	}
}

void Object::loadTexture(const char * file, GLuint & texture){
	Magick::Blob blob;
	Magick::Image * img;
	img = new Magick::Image(file);
	img->write(&blob, "RGBA");
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,img->columns(), img->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	
	delete img;
}
