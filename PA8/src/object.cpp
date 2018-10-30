#include "object.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>

#include <Magick++.h>

#include <algorithm>

//does this need to keep constructor and destructor?
Object::Object(const std::string & objFile, const glm::vec3 & translation, const glm::vec3 & rotationAngles,
		const glm::vec3 & scale, bool loadBtMesh) :
m_model(1.0), m_translation(translation), m_scale(scale), m_rotationAngles(rotationAngles), VB(0), mbt_mesh(
		nullptr) {
	//vertex attributes: vec3 position, vec3 color, vec2 uv, vec3 normal
	if (!loadObjAssimp(objFile, loadBtMesh)) {
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

	delete mbt_mesh;
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

		glDrawElements(GL_TRIANGLES, m_indices[i].size(), GL_UNSIGNED_INT, 0);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

glm::mat4 Object::GetModel(void) {
	return m_model;
}

void Object::SetName(const std::string & name) {
	m_name = name;
}

std::string Object::GetName(void) const {
	return m_name;
}

void Object::SetMass(unsigned int mass) {
	m_mass = mass;
}

unsigned int Object::GetMass(void) const {
	return m_mass;
}

float Object::GetDistanceFromPoint(glm::vec3 point) {
	return glm::distance(m_translation, point);
}

bool Object::loadObjAssimp(const std::string & objFile, bool loadBtMesh) {
	Assimp::Importer importer;
	const aiScene * scene = importer.ReadFile(objFile, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
	const aiMesh * currMesh;

	glm::vec3 tempVertex;

	glm::vec2 tempUV;
	const aiVector3D * uv;
	const aiMaterial * tempMat;
	aiString path;
	std::vector < aiString > textureFiles;

	btVector3 triArray[3];
	if (loadBtMesh)
		mbt_mesh = new btTriangleMesh();

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

					//load mesh into bullet if requested
					if (loadBtMesh) {
						triArray[indexNum] = btVector3(tempVertex.x, tempVertex.y, tempVertex.z);
						mbt_mesh->addTriangle(triArray[0], triArray[1], triArray[2]);
					}
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
