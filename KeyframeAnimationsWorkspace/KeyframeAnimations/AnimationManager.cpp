#include "AnimationManager.h"
#include <glm/gtx/easing.hpp>

AnimationManager::AnimationManager()
{
}

AnimationManager::~AnimationManager()
{
}

bool AnimationManager::Load(std::string name, AnimationData animation)
{
	std::map<std::string, AnimationData>::iterator it = m_Animations.find(name);
 	if (it != m_Animations.end()) {
		printf("Animation already exist in map\n");
		return false;
	}
	m_Animations.insert(std::pair<std::string, AnimationData>(name, animation));

	return true;
}

void AnimationManager::Update(const std::vector<cMeshObject*>& meshs, float dt)
{
	for (int i = 0; i < meshs.size(); i++)
	{
		cMeshObject* go = meshs[i];

		if (!go->enabled)
			continue;

		if (go->animation.AnimationType.length() != 0)
		{
			Animation& animation = go->animation;
			std::map<std::string, AnimationData>::iterator itFind = m_Animations.find(go->animation.AnimationType);
			const AnimationData& animationData = itFind->second;

			if (itFind != m_Animations.end())
			{
				if (animation.IsPlaying && animation.Speed != 0.0f)
				{
					animation.AnimationTime += dt * animation.Speed;
					if (animation.AnimationTime > animationData.Duration)
					{
						if (animation.IsLooping)
						{
							if (animation.Speed > 0)
							{
								animation.AnimationTime = 0.0f;
							}
							else
							{
								animation.AnimationTime = animationData.Duration;
							}
						}
						else
						{
							animation.AnimationTime = animationData.Duration;
							animation.IsPlaying = false;
						}

					}
					else if (animation.AnimationTime < 0.f)
					{
						if (animation.IsLooping)
						{
							if (animation.Speed < 0)
							{
								animation.AnimationTime = animationData.Duration;
							}
							else
							{
								animation.AnimationTime = 0.f;
							}
						}
						else
						{
							animation.AnimationTime = 0.f;
							animation.IsPlaying = false;
						}
					}
				}

				go->position = GetAnimationPosition(itFind->second, animation.AnimationTime);
				go->scaleXYZ = GetAnimationScale(itFind->second, animation.AnimationTime);
				go->qRotation = GetAnimationRotation(itFind->second, animation.AnimationTime);
				glm::vec3 newColor = GetAnimationColor(itFind->second, animation.AnimationTime);
				go->RGBA_colour = glm::vec4(newColor.x, newColor.y, newColor.z, 1);
			}
		}
	}
}

int AnimationManager::FindPositionKeyFrameIndex(const AnimationData& animation, float time)
{
	for (int i = 0; i < animation.PositionKeyFrames.size(); i++)
	{
		if (animation.PositionKeyFrames[i].time > time)
			return i - 1;
	}

	return animation.PositionKeyFrames.size() - 1;
}

int AnimationManager::FindScaleKeyFrameIndex(const AnimationData& animation, float time)
{
	for (int i = 0; i < animation.ScaleKeyFrames.size(); i++)
	{
		if (animation.ScaleKeyFrames[i].time > time)
			return i - 1;
	}

	return animation.ScaleKeyFrames.size() - 1;
}

int AnimationManager::FindRotationKeyFrameIndex(const AnimationData& animation, float time)
{
	for (int i = 0; i < animation.RotationKeyFrames.size(); i++)
	{
		if (animation.RotationKeyFrames[i].time > time)
			return i - 1;
	}

	return animation.RotationKeyFrames.size() - 1;
}

int AnimationManager::FindColorKeyFrameIndex(const AnimationData& animation, float time)
{
	for (int i = 0; i < animation.ColorKeyFrames.size(); i++)
	{
		if (animation.ColorKeyFrames[i].time > time)
			return i - 1;
	}

	return animation.ColorKeyFrames.size() - 1;
}

glm::vec3 AnimationManager::GetAnimationPosition(const AnimationData& animation, float time)
{
	if (animation.PositionKeyFrames.size() == 1)
		return animation.PositionKeyFrames[0].value;

	int positionKeyFrameIndex = FindPositionKeyFrameIndex(animation, time);

	if (animation.PositionKeyFrames.size() - 1 == positionKeyFrameIndex)
		return animation.PositionKeyFrames[positionKeyFrameIndex].value;

	int nextPositionKeyFrameIndex = positionKeyFrameIndex + 1;
	PositionKeyFrame positionKeyFrame = animation.PositionKeyFrames[positionKeyFrameIndex];
	PositionKeyFrame nextPositionKeyFrame = animation.PositionKeyFrames[nextPositionKeyFrameIndex];
	float difference = nextPositionKeyFrame.time - positionKeyFrame.time;
	float ratio = (time - positionKeyFrame.time) / difference;


	switch (positionKeyFrame.type)
	{
	case EaseIn:
		ratio = glm::sineEaseIn(ratio);
		break;

	case EaseOut:
		ratio = glm::sineEaseOut(ratio);
		break;

	case EaseInOut:
		ratio = glm::sineEaseInOut(ratio);
		break;

	case None:
	default:
		break;
	}

	glm::vec3 result = glm::mix(positionKeyFrame.value, nextPositionKeyFrame.value, ratio);

	printf("[%.2f : %.2f%%] (%.2f, %.2f) -> (%.2f, %.2f) = (%.2f, %.2f)\n", time, ratio,
		positionKeyFrame.value.x, positionKeyFrame.value.y,
		nextPositionKeyFrame.value.x, nextPositionKeyFrame.value.y,
		result.x, result.y);

	return result;
}

glm::vec3 AnimationManager::GetAnimationScale(const AnimationData& animation, float time)
{
	if (animation.ScaleKeyFrames.size() == 1)
		return animation.ScaleKeyFrames[0].value;

	int scaleKeyFrameIndex = FindScaleKeyFrameIndex(animation, time);

	if (animation.ScaleKeyFrames.size() - 1 == scaleKeyFrameIndex)
		return animation.ScaleKeyFrames[scaleKeyFrameIndex].value;

	int nextScaleKeyFrameIndex = scaleKeyFrameIndex + 1;
	ScaleKeyFrame scaleKeyFrame = animation.ScaleKeyFrames[scaleKeyFrameIndex];
	ScaleKeyFrame nextScaleKeyFrame = animation.ScaleKeyFrames[nextScaleKeyFrameIndex];
	float difference = nextScaleKeyFrame.time - scaleKeyFrame.time;
	float ratio = (time - scaleKeyFrame.time) / difference;

	glm::vec3 result = glm::mix(scaleKeyFrame.value, nextScaleKeyFrame.value, ratio);

	return result;
}

glm::quat AnimationManager::GetAnimationRotation(const AnimationData& animation, float time)
{
	if (animation.RotationKeyFrames.size() == 1)
		return animation.RotationKeyFrames[0].value;

	int rotationKeyFrameIndex = FindRotationKeyFrameIndex(animation, time);

	if (animation.RotationKeyFrames.size() - 1 == rotationKeyFrameIndex)
		return animation.RotationKeyFrames[rotationKeyFrameIndex].value;

	int nextRotationKeyFrameIndex = rotationKeyFrameIndex + 1;
	RotationKeyFrame rotationKeyFrame = animation.RotationKeyFrames[rotationKeyFrameIndex];
	RotationKeyFrame nextRotationKeyFrame = animation.RotationKeyFrames[nextRotationKeyFrameIndex];
	float difference = nextRotationKeyFrame.time - rotationKeyFrame.time;
	float ratio = (time - rotationKeyFrame.time) / difference;

	glm::quat result;
	if (rotationKeyFrame.useSlerp)
		result = glm::slerp(rotationKeyFrame.value, nextRotationKeyFrame.value, ratio);
	else
		result = glm::mix(rotationKeyFrame.value, nextRotationKeyFrame.value, ratio);

	return result;
}

glm::vec3 AnimationManager::GetAnimationColor(const AnimationData& animation, float time)
{
	if (animation.ColorKeyFrames.size() == 1)
		return animation.ColorKeyFrames[0].value;

	int colorKeyFrameIndex = FindColorKeyFrameIndex(animation, time);

	if (animation.ColorKeyFrames.size() - 1 == colorKeyFrameIndex)
		return animation.ColorKeyFrames[colorKeyFrameIndex].value;

	int nextColorKeyFrameIndex = colorKeyFrameIndex + 1;
	ColorKeyFrame colorKeyFrame = animation.ColorKeyFrames[colorKeyFrameIndex];
	ColorKeyFrame nextColorKeyFrame = animation.ColorKeyFrames[nextColorKeyFrameIndex];
	float difference = nextColorKeyFrame.time - colorKeyFrame.time;
	float ratio = (time - colorKeyFrame.time) / difference;

	glm::vec3 result = glm::mix(colorKeyFrame.value, nextColorKeyFrame.value, ratio);

	return result;
}
