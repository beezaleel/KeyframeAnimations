#include "AnimationLoader.h"

AnimationLoader::AnimationLoader()
{
}

AnimationLoader::~AnimationLoader()
{
}

bool AnimationLoader::LoadFromXML(std::map<std::string, AnimationData>& animationMap)
{
	AnimationData data = AnimationData();
	pugi::xml_document document;
	pugi::xml_parse_result result = document.load_file("animation_data.xml");

	if (!result) {
		animationMap = std::map<std::string, AnimationData>();
		return false;
	}

	pugi::xml_object_range<pugi::xml_node_iterator> animationWareHouse = document.child("animationWareHouse").children();
	if (!result) {
		animationMap = std::map<std::string, AnimationData>();
		return false;
	}

	pugi::xml_node_iterator animationWareHouseIt;
	for (animationWareHouseIt = animationWareHouse.begin();
		animationWareHouseIt != animationWareHouse.end();
		animationWareHouseIt++) {
		pugi::xml_node animationWareHouseValue = *animationWareHouseIt;
		pugi::xml_attribute nameattribute = animationWareHouseValue.attribute("name");
		pugi::xml_attribute durationattribute = animationWareHouseValue.attribute("duration");
		std::string name = "";
		float duration = 0.0f;

		if (std::string(animationWareHouseValue.name()) == "animations") {
			name = nameattribute.value();
			duration = std::stof(durationattribute.value());
		}

		pugi::xml_node_iterator animationIt;
		for (animationIt = animationWareHouseValue.children().begin(); 
			animationIt != animationWareHouseValue.children().end(); 
			animationIt++) {
			pugi::xml_node animationNode = *animationIt;
			pugi::xml_attribute parentName = animationNode.attribute("parent");

			if (std::string(animationNode.name()) == "animation" && std::string(nameattribute.value()) == std::string(parentName.value())) {

				pugi::xml_node_iterator animationValuesIt;
				for (animationValuesIt = animationNode.children().begin();
					animationValuesIt != animationNode.children().end();
					animationValuesIt++) {
					pugi::xml_node animationValaueNode = *animationValuesIt;

					// For positionKeyFrame
					if (std::string(animationValaueNode.name()) == "positionKeyFrame") {
						glm::vec3 value = glm::vec3(0.0f);
						float time = 0.0f;
						EasingType type = EasingType::None;

						pugi::xml_node_iterator positionKeyFrameIt;
						for (positionKeyFrameIt = animationValaueNode.children().begin();
							positionKeyFrameIt != animationValaueNode.children().end();
							positionKeyFrameIt++) {
							pugi::xml_node positionKeyFrameNode = *positionKeyFrameIt;
							if (std::string(positionKeyFrameNode.name()) == "time") {
								time = std::stof(positionKeyFrameNode.child_value());
							}
							if (std::string(positionKeyFrameNode.name()) == "easingType") {
								type = (EasingType)atoi(positionKeyFrameNode.child_value());
							}
							if (std::string(positionKeyFrameNode.name()) == "value") {
								pugi::xml_node_iterator valueIt;
								for (valueIt = positionKeyFrameNode.children().begin();
									valueIt != positionKeyFrameNode.children().end();
									valueIt++) {
									pugi::xml_node valueNode = *valueIt;
									if (std::string(valueNode.name()) == "x") {
										value.x = std::stof(valueNode.child_value());
									}
									if (std::string(valueNode.name()) == "y") {
										value.y = std::stof(valueNode.child_value());
									}
									if (std::string(valueNode.name()) == "z") {
										value.z = std::stof(valueNode.child_value());
									}
								}
							}
						}
						data.PositionKeyFrames.push_back(PositionKeyFrame(value, time, type));
					}

					// For scaleKeyFrame
					if (std::string(animationValaueNode.name()) == "scaleKeyFrame") {
						glm::vec3 value = glm::vec3(0.0f);
						float time = 0.0f;
						EasingType type = EasingType::None;

						pugi::xml_node_iterator positionKeyFrameIt;
						for (positionKeyFrameIt = animationValaueNode.children().begin();
							positionKeyFrameIt != animationValaueNode.children().end();
							positionKeyFrameIt++) {
							pugi::xml_node positionKeyFrameNode = *positionKeyFrameIt;
							if (std::string(positionKeyFrameNode.name()) == "time") {
								time = std::stof(positionKeyFrameNode.child_value());
							}
							if (std::string(positionKeyFrameNode.name()) == "easingType") {
								type = (EasingType)atoi(positionKeyFrameNode.child_value());
							}
							if (std::string(positionKeyFrameNode.name()) == "value") {
								pugi::xml_node_iterator valueIt;
								for (valueIt = positionKeyFrameNode.children().begin();
									valueIt != positionKeyFrameNode.children().end();
									valueIt++) {
									pugi::xml_node valueNode = *valueIt;
									if (std::string(valueNode.name()) == "x") {
										value.x = std::stof(valueNode.child_value());
									}
									if (std::string(valueNode.name()) == "y") {
										value.y = std::stof(valueNode.child_value());
									}
									if (std::string(valueNode.name()) == "z") {
										value.z = std::stof(valueNode.child_value());
									}
								}
							}
						}
						data.ScaleKeyFrames.push_back(ScaleKeyFrame(value, time, type));
					}

					// For colorKeyFrame
					if (std::string(animationValaueNode.name()) == "colorKeyFrame") {
						glm::vec3 value = glm::vec3(0.0f);
						float time = 0.0f;
						EasingType type = EasingType::None;

						pugi::xml_node_iterator positionKeyFrameIt;
						for (positionKeyFrameIt = animationValaueNode.children().begin();
							positionKeyFrameIt != animationValaueNode.children().end();
							positionKeyFrameIt++) {
							pugi::xml_node positionKeyFrameNode = *positionKeyFrameIt;
							if (std::string(positionKeyFrameNode.name()) == "time") {
								time = std::stof(positionKeyFrameNode.child_value());
							}
							if (std::string(positionKeyFrameNode.name()) == "easingType") {
								type = (EasingType)atoi(positionKeyFrameNode.child_value());
							}
							if (std::string(positionKeyFrameNode.name()) == "value") {
								pugi::xml_node_iterator valueIt;
								for (valueIt = positionKeyFrameNode.children().begin();
									valueIt != positionKeyFrameNode.children().end();
									valueIt++) {
									pugi::xml_node valueNode = *valueIt;
									if (std::string(valueNode.name()) == "x") {
										value.x = std::stof(valueNode.child_value());
									}
									if (std::string(valueNode.name()) == "y") {
										value.y = std::stof(valueNode.child_value());
									}
									if (std::string(valueNode.name()) == "z") {
										value.z = std::stof(valueNode.child_value());
									}
								}
							}
						}
						data.ColorKeyFrames.push_back(ColorKeyFrame(value, time, type));
					}

					// For RotationKeyFrame
					if (std::string(animationValaueNode.name()) == "rotationKeyFrame") {
						glm::quat value = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
						float time = 0.0f;
						bool useSlerp = false;

						pugi::xml_node_iterator positionKeyFrameIt;
						for (positionKeyFrameIt = animationValaueNode.children().begin();
							positionKeyFrameIt != animationValaueNode.children().end();
							positionKeyFrameIt++) {
							pugi::xml_node positionKeyFrameNode = *positionKeyFrameIt;
							if (std::string(positionKeyFrameNode.name()) == "time") {
								time = std::stof(positionKeyFrameNode.child_value());
							}
							if (std::string(positionKeyFrameNode.name()) == "useSlerp") {
								useSlerp = atoi(positionKeyFrameNode.child_value());
							}
							if (std::string(positionKeyFrameNode.name()) == "value") {
								pugi::xml_node_iterator valueIt;
								for (valueIt = positionKeyFrameNode.children().begin();
									valueIt != positionKeyFrameNode.children().end();
									valueIt++) {
									pugi::xml_node valueNode = *valueIt;
									if (std::string(valueNode.name()) == "x") {
										value.x = std::stof(valueNode.child_value());
									}
									if (std::string(valueNode.name()) == "y") {
										value.y = std::stof(valueNode.child_value());
									}
									if (std::string(valueNode.name()) == "z") {
										value.z = std::stof(valueNode.child_value());
									}
									if (std::string(valueNode.name()) == "w") {
										value.w = std::stof(valueNode.child_value());
									}
								}
							}
						}
						data.RotationKeyFrames.push_back(RotationKeyFrame(value, time, useSlerp));
					}
				}
			}
		}
		data.Duration = duration;
		animationMap.insert(std::pair<std::string, AnimationData>(name, data));
	}	
}
