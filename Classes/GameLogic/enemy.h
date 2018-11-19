#pragma once

#include "cocos2d.h"

class b2Body;

// inherit player from sprite so we get lots of usefull data already implemented
class Enemy : public cocos2d::Sprite {
public:
	/*
	* Create Enemy game object using cocos2D memory management pattern. This is an autorelease object.
	* @Param Parent node and in this case our GameWorld. This class retains parent node autorelease pointer.
	* @Param std::string filename used for sprite.
	* @Return Pointer to Enemy.
	*/
	static Enemy* createWithSpriteFrameName(cocos2d::Node* parent, const std::string filename);

	/*
	* Update Enemy status.
	* @Param Float time since last frame.
	*/
	void update(float delta);

	// Return physics body
	b2Body* getPhysicsBody() const;

	int hitpoints_;
	virtual void onExit() override;
private:
	/*
	* Enemy is inherited from Sprite. Initializes Sprite and local variables.
	* @Param Filename for SpriteFrame
	*/
	virtual bool initWithSpriteFrameName(const std::string filename);

	// Private constructor
	Enemy() {}
	// Private destructor
	virtual ~Enemy();

	// Parent game world. Used to access certain variables controlled by parent
	cocos2d::Node* parent_;

	// Box2d physical presentation of this game object.
	b2Body* physicsBody_;
};

inline b2Body* Enemy::getPhysicsBody() const
{
	return physicsBody_;
}