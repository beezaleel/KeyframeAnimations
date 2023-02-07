#ifndef _cLight_HG_
#define _cLight_HG_

#include <glm/glm.hpp>
#include <glm/vec4.hpp>

class cLight
{
public:
	cLight();
	glm::vec4 position;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 atten;
	glm::vec4 direction;
	glm::vec4 param1;
	glm::vec4 param2;

	int position_UniformLocation = -1;
	int diffuse_UniformLocation = -1;
	int specular_UniformLocation = -1;
	int atten_UniformLocation = -1;
	int direction_UniformLocation = -1;
	int param1_UniformLocation = -1;
	int param2_UniformLocation = -1;

	void setConstantAttenuation(float newConstAtten);
	void setLinearAttenuation(float newLinearAtten);
	void setQuadraticAttenuation(float newQuadAtten);

	void TurnOn(void);
	void TurnOff(void);
	enum eLightType
	{
		POINT_LIGHT,
		DIRECTIONAL_LIGHT,
		SPOT_LIGHT
	};
};

#endif
