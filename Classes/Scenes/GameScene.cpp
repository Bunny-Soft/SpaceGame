#include "GameScene.h"
#include "EndingScene.h"
#include "SimpleAudioEngine.h"
#include "GameLogic/gameWorld.h"

USING_NS_CC;

Scene* GameScene::createScene() {
	// Create cocos scene
	auto scene = Scene::create();
	// Create GameScene as layer for the scene
	auto layer = GameScene::create();
	// Add GameScene as layer for the scene
	scene->addChild(layer);

	return scene;
}

bool GameScene::init() {
	if (!Layer::init()) return false;

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("spaceShooter.plist");

	// Add listener for ESC/Back-key
	auto backKeyListener = EventListenerKeyboard::create();

	// Use lambda 
	backKeyListener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* ev) {
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			auto scene = EndingScene::createScene();
			// Director replaces current scene with GameScene with 2 second fade.
			Director::getInstance()->replaceScene(TransitionFade::create(2.0f, scene));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			exit(0);
#endif
			break;
		}
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(backKeyListener, this);

	// Create gameworld
	world_ = GameWorld::create(this);
	this->addChild(world_);

	// Start running update loop
	this->scheduleUpdate();

	return true;
}
void GameScene::update(float delta) {
	// Update gameworld
	world_->update(delta);
}

void GameScene::restartGame(float dt) {
	// GameScene instance created before given to director for loading.
	auto scene = GameScene::createScene();
	// Director replaces current scene with GameScene with 2 second fade.
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(1.0f, scene));
}