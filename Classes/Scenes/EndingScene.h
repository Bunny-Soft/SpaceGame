#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

class EndingScene : public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();

	virtual bool init() override;

	// implement the "static create()" method manually
	CREATE_FUNC(EndingScene);

protected:

private:
	void quitGame(float dt);
	void playAudioEffect(float dt);
	CocosDenshion::SimpleAudioEngine* audio_;
};