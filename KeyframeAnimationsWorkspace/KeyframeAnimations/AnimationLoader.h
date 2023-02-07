#pragma once
#include <map>
#include <string>
#include <pugixml.hpp>
#include "Animation.h"

class AnimationLoader
{
public:
	AnimationLoader();
	~AnimationLoader();
	bool LoadFromXML(std::map<std::string, AnimationData>& animationMap);

private:

};
