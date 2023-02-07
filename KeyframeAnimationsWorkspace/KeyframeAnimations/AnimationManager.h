#pragma once
#include <string>
#include <vector>
#include "cMeshObject.h"
#include "Animation.h"
#include <map>

class AnimationManager
{
public:
	AnimationManager();
	~AnimationManager();
	bool Load(std::string name, AnimationData animation);
	void Update(const std::vector<cMeshObject*>& meshs, float dt);

private:
	int FindPositionKeyFrameIndex(const AnimationData& animation, float time);
	int FindScaleKeyFrameIndex(const AnimationData& animation, float time);
	int FindRotationKeyFrameIndex(const AnimationData& animation, float time);
	int FindColorKeyFrameIndex(const AnimationData& animation, float time);

	glm::vec3 GetAnimationPosition(const AnimationData& animation, float time);
	glm::vec3 GetAnimationScale(const AnimationData& animation, float time);
	glm::quat GetAnimationRotation(const AnimationData& animation, float time);
	glm::vec3 GetAnimationColor(const AnimationData& animation, float time);

	std::map<std::string, AnimationData> m_Animations;
};
