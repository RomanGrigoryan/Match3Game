#include "AnimationTask.hpp"

AnimationTask::AnimationTask(ObjectPtr objectPtr, sf::Vector2f targetLocation, float duration)
	: _currentTime(0)
	, _objectPtr(objectPtr)
	, _targetLocation(targetLocation)
	, _duration(duration)
{
	_step = (_targetLocation - _objectPtr->getPos() ) / _duration;
}
void AnimationTask::exec(float delta)
{
	_currentTime += delta;
	if(isFinished()) {
		_objectPtr->setPos(_targetLocation);
	} else {
		_objectPtr->setPos(_objectPtr->getPos() + (_step * delta));
	}
}

bool AnimationTask::isFinished() const
{
	return _currentTime > _duration;
}