#include "gameWorld.h"
#include "Scenes/GameScene.h"
#include "player.h"
#include "enemy.h"
#include "physicsWorld.h"
#include "Utils/GLES-Render.h"
#include "Utils/b2DebugDrawLayer.h"
#include "Defines.h"
#include "Utils/FileOperation.h"
#include "contactListener.h"
#include "Scenes\EndingScene.h"
#include "AudioEngine.h"
#include "spaceMenuFactory.h"

USING_NS_CC;

GameWorld* GameWorld::create(Node* parent) {
	// Create GameWorld which controls all of our game objects
	GameWorld* pRet = new GameWorld();

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

bool GameWorld::init() {

	if (!Layer::init()) {
		return false;
	}

	// Basic info of our screensize
	cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// Create physics world and set contact listener
	physics_ = physicsWorld::create(this);
	addChild(physics_);
	cListener_ = contactListener::create(this, physics_->world_);
	cListener_->retain();
	physics_->createStatics(visibleSize, origin);

	// This draws Box2d debug layer on top of our game world.
#ifndef NDEBUG
	auto listener = cocos2d::EventListenerKeyboard::create();
	listener->onKeyPressed = [&](cocos2d::EventKeyboard::KeyCode code, cocos2d::Event * event)->void {
		if (code == EventKeyboard::KeyCode::KEY_D) {
			Node* node = getChildByName("DebugDrawLayer");
			if (node != nullptr)
				removeChild(node);
			else
				this->addChild(B2DebugDrawLayer::create(physics_->world_, BOX2D_TO_GL_COORD), 9999, "DebugDrawLayer");
		}
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#endif

	// State machine
	currentGameState_ = GAME_STATE::gameInMainMenu;
	gameEndingTime_ = 0;

	// Load assets and create menus
	loadAudioAssets();

	// Create twinkling star background
	createStarParticles(visibleSize, origin);
	// Create scrolling planes
	std::vector<std::string> bgPlanes;
	bgPlanes.push_back("black.png");
	bgPlanes.push_back("blue.png");
	bgPlanes.push_back("darkPurple.png");
	bgPlanes.push_back("purple.png");
	createBackground(&bgPlanes, visibleSize, origin);

	// Create ground planes
	std::vector<std::string> ground;
	ground.push_back("Ground/groundDirt.png");
	ground.push_back("Ground/groundSnow.png");
	ground.push_back("Ground/groundIce.png");
	ground.push_back("Ground/groundGrass.png");
	createGround(&ground, visibleSize, origin);

	createGUI(visibleSize, origin);
	createPlayerShip(visibleSize, origin);
	createEnemies(visibleSize, origin);

	// Spawn timers and such
	lastSpawnTime_ = 0.0f;
	spawnInterval_ = 1.0f;
	score_ = 0;
	timeSinceStart_ = 0;
	timeSinceLastBGFade_ = 99;
	currentLayer_ = 0;

	std::string highScoreFromFile;

	if (!FileOperation::readFile(&highScoreFromFile)) {
		// There was no file
		cocos2d::log("No high score file!");
		highscore_ = 0;
	}
	else {
		cocos2d::log("High-score loaded: %s", highScoreFromFile.c_str());
		highscore_ = std::stoi(highScoreFromFile);
	}

	return true;
}

void GameWorld::createStarParticles(Size size, Vec2 origin) {
	// Particle quad placeholder. This is going to separate class which handles all the particle creations and distribution.
	ParticleSystemQuad* quadTwinkly;
	quadTwinkly = ParticleSystemQuad::create("particleEffects/TwinklyStars.plist");
	auto sf = SpriteFrameCache::getInstance()->getSpriteFrameByName("Particles/TwinklyStars.png");
	quadTwinkly->setTextureWithRect(sf->getTexture(), sf->getRect());
	this->addChild(quadTwinkly, 2);
	quadTwinkly->setPosition(size.width/2, size.height / 2);
	quadTwinkly->setAutoRemoveOnFinish(true);

	ParticleSystemQuad* quadDust;
	quadDust = ParticleSystemQuad::create("particleEffects/SpaceDust.plist");
	auto sf2 = SpriteFrameCache::getInstance()->getSpriteFrameByName("Particles/SpaceDust.png");
	quadDust->setTextureWithRect(sf2->getTexture(), sf2->getRect());
	this->addChild(quadDust, 2);
	quadDust->setPosition(size.width / 3 * 2, size.height / 2);
	quadDust->setAutoRemoveOnFinish(true);

}

void GameWorld::update(float delta) {
	timeSinceLastBGFade_ += delta;
	fadeBackgroundElements(delta);
	updateBackgroundScrolling(delta);

	switch (currentGameState_) {
	case GAME_STATE::gameInMainMenu:
		break;
	case GAME_STATE::gameInScoreMenu:
		break;
	case GAME_STATE::gameStarting:
		playerShip_->startListeningInput();
		currentGameState_ = GAME_STATE::gameRunning;
		break;
	case GAME_STATE::gameRunning:
		// Update physicsWorld. No delta needed because physicsworld has its own timestep
		physics_->update();

		// Get bodies from physics world and update sprite positions
		updateGameObjects(delta, physics_->world_->GetBodyList());

		// Spawn more meteors
		spawnMeteorsIfAvailable(delta);

		// Update score
		updateScore();

		// Cleanup for bullets and meteors.
		cleanupJobs();
		break;
	case GAME_STATE::playerDied:
		// Update physicsWorld. No delta needed because physicsworld has its own timestep
		physics_->update();

		// Get bodies from physics world and update sprite positions
		updateGameObjects(delta, physics_->world_->GetBodyList());

		if (score_ > highscore_)
			FileOperation::saveFile(std::string(std::to_string(score_)));

		currentGameState_ = GAME_STATE::gameEnding;

		parent_->scheduleOnce(schedule_selector(GameScene::restartGame), 3.0f);
		break;
	case GAME_STATE::gamePaused:
		break;
	case GAME_STATE::gameEnding:
		// Update physicsWorld. No delta needed because physicsworld has its own timestep
		physics_->update();

		// Get bodies from physics world and update sprite positions
		updateGameObjects(delta, physics_->world_->GetBodyList());

		// Cleanup for bullets and meteors.
		cleanupJobs();

		// Increase timer before state moves to gameEnded
		gameEndingTime_ += delta;
		if (gameEndingTime_ > 2.8f)
			currentGameState_ = GAME_STATE::gameEnded;
		break;
	case GAME_STATE::gameEnded:
		// Game ended. Cleanup! Physics world is guaranteed to not be in update mode.
		//currentGameState_ = GAME_STATE::gameOver;
		break;
	}
}

void GameWorld::updateScore() {
	char buf[16];
	score_ += cListener_->getScore();
	sprintf(buf, "%d", score_);
	pointsLabel_->setString(std::string(buf));
}

void GameWorld::spawnMeteorsIfAvailable(float delta) {
	// Spawn meteors
	timeSinceStart_ += delta;
	lastSpawnTime_ += delta;
	// Accelerate spawn time every frame.
	if (spawnInterval_ > 0.15f)
		spawnInterval_ -= (delta / 150);

	if (!enemyStorage_.empty() && lastSpawnTime_ > spawnInterval_) {
		lastSpawnTime_ = 0;
		Enemy *e = enemyStorage_.top();
		enemyStorage_.pop();
		e->getPhysicsBody()->SetActive(true);
		auto showAction = Show::create();
		e->runAction(showAction);
		// Spawn meteors at accelerating rate
		e->getPhysicsBody()->ApplyLinearImpulse(b2Vec2(timeSinceStart_ * -0.5f, 0), e->getPhysicsBody()->GetWorldCenter(), true);
	}
}

void GameWorld::updateGameObjects(float delta, b2Body* bodies) {
	while (bodies != NULL) {
		// Check ID from body. 1) player, 2) enemy, ...
		switch (bodies->GetFixtureList()->GetFilterData().categoryBits) {
		case physicsWorld::ENTITY_PLAYER:
			Player* p;
			p = (Player*)bodies->GetUserData();
			p->update(delta);
			if (!p->getPlayerStatus()) {
				auto action = cocos2d::FadeIn::create(2.0f);
				loseLabel_->runAction(action);
				currentGameState_ = GAME_STATE::playerDied;
				p->stopListeningInput();
			}
			break;
		case physicsWorld::ENTITY_METEOR:
		{
			if (bodies->IsActive()) {
				((Enemy*)(bodies->GetUserData()))->update(delta);
			}
		}
		break;
		case physicsWorld::ENTITY_PROJECTILE:
		{
			// projectile
			if (bodies->IsActive()) {
				float posX = bodies->GetPosition().x * BOX2D_TO_GL_COORD;
				float posY = bodies->GetPosition().y * BOX2D_TO_GL_COORD;
				float angle = bodies->GetAngle();
				cocos2d::Sprite* spr = static_cast<cocos2d::Sprite*>(bodies->GetUserData());
				if (spr != NULL) {
					spr->setPosition(posX, posY);
					// Bullet goes straight ahead. We do not need rotation angle.
					//spr->setRotation(angle * RADTODEG);
				}
				bodies->ApplyForce(bodies->GetMass() * -getPhysics()->world_->GetGravity(), bodies->GetWorldCenter(), true);
			}
		}
		break;
		}

		bodies = bodies->GetNext();
	}
}

void GameWorld::cleanupJobs() {

	// Get references from collisionListener
	std::set<b2Body*>& bulletRemoval_ = cListener_->getBulletsMarkedForRemoval();
	std::set<Enemy*>& meteorRemoval_ = cListener_->getEnemiesMarkedForRemoval();

	// Remove collided bullets from the scene
	for (std::set<b2Body*>::iterator it = bulletRemoval_.begin(); it != bulletRemoval_.end(); ++it) {
		b2Body* body = (*it);
		body->SetActive(false);
		body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		if (playerShip_ != nullptr)
			playerShip_->storeBullet(body);
		cocos2d::Sprite* spr = (cocos2d::Sprite*)body->GetUserData();
		auto action = cocos2d::Hide::create();
		spr->runAction(action);
		cocos2d::log("Storing bullet! %d", bulletRemoval_.size());
	}

	// Remove destroyed meteors from the scene
	for (std::set<Enemy*>::iterator it = meteorRemoval_.begin(); it != meteorRemoval_.end(); ++it) {
		Enemy* e = *it;
		this->removeChild(e);

		// Particle quad placeholder. This is going to separate class which handles all the particle creations and distribution.
		/*ParticleSystemQuad* quad;
		quad = ParticleSystemQuad::create("particleEffects/MeteorExplosion.plist");
		{ auto sf2 = SpriteFrameCache::getInstance()->getSpriteFrameByName("Meteors/meteorBrown_small2.png");
		quad->setTextureWithRect(sf2->getTexture(), sf2->getRect()); }
		quad->setGravity(Vec2(0, -15 * 40));
		this->addChild(quad, 50);
		quad->setPosition(e->getPosition().x, e->getPosition().y);*/
	}
	if (playerShip_ != nullptr && !playerShip_->getPlayerStatus()) {
		// Player is dead
		removeChild(playerShip_);
		playerShip_ = nullptr;
	}

	// clear lists for next round
	meteorRemoval_.clear();
	bulletRemoval_.clear();
}

void GameWorld::updateBackgroundScrolling(float delta) {
	// Screen data
	cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();
	// Background texture data
	for (int i = 0; i < bg1Vector_.size(); ++i) {
		auto background1_ = bg1Vector_.at(i);
		auto background2_ = bg2Vector_.at(i);
		auto ground1_ = gnd1Vector_.at(i);
		auto ground2_ = gnd2Vector_.at(i);

		auto currentBG1pos = background1_->getPosition();
		auto currentBG2pos = background2_->getPosition();
		auto currentGround1pos = ground1_->getPosition();
		auto currentGround2pos = ground2_->getPosition();

		float hMulti = visibleSize.width / background1_->getContentSize().width;
		float vMulti = visibleSize.height / background1_->getContentSize().height;

		// Update space background scrolling first
		currentBG1pos.x -= bgVelocity_*delta;
		currentBG2pos.x -= bgVelocity_*delta;
		if ((origin.x + currentBG1pos.x) < (-background1_->getBoundingBox().size.width * hMulti)) {
			// Correct sprite position caused by velocity and delta time
			float correction = (background1_->getContentSize().width * ceil(hMulti)) + (origin.x + currentBG1pos.x);
			currentBG1pos = cocos2d::Vec2(origin.x + background1_->getContentSize().width * ceil(hMulti) + correction, origin.y);
		}
		if ((origin.x + currentBG2pos.x) < (-background2_->getBoundingBox().size.width * hMulti)) {
			// Correct sprite position caused by velocity and delta time
			float correction = (background2_->getContentSize().width * ceil(hMulti)) + (origin.x + currentBG2pos.x);
			currentBG2pos = cocos2d::Vec2(origin.x + background2_->getContentSize().width * ceil(hMulti) + correction, origin.y);
		}

		//background1_->setPosition(currentBG1pos.x, currentBG1pos.y);
		//background2_->setPosition(currentBG2pos.x, currentBG2pos.y);

		hMulti = visibleSize.width / ground1_->getContentSize().width;

		currentGround1pos.x -= fgVelocity_*delta;
		currentGround2pos.x -= fgVelocity_*delta;

		// Update rock ground scrolling second
		if ((origin.x + currentGround1pos.x) < (-ground1_->getContentSize().width * ceil(hMulti))) {
			// Correct sprite position caused by velocity and delta time
			float correction = (ground1_->getContentSize().width * ceil(hMulti)) + (origin.x + currentGround1pos.x);
			currentGround1pos = cocos2d::Vec2(origin.x + ground1_->getContentSize().width * ceil(hMulti) + correction, origin.y);
		}
		if ((origin.x + currentGround2pos.x) < (-ground2_->getContentSize().width * ceil(hMulti))) {
			// Correct sprite position caused by velocity and delta time
			float correction = (ground2_->getContentSize().width * ceil(hMulti)) + (origin.x + currentGround2pos.x);
			currentGround2pos = cocos2d::Vec2(origin.x + ground2_->getContentSize().width * ceil(hMulti) + correction, origin.y);
		}

		ground1_->setPosition(currentGround1pos.x, currentGround1pos.y);
		ground2_->setPosition(currentGround2pos.x, currentGround2pos.y);
	}
}

void GameWorld::createBackground(std::vector<std::string>* filenames, cocos2d::Size visibleSize, cocos2d::Vec2 origin) {
	cocos2d::Sprite* background1_, *background2_;
	for (int i = 0; i < filenames->size(); ++i) {
		std::string filename = filenames->at(i);
		cocos2d::log("Creating background using: %s", filename.c_str());
		background1_ = cocos2d::Sprite::createWithSpriteFrameName(filename);
		background1_->setPosition(origin.x, origin.y);
		float hMulti = visibleSize.width / background1_->getContentSize().width;
		float vMulti = visibleSize.height / background1_->getContentSize().height;
		for (int i = 0; i < std::ceil(hMulti+1); ++i) {
			for (int j = 0; j < std::ceil(vMulti+1); ++j) {
				if (i != 0 || j != 0) {
					auto bgChild = cocos2d::Sprite::createWithSpriteFrameName(filename);
					bgChild->setPosition((background1_->getContentSize().width * i + bgChild->getContentSize().width / 2), (background1_->getContentSize().width * j + bgChild->getContentSize().height / 2));
					background1_->addChild(bgChild, 1);
					auto hideAction = cocos2d::FadeOut::create(0);
					bgChild->runAction(hideAction);
				}
			}
		}
		auto hideAction1 = cocos2d::FadeOut::create(0);
		background1_->runAction(hideAction1);
		bg1Vector_.push_back(background1_);
		this->addChild(background1_, 1);

		background2_ = cocos2d::Sprite::createWithSpriteFrameName(filename);
		background2_->setPosition(origin.x + background2_->getContentSize().width * ((int)hMulti + 1), origin.y);
		for (int i = 0; i < hMulti; ++i) {
			for (int j = 0; j < vMulti; ++j) {
				if (i != 0 || j != 0) {
					auto bgChild = cocos2d::Sprite::createWithSpriteFrameName(filename);
					bgChild->setPosition((background2_->getContentSize().width * i + bgChild->getContentSize().width / 2), (background2_->getContentSize().width * j + bgChild->getContentSize().height / 2));
					background2_->addChild(bgChild, 1);
					auto hideAction = cocos2d::FadeOut::create(0);
					bgChild->runAction(hideAction);
				}
			}
		}
		auto hideAction2 = cocos2d::FadeOut::create(0);
		background2_->runAction(hideAction2);
		bg2Vector_.push_back(background2_);
		this->addChild(background2_, 1);
	}
	bgVelocity_ = 20.0f;
}

void GameWorld::createGround(std::vector<std::string>* filenames, cocos2d::Size visibleSize, cocos2d::Vec2 origin) {
	cocos2d::Sprite* ground1_, *ground2_;
	for (int i = 0; i < filenames->size(); ++i) {
		std::string filename = filenames->at(i);
		cocos2d::log("Creating ground using: %s", filename.c_str());
		ground1_ = cocos2d::Sprite::createWithSpriteFrameName(filename);
		// Horizontal multiplier for ground
		float hMulti = visibleSize.width / ground1_->getContentSize().width;
		ground1_->setAnchorPoint(Vec2(0.0f, 0.0f));
		ground1_->setPosition(origin.x, origin.y);
		for (int i = 0; i < hMulti; ++i) {
			if (i != 0) {
				auto gndChild = cocos2d::Sprite::createWithSpriteFrameName(filename);
				gndChild->setPosition((ground1_->getContentSize().width * i + gndChild->getContentSize().width / 2), origin.y + ground1_->getContentSize().height / 2);
				ground1_->addChild(gndChild, 3);
				auto hideAction = cocos2d::FadeOut::create(0);
				gndChild->runAction(hideAction);
			}
		}
		auto hideAction1 = cocos2d::FadeOut::create(0);
		ground1_->runAction(hideAction1);
		gnd1Vector_.push_back(ground1_);
		this->addChild(ground1_, 3);

		// Ground sprites
		ground2_ = cocos2d::Sprite::createWithSpriteFrameName(filename);
		ground2_->setAnchorPoint(Vec2(0.0f, 0.0f));
		ground2_->setPosition(origin.x + ground2_->getContentSize().width * ceil(hMulti), origin.y);
		for (int i = 0; i < hMulti; ++i) {
			if (i != 0) {
				auto gndChild = cocos2d::Sprite::createWithSpriteFrameName(filename);
				gndChild->setPosition((ground2_->getContentSize().width * i + gndChild->getContentSize().width / 2), origin.y + ground2_->getContentSize().height / 2);
				ground2_->addChild(gndChild, 3);
				auto hideAction = cocos2d::FadeOut::create(0);
				gndChild->runAction(hideAction);
			}
		}
		auto hideAction2 = cocos2d::FadeOut::create(0);
		ground2_->runAction(hideAction2);
		gnd2Vector_.push_back(ground2_);
		this->addChild(ground2_, 3);
	}
	fgVelocity_ = 200.0f;
}

void GameWorld::createPlayerShip(cocos2d::Size visibleSize, cocos2d::Vec2 origin) {
	playerShip_ = Player::createWithSpriteFrameName(this, "Ships/spaceShips_007.png");
}

void GameWorld::createGUI(cocos2d::Size visibleSize, cocos2d::Vec2 origin) {
	
	// Create main menu UI using new menuFactory
	menuFactory_ = spaceMenuFactory::create(this);
	menuFactory_->retain();
	auto layer = menuFactory_->createMenu(menuFactory::MENU_TYPE::MAIN_MENU);
	auto buttonMenu = (Menu*)layer->getChildByName("buttonMenu");
	auto startButton = (MenuItemImage*)buttonMenu->getChildByName("startButton");
	auto scoreButton = (MenuItemImage*)buttonMenu->getChildByName("scoreButton");
	auto quitButton = (MenuItemImage*)buttonMenu->getChildByName("quitButton");
	startButton->setCallback(CC_CALLBACK_1(GameWorld::startGameCallback, this));
	scoreButton->setCallback(CC_CALLBACK_1(GameWorld::showHighScoreCallback, this));
	quitButton->setCallback(CC_CALLBACK_1(GameWorld::quitButtonPressed, this));

	addChild(layer, 100);

	musicLabel_ = cocos2d::Label::createWithTTF("Music: DaXX - Sanxion Bigroom Instrumental (C64 Remix)", "fonts/kenvector_future.ttf", 36);
	urlLabel_ = cocos2d::Label::createWithSystemFont("youtu.be/UIUd5zRJoHs", "fonts/kenvector_future.ttf", 24);
	urlLabel_->setAnchorPoint(cocos2d::Vec2(0.0f, 0.0f));
	musicLabel_->addChild(urlLabel_, 99);
	urlLabel_->setPosition(0, -urlLabel_->getContentSize().height);
	musicLabel_->setAnchorPoint(cocos2d::Vec2(0.0, 0.0));
	musicLabel_->setPosition(20, musicLabel_->getContentSize().height + gnd1Vector_.at(0)->getBoundingBox().size.height + 20);
	this->addChild(musicLabel_, 99);
	pointsLabel_ = cocos2d::Label::createWithTTF(" ", "fonts/kenpixel_blocks.ttf", 72);
	pointsLabel_->setPosition(visibleSize.width / 2, visibleSize.height - pointsLabel_->getContentSize().height / 2);
	this->addChild(pointsLabel_, 99);
	loseLabel_ = cocos2d::Label::createWithTTF("GAME\nOVER", "fonts/kenvector_future.ttf", 72,Size::ZERO, CCTextAlignment::CENTER);
	loseLabel_->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	// Sets lose-label on top of graphics.
	this->addChild(loseLabel_, 99);
	auto action = cocos2d::FadeOut::create(0.0f);
	loseLabel_->runAction(action);
}
void GameWorld::quitButtonPressed(Ref* pSender) {
	currentGameState_ = GAME_STATE::gameEnded;
	auto scene = EndingScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1.5f, scene));
}
void GameWorld::menuCloseCallback(float dt)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void GameWorld::loadAudioAssets() {
	audioId_ = experimental::AudioEngine::INVALID_AUDIO_ID;
	// Create audio for our background music. Use different files on different platforms
}

void GameWorld::startGameCallback(cocos2d::Ref* pSender) {
	float height = Director::getInstance()->getVisibleSize().height;
	auto layer = getChildByName("MainMenuLayer");
	if (layer)
		layer->runAction(cocos2d::MoveBy::create(0.5f, cocos2d::Vec2(0, height)));
	musicLabel_->runAction(cocos2d::FadeOut::create(5.0f));
	urlLabel_->runAction(cocos2d::FadeOut::create(5.0f));

	currentGameState_ = GAME_STATE::gameStarting;
}

void GameWorld::showHighScoreCallback(cocos2d::Ref* pSender) {
	// Create high score menu
	auto layerHigh = menuFactory_->createMenu(menuFactory::MENU_TYPE::SCORE_MENU);
	// Get scorelabel and set high score.
	auto scoreLabel = (Label*)layerHigh->getChildByName("parentChild")->getChildByName("scoreLabel");
	std::string scoreText = std::to_string(highscore_);
	scoreLabel->setString(scoreText);
	// Set close button callback
	auto closeButton = (MenuItemImage*)layerHigh->getChildByName("parentChild")->getChildByName("buttonMenu")->getChildByName("closeButton");
	closeButton->setCallback(CC_CALLBACK_1(GameWorld::closeHighScore, this));
	// Make layer visible.
	auto mainMenu = getChildByName("MainMenuLayer");
	mainMenu->runAction(cocos2d::Hide::create());
	addChild(layerHigh, 101);
}

void GameWorld::closeHighScore(cocos2d::Ref* pSender) {
	removeChildByName("scoreLayer");
	auto mainMenu = getChildByName("MainMenuLayer");
	mainMenu->runAction(cocos2d::Show::create());
}


physicsWorld * GameWorld::getPhysics() const
{
	return physics_;
}

void GameWorld::createEnemies(cocos2d::Size visibleSize, cocos2d::Vec2 origin) {
	int i = 0;
	std::string filename = "";
	while (i < MAX_ENEMIES) {
		switch (cocos2d::RandomHelper::random_int(0, 7)) {
		case 0:
			filename = "Meteors/meteorBrown_big1.png";
			break;
		case 1:
			filename = "Meteors/meteorBrown_big2.png";
			break;
		case 2:
			filename = "Meteors/meteorBrown_big3.png";
			break;
		case 3:
			filename = "Meteors/meteorBrown_big4.png";
			break;
		case 4:
			filename = "Meteors/meteorGrey_big1.png";
			break;
		case 5:
			filename = "Meteors/meteorGrey_big2.png";
			break;
		case 6:
			filename = "Meteors/meteorGrey_big3.png";
			break;
		case 7:
			filename = "Meteors/meteorGrey_big4.png";
			break;
		default:
			filename = "Meteors/meteorBrown_big1.png";
		}
		Enemy* e = Enemy::createWithSpriteFrameName(this, filename);
		enemyStorage_.push(e);
		++i;
	}
}

void GameWorld::fadeBackgroundElements(float delta) {
	if (timeSinceLastBGFade_ > 20) {
		timeSinceLastBGFade_ = 0;

		auto bgFadeOut1 = bg1Vector_.at(currentLayer_);
		auto gndFadeOut1 = gnd1Vector_.at(currentLayer_);
		auto bgFadeOut2 = bg2Vector_.at(currentLayer_);
		auto gndFadeOut2 = gnd2Vector_.at(currentLayer_);

		// Fade out children of our arrays
		cocos2d::Vector<cocos2d::Node*> children = bgFadeOut1->getChildren();
		for (int i = 0; i < children.size(); ++i) {
			auto fadeOutAction = cocos2d::FadeOut::create(5);
			children.at(i)->runAction(fadeOutAction);
		}
		children = gndFadeOut1->getChildren();
		for (int i = 0; i < children.size(); ++i) {
			auto fadeOutAction = cocos2d::FadeOut::create(5);
			children.at(i)->runAction(fadeOutAction);
		}
		children = bgFadeOut2->getChildren();
		for (int i = 0; i < children.size(); ++i) {
			auto fadeOutAction = cocos2d::FadeOut::create(5);
			children.at(i)->runAction(fadeOutAction);
		}
		children = gndFadeOut2->getChildren();
		for (int i = 0; i < children.size(); ++i) {
			auto fadeOutAction = cocos2d::FadeOut::create(5);
			children.at(i)->runAction(fadeOutAction);
		}
		// Fade out parents
		auto fadeOutAction1 = cocos2d::FadeOut::create(5);
		auto fadeOutAction2 = cocos2d::FadeOut::create(5);
		auto fadeOutAction3 = cocos2d::FadeOut::create(5);
		auto fadeOutAction4 = cocos2d::FadeOut::create(5);
		bgFadeOut1->runAction(fadeOutAction1);
		gndFadeOut1->runAction(fadeOutAction2);
		bgFadeOut2->runAction(fadeOutAction3);
		gndFadeOut2->runAction(fadeOutAction4);

		currentLayer_++;
		if (currentLayer_ > 3)
			currentLayer_ = 0;

		// Fade in next layer
		auto bgFadeIn1 = bg1Vector_.at(currentLayer_);
		auto gndFadeIn1 = gnd1Vector_.at(currentLayer_);
		auto bgFadeIn2 = bg2Vector_.at(currentLayer_);
		auto gndFadeIn2 = gnd2Vector_.at(currentLayer_);
		children = bgFadeIn1->getChildren();
		for (int i = 0; i < children.size(); ++i) {
			auto fadeInAction = cocos2d::FadeIn::create(5);
			children.at(i)->runAction(fadeInAction);
		}
		children = gndFadeIn1->getChildren();
		for (int i = 0; i < children.size(); ++i) {
			auto fadeInAction = cocos2d::FadeIn::create(5);
			children.at(i)->runAction(fadeInAction);
		}
		children = bgFadeIn2->getChildren();
		for (int i = 0; i < children.size(); ++i) {
			auto fadeInAction = cocos2d::FadeIn::create(5);
			children.at(i)->runAction(fadeInAction);
		}
		children = gndFadeIn2->getChildren();
		for (int i = 0; i < children.size(); ++i) {
			auto fadeInAction = cocos2d::FadeIn::create(5);
			children.at(i)->runAction(fadeInAction);
		}
		auto fadeInAction1 = cocos2d::FadeIn::create(5);
		auto fadeInAction2 = cocos2d::FadeIn::create(5);
		auto fadeInAction3 = cocos2d::FadeIn::create(5);
		auto fadeInAction4 = cocos2d::FadeIn::create(5);
		bgFadeIn1->runAction(fadeInAction1);
		gndFadeIn1->runAction(fadeInAction2);
		bgFadeIn2->runAction(fadeInAction3);
		gndFadeIn2->runAction(fadeInAction4);
	}

}

void GameWorld::onExit() {
	cocos2d::log("game world onExit!");
	menuFactory_->release();
	Node::onExit();
}

GameWorld::~GameWorld() {
	cocos2d::log("game world destructor!");
	physics_->release();
	cListener_->release();
}