#include "physicsWorld.h"
#include "Box2D/Dynamics/b2Body.h"
#include "Defines.h"
#include "Utils/PhysicsBodyEditorLoader.h"	

physicsWorld* physicsWorld::create(Node* parent)
{
	physicsWorld* pRet = new physicsWorld();

	if (pRet) {
		pRet->parent_ = parent;
		if (pRet->init()) {
			pRet->autorelease();
		}
		else {
			CC_SAFE_RELEASE(pRet);
		}
	}
	else {
		CC_SAFE_RELEASE(pRet);
	}

	return pRet;
}

bool physicsWorld::init()
{
	if (!Node::init()) return false;

	// Gravity
	b2Vec2 gravity(0.0f, -15.0f);
	cocos2d::log("Gravity vector set to (0,-15)");
	// Make gravity 0 and allow objects to sleep
	world_ = new b2World(gravity);
	cocos2d::log("Physics world created!");
	
	timeStep_ = 1 / 60.0f;
	velocityIterations_ = 8;   
	positionIterations_ = 3;

	cocos2d::log("Using PBE loader to load fixture data!");
	loader_ = new PhysicsBodyEditorLoader("graphics/fixtures.json");

	return true;
}

physicsWorld::~physicsWorld() {
	// Release memory we allocated at init
	cocos2d::log("destroying physics world!");
	CC_SAFE_DELETE(world_);
}

void physicsWorld::onExit() {
	cocos2d::log("Physics onExit");
	CC_SAFE_DELETE(loader_);
	Node::onExit();
}

void physicsWorld::update()
{
	world_->Step(timeStep_, velocityIterations_, positionIterations_);
}

b2Body * physicsWorld::createBody(cocos2d::Vec3 orientation, int category, std::string filename, cocos2d::Node* node)
{
	// Create body definition depending of the category of the entity
	b2BodyDef myBodyDef;
	if (category == ENTITY_PLAYER) {
		myBodyDef.type = b2_kinematicBody;
		myBodyDef.angle = -orientation.z * DEGTORAD;
	}
	else if (category == ENTITY_METEOR) {
		myBodyDef.type = b2_dynamicBody;
		myBodyDef.angle = orientation.z * DEGTORAD;
		myBodyDef.active = false;
	}
	else if (category == ENTITY_PROJECTILE) {
		myBodyDef.type = b2_dynamicBody;
		myBodyDef.angle = orientation.z * DEGTORAD;
		// This is bullet
		myBodyDef.bullet = true;
		// Initially this bullet is inactive at first.
		myBodyDef.active = false;
	}
	else {
		cocos2d::log("Entity created: %d", category);
	}
	myBodyDef.position.Set(orientation.x * GL_TO_BOX2D_COORD, orientation.y * GL_TO_BOX2D_COORD);
	
	// Create body
	b2Body* dynamicBody = world_->CreateBody(&myBodyDef);

	// Fixture with masks depending of the category
	b2FixtureDef fixture;
	fixture.density = 1;
	if (category == ENTITY_PLAYER) {
		fixture.filter.categoryBits = ENTITY_PLAYER;
		fixture.filter.maskBits = ENTITY_METEOR;
		// Create sensor to player
		b2FixtureDef fixtureSensor;
		b2CircleShape circleShape;
		circleShape.m_radius = 8;
		fixtureSensor.shape = &circleShape;
		fixtureSensor.isSensor = true;
		fixtureSensor.filter.categoryBits = PLAYER_SENSOR;
		fixtureSensor.filter.maskBits = ENTITY_METEOR;
		dynamicBody->CreateFixture(&fixtureSensor);
	}
	else if (category == ENTITY_METEOR) {
		fixture.density = 3;
		fixture.filter.categoryBits = ENTITY_METEOR;
		fixture.filter.maskBits = ENTITY_PLAYER | ENTITY_PROJECTILE | ENTITY_STATICS_GENERAL | ENTITY_STATICS_METEOR_CATCHER | ENTITY_METEOR | PLAYER_SENSOR;
	}
	else if (category == ENTITY_PROJECTILE) {
		fixture.density = 0.1f;
		fixture.filter.categoryBits = ENTITY_PROJECTILE;
		fixture.filter.maskBits = ENTITY_METEOR | ENTITY_STATICS_GENERAL;
	}
	// This is our own PBE-loader. It finds and applies vertices as fixture shape from the json file provided
	loader_->attachFixture(dynamicBody, filename, &fixture, node);
	dynamicBody->CreateFixture(&fixture);

	return dynamicBody;
}

void physicsWorld::createStatics(cocos2d::Size size, cocos2d::Vec2 origin) {
	int bodyWidth = 1;

	// Bullet catcher
	b2BodyDef catcher1bodyDef;
	catcher1bodyDef.type = b2_staticBody; //this will be a static body
	catcher1bodyDef.position.Set(size.width * GL_TO_BOX2D_COORD + bodyWidth*20, size.height / 2 * GL_TO_BOX2D_COORD);
	b2Body* staticBody = world_->CreateBody(&catcher1bodyDef); //add body to world
	b2FixtureDef catcher1Fixture;
	b2PolygonShape catcher1Shape;
	catcher1Shape.SetAsBox(bodyWidth*4, size.height);
	catcher1Fixture.shape = &catcher1Shape;
	catcher1Fixture.filter.categoryBits = ENTITY_STATICS_GENERAL;
	catcher1Fixture.filter.maskBits = ENTITY_PROJECTILE;
	catcher1Fixture.density = 1;
	staticBody->CreateFixture(&catcher1Fixture); //add fixture to body

	// Meteor catcher
	b2BodyDef catcher2bodyDef;
	catcher2bodyDef.type = b2_staticBody; //this will be a static body
	catcher2bodyDef.position.Set(bodyWidth * 2 -bodyWidth * 6, size.height / 2 * GL_TO_BOX2D_COORD);
	b2Body* catcher2StaticBody = world_->CreateBody(&catcher2bodyDef); //add body to world
	b2FixtureDef catcher2Fixture;
	b2PolygonShape catcher2Shape;
	catcher2Shape.SetAsBox(bodyWidth, size.height);
	catcher2Fixture.shape = &catcher2Shape;
	catcher2Fixture.filter.categoryBits = ENTITY_STATICS_METEOR_CATCHER;
	catcher2Fixture.filter.maskBits = ENTITY_METEOR;
	catcher2Fixture.density = 1;
	catcher2StaticBody->CreateFixture(&catcher2Fixture); //add fixture to body

	// Top collider
	b2BodyDef catcher3bodyDef;
	catcher3bodyDef.type = b2_staticBody; //this will be a static body
	catcher3bodyDef.position.Set(size.width / 2 * GL_TO_BOX2D_COORD, size.height * GL_TO_BOX2D_COORD + bodyWidth);
	b2Body* catcher3StaticBody = world_->CreateBody(&catcher3bodyDef); //add body to world
	b2FixtureDef catcher3Fixture;
	b2PolygonShape catcher3Shape;
	catcher3Shape.SetAsBox(size.width, bodyWidth);
	catcher3Fixture.shape = &catcher3Shape;
	catcher3Fixture.filter.categoryBits = ENTITY_STATICS_GENERAL;
	catcher3Fixture.filter.maskBits = ENTITY_METEOR | ENTITY_PLAYER;
	catcher3Fixture.density = 1;
	catcher3StaticBody->CreateFixture(&catcher3Fixture); //add fixture to body

	// Bottom collider
	b2BodyDef catcher4bodyDef;
	catcher4bodyDef.type = b2_staticBody;
	catcher4bodyDef.position.Set(size.width / 2 * GL_TO_BOX2D_COORD, -bodyWidth / 2);
	b2Body* catcher4StaticBody = world_->CreateBody(&catcher4bodyDef);
	b2FixtureDef catcher4Fixture;
	b2PolygonShape catcher4Shape;
	catcher4Shape.SetAsBox(size.width, bodyWidth / 2);
	catcher4Fixture.shape = &catcher4Shape;
	catcher4Fixture.filter.categoryBits = ENTITY_STATICS_GENERAL;
	catcher4Fixture.filter.maskBits = ENTITY_METEOR | ENTITY_PLAYER;
	catcher4Fixture.density = 1;
	catcher4StaticBody->CreateFixture(&catcher4Fixture);
}
