#pragma once

#include "cLight.h"
#include <vector>
#include <string>

class cLightManager
{
public:
	cLightManager();

	static const unsigned int NUMBER_OF_LIGHTS_IM_GONNA_USE = 10;
	std::vector<cLight> vecTheLights;

	void LoadLightUniformLocations(unsigned int shaderID);

	void CopyLightInformationToShader(unsigned int shaderID);

};
