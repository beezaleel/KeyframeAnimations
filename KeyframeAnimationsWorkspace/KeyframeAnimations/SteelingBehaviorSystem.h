#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>

class SteelingBehaviorSystem
{
public:
	SteelingBehaviorSystem();
	~SteelingBehaviorSystem();
	glm::vec3 face(
		glm::vec3 currentPosition, 
		glm::vec3 targetPosition,
		glm::vec3 facingPosition);
	glm::vec3 seek(
		glm::vec3 currentPosition, 
		glm::vec3 targetPosition, 
		glm::vec3 currentVelocity, 
		float speed, 
		float mass);
	glm::vec3 flee(
		glm::vec3 currentPosition, 
		glm::vec3 targetPosition, 
		glm::vec3 currentVelocity, 
		float speed);
	glm::vec3 pursue(
		glm::vec3 currentPosition,
		glm::vec3 targetPosition,
		glm::vec3 targetVelocity,
		glm::vec3 currentVelocity,
		float speed, 
		float totalPrediction);
	glm::vec3 evade(
		glm::vec3 currentPosition,
		glm::vec3 targetPosition,
		glm::vec3 targetVelocity,
		glm::vec3 currentVelocity,
		float speed,
		float totalPrediction);
	glm::vec3 approach(
		glm::vec3 currentPosition, 
		glm::vec3 targetPosition, 
		glm::vec3 currentVelocity, 
		float speed, 
		float targetRadius, 
		float slowTargetRadius);;

private:

};
