#include "globalThings.h"

// The variable is HERE.
cLightManager* g_pTheLightManager = NULL;
std::vector< cMeshObject* > vec_pMeshObjects;

void move(cMeshObject* meshObject, float horizontal, float vertical, float speed, float timeDelta) {
	glm::vec3 direction = glm::vec3(horizontal, 0.0f, vertical);
	direction = glm::normalize(direction);
	static constexpr float magnitude = direction.length();

	if (magnitude >= 0.1f) {
		float targetAngle = glm::degrees(glm::atan(direction.x, direction.y));
		//meshObject->adjustRoationAngleFromEuler();
		meshObject->setRotationFromEuler(glm::vec3(0.0f, targetAngle, 0.0f));
		direction = direction * speed * timeDelta;
		meshObject->position.x += direction.x;
		meshObject->position.z += direction.z;
		//printf("x, y, z: %.2f, %.2f, %.2f\n", direction.x, direction.y, direction.z);
		//printf("meshObject: %.2f, %.2f, %.2f\n", meshObject->position.x, meshObject->position.y, meshObject->position.z);
	}
}
