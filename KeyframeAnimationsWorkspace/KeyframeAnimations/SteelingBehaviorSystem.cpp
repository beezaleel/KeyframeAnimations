#include "SteelingBehaviorSystem.h"

SteelingBehaviorSystem::SteelingBehaviorSystem()
{
}

SteelingBehaviorSystem::~SteelingBehaviorSystem()
{
}

glm::vec3 SteelingBehaviorSystem::face(
	glm::vec3 currentPosition, 
	glm::vec3 targetPosition, 
	glm::vec3 facingPosition)
{
	glm::vec3 direction = glm::normalize(targetPosition - currentPosition);
	glm::vec3 result = glm::normalize(glm::mix(facingPosition, direction, 0.5f));

	return result;
	//return direction;
}

glm::vec3 SteelingBehaviorSystem::seek(
	glm::vec3 currentPosition, 
	glm::vec3 targetPosition, 
	glm::vec3 currentVelocity, 
	float speed, 
	float mass)
{
	glm::vec3 desiredVelocity = glm::normalize(targetPosition - currentPosition) * speed;

	return desiredVelocity;
}

glm::vec3 SteelingBehaviorSystem::flee(
	glm::vec3 currentPosition, 
	glm::vec3 targetPosition, 
	glm::vec3 currentVelocity, 
	float speed)
{
	glm::vec3 desiredVelocity = glm::normalize(currentPosition - targetPosition) * speed;
	glm::vec3 steeringForce = desiredVelocity - currentVelocity;
	glm::vec3 velocityPerFrame = steeringForce / speed;

	return velocityPerFrame;
}

glm::vec3 SteelingBehaviorSystem::pursue(
	glm::vec3 currentPosition, 
	glm::vec3 targetPosition,
	glm::vec3 targetVelocity,
	glm::vec3 currentVelocity, 
	float speed, 
	float totalPrediction)
{
	glm::vec3 direction = targetPosition - currentPosition;
	float distance = glm::length(direction);
	float predictTime = distance / speed;
	predictTime = glm::clamp(predictTime, 0.0f, totalPrediction);

	glm::vec3 predictedTargetPosition = targetPosition + targetVelocity * predictTime;

	return predictedTargetPosition;
}

glm::vec3 SteelingBehaviorSystem::evade(
	glm::vec3 currentPosition, 
	glm::vec3 targetPosition, 
	glm::vec3 targetVelocity, 
	glm::vec3 currentVelocity, 
	float speed, 
	float totalPrediction)
{
	glm::vec3 direction = targetPosition - currentPosition;
	float distance = glm::length(direction);
	float predictTime = distance / speed;
	predictTime = glm::clamp(predictTime, 0.0f, totalPrediction);
	glm::vec3 predictedTargetPosition = targetPosition + targetVelocity * predictTime;

	return predictedTargetPosition;
}

glm::vec3 SteelingBehaviorSystem::approach(
	glm::vec3 currentPosition, 
	glm::vec3 targetPosition, 
	glm::vec3 currentVelocity, 
	float speed, 
	float targetRadius, 
	float slowTargetRadius)
{
	glm::vec3 direction = targetPosition - currentPosition;
	float distance = glm::length(direction);
	printf("distance: %.2f\n", distance);
	if (distance < targetRadius)
	{
		printf("Returned distance 1: %.2f\n", distance);
		return glm::vec3(0);
	}
	direction = glm::normalize(direction);

	if (distance < slowTargetRadius) {
		float slowSpeed = speed * distance / slowTargetRadius;
		printf("Returned distance 2: %.2f\n", slowSpeed);
		direction = slowSpeed * direction;
		printf("Returned distance 3: %.2f, %.2f, %.2f\n", direction.x, direction.y, direction.z);
		return direction;
	}
	else {
		printf("Returned speed 3: %.2f\n", speed );
		direction = speed * direction;
		printf("Returned distance 3: %.2f, %.2f, %.2f\n", direction.x, direction.y, direction.z);
		return direction;
	}
}
