#include "object.h"

Object::Object(const std::string & objFile, bool readColor) :
		m_model(1.0), m_translation(0.0, 0.0, 0.0), m_scale(1.0, 1.0, 1.0), m_rotationAngles(0.0, 0.0, 0.0), m_scene(nullptr) {

	loadObjAssimp(objFile, readColor);

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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, color));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

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

void Object::loadObjAssimp(const std::string & objFile, bool readColor) {

	std::srand(time(0));

	m_scene = m_importer.ReadFile(objFile, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
	const aiMesh * currMesh;
	glm::vec3 tempVertex, tempColor;

	if (!m_scene) {
		printf("Error loading object: %s \n", m_importer.GetErrorString());
	} else {

		for (unsigned int meshNum = 0; meshNum < m_scene->mNumMeshes; ++meshNum) {
			currMesh = m_scene->mMeshes[meshNum];

			for (unsigned int faceNum = 0; faceNum < currMesh->mNumFaces; ++faceNum) {

				//iterate through each index of face - should be 3 for triangles
				for (unsigned int indexNum = 0; indexNum < currMesh->mFaces[faceNum].mNumIndices; ++indexNum) {
					tempVertex = {currMesh->mVertices[currMesh->mFaces[faceNum].mIndices[indexNum]].x, currMesh->mVertices[currMesh->mFaces[faceNum].mIndices[indexNum]].y, currMesh->mVertices[currMesh->mFaces[faceNum].mIndices[indexNum]].z};

					//get color
					if(!readColor) {
						tempColor = glm::vec3(std::rand() / float(RAND_MAX), std::rand() / float(RAND_MAX), std::rand() / float(RAND_MAX));
					} else {
						const aiMaterial * tempMat = m_scene->mMaterials[currMesh->mMaterialIndex];
						aiColor4D diffuse;
						
						if (AI_SUCCESS == aiGetMaterialColor(tempMat, AI_MATKEY_COLOR_DIFFUSE, &diffuse)) {
							tempColor = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
						} else {
							printf("Error getting material color a vertex. Using default color of black\n");
							tempColor = glm::vec3(0.0, 0.0, 0.0);
						}
					}

					Vertices.push_back(Vertex(tempVertex, tempColor));
					Indices.push_back(Vertices.size()-1);
				}
			}
		}
	}
}
