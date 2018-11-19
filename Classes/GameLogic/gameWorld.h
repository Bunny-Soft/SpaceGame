#pragma once

#include "cocos2d.h"
#include "AudioEngine.h"
#include "Box2D/Box2D.h"

// forward declarations
class Player;
class Enemy;
class physicsWorld;
class b2Contact;
class contactListener;
class spaceMenuFactory;

class GameWorld : public b2ContactListener, public cocos2d::Layer {
public:
	// Constructor
	static GameWorld* create(cocos2d::Node* parent_);
	
	virtual bool init() override;
	void update(float delta);

	// Updates and initializers for various assets
	void updateBackgroundScrolling(float delta);
	void createBackground(std::vector<std::string>* filenames, cocos2d::Size visibleSize, cocos2d::Vec2 origin);
	void createGround(std::vector<std::string>* filenames, cocos2d::Size visibleSize, cocos2d::Vec2 origin);
	void createPlayerShip(cocos2d::Size visibleSize, cocos2d::Vec2 origin);
	void createStarParticles(cocos2d::Size visibleSize, cocos2d::Vec2 origin);
	void createEnemies(cocos2d::Size visibleSize, cocos2d::Vec2 origin);
	void createGUI(cocos2d::Size visibleSize, cocos2d::Vec2 origin);
	void loadAudioAssets();

	// Query touch from GameScene. This would be better if we had inputmapper class.
	void startGameCallback(cocos2d::Ref* pSender);
	void showHighScoreCallback(cocos2d::Ref* pSender);

	// Yes, we have physics. Yay!
	physicsWorld* getPhysics() const;

	Node* getParent() const;

	virtual void onExit() override;

	~GameWorld();

private:
	enum class GAME_STATE {
		// State machine
		gameInMainMenu = 0,
		gameInScoreMenu = 1,
		gameStarting = 2,
		gameRunning = 3,
		playerDied = 4,
		gamePaused = 5,
		gameEnding = 6,
		gameEnded = 7,
		gameOver = 8,
	} currentGameState_;

	//GAME_STATE state_;
	// Private constructor
	GameWorld() {};

	// Menu factory
	spaceMenuFactory* menuFactory_;

	// Contact listener for box2d
	contactListener* cListener_;

	// Player wants to quit
	void quitButtonPressed(cocos2d::Ref*);

	// callback to close highscore table
	void closeHighScore(cocos2d::Ref* pSender);

	// a selector callback
	void menuCloseCallback(float dt);

	// Fade BG elements
	void fadeBackgroundElements(float deltatime);

	// Cleanup
	void cleanupJobs();

	// Score update
	void updateScore();

	void updateGameObjects(float delta, b2Body* bodies);

	void spawnMeteorsIfAvailable(float delta);

	// Pointer to our GameScene instance. Used to insert our gameobjects to GameScene.
	Node* parent_;

	// PhysicsWorld
	physicsWorld* physics_;

	// Audio engine is controlled here.
	int audioId_;
	// Enemies. Storage contains preallocated enemies. Active enemies go to activeEnemies_
	std::stack<Enemy*> enemyStorage_;
	std::set<Enemy*> activeEnemies_;
	float spawnInterval_;
	float lastSpawnTime_;
	float gameEndingTime_;
	int highscore_;

	// Player ship
	Player* playerShip_;

	// Background, labels and foreground textures
	std::vector<cocos2d::Sprite*> bg1Vector_;
	std::vector<cocos2d::Sprite*> bg2Vector_;
	std::vector<cocos2d::Sprite*> gnd1Vector_;
	std::vector<cocos2d::Sprite*> gnd2Vector_;
	cocos2d::Label* musicLabel_;
	cocos2d::Label* urlLabel_;
	cocos2d::Label* pointsLabel_;
	cocos2d::Label* headerLabel_;
	cocos2d::Label* loseLabel_;

	//cocos2d::Sprite* highScoreButton_;
	float bgVelocity_;
	float fgVelocity_;
	int score_;

	// Current level
	//unsigned short int currentLevel_;
	//unsigned short int maxLevel_;
	float timeSinceStart_;
	float timeSinceLastBGFade_;
	int currentLayer_;

	std::vector<Enemy*> enemiesInSensor_;
};

// Define methods inline if they are small and preferably do not modify any members. Inline directive is ignored by modern compilers anyway if human is wrong.
inline cocos2d::Node* GameWorld::getParent() const {
	return parent_;
}