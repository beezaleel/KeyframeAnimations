#ifndef _cLightHelper_HG_
#define _cLightHelper_HG_

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

class cLightHelper
{
public:

	static const float DEFAULT_ATTEN_CONST;	
	static const float DEFAULT_ATTEN_LINEAR;
	static const float DEFAULT_ATTEN_QUADRATIC;	
	static const float DEFAULT_AMBIENT_TO_DIFFUSE_RATIO;
	static const unsigned int DEFAULTMAXITERATIONS = 50;
	static const float DEFAULTINFINITEDISTANCE;
	static const float DEFAULDIFFUSEACCURACYTHRESHOLD;
	static const float DEFAULTZEROTHRESHOLD;

	float calcApproxDistFromAtten( float targetLightLevel );
	float calcApproxDistFromAtten( float targetLightLevel, float accuracy );
	float calcApproxDistFromAtten( float targetLightLevel, float accuracy, float infiniteDistance, 
	                               float constAttenuation, float linearAttenuation,  float quadraticAttenuation, 
	                               unsigned int maxIterations = DEFAULTMAXITERATIONS);
	float calcDiffuseFromAttenByDistance( float distance, 
										  float constAttenuation, 
										  float linearAttenuation, 
										  float quadraticAttenuation,
										  float zeroThreshold = DEFAULTZEROTHRESHOLD);
};

#endif
