#include <SFML/Graphics.hpp>
#include "GameController.hpp"
#include "Settings.hpp"
#include "ResourceManager.hpp"
#include "AnimationManager.hpp"
#include "HorizontalMatcher.hpp"
#include "VerticalMatcher .hpp"
#include "SquareMatcher.hpp"
#include "HorizontalBooster.hpp"
#include "VerticalBooster.hpp"
#include "SquareBooster.hpp"

#include <math.h>
#include <iostream>

GameController::GameController()
{
	initMatchers();
	initBoosters();

}

GameController& GameController::instance()
{
	static GameController instance;
	return instance;
}

void GameController::initMatchers()
{
	_matchers.insert(new HorizontalMatcher());
	_matchers.insert(new VerticalMatcher());
	_matchers.insert(new SquareMatcher());
}

void GameController::initBoosters()
{
	IBooster* booster = new HorizontalBooster();
	_boosters[booster->getType()] = booster;

	booster = new VerticalBooster();
	_boosters[booster->getType()] = booster;

	booster = new SquareBooster();
	_boosters[booster->getType()] = booster;
}

void GameController::updateGameStatus(GameStatus &status) 
{
    _gameStatus = status;
}

void GameController::initTopBar()
{
	_moveCount = Settings::instance().getMovesCount();
	_objectives = Settings::instance().getObjectives();
	_topBar = new TopBar(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(_renderWindow->getSize().x, 100.0f));
	
	for (auto objectiv : _objectives)
	{
		_topBar->insertObjectives(objectiv.first, std::to_string(objectiv.second));
	}
	_topBar->setMoveLabelText(std::to_string(_moveCount));
}

void GameController::initGame()
{
	initTopBar();
	_gameStatusLable.setFont(ResourceManager::instance().getFont());
	_gameStatusLable.setCharacterSize(100);
	_pressEnterToContinueLable.setFont(ResourceManager::instance().getFont());

	_pressEnterToContinueLable.setString("Press Enter To Restart");
	_pressEnterToContinueLable.setCharacterSize(50);
	_pressEnterToContinueLable.setColor(sf::Color::Black);
	auto localBounds = _pressEnterToContinueLable.getLocalBounds();
	_pressEnterToContinueLable.setOrigin(localBounds.left + localBounds.width / 2.0f, localBounds.top + localBounds.height / 2.0f);
	_pressEnterToContinueLable.setPosition(375, 600);

	_board = new Board(Settings::instance().getAvailableTypes(),
		Settings::instance().getRowCount(),
		Settings::instance().getColumnCount(),
		sf::IntRect(50, 300, 700, 600)
	);
	_board->fill();
	_status = Shuffle;
	_gameStatus = GameStatus::Started;
	_drawableItems.push_back(_board);
	_drawableItems.push_back(_topBar);
}

void GameController::inGame()
{
	AnimationManager::instance().exec();
	if (AnimationManager::instance().empty()) {
		switch (_status)
		{
		case GameController::HendlingEvents:
			sf::Event event;
			while (_renderWindow->pollEvent(event))
				handleEvent(event);
			break;
		case GameController::Swap:
			_board->swapObjects(_firstSelectedItem, _secondSelectedItem);
			_status = CheckForMatchis;
			break;
		case GameController::CheckForMatchis:
			handelMatchis();
			break;
		case GameController::ActivateBooster:
			activateBooster();
			break;
		case GameController::DeleteMatchis:
			deleteObjects();
			_status = CheckForWin;
			break;
		case GameController::CheckForWin:
			checkForWin();
			break;
		case GameController::FillBooster:
			fillBooster();
			_status = Refill;
			break;
		case GameController::Refill:
			fillObjects();
			_status = CheckForNewMatchis;
			break;
		case GameController::CheckForNewMatchis:
			handelNewMatchis();
			break;
		case GameController::Shuffle:
			shuffle();
			_status = GameController::HendlingEvents;
			break;
		case GameController::CancleSwap:
			_board->swapObjects(_firstSelectedItem, _secondSelectedItem);
			resetSelection();
			_status = HendlingEvents;
			break;
		default:
			break;
		}
	}
}

void GameController::startGame() 
{
	if (!Settings::instance().loadSettings())
	{
		std::cout << "Settings are incorrect!!!" << std::endl;
		return;
	}
	
	_renderWindow = new sf::RenderWindow(sf::VideoMode(800, 1000), "Game", sf::Style::Close);
	
	_renderWindow->setFramerateLimit(60);
	run();
}

void GameController::handelMatchis()
{
	bool matched = false;
	for (auto matcher: _matchers) {
		if ( (*matcher)(_firstSelectedItem, *_board)) {
			std::list<Board::Index> indexList = matcher->getMatchList();
			_itemsToDelete.splice(_itemsToDelete.end(), indexList);
			if (matcher->hasBooster())
				_avalibaleBoosters.push_back(std::make_pair(matcher->getBooster(), _firstSelectedItem));
			matched = true;
			break;
		}
	}

	for (auto matcher : _matchers) {
		if ((*matcher)(_secondSelectedItem, *_board)) {
			std::list<Board::Index> indexList = matcher->getMatchList();
			_itemsToDelete.splice(_itemsToDelete.end(), indexList);
			if (matcher->hasBooster())
				_avalibaleBoosters.push_back(std::make_pair(matcher->getBooster(), _secondSelectedItem));
			matched = true;
			break;
		}
	}
	if (matched) {
		_moveCount--;
		_topBar->setMoveLabelText(std::to_string(_moveCount));
	}
	_status = matched ? DeleteMatchis : CancleSwap;
}

void GameController::handleEvent(sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::Closed:
	{
		// "close requested" event: we close the window
		_renderWindow->close();
		break;
	}
	case sf::Event::MouseButtonReleased:
	{
		if (!_firstSelectedItem.isValid()) {
			_firstSelectedItem = _board->indexByCordinates(event.mouseButton.x, event.mouseButton.y);
			_board->getObject(_firstSelectedItem)->setSelected(true);
			if (_board->getObject(_firstSelectedItem)->isOfType(EObjectMask::BOOSTER))
			{
				_status = ActivateBooster;
				_boosterIndex = _firstSelectedItem;
			}
			break;
		}
		_secondSelectedItem =  _board->indexByCordinates(event.mouseButton.x, event.mouseButton.y);
		if (_board->getObject(_secondSelectedItem)->isOfType(EObjectMask::BOOSTER)) {
			_status = ActivateBooster;
			_boosterIndex = _secondSelectedItem;
			break;
		}
		_board->getObject(_secondSelectedItem)->setSelected(true);
		if (((std::abs(int(_firstSelectedItem.row) - int(_secondSelectedItem.row)) +
			(std::abs(int(_firstSelectedItem.column) - int(_secondSelectedItem.column))) == 1))
			&&( _board->getObject(_firstSelectedItem)->type() != _board->getObject(_secondSelectedItem)->type() )){
			_status = Swap;
			break;
		}
		else {
			resetSelection();
		}
		break;
	}

	default:
		break;
	}
}

void GameController::resetSelection()
{
	if (_firstSelectedItem.isValid()) _board->getObject(_firstSelectedItem)->setSelected(false);
	if (_secondSelectedItem.isValid()) _board->getObject(_secondSelectedItem)->setSelected(false);
	_firstSelectedItem = Board::Index();
	_secondSelectedItem = Board::Index();
}

void GameController::deleteObjects()
{
	resetSelection();
	for (auto index: _itemsToDelete){
		auto type = _board->getObject(index)->type();
		if (_objectives[type] != 0) {
			_objectives[type]--;
			_topBar->setObjectiveLabelText(type, std::to_string(_objectives[type]));
		}
		_board->deleteObject(index);
	}
	_itemsToDelete.clear();
}

void GameController::fillBooster()
{
	for (auto booster : _avalibaleBoosters){
		_board->insertObject(booster.first, booster.second, true);
	}
	_avalibaleBoosters.clear();
}

void GameController::fillObjects()
{
	_newlyAddedItems = _board->refill();
}

void GameController::handelNewMatchis()
{
	bool matched = false;
	for (auto index : _newlyAddedItems) {
		for (auto matcher : _matchers) {
			auto item = std::find(_itemsToDelete.begin(), _itemsToDelete.end(), index);
			if (item != _itemsToDelete.end() || _board->getObject(index)->isOfType(EObjectMask::BOOSTER))
				break;
			if ((*matcher)(index, *_board)) {
				std::list<Board::Index> indexList = matcher->getMatchList();
				_itemsToDelete.splice(_itemsToDelete.end(), indexList);
				if (matcher->hasBooster())
					_avalibaleBoosters.push_back(std::make_pair(matcher->getBooster(), index));
				matched = true;
				break;
			}
		}
	}

	
	_itemsToDelete.sort();
	_itemsToDelete.unique();
	if (matched)
		_status = DeleteMatchis;
	else
		_status = Shuffle;
}

void GameController::checkForWin()
{
	bool won = true;
	for (auto objective : _objectives) {
		if (objective.second != 0) {
			won = false;
			break;
		}
	}
	if (won) {
		_gameStatus = GameStatus::Won;
	}
	else if(_moveCount == 0){
		_gameStatus = GameStatus::Failed;
	}
	else {
		_status = FillBooster;
	}
}

void GameController::shuffle()
{
	bool canContinue = false;
	while (true)
	{
		//check
		for (int r = 0; r < _board->getRowCount(); r++) {
			bool matched = false;
			for (int c = 0; c < _board->getColumnCount() - 1; c++) {
				Board::Index firstIndex(r, c);
				Board::Index secondIndex(r, c + 1);
				if (_board->getObject(firstIndex)->isOfType(EObjectMask::BOOSTER) 
					|| _board->getObject(secondIndex)->isOfType(EObjectMask::BOOSTER))
					return;
				_board->swapObjects(firstIndex, secondIndex, true);
				for (auto matcher : _matchers) {
					if ((*matcher)(firstIndex, *_board)) {
						matched = true;
						break;
					}
					if ((*matcher)(firstIndex, *_board)) {
						matched = true;
						break;
					}
				}
				_board->swapObjects(firstIndex, secondIndex, true);
				if (matched)
					return;
			}
		}
		for (int c = 0; c < _board->getColumnCount(); c++) {
			bool matched = false;
			for (int r = 0; r < _board->getRowCount() - 1; r++) {
				Board::Index firstIndex(r, c);
				Board::Index secondIndex(r + 1, c);

				if (_board->getObject(firstIndex)->isOfType(EObjectMask::BOOSTER)
					|| _board->getObject(secondIndex)->isOfType(EObjectMask::BOOSTER))
					return;

				_board->swapObjects(firstIndex, secondIndex, true);
				for (auto matcher : _matchers) {
					if ((*matcher)(firstIndex, *_board)) {
						matched = true;
						break;
					}
					if ((*matcher)(firstIndex, *_board)) {
						matched = true;
						break;
					}
				}
				_board->swapObjects(firstIndex, secondIndex, true);
				if (matched)
					return;
			}
		}
		_board->shuffle();
	}
}

void GameController::won() 
{
	_gameStatusLable.setString("You Win!");
	_gameStatusLable.setFillColor(sf::Color::Red);

	auto localBounds = _gameStatusLable.getLocalBounds();
	_gameStatusLable.setOrigin(localBounds.left + localBounds.width / 2.0f, localBounds.top + localBounds.height / 2.0f);
	_gameStatusLable.setPosition(375, 400);
	
	_board->clean();

	_drawableItems.push_back(&_gameStatusLable);
	_drawableItems.push_back(&_pressEnterToContinueLable);

	_gameStatus = WaitForRestart;
}

void GameController::failed()
{
	
	_gameStatusLable.setString("You Lose!");
	_gameStatusLable.setFillColor(sf::Color::Yellow);
	
	auto localBounds = _gameStatusLable.getLocalBounds();
	_gameStatusLable.setOrigin(localBounds.left + localBounds.width / 2.0f, localBounds.top + localBounds.height / 2.0f);
	_gameStatusLable.setPosition(375, 400);

	_board->clean();
	_drawableItems.push_back(&_gameStatusLable);
	_drawableItems.push_back(&_pressEnterToContinueLable);
	_gameStatus = WaitForRestart;
}

void GameController::waitForRestart()
{
	sf::Event event;
	while (_renderWindow->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
		{
			// "close requested" event: we close the window
			_renderWindow->close();
			break;
		}
		case sf::Event::KeyPressed:
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
			{
				_gameStatus = Restart;
			}
			break;
		}

		default:
			break;
		}
	
	}
}

void GameController::restart()
{
	_drawableItems.clear();
	_drawableItems.push_back(_board);
	_drawableItems.push_back(_topBar);

	_moveCount = Settings::instance().getMovesCount();
	_objectives = Settings::instance().getObjectives();
	
	for (auto objectiv : _objectives)
	{
		_topBar->setObjectiveLabelText(objectiv.first, std::to_string(objectiv.second));
	}
	
	_topBar->setMoveLabelText(std::to_string(_moveCount));
	_board->fill();
	
	_status = HendlingEvents;
	_gameStatus = GameStatus::Started;
}

void GameController::run()
{
	while (_renderWindow->isOpen()) {

		switch (_gameStatus)
		{
		case NotStarted:
			initGame();
			break;
		case Started:
			inGame();
			break;
		case Won:
			won();
			break;
		case Failed:
			failed();
			break;
		case WaitForRestart:
			waitForRestart();
			break;
		case Restart:
			restart();
			break;
		default:
			break;
		}
		
		
		_renderWindow->clear(sf::Color(50, 70, 80, 255));
		for (auto drawable : _drawableItems)
			_renderWindow->draw(*drawable);
		_renderWindow->display();
		sf::Time delayTime = sf::milliseconds(100);
		sf::sleep(delayTime);
	}
}

void GameController::activateBooster()
{
	EObjectType type = _board->getObject(_boosterIndex)->type();
	if (_boosters.find(type) != _boosters.end()) {
		_itemsToDelete.splice(_itemsToDelete.end(), _boosters[type]->getInfluence(_boosterIndex, *_board));
		_status = DeleteMatchis;
		_moveCount--;
		_topBar->setMoveLabelText(std::to_string(_moveCount));
	} else {
		_status = HendlingEvents;
	}
}