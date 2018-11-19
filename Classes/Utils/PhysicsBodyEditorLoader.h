#pragma once

#include "cocos2d.h"
#include "Utils/JSON/json.hpp"
#include "Box2D/Box2D.h"

using json = nlohmann::json;

class PhysicsBodyEditorLoader {
public:
	// Load json file at constructor
	PhysicsBodyEditorLoader(const std::string filename);

	void attachFixture(b2Body* body, std::string nameOfTheFixture, b2FixtureDef* fixtureDefinition, cocos2d::Node* node);
	//json printValues(json* it);

	~PhysicsBodyEditorLoader();

private:
	// No default constructor access
	PhysicsBodyEditorLoader() {}
	// No copy constructor access
	PhysicsBodyEditorLoader(PhysicsBodyEditorLoader&) {}

	void parseData();

	// Main content loaded
	json content_;
	// Maps for json data
	std::map<std::string, b2Vec2> origins_;
	std::map<std::string, std::vector<b2Vec2> > vertices_;
};

PhysicsBodyEditorLoader::PhysicsBodyEditorLoader(const std::string filename) {
	cocos2d::FileUtils* fu = cocos2d::FileUtils::getInstance();
	if (fu->isFileExist(filename)) {
		// File exists
		std::string s = fu->getStringFromFile(filename);
		content_ = json::parse(s);
		parseData();
	}
	else {
		cocos2d::log("File does not exist: %s", filename.c_str());
	}
}

void PhysicsBodyEditorLoader::parseData() {
	if (!content_.is_null()) {
		if (content_.find("rigidBodies") != content_.end()) {
			// Rigidbodies found
			json::iterator it = content_.find("rigidBodies");
			json rigidbodies = it.value();
			int size = rigidbodies.size();
			b2Vec2 origin;
			for (int i = 0; i < size; ++i) {
				// What is current name
				std::string name = rigidbodies.at(i).find("name").value();
				// This is the data we are looking for
				it = rigidbodies.at(i).find("origin");

				// Origin saved to map
				origin = b2Vec2((float)it.value()["x"], (float)it.value()["y"]);
				origins_.insert(std::pair<std::string, b2Vec2>(name, origin));

				it = rigidbodies.at(i).find("shapes");
				json shapes = it.value();
				int size = shapes.size();
				for (int i = 0; i < size; ++i) {
					if (shapes.at(i).find("vertices") != shapes.at(i).end()) {
						it = shapes.at(i).find("vertices");
						json vertices = it.value();
						std::vector<b2Vec2> verts;
						for (int i = 0; i < vertices.size(); ++i) {
							b2Vec2 vec((float)vertices.at(i)["x"], (float)vertices.at(i)["y"]);
							// Here we have all the vertices as b2Vec2 array. Assign it to fixture def
							verts.push_back(vec);
						}
						// Save vertices to map
						vertices_.insert(std::pair<std::string, std::vector<b2Vec2> >(name, verts));
						verts.clear();
					}
				}
			}
		}
	}
	else
		cocos2d::log("Content is null! Are you sure you loaded correct file?");

}

void PhysicsBodyEditorLoader::attachFixture(b2Body* body, std::string nameOfTheFixture, b2FixtureDef* fixtureDefinition, cocos2d::Node* node) {

	// some offsets to box2d world coordinates
	float width = node->getContentSize().width * GL_TO_BOX2D_COORD;
	float offx = -node->getAnchorPoint().x*node->getContentSize().width * GL_TO_BOX2D_COORD;
	float offy = -node->getAnchorPoint().y*node->getContentSize().height * GL_TO_BOX2D_COORD;

	b2PolygonShape* shape = new b2PolygonShape();
	std::map<std::string, std::vector<b2Vec2> >::iterator it = vertices_.find(nameOfTheFixture);
	std::vector<b2Vec2> vertices = it->second;
	std::vector<b2Vec2> v;
	for (int i = 0; i < vertices.size(); ++i) {
		b2Vec2 vec(offx + width * vertices.at(i).x, offy + width * vertices.at(i).y);
		v.push_back(vec);
	}
	shape->Set(v.data(), v.size());
	fixtureDefinition->shape = shape;
}

PhysicsBodyEditorLoader::~PhysicsBodyEditorLoader() {
	content_.clear();
	origins_.clear();
	vertices_.clear();
}