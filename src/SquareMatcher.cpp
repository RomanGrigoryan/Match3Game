#include "SquareMatcher.hpp"
#include "Board.hpp"

SquareMatcher::SquareMatcher()
	: IMatcher()
{
	_priority = MatcherPriority::SquareMatch;
}

bool SquareMatcher::operator()(const Board::Index& index, const Board& board)
{
	std::vector<Board::Index> matchVector = {
		Board::Index(index.row, index.column - 1),
		Board::Index(index.row - 1, index.column - 1),
		Board::Index(index.row - 1, index.column),
		Board::Index(index.row - 1, index.column + 1),
		Board::Index(index.row , index.column + 1),
		Board::Index(index.row + 1, index.column + 1),
		Board::Index(index.row + 1, index.column),
		Board::Index(index.row + 1, index.column -1),
		Board::Index(index.row, index.column - 1)
	};
	int count = 1;
	for (auto i = matchVector.begin(); i != matchVector.end(); ++i, ++count) {
		if (i->row < 0 || i->column < 0
			|| i->row >= board.getRowCount() || i->column >= board.getColumnCount())
		{
			_matchedList.clear();
		} else if(board.getObject(index)->type() == board.getObject(*i)->type()) {
			_matchedList.push_back(*i);
			if (_matchedList.size() == 3) {
				return true;
			}
		}
		else {
			_matchedList.clear();
		}
		if (count == 3) {
			i--;
			count = 0;
			_matchedList.clear();
		}
	}
	return false;
}

EObjectType SquareMatcher::getBooster()
{
	return EObjectType::SBOMB;
}

bool SquareMatcher::hasBooster()
{
	return true;
}