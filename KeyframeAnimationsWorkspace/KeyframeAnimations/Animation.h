#pragma once
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <string>
#include <vector>

enum EasingType
{
	EaseIn = 0,
	EaseOut = 1,
	EaseInOut = 2,
	None = 3
};

struct PositionKeyFrame
{
	PositionKeyFrame(glm::vec3 value, float time, EasingType type = None)
		: value(value), time(time), type(type) { }

	glm::vec3 value;
	float time;
	EasingType type;
};

struct ColorKeyFrame
{
	ColorKeyFrame(glm::vec3 value, float time, EasingType type = None)
		: value(value), time(time), type(type) { }

	glm::vec3 value;
	float time;
	EasingType type;
};

struct ScaleKeyFrame
{
	ScaleKeyFrame(glm::vec3 value, float time, EasingType type = None)
		: value(value), time(time), type(type) { }

	glm::vec3 value;
	float time;
	EasingType type;
};

struct RotationKeyFrame
{
	RotationKeyFrame(glm::quat value, float time, bool useSlerp)
		: value(value), time(time), useSlerp(useSlerp) { }
	glm::quat value;
	float time;
	bool useSlerp;
};

struct AnimationData
{
	std::vector<PositionKeyFrame> PositionKeyFrames;
	std::vector<ScaleKeyFrame> ScaleKeyFrames;
	std::vector<ColorKeyFrame> ColorKeyFrames;
	std::vector<RotationKeyFrame> RotationKeyFrames;
	float Duration;
};

struct Animation
{
	std::string AnimationType;
	float AnimationTime;
	bool IsPlaying;
	bool IsLooping;
	float Speed;
};