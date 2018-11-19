#include "contactListener.h"
#include "physicsWorld.h"
#include "player.h"
#include "enemy.h"
#include "Defines.h"
#include "gameWorld.h"

USING_NS_CC;

// Creates Enemy as autorelease object 
contactListener* contactListener::create(Node* parent, b2World* physicsWorld)
{
	contactListener* pRet = new contactListener();

	if (pRet) {
		pRet->parent_ = parent;
		// This object retains parent object. Releases on destruction.
		if (pRet->initWithWorld(physicsWorld)) {
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

bool contactListener::initWithWorld(b2World* physicsWorld) {
	if (!Node::init()) 
		return false;
	
	physicsWorld->SetContactListener(this);
	collisionId_ = 0;

	return true;
}

void contactListener::BeginContact(b2Contact* contact) {
	//check first fixture
	uint16 categoryA, categoryB;
	categoryA = contact->GetFixtureA()->GetFilterData().categoryBits;
	categoryB = contact->GetFixtureB()->GetFilterData().categoryBits;

	switch (categoryA) {
	case physicsWorld::ENTITY_PLAYER:
		switch (categoryB) {
		case physicsWorld::ENTITY_METEOR:
			Player* p;
			Enemy* e;

			p = (Player*)contact->GetFixtureA()->GetBody()->GetUserData();
			e = (Enemy*)contact->GetFixtureB()->GetBody()->GetUserData();

			p->setPlayerStatus(false);
			e->hitpoints_ -= 1;
			if (e->hitpoints_ < 1) {
				enemiesMarkedForRemoval_.insert(e);
				playerScore_ += 50;
			}
			break;
		case physicsWorld::ENTITY_STATICS_GENERAL:
			// Player hit statics
			break;
		}
		break;
	case physicsWorld::ENTITY_METEOR:
		switch (categoryB) {
		case physicsWorld::ENTITY_PLAYER:
		{
			Player* p;
			Enemy* e;

			p = (Player*)contact->GetFixtureB()->GetBody()->GetUserData();
			e = (Enemy*)contact->GetFixtureA()->GetBody()->GetUserData();

			p->setPlayerStatus(false);
			e->hitpoints_ -= 1;
		}
			break;
		case physicsWorld::ENTITY_PROJECTILE:
		{
			// Meteor hit projectile
			Enemy* e;
			e = (Enemy*)(contact->GetFixtureA()->GetBody()->GetUserData());
			e->hitpoints_ -= 1;
			playerScore_ += 5;
			if (e->hitpoints_ < 1) {
				enemiesMarkedForRemoval_.insert(e);
				playerScore_ += 50;
			}
			// Get world manifold and use that to store collision data
			b2WorldManifold worldManifold;
			contact->GetWorldManifold(&worldManifold);
			collisionData cData(collisionId_++, Vec2(worldManifold.points[0].x * BOX2D_TO_GL_COORD, worldManifold.points[0].y * BOX2D_TO_GL_COORD), COLLISION_TYPE::PROJECTILE_TO_METEOR);
			collisionsThisFrame_.insert(cData);
			bulletsMarkedForRemoval_.insert(contact->GetFixtureB()->GetBody());
		}
			break;
		case physicsWorld::ENTITY_STATICS_GENERAL:
			// Meteor hit statics
			break;
		case physicsWorld::ENTITY_STATICS_METEOR_CATCHER:
			Enemy* e;
			e = (Enemy*)(contact->GetFixtureA()->GetBody()->GetUserData());
			enemiesMarkedForRemoval_.insert(e);
			playerScore_ -= 25;
			break;
		case physicsWorld::PLAYER_SENSOR:
			b2Body* body = contact->GetFixtureB()->GetBody();
			Player* player = (Player*)body->GetUserData();
			if (player != nullptr) {
				cocos2d::log("Meteor entered!");
				player->addNearbyEnemy(contact->GetFixtureA()->GetBody());
			}
			break;
		}
		break;
	case physicsWorld::ENTITY_PROJECTILE:
		switch (categoryB) {
		case physicsWorld::ENTITY_METEOR:
		{
			// Meteor hit projectile
			Enemy* e;
			e = (Enemy*)(contact->GetFixtureB()->GetBody()->GetUserData());
			e->hitpoints_ -= 1;
			playerScore_ += 5;
			if (e->hitpoints_ < 1) {
				enemiesMarkedForRemoval_.insert(e);
				playerScore_ += 50;
			}

			//Get world manifold for accurate collision location
			b2WorldManifold worldManifold;
			contact->GetWorldManifold(&worldManifold);
			collisionData cData(collisionId_++, Vec2(worldManifold.points[0].x * BOX2D_TO_GL_COORD, worldManifold.points[0].y * BOX2D_TO_GL_COORD), COLLISION_TYPE::PROJECTILE_TO_METEOR);
			collisionsThisFrame_.insert(cData);
			bulletsMarkedForRemoval_.insert(contact->GetFixtureA()->GetBody());
			cocos2d::log("There are %d bullets and %d collisions waiting to be processed!", bulletsMarkedForRemoval_.size(), collisionsThisFrame_.size());
		}
			break;
		case physicsWorld::ENTITY_STATICS_GENERAL:
			// Projectile hit statics
			bulletsMarkedForRemoval_.insert(contact->GetFixtureA()->GetBody());
			break;
		}
		break;
	case physicsWorld::ENTITY_STATICS_GENERAL:
		switch (categoryB) {
		case physicsWorld::ENTITY_PLAYER:
			// Meteor hit player
			break;
		case physicsWorld::ENTITY_PROJECTILE:
			// Meteor hit projectile
			bulletsMarkedForRemoval_.insert(contact->GetFixtureB()->GetBody());
			break;
		case physicsWorld::ENTITY_METEOR:
			// Meteor hit statics
			break;
		}
		break;
	case physicsWorld::ENTITY_STATICS_METEOR_CATCHER:
		switch (categoryB) {
		case physicsWorld::ENTITY_METEOR:
			Enemy* e;
			e = (Enemy*)(contact->GetFixtureB()->GetBody()->GetUserData());
			enemiesMarkedForRemoval_.insert(e);
			playerScore_ -= 25;
			break;
		}
		break;
	case physicsWorld::PLAYER_SENSOR:
		switch (categoryB) {
		case physicsWorld::ENTITY_METEOR:
			b2Body* body = contact->GetFixtureA()->GetBody();
			Player* player = (Player*)body->GetUserData();
			if (player != nullptr) {
				cocos2d::log("Meteor entered!");
				player->addNearbyEnemy(contact->GetFixtureB()->GetBody());
			}
			break;
		}
		break;
	}
}

void contactListener::EndContact(b2Contact* contact) {

	//check first fixture
	uint16 categoryA, categoryB;
	categoryA = contact->GetFixtureA()->GetFilterData().categoryBits;
	categoryB = contact->GetFixtureB()->GetFilterData().categoryBits;

	switch (categoryA) {
	case physicsWorld::PLAYER_SENSOR:
		switch (categoryB) {
		case physicsWorld::ENTITY_METEOR:
			b2Body* body = contact->GetFixtureA()->GetBody();
			Player* player = (Player*)body->GetUserData();
			if (player != nullptr) {
				cocos2d::log("Meteor left!");
				player->removeNearbyEnemy(contact->GetFixtureB()->GetBody());
			}
			break;
		}
		break;
	case physicsWorld::ENTITY_METEOR:
		switch (categoryB) {
		case physicsWorld::PLAYER_SENSOR:
			b2Body* body = contact->GetFixtureB()->GetBody();
			Player* player = (Player*)body->GetUserData();
			if (player != nullptr) {
				cocos2d::log("Meteor left!");
				player->removeNearbyEnemy(contact->GetFixtureA()->GetBody());
			}
			break;
		}
	}

}

const uint32_t contactListener::getScore()
{
	uint32_t returnValue = playerScore_;
	playerScore_ = 0;
	return returnValue;
}

std::set<b2Body*>& contactListener::getBulletsMarkedForRemoval()
{
	return bulletsMarkedForRemoval_;
}

std::set <Enemy *>& contactListener::getEnemiesMarkedForRemoval()
{
	return enemiesMarkedForRemoval_;
}

std::set< collisionData >& contactListener::getCollisionsThisStep()
{
	return collisionsThisFrame_;
}