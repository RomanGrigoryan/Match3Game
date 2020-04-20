#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <string>
#include <vector>
#include <map>

#include "Object.hpp"

#include "../include/rapidjson/document.h"

class Settings
{
private:
	unsigned _rowCount;
	unsigned _columnCount;
	unsigned _movesCount;
	std::map<EObjectType, unsigned> _objectives;
	std::vector<EObjectType> _availableTypes;
	unsigned _figuresColorsCount;

private:
	Settings();
	Settings(const Settings&) = delete;
	Settings& operator=(const Settings &) = delete;
	Settings(Settings&&) = delete;
	Settings & operator=(Settings &&) = delete;

	bool parseJson(FILE* jsonFile, FILE* jsonSchema);

	void parseSettings(const rapidjson::Document& document);
	inline EObjectType EObjectByString(const std::string& string);


public:
	static Settings& instance();

	bool loadSettings();

	unsigned getRowCount() const;
	unsigned getColumnCount() const;
	unsigned getMovesCount() const;
	const std::map<EObjectType, unsigned>& getObjectives()const;
	unsigned getFiguresColorsCount() const;
	const std::vector<EObjectType>& Settings::getAvailableTypes() const;
};
#endif // !SETTINGS_HPP


