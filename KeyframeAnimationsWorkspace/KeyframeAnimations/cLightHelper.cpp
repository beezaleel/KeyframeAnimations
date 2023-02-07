#include "cLightHelper.h"


const float cLightHelper::DEFAULT_ATTEN_CONST = 0.1f;
const float cLightHelper::DEFAULT_ATTEN_LINEAR = 0.1f;
const float cLightHelper::DEFAULT_ATTEN_QUADRATIC = 0.1f;
const float cLightHelper::DEFAULT_AMBIENT_TO_DIFFUSE_RATIO = 0.8f;
const float cLightHelper::DEFAULTINFINITEDISTANCE = 10000.0f;	
const float cLightHelper::DEFAULDIFFUSEACCURACYTHRESHOLD = 0.001f;

float cLightHelper::calcApproxDistFromAtten( float targetLightLevel )
{
	return this->calcApproxDistFromAtten( targetLightLevel, cLightHelper::DEFAULDIFFUSEACCURACYTHRESHOLD );
}

float cLightHelper::calcApproxDistFromAtten(float targetLightLevel, float accuracy )
{
	return this->calcApproxDistFromAtten( targetLightLevel, accuracy, 
										  cLightHelper::DEFAULT_ATTEN_CONST,
										  cLightHelper::DEFAULT_ATTEN_LINEAR, 
										  cLightHelper::DEFAULT_ATTEN_QUADRATIC, 
										  cLightHelper::DEFAULTINFINITEDISTANCE, cLightHelper::DEFAULTMAXITERATIONS );
}
                                                      
float cLightHelper::calcApproxDistFromAtten( float targetLightLevel, float accuracy, 
                                             float infiniteDistance, 
											 float constAttenuation, 
											 float linearAttenuation, 
											 float quadraticAttenuation, 
											 unsigned int maxIterations /*= DEFAULTMAXITERATIONS = 50*/ )
{
	if ( targetLightLevel != 0.0f )	
	{	
		if ( ( accuracy * 10.0f) >= targetLightLevel * 10.0f )
		{	
			accuracy = targetLightLevel / 10.0f;
		}
	}

	float targetLightLevelLow = targetLightLevel - accuracy;
	float targetLightLevelHigh = targetLightLevel + accuracy;

	if ( this->calcDiffuseFromAttenByDistance( cLightHelper::DEFAULTINFINITEDISTANCE, 
		                                       constAttenuation, linearAttenuation, quadraticAttenuation,
	                                           accuracy ) > targetLightLevelHigh )
	{
		return cLightHelper::DEFAULTINFINITEDISTANCE;
	}

	float distanceGuessLow = 0.0f;
	float distanceGuessHigh = cLightHelper::DEFAULTINFINITEDISTANCE;

	unsigned int iterationCount = 0;
	while ( iterationCount < maxIterations )
	{
		float curDistanceGuess = ( (distanceGuessHigh - distanceGuessLow) / 2.0f ) + distanceGuessLow;

		float curDiffuseAtGuessDistance 
			= this->calcDiffuseFromAttenByDistance( curDistanceGuess, constAttenuation, 
													linearAttenuation, quadraticAttenuation, DEFAULTZEROTHRESHOLD );

		if ( curDiffuseAtGuessDistance < targetLightLevelLow )
		{	
			distanceGuessHigh = curDistanceGuess;
		}
		else if ( curDiffuseAtGuessDistance > targetLightLevelHigh )
		{	
			distanceGuessLow = curDistanceGuess;
		}
		else 
		{	
			return curDistanceGuess;
		}

		iterationCount++;

	}
	float distance = (distanceGuessHigh - distanceGuessLow) / 2.0f;

	return distance;
}

const float cLightHelper::DEFAULTZEROTHRESHOLD = 0.0001f;

float cLightHelper::calcDiffuseFromAttenByDistance(
		float distance, 
		float constAttenuation, 
		float linearAttenuation, 
		float quadraticAttenuation,
		float zeroThreshold)
{
	float diffuse = 1.0f;

	float denominator = constAttenuation + 
	                    linearAttenuation * distance +
						quadraticAttenuation * distance * distance;

	if ( denominator <= zeroThreshold )
	{	
		diffuse = 1.0f;
	}
	else
	{
		float atten = 1.0f / denominator;
		diffuse *= atten;
		if ( diffuse > 1.0f )
		{
			diffuse = 1.0f;
		}
	}
	return diffuse;
}