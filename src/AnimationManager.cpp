#include "AnimationManager.hpp"

AnimationManager::AnimationManager()
{

}

AnimationManager& AnimationManager::instance()
{
	static AnimationManager instance;
	return instance;
}

void AnimationManager::exec()
{
	sf::Time dt = _deltaClock.restart();
	for (auto& animTask : _animationTasks)
		animTask.exec(dt.asSeconds());
	//clean up
	_animationTasks.erase( std::remove_if(_animationTasks.begin(), _animationTasks.end(),
		[](const AnimationTask& anim) { return anim.isFinished(); }), _animationTasks.end());
}

void AnimationManager::pushBackAnimationTusk(const AnimationTask& animationTask)
{
	_animationTasks.push_back(animationTask);
}

bool AnimationManager::empty() const
{
	return _animationTasks.empty();
}