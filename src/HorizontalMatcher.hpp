#ifndef HORIZONTALMATCHER_HPP
#define HORIZONTALMATCHER_HPP

#include "IMatcher.hpp"

class HorizontalMatcher : public IMatcher
{
public:
	HorizontalMatcher();
	virtual bool operator()(const Board::Index& index, const Board& board) ;
	virtual EObjectType getBooster();
	virtual bool hasBooster();
};

#endif // !HORIZONTALMATCHER_HPP

