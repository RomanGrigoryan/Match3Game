#ifndef IMATCHER_HPP
#define IMATCHER_HPP

#include <list>
#include "Board.hpp"

enum class MatcherPriority : unsigned
{
	SympalMatch = 0,
	SquareMatch,
};

class IMatcher
{

protected:
	MatcherPriority _priority;
	std::list<Board::Index> _matchedList;
public:
	struct comparator {
		bool operator()(const IMatcher* first, const IMatcher* second) {
			return *first < *second;
		}
	};

	IMatcher();
	virtual bool operator()(const Board::Index& index, const Board& board) = 0;
	
	virtual EObjectType getBooster() = 0;
	virtual bool hasBooster() = 0;
	const std::list<Board::Index> getMatchList() const;

	friend bool operator<(const IMatcher& first, const IMatcher& second);
};


#endif // !IMATCHER_HPP


