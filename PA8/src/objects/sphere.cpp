#include "objects/sphere.h"

Sphere::Sphere(const std::string & objFile, unsigned int mass, const glm::vec3 & translation, const glm::vec3 & rotationAngles,
		const glm::vec3 & scale) :
		Object(objFile, translation, rotationAngles, scale), mbt_rigidBody(nullptr) {
	SetMass(mass);
}

Sphere::~Sphere(void) {
	m_vertices.clear();

	for (std::vector<unsigned int> & temp : m_indices)
		temp.clear();
	m_indices.clear();

	m_textures.clear();
}

void Sphere::IntializeBt(btDiscreteDynamicsWorld * dynamicsWorld) {

	btCollisionShape * shape = new btSphereShape(btScalar((m_scale.x + m_scale.y + m_scale.z) / 3));

	btQuaternion startRotations;
	startRotations.setEulerZYX(m_rotationAngles.z, m_rotationAngles.y, m_rotationAngles.x);
	btTransform startTransform(startRotations, btVector3(m_translation.x, m_translation.y, m_translation.z));
	btDefaultMotionState * shapeMotionState = new btDefaultMotionState(startTransform);

	btScalar mass(m_mass);
	btVector3 inertia(0, 0, 0);
	shape->calculateLocalInertia(mass, inertia);

	btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, shape, inertia);

	mbt_rigidBody = new btRigidBody(shapeRigidBodyCI);
	dynamicsWorld->addRigidBody(mbt_rigidBody);
}

void Sphere::Update(void) {
	btTransform trans;
	btScalar m[16];

	mbt_rigidBody->getMotionState()->getWorldTransform(trans);
	trans.getOpenGLMatrix(m);
	m_model = glm::make_mat4(m) * glm::scale(m_scale);
}

void Sphere::applyImpulse(const glm::vec3 & impulse, const glm::vec3 & spin) {
	mbt_rigidBody->applyImpulse(btVector3(impulse.x, impulse.y, impulse.z), btVector3(spin.x, spin.y, spin.z));
}

