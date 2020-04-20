#include "SquareBooster.hpp"

SquareBooster::SquareBooster()
	: IBooster()
{
	_objectType = EObjectType::SBOMB;
}

std::list<Board::Index> SquareBooster::getInfluence(const Board::Index& boosterIndx, const Board& board)
{
	std::list<Board::Index> result;
	std::vector<Board::Index> matchVector = {
	Board::Index(boosterIndx.row, boosterIndx.column - 1),
	Board::Index(boosterIndx.row - 1, boosterIndx.column - 1),
	Board::Index(boosterIndx.row - 1, boosterIndx.column),
	Board::Index(boosterIndx.row - 1, boosterIndx.column + 1),
	Board::Index(boosterIndx.row , boosterIndx.column + 1),
	Board::Index(boosterIndx.row + 1, boosterIndx.column + 1),
	Board::Index(boosterIndx.row + 1, boosterIndx.column),
	Board::Index(boosterIndx.row + 1, boosterIndx.column - 1)
	};
	int count = 1;
	for (auto i = matchVector.begin(); i != matchVector.end(); ++i, ++count) {
		if (i->row >= 0 && i->column >= 0
			&& i->row < board.getRowCount() && i->column < board.getColumnCount())
		{
			result.push_back(*i);
		}
	}
	result.push_back(boosterIndx);
	return result;
}