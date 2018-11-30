#ifndef CHIPOTLE_H
#define CHIPOTLE_H
#include <gtkmm.h>
#include <vector>
#include <iostream>



extern int check;
extern int ship_type;

using namespace std;


class MainWindow:public Gtk::Window
{
    public:
    MainWindow();
    virtual        ~MainWindow();


    protected:
      void        Play();
      void        Tutorial();
      void        Exit();

    Gtk::Image image;
    Gtk::Button play_game,tutorial,exit;
    Gtk::Grid grid;


};

class ShipWindow:public Gtk::Window
{
public:
  ShipWindow();
  virtual     ~ShipWindow();

protected:

  void        Selected_Cruiser();
  void        Selected_Bulwark();
  void        Selected_Mining();


    Gtk::Image ship1,ship2,ship3;
    Gtk::Button button1,button2,button3;
    Gtk::Grid grid;
};

class TutorialWindow:public Gtk::Window
{
    public:
    TutorialWindow();
    virtual        ~TutorialWindow();


    protected:

      Gtk::Label  label1,label2,label3;
      Gtk::Grid    grid;


};

class PlayWindow:public Gtk::Window
{
public:
  PlayWindow();
  virtual     ~PlayWindow();

protected:
  void        Combat_Status();
  void        Repair();
  void        Generate_power();
  void        show_ship_status();

  Gtk::Grid   grid;
  Gtk::Image  ship1;
  Gtk::Button button2,button3,button4,button5;

};

#endif
