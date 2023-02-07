#include "cLight.h"

cLight::cLight()
{
	this->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	glm::vec4 atten = glm::vec4(0.01f, 0.01f, 0.0f, 1.0f);;
	glm::vec4 direction = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);;
	
	this->param1 = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->param2 = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

}

void cLight::setConstantAttenuation(float newConstAtten)
{
	this->atten.x = newConstAtten;
	return;
}
void cLight::setLinearAttenuation(float newLinearAtten)
{
	this->atten.y = newLinearAtten;
	return;
}

void cLight::setQuadraticAttenuation(float newQuadAtten)
{
	this->atten.z = newQuadAtten;
	return;
}

void cLight::TurnOn(void)
{
	this->param2.x = 1;
	return;
}

void cLight::TurnOff(void)
{
	this->param2.x = 0;
	return;
}
