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

      virtual void get_ship_integrity()=0;
      virtual void get_shield_capacity()=0;
      virtual void get_attack()=0;
      virtual void get_shield_regen()=0;
      virtual void get_system_break_chance()=0;
      virtual void get_power()=0;
      virtual void get_power_drain()=0;
    };


    /////////////////////////////////////////////////////////////////////////////////////////////////

                              /* Cruiser Ship Class */

    ////////////////////////////////////////////////////////////////////////////////////////////////

    class cruiser:public ships{

    public:

      //for Cruiser class, attack and shield regen should be +1
      cruiser(){

        ship_integrity = 0;
        shield_capacity = 0;
        attack = 0;
        shield_regen = 0;
        system_break_chance = 0;
        power = 0;
        power_drain = 0;


      }
      ~cruiser(){}

    private:

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

      float set_ship_integrity()
      {
        return ship_integrity;
      }

      float set_shield_capacity()
      {
        return shield_capacity;
      }

      float set_attack()
      {
        return attack;
      }

      float set_shield_regen()
      {
        return shield_regen;
      }

      float set_system_break_chance(float n)
      {
        return system_break_chance;
      }

      float set_power()
      {
        return power;
      }

      float set_power_drain()
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

        ship_integrity = 0;
        shield_capacity = 0;
        attack = 0;
        shield_regen = 0;
        system_break_chance = 0;
        power = 0;
        power_drain = 0;

      }
      ~bulwark(){}
    private:

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

      float set_ship_integrity()
      {
        return ship_integrity;
      }

      float set_shield_capacity()
      {
        return shield_capacity;
      }

      float set_attack()
      {
        return attack;
      }

      float set_shield_regen()
      {
        return shield_regen;
      }

      float set_system_break_chance(float n)
      {
        return system_break_chance;
      }

      float set_power()
      {
        return power;
      }

      float set_power_drain()
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

        ship_integrity = 0;
        shield_capacity = 0;
        attack = 0;
        shield_regen = 0;
        system_break_chance = 0;
        power = 0;
        power_drain = 0;

      }
      ~mining(){}
    private:

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

      float set_ship_integrity()
      {
        return ship_integrity;
      }

      float set_shield_capacity()
      {
        return shield_capacity;
      }

      float set_attack()
      {
        return attack;
      }

      float set_shield_regen()
      {
        return shield_regen;
      }

      float set_system_break_chance(float n)
      {
        return system_break_chance;
      }

      float set_power()
      {
        return power;
      }

      float set_power_drain()
      {
        return power_drain;
      }

    };

#endif
