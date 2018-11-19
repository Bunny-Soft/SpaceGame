#include "player.h"
#include "SimpleAudioEngine.h"
#include "gameWorld.h"
#include "physicsWorld.h"
#include "Defines.h"
#include "Scenes/GameScene.h"
#include <algorithm>

USING_NS_CC;

Player* Player::createWithSpriteFrameName(Node* parent, const std::string filename)
{
	Player* pRet = new Player();

	if (pRet) {
		pRet->parent_ = parent;
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

bool Player::initWithSpriteFrameName(const std::string filename) {
	if (!Sprite::initWithSpriteFrameName(filename)) {
		// Init failure
		return false;
	}
	mouseDown_ = false;
	// Define private variables
	isOdd_ = true;
	reloadTime_ = 0.075f;
	timeSinceFire_ = 1.0f;
	playerShipVelocity_ = 500;
	soundid_ = 0;
	alreadySpawned_ = false;
	// Create player physics body. Store it for future use.
	GameWorld* gWorld = (GameWorld*)parent_;
	playerBody_ = gWorld->getPhysics()->createBody(cocos2d::Vec3(Director::getInstance()->getVisibleSize().width / 15, Director::getInstance()->getVisibleSize().height / 2, -90.0f), physicsWorld::ENTITY_PLAYER, "spaceShips_007.png", this);
	playerBody_->SetUserData(this);
	setPosition(Director::getInstance()->getVisibleSize().width / 15, Director::getInstance()->getVisibleSize().height / 2);
	goToPos_ = Vec2(Director::getInstance()->getVisibleSize().width / 15, Director::getInstance()->getVisibleSize().height / 2);
	setRotation(-90);

	// NOTE: This particle code is placeholder here! No runtime memory allocations allowed like this!
	ParticleSystemQuad* quad1;
	quad1 = ParticleSystemQuad::create("particleEffects/Booster.plist");
	// Load spriteframe from file and initialize particle with texture and rect.
	auto sf1 = SpriteFrameCache::getInstance()->getSpriteFrameByName("Particles/Booster.png");
	quad1->setTextureWithRect(sf1->getTexture(), sf1->getRect());
	quad1->setPosition(getContentSize().width / 3, getContentSize().height - quad1->getContentSize().height / 2);
	this->addChild(quad1, 0);
	ParticleSystemQuad* quad2;
	quad2 = ParticleSystemQuad::create("particleEffects/Booster.plist");
	// Load spriteframe from file and initialize particle with texture and rect.
	auto sf2 = SpriteFrameCache::getInstance()->getSpriteFrameByName("Particles/Booster.png");
	quad2->setTextureWithRect(sf2->getTexture(), sf2->getRect());
	quad2->setPosition(getContentSize().width / 3 * 2, getContentSize().height - quad2->getContentSize().height / 2);
	this->addChild(quad2, 0);


	// TODO: This should be done in separate class.
	for (int i = 0; i < 100; ++i) {
		// Create 100 bullet bodies
		auto sprite = cocos2d::Sprite::createWithSpriteFrameName("Lasers/laserBlue02.png");
		auto action = cocos2d::Hide::create();
		sprite->runAction(action);
		b2Body* body = gWorld->getPhysics()->createBody(cocos2d::Vec3(0.0f, 0.0f, 90.0f), physicsWorld::ENTITY_PROJECTILE, "laserBlue02.png", sprite);
		sprite->setRotation(90);
		body->SetUserData(sprite);
		bulletBodies_.push(body);
		gWorld->getParent()->addChild(sprite, 1);
	}

	playerAlive_ = true;
	gWorld->addChild(this, 4);

	return true;
}

std::vector<b2Body*> Player::getNearbyEnemies() {
	return enemiesInSensorRange_;
}

void Player::startListeningInput() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseDown = CC_CALLBACK_1(Player::onMouseDown, this);
	mouseListener->onMouseUp = CC_CALLBACK_1(Player::onMouseUp, this);
	mouseListener->onMouseMove = CC_CALLBACK_1(Player::onMouseMove, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = [=](EventKeyboard::KeyCode code, Event* usedEvent) {
		switch (code) {
		case EventKeyboard::KeyCode::KEY_SPACE:
			std::vector<b2Body*> vec = getNearbyEnemies();
			std::vector<b2Body*>::iterator it = vec.begin();
			std::vector<b2Body*>::iterator end = vec.end();
			for (it; it != end; ++it) {
				b2Body* body = *it;
				b2Vec2 normal = body->GetPosition() - playerBody_->GetPosition();
				b2Vec2 force = b2Vec2(normal.x * 20, normal.y * 20);
				body->ApplyLinearImpulse(force, body->GetWorldCenter(), true);
			}
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
#else
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(Player::touchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Player::touchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(Player::touchMoved, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(Player::touchCanceled, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
#endif
}

void Player::stopListeningInput() {
	_eventDispatcher->removeEventListenersForTarget(this);
}

void Player::update(float delta)
{
	// Update player sprite position to match physicsBody
	b2Vec2 pos = playerBody_->GetPosition();
	setPosition(pos.x * BOX2D_TO_GL_COORD, pos.y * BOX2D_TO_GL_COORD);
	setRotation(-playerBody_->GetAngle() * RADTODEG);

	// Increment time since last fire
	timeSinceFire_ += delta;

	GameWorld* gWorld = (GameWorld*)parent_;

	if (1) {
		if (playerAlive_) {
			cocos2d::Vec2 pPos = this->getPosition();
			b2Vec2 vec(pPos.x - goToPos_.x, pPos.y - goToPos_.y);
			playerBody_->SetLinearVelocity(-vec);

			if (timeSinceFire_ > reloadTime_ && mouseDown_) {

				timeSinceFire_ = 0;

				if (!bulletBodies_.empty()) {
					soundid_ = experimental::AudioEngine::play2d("soundEffects/basicDischarge.ogg", false);
					b2Body* proj = bulletBodies_.top();
					// Switch position between turrets
					if (isOdd_) {
						isOdd_ = false;
						float posX = playerBody_->GetPosition().x * BOX2D_TO_GL_COORD;
						float posY = playerBody_->GetPosition().y * BOX2D_TO_GL_COORD;
						posX += getContentSize().width / 2 - ((Sprite*)(proj->GetUserData()))->getContentSize().height / 2;
						posY += getContentSize().height / 4 - ((Sprite*)(proj->GetUserData()))->getContentSize().width / 2;

						proj->SetTransform(b2Vec2(posX * GL_TO_BOX2D_COORD, posY * GL_TO_BOX2D_COORD), 90 * DEGTORAD);
					}
					else
					{
						isOdd_ = true;
						float posX = playerBody_->GetPosition().x * BOX2D_TO_GL_COORD;
						float posY = playerBody_->GetPosition().y * BOX2D_TO_GL_COORD;
						posX += getContentSize().width / 2 - ((Sprite*)(proj->GetUserData()))->getContentSize().height / 2;
						posY -= getContentSize().height / 4 - ((Sprite*)(proj->GetUserData()))->getContentSize().width / 2;

						proj->SetTransform(b2Vec2(posX * GL_TO_BOX2D_COORD, posY * GL_TO_BOX2D_COORD), 90 * DEGTORAD);
					}

					bulletBodies_.pop();
					proj->SetLinearVelocity(b2Vec2(2000, 0.0f));
					proj->SetAngularVelocity(0.0f);
					proj->SetActive(true);
					proj->ApplyForce(proj->GetMass() * -proj->GetWorld()->GetGravity(), proj->GetWorldCenter(), true);
					cocos2d::Sprite* spr = (cocos2d::Sprite*)proj->GetUserData();
					auto action = cocos2d::Show::create();
					spr->runAction(action);
				}
				else {
					cocos2d::log("No bullets!");
				}
			}
		}
		else {
			// Player dead
			auto actionHide = Hide::create();
			cocos2d::Vector<cocos2d::Node*> children = getChildren();
			for (int i = 0; i < children.size(); ++i) {
				auto action = Hide::create();
				children.at(i)->runAction(action);
			}
			runAction(actionHide);
			cocos2d::log("HIDE PLAYER!");
		}
	}
	else {
		if (playerBody_)
			playerBody_->SetLinearVelocity(b2Vec2(0, 0));
		soundid_ = 0;
	}
}

void Player::setPlayerStatus(bool alive)
{
	playerAlive_ = alive;

	if (!alreadySpawned_) {
		cocos2d::log("EXPLOSIOON!");
		experimental::AudioEngine::play2d("soundEffects/shipExplosion.ogg");
		alreadySpawned_ = true;
		ParticleSystemQuad* quad3;
		quad3 = ParticleSystemQuad::create("particleEffects/PlayerShipExplosion.plist");
		auto sf3 = SpriteFrameCache::getInstance()->getSpriteFrameByName("Particles/Booster.png");
		quad3->setTextureWithRect(sf3->getTexture(), sf3->getRect());
		Vec2 cPos = getPosition();
		quad3->setPosition(cPos);
		quad3->setGravity(Vec2(playerBody_->GetLinearVelocity().x / 4 * BOX2D_TO_GL_COORD, playerBody_->GetLinearVelocity().y / 4 * BOX2D_TO_GL_COORD));
		parent_->addChild(quad3, 100);
		quad3->setAutoRemoveOnFinish(true);

		std::vector<b2Body*> vec = getNearbyEnemies();
		std::vector<b2Body*>::iterator it = vec.begin();
		std::vector<b2Body*>::iterator end = vec.end();
		for (it; it != end; ++it) {
			b2Body* body = *it;
			b2Vec2 normal = body->GetPosition() - playerBody_->GetPosition();
			b2Vec2 force = b2Vec2(normal.x * 20, normal.y * 20);
			cocos2d::log("ForceX: %f, forceY: %f", force.x, force.y);
			body->ApplyLinearImpulse(force, body->GetWorldCenter(), true);
		}
	}
}

const bool Player::getPlayerStatus() const
{
	return playerAlive_;
}

void Player::addNearbyEnemy(b2Body *body)
{
	enemiesInSensorRange_.push_back(body);
}

void Player::removeNearbyEnemy(b2Body *body)
{
	std::vector<b2Body*>::iterator it = std::find(enemiesInSensorRange_.begin(), enemiesInSensorRange_.end(), body);
	std::vector<b2Body*>::iterator end = enemiesInSensorRange_.end();
	if (it != end)
		enemiesInSensorRange_.erase(it);
}

void Player::storeBullet(b2Body *bullet)
{
	bulletBodies_.push(bullet);
}

// Private destructor because this is using cocos2d memory management
Player::~Player() { 
	cocos2d::log("Player destructor");
	//playerBody_->GetWorld()->DestroyBody(playerBody_);
}

void Player::onExit() {
	playerBody_->GetWorld()->DestroyBody(playerBody_);
	Sprite::onExit();
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
void Player::onMouseDown(cocos2d::Event* usedEvent)
{
	EventMouse* mouse = (EventMouse*)usedEvent;
	if (mouse->getMouseButton() == MOUSE_BUTTON_RIGHT) {
		cocos2d::log("Mouse right pressed!");
	}
	else {
		cocos2d::log("Mouse left pressed!");
		mouseDown_ = true;
	}
	
}

void Player::onMouseUp(cocos2d::Event* usedEvent)
{
	mouseDown_ = false;
}

void Player::onMouseMove(cocos2d::Event* usedEvent)
{
	// On Windows/Linux you have to use getLocationInView(). On mobile devices it is just getLocation(). WT*?
	EventMouse* mouse = (EventMouse*)usedEvent;
	Point p = mouse->getLocationInView();
	goToPos_ = Vec2(p.x, p.y);
}

#else
bool Player::touchBegan(cocos2d::Touch * touch, cocos2d::Event* usedEvent)
{
	// On Windows/Linux you have to use getLocationInView(). On mobile devices it is just getLocation(). WT*?
	goToPos_ = touch->getLocation();
	mouseDown_ = true;
	return true;
}

void Player::touchMoved(cocos2d::Touch * touch, cocos2d::Event* usedEvent)
{
	// On Windows/Linux you have to use getLocationInView(). On mobile devices it is just getLocation(). WT*?
	goToPos_ = touch->getLocation();
}

void Player::touchEnded(cocos2d::Touch * touch, cocos2d::Event* usedEvent)
{
	// On Windows/Linux you have to use getLocationInView(). On mobile devices it is just getLocation(). WT*?
	goToPos_ = touch->getLocation();
	mouseDown_ = false;
}

void Player::touchCanceled(cocos2d::Touch * touch, cocos2d::Event* usedEvent)
{
	mouseDown_ = false;
}
#endif