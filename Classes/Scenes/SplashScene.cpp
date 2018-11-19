#include "SplashScene.h"
#include "GameScene.h"
#include "AudioEngine.h"

USING_NS_CC;

Scene* SplashScene::createScene() {
	// Create cocos scene
	auto scene = Scene::create();
	// Create GameScene as layer for the scene
	auto layer = SplashScene::create();
	// Add GameScene as layer for the scene
	scene->addChild(layer);

	return scene;
}

bool SplashScene::init() {
	if (!Layer::init()) return false;

	auto visibleSize_ = Director::getInstance()->getVisibleSize();
	auto visibleOrigin_ = Director::getInstance()->getVisibleOrigin();

	// Load audio engine instance here
	experimental::AudioEngine::preload("soundEffects/spaceDronePassby.ogg", CC_CALLBACK_0(SplashScene::playAudioEffect, this));
	experimental::AudioEngine::preload("music/DaxxFull.mp3");
	experimental::AudioEngine::preload("soundEffects/basicDischarge.ogg");
	experimental::AudioEngine::preload("soundEffects/basicDischarge.ogg");
	experimental::AudioEngine::preload("soundEffects/shipExplosion.ogg");

	// Delay next scene load. This is a splash screen.
	this->scheduleOnce(schedule_selector(SplashScene::goToGame), 3); 

	// Background planes
	auto background1_ = cocos2d::Sprite::create("graphics/stars.jpg");
	background1_->setScale(visibleSize_.width / background1_->getBoundingBox().size.width, visibleSize_.height / background1_->getContentSize().height);
	background1_->setPosition(visibleOrigin_.x + visibleSize_.width / 2, visibleOrigin_.y + visibleSize_.height / 2);
	this->addChild(background1_);

	// Labels. We are using music from youtube so let's make clear it is not inhouse production and credit goes to the composer. Would you download a car?!
	auto musicLabel_ = cocos2d::Label::createWithTTF("BunnySoft Studios\npresents", "fonts/kenvector_future.ttf", 38, Size::ZERO, cocos2d::CCTextAlignment::CENTER);
	auto urlLabel_ = cocos2d::Label::createWithTTF("\n\nProduced by\nLuis\nMeron\nGreg\nChris\n", "fonts/kenvector_future.ttf", 26, Size::ZERO, cocos2d::CCTextAlignment::CENTER);
	musicLabel_->setPosition(visibleSize_.width / 2, visibleSize_.height / 2 + musicLabel_->getContentSize().height / 2);
	urlLabel_->setPosition(visibleSize_.width / 2, visibleSize_.height / 2 - musicLabel_->getContentSize().height / 2 - 200);
	this->addChild(musicLabel_);
	this->addChild(urlLabel_);

	return true;
}

void SplashScene::goToGame(float dt) {
	// GameScene instance created before given to director for loading.
	auto scene = GameScene::createScene();
	// Director replaces current scene with GameScene with 2 second fade.
	Director::getInstance()->replaceScene(TransitionFade::create(2.0f, scene));
	experimental::AudioEngine::play2d("music/DaxxFull.ogg", true);
}

void SplashScene::playAudioEffect() {
	// Audio is preloaded. Play it
	experimental::AudioEngine::play2d("soundEffects/spaceDronePassby.ogg", false, 0.85f);
}