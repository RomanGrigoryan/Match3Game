#ifndef ANIMATIONMANAGER_HPP
#define ANIMATIONMANAGER_HPP

#include <SFML/System/Clock.hpp>

#include <list>

#include "AnimationTask.hpp"

class AnimationManager
{
	
	std::list<AnimationTask> _animationTasks;
	sf::Clock _deltaClock;

private:
	AnimationManager();
	AnimationManager(const AnimationManager&) = delete;
	AnimationManager& operator=(const AnimationManager&) = delete;
	AnimationManager(AnimationManager&&) = delete;
	AnimationManager& operator=(AnimationManager&&) = delete;

public:
	static AnimationManager& instance();
	void pushBackAnimationTusk(const AnimationTask& animationTask);
	void exec();
	bool empty()const ;
};

#endif // !ANIMATIONMANAGER_HPP