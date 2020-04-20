#include "VerticalMatcher .hpp"
#include "Board.hpp"
VerticalMatcher::VerticalMatcher() 
	: IMatcher()
{
	_priority = MatcherPriority::SympalMatch;
}

bool VerticalMatcher::operator()(const Board::Index& index, const Board& board)
{
	Board::Index next = index;
	
	_matchedList.clear();
	_matchedList.push_back(index);

	while (true)
	{
		next.row++;
		if ( !(next.row < board.getRowCount()) ) break;
		
		if (board.getObject(index)->type() != board.getObject(next)->type())
			break;
		_matchedList.push_back(next);
	}

	next = index;
	while (true)
	{
		if (!next.row) break;
		next.row--;
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

EObjectType VerticalMatcher::getBooster()
{
	return EObjectType::VBOMB;
}

bool VerticalMatcher::hasBooster()
{
	return _matchedList.size() > 3;
}