#pragma once

#include "cocos2d.h"

class menuFactory {
public:	
	enum class MENU_TYPE {
		MAIN_MENU = 0,
		PAUSE_MENU = 1,
		SCORE_MENU = 2,
	};

	/**
	* Create menu using this factory. Remember base class is pure virtual.
	*
	*
	* @param   Menu type to create.
	* @return  Layer autorelease object
	*/
	virtual cocos2d::Layer* createMenu(MENU_TYPE) const = 0;

protected:
	menuFactory() {}
	virtual ~menuFactory() {}
};