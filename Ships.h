#ifndef SHIPS_H
#define SHIPS_H
#include <gtkmm.h>


/////////////////////////////////////////////////////////////////////////////////////////////////

                          /* Base Ship Class */

////////////////////////////////////////////////////////////////////////////////////////////////

  class ships{
    public:
      ships(){}
      virtual ~ships(){}
    protected:
      float ship_integrity;
      float shield_capacity;
      float attack;
      float shield_regen;
      float system_break_chance;
      float power;
      float power_drain;

      virtual void set_ship_integrity()=0;
      virtual void set_shield_capacity()=0;
      virtual void set_attack()=0;
      virtual void set_shield_regen()=0;
      virtual void set_system_break_chance()=0;
      virtual void set_power()=0;
      virtual void set_power_drain()=0;

    };


    /////////////////////////////////////////////////////////////////////////////////////////////////

                              /* Cruiser Ship Class */

    ////////////////////////////////////////////////////////////////////////////////////////////////

    class cruiser:public ships{

    public:

      //for Cruiser class, attack and shield regen should be +1
      cruiser(){

        ship_integrity = 20;
        shield_capacity = 25;
        attack = 5;
        shield_regen = 8;
        system_break_chance = 0.05;
        power = 10;
        //power drain is one every turn.
        power_drain = 1;


      }
      ~cruiser(){}

    protected:

      //////////////////////////////////////////////////////
            /* Setter methods for cruiser class */
      ///////////////////////////////////////////////////////

      void set_ship_integrity(float n)
      {
        ship_integrity = n;
      }

      void set_shield_capacity(float n)
      {
        shield_capacity = n;
      }

      void set_attack(float n)
      {
        attack = n;
      }

      void set_shield_regen(float n)
      {
        shield_regen = n;
      }

      void set_system_break_chance(float n)
      {
        system_break_chance = n;
      }

      void set_power(float n)
      {
        power = n;
      }

      void set_power_drain(float n)
      {
        power_drain = n;
      }

      //////////////////////////////////////////////////////
            /* Getter methods for cruiser class */
      ///////////////////////////////////////////////////////

      float get_ship_integrity()
      {
        return ship_integrity;
      }

      float get_shield_capacity()
      {
        return shield_capacity;
      }

      float get_attack()
      {
        return attack;
      }

      float get_shield_regen()
      {
        return shield_regen;
      }

      float get_system_break_chance(float n)
      {
        return system_break_chance;
      }

      float get_power()
      {
        return power;
      }

      float get_power_drain()
      {
        return power_drain;
      }

    };


    /////////////////////////////////////////////////////////////////////////////////////////////////

                              /* Bulwark Ship Class */

    ////////////////////////////////////////////////////////////////////////////////////////////////

    class bulwark:public ships{
    public:

      //for Bulwark class, ship integrity and shield capacity should be +5, and -1 for attack
      bulwark(){

        ship_integrity = 25;
        shield_capacity = 30;
        attack = 3;
        shield_regen = 7;
        system_break_chance = 0.05;
        power = 10;
        //power drain is one every turn.
        power_drain = 1;

      }
      ~bulwark(){}
    protected:

      //////////////////////////////////////////////////////
            /* Setter methods for bulwark class */
      ///////////////////////////////////////////////////////

      void set_ship_integrity(float n)
      {
        ship_integrity = n;
      }

      void set_shield_capacity(float n)
      {
        shield_capacity = n;
      }

      void set_attack(float n)
      {
        attack = n;
      }

      void set_shield_regen(float n)
      {
        shield_regen = n;
      }

      void set_system_break_chance(float n)
      {
        system_break_chance = n;
      }

      void set_power(float n)
      {
        power = n;
      }

      void set_power_drain(float n)
      {
        power_drain = n;
      }

      //////////////////////////////////////////////////////
            /* Getter methods for bulwark class */
      ///////////////////////////////////////////////////////

      float get_ship_integrity()
      {
        return ship_integrity;
      }

      float get_shield_capacity()
      {
        return shield_capacity;
      }

      float get_attack()
      {
        return attack;
      }

      float get_shield_regen()
      {
        return shield_regen;
      }

      float get_system_break_chance(float n)
      {
        return system_break_chance;
      }

      float get_power()
      {
        return power;
      }

      float get_power_drain()
      {
        return power_drain;
      }

    };


    /////////////////////////////////////////////////////////////////////////////////////////////////

                              /* Mining Ship Class */

    ////////////////////////////////////////////////////////////////////////////////////////////////



    class mining:public ships{
    public:

      //for mining ship, should have plus one mining per return (done in other class)
      mining(){

        ship_integrity = 25;
        shield_capacity = 30;
        attack = 3;
        shield_regen = 7;
        system_break_chance = 0.05;
        power = 10;
        //power drain is one every turn.
        power_drain = 1;

      }
      ~mining(){}
    protected:

      //////////////////////////////////////////////////////
            /* Setter methods for mining class */
      ///////////////////////////////////////////////////////

      void set_ship_integrity(float n)
      {
        ship_integrity = n;
      }

      void set_shield_capacity(float n)
      {
        shield_capacity = n;
      }

      void set_attack(float n)
      {
        attack = n;
      }

      void set_shield_regen(float n)
      {
        shield_regen = n;
      }

      void set_system_break_chance(float n)
      {
        system_break_chance = n;
      }

      void set_power(float n)
      {
        power = n;
      }

      void set_power_drain(float n)
      {
        power_drain = n;
      }

      //////////////////////////////////////////////////////
            /* Getter methods for mining class */
      ///////////////////////////////////////////////////////

      float get_ship_integrity()
      {
        return ship_integrity;
      }

      float get_shield_capacity()
      {
        return shield_capacity;
      }

      float get_attack()
      {
        return attack;
      }

      float get_shield_regen()
      {
        return shield_regen;
      }

      float get_system_break_chance(float n)
      {
        return system_break_chance;
      }

      float get_power()
      {
        return power;
      }

      float get_power_drain()
      {
        return power_drain;
      }

    };

#endif
