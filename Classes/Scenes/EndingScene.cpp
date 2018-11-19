#include "EndingScene.h"
#include "GameScene.h"


USING_NS_CC;

Scene* EndingScene::createScene() {
	// Create cocos scene
	auto scene = Scene::create();
	// Create GameScene as layer for the scene
	auto layer = EndingScene::create();
	// Add GameScene as layer for the scene
	scene->addChild(layer);

	return scene;
}

bool EndingScene::init() {
	if (!Layer::init()) return false;

	auto visibleSize_ = Director::getInstance()->getVisibleSize();
	auto visibleOrigin_ = Director::getInstance()->getVisibleOrigin();

	auto cocosImage = Sprite::create("graphics/HelloWorld.png");
	cocosImage->setPosition(visibleSize_.width / 3 * 2, visibleSize_.height / 2);
	cocosImage->setScale(visibleSize_.height / cocosImage->getContentSize().height / 3 * 2);
	//addChild(cocosImage);

	auto kenneyImage = Sprite::create("graphics/kenneyLogo.png");
	kenneyImage->setPosition(visibleSize_.width / 3, visibleSize_.height / 2);
	kenneyImage->setScale(2);
	//addChild(kenneyImage);

	auto musicLabel_ = cocos2d::Label::createWithTTF("Please vote for us,\nwe need the bonus 5 points", "fonts/kenvector_future.ttf", 38, Size::ZERO, cocos2d::CCTextAlignment::CENTER);
	musicLabel_->setPosition(visibleSize_.width / 2, visibleSize_.height / 2 + musicLabel_->getContentSize().height / 2);
	this->addChild(musicLabel_);

	// Delay next scene load. This is a splash screen.
	this->scheduleOnce(schedule_selector(EndingScene::quitGame), 5); 

	return true;
}

void EndingScene::quitGame(float dt) {
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}