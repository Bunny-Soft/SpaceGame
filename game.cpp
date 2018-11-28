#include <string>
#include <vector>
#include <iostream>

using namespace std;

class game_info
{
	protected:
		int points;
		int turns;
		int explored_areas;
	public:
		game_info()
		{
			points = 0;
			turns = 1;
			explored_areas = 0;
		}
		void repair_points()
		{
			points = points + 5;
		}
		void pirate_killed()
		{
			points = points + 4;
		}
		void clear_area(int a, int p, int b, int c)//a is the asteroids cleared, p is the pirates cleared, b is the total asteroids in the area, c is the total asteroids in the area
		{
			if(a ==0 && p ==0)
			{
				points = points + 2;
			}
			if(a >= b/2)
			{
				points = points + 2;
			}
			if(a == b)
			{
				points = points + 5;
			}
			if(p >=c/2)
			{
				points = points + 3;
			}
			if(p == c)
			{
				points = points + 6;
			}

		}
		int get_points()
		{
			return points;
		}
		int get_turns()
		{
			return turns;
		}
		int get_explored_areas()
		{
			return explored_areas;
		}
		void inc_turn()
		{
			turns ++;
		}
		void inc_areas()
		{
			explored_areas ++;
		}
};	

class restoration: public cruiser, public bulwark, public mining
{
	public:
		int solar_pressed()
		{
			power_drain = 2;
			power = power + power_drain;
			pirate_chance + 0.05; //need to connect this

			return power;
		}
		int asteroid_pressed(int turns)
		{
			if(turns >=6)
			{
				engine = false;
				power_drain = 3;
				ship_integrity -6;
				power = power + power_drain;
				return power;
			}	
			else if(turns >= 4)
			{
				life_support = false;
				power_drain = 3;
				ship_integrity -4;
				power = power + power_drain;
				return power;
			}
			else if(turns >= 2)
			{
				ship_integrity -2;
				power_drain = 3;
				power = power+power_drain;
				return power;
			}
			else
			{
				ship_integrity--;
				power_drain = 3;
				power = power+power_drain;
				return power;
			}
		}
};
