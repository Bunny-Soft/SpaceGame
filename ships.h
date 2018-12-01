#ifndef SHIPS_H
#define SHIPS_H
#include <gtkmm.h>

/////////////////////////////////////////////////////////////////////////////////////////////////
/* Base Ship Class */
////////////////////////////////////////////////////////////////////////////////////////////////

class ships {
	public:
		float ship_integrity;
		float shield_capacity;
		float shield_amount;
		float attack;
		float power_drain;
		float mine_rate;
		float power_level; // its over 9000!


        ships(){}
    	~ships(){}

	void deploySolar(){
		power_level += 5;
	}
	void mine(){
		ship_integrity += mine_rate;
	}
	void divertPowerToShields(){
		if(power_level > 10) {
			shield_amount *= 1.5;
			power_level -= 10;
		}
	}
	void divertPowerToWeapons(){
		if(power_level > 10) {
			attack *= 2;
			power_level -= 10;
		}
	}

	//////////////////////////////////////////////////////
    /* Setter methods for ship class */
    ///////////////////////////////////////////////////////

    void set_ship_integrity(float n) {
        ship_integrity = n;
    }

    void set_shield_capacity(float n) {
        shield_capacity = n;
    }

    void set_attack(float n) {
        attack = n;
    }

    void set_shield_amount(float n) {
        shield_amount = n;
    }

    void set_power_drain(float n) {
        power_drain = n;
    }

    //////////////////////////////////////////////////////
    /* Getter methods for ship class */
    ///////////////////////////////////////////////////////

    float get_ship_integrity() {
        return ship_integrity;
    }

    float get_shield_capacity() {
        return shield_capacity;
    }

    float get_attack() {
        return attack;
    }
    float get_shield_amount() {
        return shield_amount;
    }

    float get_power_drain() {
        return power_drain;
    }

	float get_power_level() {
		return power_level;
	}

};

/////////////////////////////////////////////////////////////////////////////////////////////////
/* Cruiser Ship Class */
////////////////////////////////////////////////////////////////////////////////////////////////

class cruiser: public ships {
    public:
        //for Cruiser class, attack and shield regen should be +1
        cruiser() {
            ship_integrity = 20;
            shield_capacity = 25;
            shield_amount = 25;
            attack = 5;
            //power drain is one every turn.
            power_drain = 1;
			mine_rate = 1;
			power_level = 0;
        }
		~cruiser() {}
};

/////////////////////////////////////////////////////////////////////////////////////////////////
/* Bulwark Ship Class */
////////////////////////////////////////////////////////////////////////////////////////////////

class bulwark: public ships {
    public:
        //for Bulwark class, ship integrity and shield capacity should be +5, and -1 for attack
        bulwark() {
            ship_integrity = 25;
            shield_capacity = 30;
            shield_amount = 30;
            attack = 3;
            power_drain = 1;
			mine_rate = 1;
			power_level = 0;
        }
		~bulwark() {}
};

/////////////////////////////////////////////////////////////////////////////////////////////////
/* Mining Ship Class */
////////////////////////////////////////////////////////////////////////////////////////////////

class mining: public ships {
    public:
        //for mining ship, should have plus one mining per return (done in other class)
        mining() {
            ship_integrity = 30;
            shield_capacity = 30;
            shield_amount = 10;
            attack = 3;
            power_drain = 1;
			mine_rate = 5;
			power_level = 0;
        }
		~mining() {}
};

class PirateKing: public ships {
    public:
        PirateKing() {
            ship_integrity = 75;
            shield_capacity = 0;
            shield_amount = 0;
            attack = 10;
            power_drain = 1;
			mine_rate = 5;
			power_level = 0;
        }
		~PirateKing() {}

		// super aDvAnCeD Ai tO DeFeAt AlL
		void runAi(ships attackingShip) {
			attackingShip.set_ship_integrity(attackingShip.get_ship_integrity() - attack);
		}
};

#endif
