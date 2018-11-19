#include "Box2D/Box2D.h"
#include "cocos2d.h"
#include <set>

class Enemy;
class Player;

enum class COLLISION_TYPE {
	PROJECTILE_TO_METEOR = 1,
	METEOR_EXPLOSION = 2,
	PLAYER_EXPLOSION = 3,
};

struct collisionData {
	uint32_t id_;
	cocos2d::Point cPoint_;
	COLLISION_TYPE type_;

public:
	collisionData(int id, cocos2d::Point p, COLLISION_TYPE t) : id_(id), cPoint_(p), type_(t) {}

	bool operator<(const collisionData &rhs) const {
		return id_ < rhs.id_;
	}
};

class contactListener : public cocos2d::Node, b2ContactListener {
public:
	static contactListener* create(cocos2d::Node*, b2World*);

	// Get bullets marked for removal
	std::set<b2Body*>& getBulletsMarkedForRemoval();
	std::set<Enemy*>& getEnemiesMarkedForRemoval();
	std::set<collisionData>& getCollisionsThisStep();

	// Get score since previous query
	const uint32_t getScore();
protected:
	virtual ~contactListener() {}
	bool initWithWorld(b2World*);

private:
	// This class is a listener for physics contacts
	virtual void BeginContact(b2Contact* contact) override;
	virtual void EndContact(b2Contact* contact) override;

	// Sets for entity removal
	std::set<b2Body*> bulletsMarkedForRemoval_;
	std::set<Enemy*> enemiesMarkedForRemoval_;

	// Collisions happened this frame. _This is used by parent class to spawn cool particle effects.
	std::set<collisionData> collisionsThisFrame_;

	// Score is calculated from various collisions
	uint32_t playerScore_;

	// Parent node
	cocos2d::Node* parent_;

	uint32_t collisionId_;
};

