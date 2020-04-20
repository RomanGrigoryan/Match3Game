#include "Settings.hpp"

#include "../include/rapidjson/document.h"
#include "../include/rapidjson/schema.h"
#include "../include/rapidjson/filereadstream.h"
#include "../include/rapidjson/stringbuffer.h"

Settings::Settings()
{}

Settings& Settings::instance()
{
	static Settings instance;
	return instance;
}

bool Settings::parseJson(FILE* jsonFile, FILE* jsonSchema)
{
	const unsigned bufferSize = 4096;
	char* buffer = new char[bufferSize];
	rapidjson::Document jsonDocument;
	rapidjson::FileReadStream jsonFileReadStream(jsonFile, buffer, sizeof(char) * bufferSize);

	jsonDocument.ParseStream(jsonFileReadStream);
	if (jsonDocument.HasParseError())
		return false;

	rapidjson::Document schemaDocument;
	rapidjson::FileReadStream schemaFileReadStream(jsonSchema, buffer, sizeof(char) * bufferSize);

	schemaDocument.ParseStream(schemaFileReadStream);
	if (schemaDocument.HasParseError())
		return false;

	rapidjson::SchemaDocument schema(schemaDocument);
	rapidjson::SchemaValidator validator(schema);

	if (!jsonDocument.Accept(validator))
		return false;
	
	parseSettings(jsonDocument);
	

	return true;
}

void Settings::parseSettings(const rapidjson::Document& document)
{
	auto board = document["Board"].GetObject();
	_rowCount = board["Row"].GetInt();
	_columnCount = board["Column"].GetInt();
	_movesCount = document["MovesCount"].GetInt();
	for (auto& objective : document["Objectives"].GetArray()) {
		EObjectType type = EObjectByString(objective["Color"].GetString());
		_objectives.insert(std::make_pair(type, objective["Value"].GetInt()));
	}
	_figuresColorsCount = document["FiguresColors"].GetInt();
	for(auto& iter: _objectives)
		_availableTypes.push_back(iter.first);
	unsigned type = (unsigned)EObjectType::RED;
	while (_availableTypes.size() != _figuresColorsCount) {
		if (std::find(_availableTypes.begin(), _availableTypes.end(), EObjectType (type)) == _availableTypes.end()) {
			_availableTypes.push_back(EObjectType(type));
		}
		type++;
	}
}

const std::vector<EObjectType>& Settings::getAvailableTypes() const
{
	return _availableTypes;
}

inline EObjectType Settings::EObjectByString(const std::string& string)
{
	if (string == "Red")
		return EObjectType::RED;
	else if (string == "Green")
		return EObjectType::GREEN;
	else if (string == "Blue")
		return EObjectType::BLUE;
	else if (string == "Orange")
		return EObjectType::ORANGE;
	else if (string == "Violet")
		return EObjectType::VIOLET;
	else
	{
		assert(0);
		return EObjectType::RED;
	}
}

bool Settings::loadSettings()
{
	FILE* settings = fopen("../Resources/settings.json", "r");

	FILE* validator = fopen("../Resources/settingsValidator.json", "r");

	bool result = parseJson(settings, validator);
	
	fclose(settings);
	fclose(validator);

	return result;
}

unsigned Settings::getRowCount() const
{
	return _rowCount;
}
unsigned Settings::getColumnCount() const
{
	return _columnCount;
}

unsigned Settings::getMovesCount() const
{
	return _movesCount;
}

const std::map<EObjectType, unsigned>& Settings::getObjectives()const
{
	return _objectives;
}

unsigned Settings::getFiguresColorsCount() const
{
	return _figuresColorsCount;
}