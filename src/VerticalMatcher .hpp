#ifndef VERTICALMATCHER_HPP
#define VERTICALMATCHER_HPP

#include "IMatcher.hpp"

class VerticalMatcher : public IMatcher
{
public:
	VerticalMatcher();
	virtual bool operator()(const Board::Index& index, const Board& board) ;
	virtual EObjectType getBooster();
	virtual bool hasBooster();
};

#endif // !VERTICALMATCHER_HPP

