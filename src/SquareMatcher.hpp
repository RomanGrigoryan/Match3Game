#ifndef SQUAREMATCHER_HPP
#define SQUAREMATCHER_HPP

#include "IMatcher.hpp"

class SquareMatcher : public IMatcher
{
public:
	SquareMatcher();
	virtual bool operator()(const Board::Index& index, const Board& board);
	virtual EObjectType getBooster();
	virtual bool hasBooster();
};

#endif // !SQUAREMATCHER_HPP
