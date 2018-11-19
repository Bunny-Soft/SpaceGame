#include "enemy.h"
#include "gameWorld.h"
#include "physicsWorld.h"
#include "Box2D/Dynamics/b2Body.h"
#include "Defines.h"

USING_NS_CC;

// Creates Enemy as autorelease object 
Enemy* Enemy::createWithSpriteFrameName(Node* parent, const std::string filename)
{
	Enemy* pRet = new Enemy();

	if (pRet) {
		pRet->parent_ = parent;
		// This object retains parent object. Releases on destruction.
		if (pRet->initWithSpriteFrameName(filename)) {
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

// Enemy is inherited from Sprite. Initializes Sprite and local variables.
bool Enemy::initWithSpriteFrameName(const std::string filename) {
	if (!Sprite::initWithSpriteFrameName(filename)) {
		// Init failure
		return false;
	}

	hitpoints_ = cocos2d::RandomHelper::random_int((int)2, (int)6);

	Size size = Director::getInstance()->getVisibleSize();
	float random = cocos2d::RandomHelper::random_real(getContentSize().height, size.height - getContentSize().height);
	// Filename without path specifier
	std::string fnWithoutPath = filename.substr(filename.find_last_of("/") + 1);
	physicsBody_ = ((GameWorld*)parent_)->getPhysics()->createBody(cocos2d::Vec3(size.width + getContentSize().width, random, 0.0f), physicsWorld::ENTITY_METEOR, fnWithoutPath, this);

	// Set this game object as user data
	physicsBody_->SetUserData(this);
	//physicsBody_->SetGravityScale(0.0f);

	// Apply velocity and rotation of meteors.
	physicsBody_->ApplyLinearImpulse(b2Vec2(cocos2d::RandomHelper::random_real(-30.0f, -20.0f), cocos2d::RandomHelper::random_real(-3.0f, 3.0f)), physicsBody_->GetWorldCenter(), true);
	physicsBody_->ApplyAngularImpulse(cocos2d::RandomHelper::random_real(-5.0f, 5.0f), true);
	// Meteors defy gravity. First impulse needs to be on creation or first tick slightly gives gravity vector momentum
	physicsBody_->ApplyForce(physicsBody_->GetMass() * -((GameWorld*)parent_)->getPhysics()->world_->GetGravity(), physicsBody_->GetWorldCenter(), true);

	// Set sprite position to body position
	this->setPosition(physicsBody_->GetPosition().x * BOX2D_TO_GL_COORD, physicsBody_->GetPosition().y * BOX2D_TO_GL_COORD);
	// Hide game object on creation. 
	auto hideAction = Hide::create();
	this->runAction(hideAction);
	parent_->addChild(this, 5);

	return true;
}

//  Update loop for Enemy
void Enemy::update(float delta)
{
	if (hitpoints_ > 0) {
		// Update sprite position using physics body
		b2Vec2 pos = physicsBody_->GetPosition();
		setPosition(pos.x * BOX2D_TO_GL_COORD, pos.y * BOX2D_TO_GL_COORD);
		setRotation(-physicsBody_->GetAngle() * RADTODEG);

		// Defy gravity
		physicsBody_->ApplyForce(physicsBody_->GetMass() * -physicsBody_->GetWorld()->GetGravity(), physicsBody_->GetWorldCenter(), true);
	}
}

Enemy::~Enemy() {
}

void Enemy::onExit() {
	physicsBody_->GetWorld()->DestroyBody(physicsBody_);
	Node::onExit();
}