#ifndef _globalThings_HG_
#define _globalThings_HG_

#include "cMeshObject.h"
#include "cLightManager.h"
#include <vector>

extern cLightManager* g_pTheLightManager;

extern std::vector< cMeshObject* > vec_pMeshObjects;

cMeshObject* findObjectByFriendlyName(std::string nameToFind,
									  std::vector< cMeshObject* > vec_pMeshObjects,
									  bool bSearchChildren = true);

float getRandomFloatInRange(float a, float b);

void move(cMeshObject* meshObject, float horizontal, float vertical, float speed, float timeDelta);

#endif
