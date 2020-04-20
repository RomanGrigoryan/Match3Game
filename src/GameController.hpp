
#ifndef GAMECONTROLLER_HPP
#define GAMECONTROLLER_HPP

#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <set>
#include "TopBar.hpp"
#include "Board.hpp"
#include "IMatcher.hpp"
#include "IBooster.hpp"

enum GameStatus {
    NotStarted,
    Started,
    Won,
    Failed,
    WaitForRestart,
    Restart,
};


class GameController 
{
    enum Status {
        HendlingEvents,
        Swap,
        CheckForMatchis,
        ActivateBooster,
        DeleteMatchis,
        CheckForWin,
        FillBooster,
        Refill,
        CheckForNewMatchis,
        Shuffle,
        CancleSwap,
    };

private:
    GameStatus _gameStatus = NotStarted;
    Status _status = HendlingEvents;
    sf::RenderWindow* _renderWindow;
    Board::Index _firstSelectedItem;
    Board::Index _secondSelectedItem;
    Board::Index _boosterIndex;

    std::multiset<IMatcher*, IMatcher::comparator> _matchers;
    std::list<Board::Index> _itemsToDelete;
    std::list<Board::Index> _newlyAddedItems;
    std::list<std::pair<EObjectType, Board::Index>> _avalibaleBoosters;
    
    std::map<EObjectType, IBooster*> _boosters;

    unsigned _moveCount;
    std::map<EObjectType, unsigned> _objectives;
    sf::Text _gameStatusLable;
    sf::Text _pressEnterToContinueLable;

    Board* _board;
    TopBar* _topBar;

    std::list<sf::Drawable*> _drawableItems;

private:
    GameController();
	GameController(const GameController&) = delete;
	GameController(GameController&&) = delete;
	GameController& operator=(const GameController&) = delete;

    void initGame();
    void inGame();

    void initMatchers();
    void initBoosters();

    void initTopBar();

    void run();
    
    void handleEvent(sf::Event& event);
    void handelMatchis();
    void handelNewMatchis();

    void checkForWin();
    void shuffle();

    void deleteObjects();
    void fillBooster();
    void fillObjects();
    void resetSelection();
    void activateBooster();

    void won();
    void failed();
    void waitForRestart();
    void restart();


public:
	static GameController& instance();
    void startGame();

    void updateGameStatus(GameStatus &status);
};

#endif /* GAMECONTROLLER_HPP */