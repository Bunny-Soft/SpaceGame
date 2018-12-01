#ifndef SHIPS_H
#define SHIPS_H
#include <gtkmm.h>

/////////////////////////////////////////////////////////////////////////////////////////////////
/* Base Ship Class */
////////////////////////////////////////////////////////////////////////////////////////////////

class ships {
	protected:
		float ship_integrity;
		float shield_capacity;
		float shield_amount;
		float attack;
		float power_drain;

    public:
        ships() {}
    	virtual~ships() {}

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
            ship_integrity = 25;
            shield_capacity = 30;
            shield_amount = 30;
            attack = 3;
            power_drain = 1;
        }
		~mining() {}
};

#endif
