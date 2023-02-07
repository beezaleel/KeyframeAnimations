#include "cMeshObject.h"

cMeshObject::cMeshObject()
{
	this->position = glm::vec3(0.0f);   
	this->qRotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
	this->SetUniformScale(1.0f);
	this->isWireframe = false;
	this->RGBA_colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->bUse_RGBA_colour = false;
	this->specular_colour_and_power = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->velocity = glm::vec3(0.0f);

	this->bDoNotLight = false;
	this->isFacingNewPosition = false;
	this->bIsVisible = true;
	this->mass = 1.0f;
	this->speed = 0.0f;
	this->counter = 0.0f;
	this->rotationAngle = 0.0f;
	this->enabled = false;
	this->hasParent = false;

	this->textureRatios[0] = 0.0f;
	this->textureRatios[1] = 0.0f;
	this->textureRatios[2] = 0.0f;
	this->textureRatios[3] = 0.0f;
	this->textureRatios[4] = 0.0f;
	this->textureRatios[5] = 0.0f;
	this->textureRatios[6] = 0.0f;
	this->textureRatios[7] = 0.0f;
}	

cMeshObject* cMeshObject::findObjectByFriendlyName(std::string nameToFind, bool bSearchChildren /*=true*/)
{
	for (std::vector< cMeshObject* >::iterator itCurrentMesh = this->vecChildMeshes.begin();
	     itCurrentMesh != this->vecChildMeshes.end();
		 itCurrentMesh++)
	{
		cMeshObject* pCurrentMesh = *itCurrentMesh;

		if (pCurrentMesh->friendlyName == nameToFind)
		{
			return pCurrentMesh;
		}

		cMeshObject* pChildMesh = pCurrentMesh->findObjectByFriendlyName(nameToFind, bSearchChildren);

		if (pChildMesh)
		{
			return pChildMesh;
		}
	}

	return NULL;
}