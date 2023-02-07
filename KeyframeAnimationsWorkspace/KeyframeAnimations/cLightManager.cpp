#include "cLightManager.h"

#include "globalOpenGL.h"
#include <sstream>

cLightManager::cLightManager()
{
	for ( unsigned int count = 0;
		  count != cLightManager::NUMBER_OF_LIGHTS_IM_GONNA_USE;
		  count++ )
	{

		cLight newLight;
		this->vecTheLights.push_back(newLight);
	}

}

void cLightManager::LoadLightUniformLocations(unsigned int shaderID)
{
	this->vecTheLights[0].position_UniformLocation = glGetUniformLocation(shaderID, "theLights[0].position");
	this->vecTheLights[0].diffuse_UniformLocation = glGetUniformLocation(shaderID, "theLights[0].diffuse");
	this->vecTheLights[0].specular_UniformLocation = glGetUniformLocation(shaderID, "theLights[0].specular");
	this->vecTheLights[0].atten_UniformLocation = glGetUniformLocation(shaderID, "theLights[0].atten");
	this->vecTheLights[0].direction_UniformLocation = glGetUniformLocation(shaderID, "theLights[0].direction");
	this->vecTheLights[0].param1_UniformLocation = glGetUniformLocation(shaderID, "theLights[0].param1");
	this->vecTheLights[0].param2_UniformLocation = glGetUniformLocation(shaderID, "theLights[0].param2");

	this->vecTheLights[1].position_UniformLocation = glGetUniformLocation(shaderID, "theLights[1].position");
	this->vecTheLights[1].diffuse_UniformLocation = glGetUniformLocation(shaderID, "theLights[1].diffuse");
	this->vecTheLights[1].specular_UniformLocation = glGetUniformLocation(shaderID, "theLights[1].specular");
	this->vecTheLights[1].atten_UniformLocation = glGetUniformLocation(shaderID, "theLights[1].atten");
	this->vecTheLights[1].direction_UniformLocation = glGetUniformLocation(shaderID, "theLights[1].direction");
	this->vecTheLights[1].param1_UniformLocation = glGetUniformLocation(shaderID, "theLights[1].param1");
	this->vecTheLights[1].param2_UniformLocation = glGetUniformLocation(shaderID, "theLights[1].param2");

	this->vecTheLights[2].position_UniformLocation = glGetUniformLocation(shaderID, "theLights[2].position");
	this->vecTheLights[2].diffuse_UniformLocation = glGetUniformLocation(shaderID, "theLights[2].diffuse");
	this->vecTheLights[2].specular_UniformLocation = glGetUniformLocation(shaderID, "theLights[2].specular");
	this->vecTheLights[2].atten_UniformLocation = glGetUniformLocation(shaderID, "theLights[2].atten");
	this->vecTheLights[2].direction_UniformLocation = glGetUniformLocation(shaderID, "theLights[2].direction");
	this->vecTheLights[2].param1_UniformLocation = glGetUniformLocation(shaderID, "theLights[2].param1");
	this->vecTheLights[2].param2_UniformLocation = glGetUniformLocation(shaderID, "theLights[2].param2");

	this->vecTheLights[3].position_UniformLocation = glGetUniformLocation(shaderID, "theLights[3].position");
	this->vecTheLights[3].diffuse_UniformLocation = glGetUniformLocation(shaderID, "theLights[3].diffuse");
	this->vecTheLights[3].specular_UniformLocation = glGetUniformLocation(shaderID, "theLights[3].specular");
	this->vecTheLights[3].atten_UniformLocation = glGetUniformLocation(shaderID, "theLights[3].atten");
	this->vecTheLights[3].direction_UniformLocation = glGetUniformLocation(shaderID, "theLights[3].direction");
	this->vecTheLights[3].param1_UniformLocation = glGetUniformLocation(shaderID, "theLights[3].param1");
	this->vecTheLights[3].param2_UniformLocation = glGetUniformLocation(shaderID, "theLights[3].param2");

	this->vecTheLights[4].position_UniformLocation = glGetUniformLocation(shaderID, "theLights[4].position");
	this->vecTheLights[4].diffuse_UniformLocation = glGetUniformLocation(shaderID, "theLights[4].diffuse");
	this->vecTheLights[4].specular_UniformLocation = glGetUniformLocation(shaderID, "theLights[4].specular");
	this->vecTheLights[4].atten_UniformLocation = glGetUniformLocation(shaderID, "theLights[4].atten");
	this->vecTheLights[4].direction_UniformLocation = glGetUniformLocation(shaderID, "theLights[4].direction");
	this->vecTheLights[4].param1_UniformLocation = glGetUniformLocation(shaderID, "theLights[4].param1");
	this->vecTheLights[4].param2_UniformLocation = glGetUniformLocation(shaderID, "theLights[4].param2");

	return;
}

void cLightManager::CopyLightInformationToShader(unsigned int shaderID)
{
	for (std::vector<cLight>::iterator itLight = this->vecTheLights.begin();
		 itLight != this->vecTheLights.end();
		 itLight++)
	{
		glUniform4f(itLight->position_UniformLocation, 
					itLight->position.x, 
					itLight->position.y, 
					itLight->position.z, 
					itLight->position.w );

		glUniform4f(itLight->diffuse_UniformLocation,
					itLight->diffuse.x,
					itLight->diffuse.y,
					itLight->diffuse.z,
					itLight->diffuse.w );

		glUniform4f(itLight->specular_UniformLocation,
					itLight->specular.x,
					itLight->specular.y,
					itLight->specular.z,
					itLight->specular.w );

		glUniform4f(itLight->atten_UniformLocation,
					itLight->atten.x,
					itLight->atten.y,
					itLight->atten.z,
					itLight->atten.w );

		glUniform4f(itLight->direction_UniformLocation,
					itLight->direction.x,
					itLight->direction.y,
					itLight->direction.z,
					itLight->direction.w );

		glUniform4f(itLight->param1_UniformLocation,
					itLight->param1.x,
					itLight->param1.y,
					itLight->param1.z,
					itLight->param1.w );

		glUniform4f(itLight->param2_UniformLocation,
					itLight->param2.x,
					itLight->param2.y,
					itLight->param2.z,
					itLight->param2.w );


	}

	return;
}

