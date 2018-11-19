#pragma once

#include "cocos2d.h"

class SplashScene : public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();

	virtual bool init() override;

	// implement the "static create()" method manually
	CREATE_FUNC(SplashScene);

protected:

private:
	void goToGame(float dt);
	void playAudioEffect();
};