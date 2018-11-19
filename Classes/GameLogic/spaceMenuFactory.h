#pragma once
#include "menuFactory.h"

class spaceMenuFactory : public menuFactory, public cocos2d::Node {
public:
	static spaceMenuFactory* create(cocos2d::Node*);
	/**
	* Create menu using this factory. Remember base class is pure virtual.
	*
	*
	* @param   Menu type to create.
	* @return  Layer autorelease object
	*/
	virtual cocos2d::Layer* createMenu(MENU_TYPE) const override;

protected:
	spaceMenuFactory() {}
	virtual ~spaceMenuFactory() {}

	virtual bool init() override;

	virtual cocos2d::Layer* createMainMenu() const;
	virtual cocos2d::Layer* createPauseMenu() const;
	virtual cocos2d::Layer* createScoreMenu() const;

private:
	cocos2d::Node* parent_;
};