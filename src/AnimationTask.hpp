#ifndef ANIMATIONTASK_HPP
#define ANIMATIONTASK_HPP

#include <SFML/System/Vector2.hpp> 
#include "Object.hpp"

class AnimationTask
{
	float _duration;
	float _currentTime;
	sf::Vector2f _step;
	sf::Vector2f _targetLocation;
	ObjectPtr _objectPtr;

public:
	AnimationTask(ObjectPtr objectPtr, sf::Vector2f targetLocation, float duration);

	void exec(float delta);
	bool isFinished() const;
};



#endif // !ANIMATIONTASK_HPP
