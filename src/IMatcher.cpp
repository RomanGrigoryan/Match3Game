#include "IMatcher.hpp"

IMatcher::IMatcher()
{
}


const std::list<Board::Index> IMatcher::getMatchList() const
{
	return _matchedList;
}

bool operator<(const IMatcher& first, const IMatcher& second)
{
	return first._priority < second._priority;
}
