#pragma once

#include "cocos2d.h"
#include "Box2D/Box2D.h"

class PhysicsBodyEditorLoader;

class physicsWorld : public cocos2d::Node {
public:

	// Constructor
	static physicsWorld* create(cocos2d::Node* parent_);

	virtual bool init() override;
	void update();

	/**
	* Interface to create box2d physics body for specific game object.
	*
	*
	* @param   Vec3 orientation where x/y are 2D position and z is rotation.
	* @return  Pointer to b2Body
	*/
	b2Body * createBody(cocos2d::Vec3 orientation, int category, std::string filename, cocos2d::Node* node);
	void createStatics(cocos2d::Size size, cocos2d::Vec2 origin);

	// This is our box2d physicsWorld. Keep it public for now
	b2World* world_;

	~physicsWorld();

	enum ENT_CATEGORY {
	// Entity identifiers; player is B(0000 0000 0000 0001), meteor is B(0000 0000 0000 0010) and so forth
	ENTITY_PLAYER = 0x0001,
	ENTITY_METEOR = 0x0002,
	ENTITY_PROJECTILE = 0x0004,
	ENTITY_STATICS_GENERAL = 0x0008,
	ENTITY_STATICS_METEOR_CATCHER = 0x0010,
	PLAYER_SENSOR = 0x0020,
	};

	virtual void onExit() override;


private:
	float32 timeStep_;			// timestep
	int32 velocityIterations_;  //velocity corrections
	int32 positionIterations_;  //position corrections

	// Physics body editor (PBE) loader
	PhysicsBodyEditorLoader* loader_;

	// Parent which means gameWorld here
	cocos2d::Node* parent_;
	
};
