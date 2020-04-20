#include "HorizontalMatcher.hpp"
#include "Board.hpp"
HorizontalMatcher::HorizontalMatcher() 
	: IMatcher()
{
	_priority = MatcherPriority::SympalMatch;
}

bool HorizontalMatcher::operator()(const Board::Index& index, const Board& board)
{
	Board::Index next = index;
	
	_matchedList.clear();
	_matchedList.push_back(index);

	while (true)
	{
		next.column++;
		if (!(next.column < board.getColumnCount())) break;
		
		if (board.getObject(index)->type() != board.getObject(next)->type())
			break;
		_matchedList.push_back(next);
	}

	next = index;
	while (true)
	{
		if (!next.column) break;
		next.column--;
		if (board.getObject(index)->type() != board.getObject(next)->type())
			break;
		_matchedList.push_back(next);
	}

	if (_matchedList.size() > 2)
	{
		return true;
	}
	return false;
}

EObjectType HorizontalMatcher::getBooster()
{
	return EObjectType::HBOMB;
}

bool HorizontalMatcher::hasBooster()
{
	return _matchedList.size() > 3;
}