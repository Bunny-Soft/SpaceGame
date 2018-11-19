#include "spaceMenuFactory.h"

USING_NS_CC;

spaceMenuFactory* spaceMenuFactory::create(Node* parent) {

	spaceMenuFactory* pRef = new spaceMenuFactory();

	if (pRef) {
		pRef->_parent = parent;
		pRef->_parent->retain();
		if (pRef->init()) {
			pRef->autorelease();
		}
		else {
			CC_SAFE_RELEASE(pRef);
		}
	}
	else {
		CC_SAFE_RELEASE(pRef);
	}

	return pRef;

}

cocos2d::Layer * spaceMenuFactory::createMenu(MENU_TYPE type) const
{
	switch (type) {
	case MENU_TYPE::MAIN_MENU:
		return createMainMenu();
		break;
	case MENU_TYPE::PAUSE_MENU:
		return createPauseMenu();
		break;
	case MENU_TYPE::SCORE_MENU:
		return createScoreMenu();
		break;
	}
}

bool spaceMenuFactory::init() {
	if (!Node::init()) return false;

	return true;
}

cocos2d::Layer * spaceMenuFactory::createMainMenu() const
{
	auto layer = Layer::create();
	layer->setPosition(Vec2::ZERO);

	// Build background layer first
	std::string topLeftCorner = "GUI/Gen UI/UI_WINDOW (8).png";
	std::string topMiddle = "GUI/Gen UI/UI_WINDOW (9).png";
	std::string topRightCorner = "GUI/Gen UI/UI_WINDOW (7).png";

	std::string centerLeft = "GUI/Gen UI/UI_WINDOW (6).png";
	std::string centerMid = "GUI/Gen UI/UI_WINDOW (1).png";
	std::string centerRight = "GUI/Gen UI/UI_WINDOW (5).png";

	std::string botLeftCorner = "GUI/Gen UI/UI_WINDOW (2).png";
	std::string botMiddle = "GUI/Gen UI/UI_WINDOW (3).png";
	std::string botRightCorner = "GUI/Gen UI/UI_WINDOW (4).png";

	auto s1 = Sprite::createWithSpriteFrameName(topLeftCorner);
	s1->setAnchorPoint(Vec2::ZERO);
	auto s2 = Sprite::createWithSpriteFrameName(topMiddle);
	s2->setAnchorPoint(Vec2::ZERO);
	auto s3 = Sprite::createWithSpriteFrameName(topRightCorner);
	s3->setAnchorPoint(Vec2::ZERO);
	auto referenceSprite = Sprite::createWithSpriteFrameName("GUI/Gen UI/UI_WINDOW (6).png");

	s1->addChild(s2, 2);
	s2->setPosition(Vec2(s1->getContentSize().width, 0));
	s1->addChild(s3, 2);
	s3->setPosition(Vec2(s1->getContentSize().width * 2, 0));
	s1->setPosition(Vec2(layer->getContentSize().width / 2 - 1.5 * referenceSprite->getContentSize().width, layer->getContentSize().height / 2 + referenceSprite->getContentSize().height * 6));
	
	auto gameLabel = Label::createWithTTF("SPACE\nSHOOTER!", "fonts/kenpixel_blocks.ttf", 48, Size::ZERO, TextHAlignment::CENTER);
	s1->addChild(gameLabel, 20);
	gameLabel->setPosition(s2->getPosition().x + s2->getContentSize().width / 2, s2->getPosition().y);

	for (int i = 1; i < 13; ++i) {
		
		if (i == 12) {
			auto s4 = Sprite::createWithSpriteFrameName(botLeftCorner);
			s4->setAnchorPoint(Vec2::ZERO);
			auto s5 = Sprite::createWithSpriteFrameName(botMiddle);
			s5->setAnchorPoint(Vec2::ZERO);
			auto s6 = Sprite::createWithSpriteFrameName(botRightCorner);
			s6->setAnchorPoint(Vec2::ZERO);

			s4->addChild(s5, i * 2);
			s5->setPosition(Vec2(s4->getContentSize().width, 0));
			s4->addChild(s6, i * 2);
			s6->setPosition(Vec2(s4->getContentSize().width * 2, 0));
			s1->addChild(s4, i * 2);
			s4->setPosition(Vec2(0, -i*(s4->getContentSize().height) + i * 2));
			
		} else {
			auto s4 = Sprite::createWithSpriteFrameName(centerLeft);
			s4->setAnchorPoint(Vec2::ZERO);
			auto s5 = Sprite::createWithSpriteFrameName(centerMid);
			s5->setAnchorPoint(Vec2::ZERO);
			auto s6 = Sprite::createWithSpriteFrameName(centerRight);
			s6->setAnchorPoint(Vec2::ZERO);

			s4->addChild(s5, i * 2);
			s5->setPosition(Vec2(s4->getContentSize().width, 0));
			s4->addChild(s6, i * 2);
			s6->setPosition(Vec2(s4->getContentSize().width * 2, 0));
			s1->addChild(s4, i * 2);
			s4->setPosition(Vec2(0,- i*(s4->getContentSize().height) + i * 2));
			
		}

		
	}
	layer->addChild(s1, 10);

	// These are buttons
	cocos2d::MenuItemImage* closeItem1 = MenuItemImage::create();
	closeItem1->setNormalImage(Sprite::createWithSpriteFrameName("GUI/Buttons/BTN_BLUE_RECT_OUT.png"));
	closeItem1->setSelectedImage(Sprite::createWithSpriteFrameName("GUI/Buttons/BTN_BLUE_RECT_IN.png"));
	closeItem1->setName("startButton");
	//
	closeItem1->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	closeItem1->setScale(0.8f);
	Label* startLabel = cocos2d::Label::createWithTTF("START", "fonts/kenvector_future.ttf", 38, Size::ZERO, CCTextAlignment::CENTER);
	closeItem1->addChild(startLabel);
	startLabel->setPosition(closeItem1->getContentSize().width / 2, closeItem1->getContentSize().height / 2);

	cocos2d::MenuItemImage* closeItem2 = MenuItemImage::create();
	closeItem2->setNormalImage(Sprite::createWithSpriteFrameName("GUI/Buttons/BTN_BLUE_RECT_OUT.png"));
	closeItem2->setSelectedImage(Sprite::createWithSpriteFrameName("GUI/Buttons/BTN_BLUE_RECT_IN.png"));
	closeItem2->setName("scoreButton");
	closeItem2->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	closeItem2->setScale(0.8f);
	Label* highLabel = cocos2d::Label::createWithTTF("HIGH\nSCORES", "fonts/kenvector_future.ttf", 38, Size::ZERO, CCTextAlignment::CENTER);
	closeItem2->addChild(highLabel);
	highLabel->setPosition(closeItem2->getContentSize().width / 2, closeItem2->getContentSize().height / 2);

	cocos2d::MenuItemImage* closeItem3 = MenuItemImage::create();
	closeItem3->setNormalImage(Sprite::createWithSpriteFrameName("GUI/Buttons/BTN_BLUE_RECT_OUT.png"));
	closeItem3->setSelectedImage(Sprite::createWithSpriteFrameName("GUI/Buttons/BTN_BLUE_RECT_IN.png"));
	closeItem3->setName("quitButton");
	closeItem3->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	closeItem3->setScale(0.8f);
	Label* quitLabel = cocos2d::Label::createWithTTF("QUIT", "fonts/kenvector_future.ttf", 38, Size::ZERO, CCTextAlignment::CENTER);
	closeItem3->addChild(quitLabel);
	quitLabel->setPosition(closeItem3->getContentSize().width / 2, closeItem3->getContentSize().height / 2);

	// create menu, it's an autorelease object
	auto menu = cocos2d::Menu::create(closeItem1, closeItem2, closeItem3, NULL);
	menu->setName("buttonMenu");
	menu->setPosition(layer->getContentSize().width/2, layer->getContentSize().height / 2);
	closeItem1->setPosition(Vec2(0, closeItem1->getContentSize().height * 1.5));
	closeItem2->setPosition(cocos2d::Vec2::ZERO);
	closeItem3->setPosition(Vec2(0, -closeItem3->getContentSize().height * 1.5));

	layer->addChild(menu, 200);

	layer->setName("MainMenuLayer");
	return layer;
}

cocos2d::Layer * spaceMenuFactory::createPauseMenu() const
{
	// Build pause menu
	return nullptr;
}

cocos2d::Layer * spaceMenuFactory::createScoreMenu() const
{
	// Build score menu
	auto layer = Layer::create();
	layer->setPosition(Vec2::ZERO);

	// Build background layer first
	std::string topLeftCorner = "GUI/Gen UI/UI_WINDOW (8).png";
	std::string topMiddle = "GUI/Gen UI/UI_WINDOW (9).png";
	std::string topRightCorner = "GUI/Gen UI/UI_WINDOW (7).png";

	std::string centerLeft = "GUI/Gen UI/UI_WINDOW (6).png";
	std::string centerMid = "GUI/Gen UI/UI_WINDOW (1).png";
	std::string centerRight = "GUI/Gen UI/UI_WINDOW (5).png";

	std::string botLeftCorner = "GUI/Gen UI/UI_WINDOW (2).png";
	std::string botMiddle = "GUI/Gen UI/UI_WINDOW (3).png";
	std::string botRightCorner = "GUI/Gen UI/UI_WINDOW (4).png";

	auto referenceSprite = Sprite::createWithSpriteFrameName(centerLeft);

	auto s1 = Sprite::createWithSpriteFrameName(topLeftCorner);
	s1->setAnchorPoint(Vec2::ZERO);
	s1->setName("parentChild");
	auto s2 = Sprite::createWithSpriteFrameName(topMiddle);
	s2->setAnchorPoint(Vec2::ZERO);
	auto s3 = Sprite::createWithSpriteFrameName(topMiddle);
	s3->setAnchorPoint(Vec2::ZERO);
	auto s4 = Sprite::createWithSpriteFrameName(topRightCorner);
	s4->setAnchorPoint(Vec2::ZERO);

	s1->addChild(s2, 2);
	s2->setPosition(Vec2(s1->getContentSize().width, 0));
	s1->addChild(s3, 2);
	s3->setPosition(Vec2(s1->getContentSize().width * 2, 0));
	s1->addChild(s4, 2);
	s4->setPosition(Vec2(s1->getContentSize().width * 3, 0));
	s1->setPosition(Vec2(layer->getContentSize().width / 2 - 2 * referenceSprite->getContentSize().width, layer->getContentSize().height / 2 + referenceSprite->getContentSize().height * 1.5f));

	for (int i = 1; i < 5; ++i) {

		if (i == 4) {
			auto s5 = Sprite::createWithSpriteFrameName(botLeftCorner);
			s5->setAnchorPoint(Vec2::ZERO);
			auto s6 = Sprite::createWithSpriteFrameName(botMiddle);
			s6->setAnchorPoint(Vec2::ZERO);
			auto s7 = Sprite::createWithSpriteFrameName(botMiddle);
			s7->setAnchorPoint(Vec2::ZERO);
			auto s8 = Sprite::createWithSpriteFrameName(botRightCorner);
			s8->setAnchorPoint(Vec2::ZERO);

			s5->addChild(s6, i * 2);
			s6->setPosition(Vec2(s5->getContentSize().width, 0));
			s5->addChild(s7, i * 2);
			s7->setPosition(Vec2(s5->getContentSize().width * 2, 0));
			s5->addChild(s8, i * 2);
			s8->setPosition(Vec2(s5->getContentSize().width * 3, 0));
			s1->addChild(s5, i * 2);
			s5->setPosition(Vec2(0, -i*(referenceSprite->getContentSize().height) + i * 2));

		}
		else {
			auto s5 = Sprite::createWithSpriteFrameName(centerLeft);
			s5->setAnchorPoint(Vec2::ZERO);
			auto s6 = Sprite::createWithSpriteFrameName(centerMid);
			s6->setAnchorPoint(Vec2::ZERO);
			auto s7 = Sprite::createWithSpriteFrameName(centerMid);
			s7->setAnchorPoint(Vec2::ZERO);
			auto s8 = Sprite::createWithSpriteFrameName(centerRight);
			s8->setAnchorPoint(Vec2::ZERO);

			s5->addChild(s6, i * 2);
			s6->setPosition(Vec2(s5->getContentSize().width, 0));
			s5->addChild(s7, i * 2);
			s7->setPosition(Vec2(s5->getContentSize().width * 2, 0));
			s5->addChild(s8, i * 2);
			s8->setPosition(Vec2(s5->getContentSize().width * 3, 0));
			s1->addChild(s5, i * 2);
			s5->setPosition(Vec2(0, -i*(referenceSprite->getContentSize().height) + i * 2));

		}
	}

	// These are buttons
	cocos2d::MenuItemImage* closeItem1 = MenuItemImage::create();
	closeItem1->setNormalImage(Sprite::createWithSpriteFrameName("GUI/Buttons/BTN_BLUE_RECT_OUT.png"));
	closeItem1->setSelectedImage(Sprite::createWithSpriteFrameName("GUI/Buttons/BTN_BLUE_RECT_IN.png"));
	closeItem1->setName("closeButton");
	closeItem1->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	closeItem1->setScale(0.8f);
	Label* startLabel = cocos2d::Label::createWithTTF("BACK", "fonts/kenvector_future.ttf", 38, Size::ZERO, CCTextAlignment::CENTER);
	closeItem1->addChild(startLabel);
	startLabel->setPosition(closeItem1->getContentSize().width / 2, closeItem1->getContentSize().height / 2);

	// create menu, it's an autorelease object
	auto menu = cocos2d::Menu::create(closeItem1, NULL);
	menu->setName("buttonMenu");
	s1->addChild(menu, 100);
	menu->setPosition(referenceSprite->getContentSize().width * 2, -referenceSprite->getContentSize().height * 3);
	closeItem1->setPosition(cocos2d::Vec2::ZERO);
	
	auto topicLabel = Label::createWithTTF("CURRENT HIGH SCORE!", "fonts/kenpixel_blocks.ttf", 24, Size::ZERO, TextHAlignment::CENTER);
	s1->addChild(topicLabel, 9);
	topicLabel->setPosition(s3->getPosition().x, s2->getPosition().y + s2->getContentSize().height / 2);

	auto scoreLabel = Label::createWithTTF("123456", "fonts/kenpixel_blocks.ttf", 58, Size::ZERO, TextHAlignment::CENTER);
	scoreLabel->setName("scoreLabel");
	s1->addChild(scoreLabel, 9);
	scoreLabel->setPosition(s3->getPosition().x, s2->getPosition().y - referenceSprite->getContentSize().height);

	layer->setName("scoreLayer");
	layer->addChild(s1);

	return layer;
}
