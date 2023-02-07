#ifndef _cMeshObject_HG_
#define _cMeshObject_HG_

#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include "Animation.h"

class cMeshObject
{
public:
	cMeshObject();
	float mass;
	float speed;
	float counter;
	float rotationAngle;
	std::string meshName;
	std::string friendlyName;

	glm::vec3 position;
	glm::vec3 velocity;
	glm::quat qRotation;

	void setRotationFromEuler( glm::vec3 newEulerAngleXYZ )
	{
		this->qRotation = glm::quat(newEulerAngleXYZ);
	}

	void adjustRoationAngleFromEuler( glm::vec3 EulerAngleXYZ_Adjust )
	{
		glm::quat qChange = glm::quat(EulerAngleXYZ_Adjust);
		this->qRotation *= qChange;
	}

	glm::vec3 scaleXYZ;
	void SetUniformScale(float newScale)
	{
		this->scaleXYZ = glm::vec3(newScale, newScale, newScale);
	}

    bool isWireframe;

    glm::vec4 RGBA_colour;
	bool bUse_RGBA_colour; 

	glm::vec4 specular_colour_and_power;

	bool bDoNotLight;
	bool isFacingNewPosition;
	bool bIsVisible;
	bool enabled;
	bool hasParent;

	std::string textures[8];

	float textureRatios[8];
	Animation animation;

	std::vector< cMeshObject* > vecChildMeshes;
	cMeshObject* findObjectByFriendlyName(std::string name, bool bSearchChildren = true);

};

#endif
