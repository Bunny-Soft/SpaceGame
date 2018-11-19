#pragma once

#include "cocos2d.h"

class b2Body;

// Inherit player from sprite so we get lots of usefull data already implemented
class Player : public cocos2d::Sprite {
public:
	/*
	* Create player game object using cocos2D memory management pattern. This is an autorelease object.
	* @Param Parent node and in this case our GameWorld. This class retains parent node autorelease pointer.
	* @Param std::string filename used for sprite.
	* @Return Pointer to Player.
	*/
	static Player* createWithSpriteFrameName(cocos2d::Node* parent, const std::string filename);
	
	// Update method to update gamelogic
	void update(float delta);

	// Player status can be changed from dead to alive
	void setPlayerStatus(bool alive);

	b2Body* playerBody_;

	// Query if player is alive or not.
	const bool getPlayerStatus() const;

	// Increment or decrement nearby enemies
	void addNearbyEnemy(b2Body*);
	void removeNearbyEnemy(b2Body*);
	std::vector<b2Body*> getNearbyEnemies();

	// When collision triggers store bullet back to player bulletStack.
	void storeBullet(b2Body*);
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	void onMouseDown(cocos2d::Event *event);
	void onMouseUp(cocos2d::Event *event);
	void onMouseMove(cocos2d::Event *event);
#else
	bool touchBegan(cocos2d::Touch * touch, cocos2d::Event* usedEvent);
	void touchMoved(cocos2d::Touch * touch, cocos2d::Event* usedEvent);
	void touchEnded(cocos2d::Touch * touch, cocos2d::Event* usedEvent);
	void touchCanceled(cocos2d::Touch * touch, cocos2d::Event* usedEvent);
#endif

	void startListeningInput();
	void stopListeningInput();

	virtual void onExit() override;
private:
	// Initialize Player and base class cocos2d::Sprite
	virtual bool initWithSpriteFrameName(const std::string filename);

	// Private constructor because this is using cocos2d memory management
	Player() {}
	virtual ~Player();

	// Parent game world.
	cocos2d::Node* parent_;

	// Player variables for reloading, velocity, etc
	float hitpoints_;
	float reloadTime_;
	float timeSinceFire_;
	float playerShipVelocity_;
	cocos2d::Vec2 goToPos_;
	bool mouseDown_;

	// Physical presentaion of player ship in Box2D-world.
	
	//b2Body* playerBody_;
	// Bullet bodies are generated here for fast usage and zero run-time memory allocations.
	std::stack<b2Body*> bulletBodies_;

	// Various different useful variables.
	unsigned int soundid_;
	bool playerAlive_;
	bool isOdd_;
	bool alreadySpawned_;

	std::vector<b2Body*> enemiesInSensorRange_;
};