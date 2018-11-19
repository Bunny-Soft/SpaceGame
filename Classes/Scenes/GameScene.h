#pragma once

#include "cocos2d.h"
class GameWorld;

class GameScene : public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();

	virtual bool init() override;
	virtual void update(float delta) override;

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);

	// Restarts GameScene
	void restartGame(float dt);

private:
	// Gameworld where whole logic runs
	GameWorld* world_;
};