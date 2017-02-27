#include "parser.h"
#include "external/pugixml/src/pugixml.hpp"
#include "external/json11/json11.hpp"
#include "graphics/polygon.h"

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cassert>
#include <iterator>
#include <algorithm>
#include <QFile>
#include <QTextStream>
#include <QString>

using namespace pugi;
using namespace json11;

static const std::string RESOURCE_PATH = "://resources/";
static const std::string ENVIRONMENT_PATH = RESOURCE_PATH + "environments/";

struct EnvironmentType {
	bool _walkable;
	std::vector<Polygon> _polygons;

	EnvironmentType() {}
	EnvironmentType(bool walkable, std::vector<Polygon> polygons) : _walkable(walkable), _polygons(polygons) {}
};

xml_node getXML(const std::string & path) {
    QFile file(QString(path.c_str()));
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        std::string error = "Could not open file: " + path;
        std::cerr << "Line: " << __LINE__ << " FILE: " << __FILE__ << std::endl;
        std::cerr << "\tError: " << error << std::endl;
        assert(false);
    }
    QTextStream textStream(&file);

	xml_document doc;
    xml_parse_result result = doc.load_string(textStream.readAll().toStdString().c_str());
    file.close();
    if (!result) {
        std::cerr << "Line: " << __LINE__ << " FILE: " << __FILE__ << std::endl;
        std::cerr << "\tError: " << result.description() << std::endl;
		assert(false);
	}
	auto children = doc.root().children();
	if (std::distance(children.begin(), children.end()) > 1) {
		std::string error = "Too many root nodes in xml-document \'" + path + "\'";
		std::cerr << "Line: " << __LINE__ << " FILE: " << __FILE__ << std::endl;
        std::cerr << "\tError: " << error << std::endl;
		assert(false);
    }
	return doc.root().first_child();
}

Color XML_color(const xml_node & parent) {
	xml_node node = parent.child("color");
	return Color(
		node.attribute("r").as_int(), 
		node.attribute("g").as_int(),
		node.attribute("b").as_int()
	);
}

Vertex XML_vertex(const xml_node & node) {
	return Vertex(
        node.attribute("x").as_float(),
        node.attribute("y").as_float(),
        node.attribute("z").as_float()
	);
}

std::vector<Vertex> XML_verticies(const xml_node & parent) {
    std::vector<Vertex> verticies;
	for (const xml_node & vert : parent.child("verticies").children()) {
        verticies.push_back(XML_vertex(vert));
    }
	return verticies;
}

Polygon XML_polygon(const xml_node & parent) {
	Color color = XML_color(parent);
	std::vector<Vertex> verticies = XML_verticies(parent);
	return Polygon(color, verticies);
}

EnvironmentType XML_environmentType(const xml_node & parent) {
	bool walkable = parent.child("walkable").text().as_bool();
    std::vector<Polygon> polygons;
    for (const xml_node & poly : parent.child("polygons").children()) {
		polygons.push_back(XML_polygon(poly));
	}

	return EnvironmentType(walkable, polygons);
}

std::map<std::string, EnvironmentType> readEnvironmentTypes() {
	std::map<std::string, EnvironmentType> environments;

	std::vector<std::string> environmentFiles = {"water.xml", "grass.xml", "stone.xml"}; // Should not be hardcoded!
	for (const std::string filename : environmentFiles) {
        xml_node parent = getXML(ENVIRONMENT_PATH + filename);
        EnvironmentType envType = XML_environmentType(parent);
		std::string name = filename.substr(0, filename.find("."));
		environments[name] = envType;
	}
	return environments;
}

std::map<std::string, EnvironmentType> _environmentTypes;

bool isInitialized = false;
void parser::initialize() {
    if (isInitialized) {
        assert(false);
    }
    isInitialized = true;

    _environmentTypes = readEnvironmentTypes();
}

// ****************************************************************************

Environment parser::parseEnvironment(const Json & data) {
    int x = data["X"].int_value();
    int y = data["Y"].int_value();
	std::string type = data["Type"].string_value();
	std::transform(type.begin(), type.end(), type.begin(), ::tolower);

	if (_environmentTypes.find(type) == _environmentTypes.end()) {
		std::string error = "No environment type specified in JSON!";
        std::cerr << "Line: " << __LINE__ << " FILE: " << __FILE__ << std::endl;
        std::cerr << "\tError: " << error << std::endl;
	}

    EnvironmentType & envType = _environmentTypes[type];
    return Environment(x, y, envType._walkable, &envType._polygons);
}

Player * parser::parsePlayer(const Json & data) {
    std::string name = data["Name"].string_value();
    int x = data["X"].number_value();
    int y = data["Y"].number_value();
    int level = data["Level"].number_value();
    int health = data["Health"].number_value();
    int mana = data["Mana"].number_value();
    return new Player(name, x, y, level, health, mana);
}

// TODO: Should be parse_creature() in the future.
Troll * parser::parseTroll(const Json & data) {
    std::string name = data["Name"].string_value();
    int x = data["X"].number_value();
    int y = data["Y"].number_value();
    int health = data["Health"].number_value();
    int mana = data["Mana"].number_value();
    return new Troll(name, x, y, health, mana);
}
