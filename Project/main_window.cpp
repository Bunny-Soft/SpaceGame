#include "mainheader.h"
#include <iostream>

int check = 0;
int ship_type =0;

////////////////////////////////////////////////////////////////////////////////////

                      /* GUI Window to display the Main menu */

////////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow()
{
  this->set_title("Main Menu");
  this->set_border_width(5);
  image.set("menu.gif");
  grid.attach(image,0,0,2,1);

  play_game.add_label("Play Game");
  play_game.signal_pressed().connect(sigc::mem_fun(*this,&MainWindow::Play));
  grid.attach(play_game,0,1,1,1);

  tutorial.add_label("Tutorial");
  tutorial.signal_pressed().connect(sigc::mem_fun(*this,&MainWindow::Tutorial));
  grid.attach(tutorial,1,1,1,1);

  exit.add_label("Exit");
  exit.signal_pressed().connect(sigc::mem_fun(*this,&MainWindow::Exit));
  grid.attach(exit,0,2,2,1);

  grid.show_all();

  add(grid);

}

MainWindow::~MainWindow()
{}

void MainWindow::Play()
{

  //open shipwindow, closing main window
  check = 1;
  hide();

}

void MainWindow::Tutorial()
{

  TutorialWindow window3;
  Gtk::Main::run(window3);


}

void MainWindow::Exit()
{
  hide();

}



////////////////////////////////////////////////////////////////////////////////////

                      /* GUI Window to allow player to select their ship. */

////////////////////////////////////////////////////////////////////////////////////



//need to add labels next to each picture, detailing the ships status points.
ShipWindow::ShipWindow()
{

    this->set_title("Select Ship");
  this->set_border_width(5);
  ship1.set("select_1.png");
  grid.attach(ship1,0,0,2,2);

  button1.add_label("Crusier");
  button1.signal_pressed().connect(sigc::mem_fun(*this,&ShipWindow::Selected_Cruiser));
  grid.attach(button1,0,3,2,2);

  ship2.set("select_2.png");
  grid.attach(ship2,5,0,2,2);

  button2.add_label("Bulwark");
  button2.signal_pressed().connect(sigc::mem_fun(*this,&ShipWindow::Selected_Bulwark));
  grid.attach(button2,5,3,2,2);

  ship3.set("select_3.png");
  grid.attach(ship3,10,0,2,2);

  button3.add_label("Mining");
  button3.signal_pressed().connect(sigc::mem_fun(*this,&ShipWindow::Selected_Mining));
  grid.attach(button3,10,3,2,2);

  grid.show_all();

  add(grid);

}

ShipWindow::~ShipWindow()
{}

void ShipWindow::Selected_Cruiser()
{

  ship_type=1;
  hide();

}

void ShipWindow::Selected_Bulwark()
{
  ship_type=2;
  hide();

}

void ShipWindow::Selected_Mining()
{
  ship_type=3;
  hide();

}

/////////////////////////////////////////////////////////////////////////////////////

                      /* GUI Window to display tutorial info */

////////////////////////////////////////////////////////////////////////////////////

TutorialWindow::TutorialWindow()
{
    this->set_title("Tutorial");

}

TutorialWindow::~TutorialWindow()
{}



  ////////////////////////////////////////////////////////////////////////////////////

                        /* GUI Window to display Play Window */

  ////////////////////////////////////////////////////////////////////////////////////




  PlayWindow::PlayWindow()
  {
      this->set_title("Main Control");
    this->set_border_width(5);

    if(ship_type==1)
    {
    ship1.set("background_1_moving.gif");
    }

    else if(ship_type==2)
    {
    ship1.set("select_2.png");
    }

    else if(ship_type==3)
    {
    ship1.set("select_3.png");
    }
    grid.attach(ship1,0,0,4,4);

    button2.add_label("Deploy solar panels");
    button2.signal_pressed().connect(sigc::mem_fun(*this,&PlayWindow::Solar));
    grid.attach(button2,0,4,2,2);


    button3.add_label("Combat");
    button3.signal_pressed().connect(sigc::mem_fun(*this,&PlayWindow::Combat_Status));
    grid.attach(button3,2,4,2,2);

    button4.add_label("Mine asteroid");
    button4.signal_pressed().connect(sigc::mem_fun(*this,&PlayWindow::Mine));
    grid.attach(button4,0,6,2,2);

    button5.add_label("Ship Status");
    button5.signal_pressed().connect(sigc::mem_fun(*this,&PlayWindow::show_ship_status));
    grid.attach(button5,2,6,2,2);



    grid.show_all();

    add(grid);

  }

  PlayWindow::~PlayWindow()
  {}


  void PlayWindow::Combat_Status()
  {

      CombatWindow window5;
      Gtk::Main::run(window5);

  }


  void PlayWindow::Mine()
  {
    Gtk::MessageDialog dialog(*this, "Mining Asteroid",false,Gtk::MESSAGE_INFO);
    dialog.set_secondary_text("....");
    dialog.run();


  }

  void PlayWindow::Solar()
  {
    Gtk::MessageDialog dialog(*this, "Deploying solar panels",false,Gtk::MESSAGE_INFO);
    dialog.set_secondary_text("...");
    dialog.run();

  }

  void PlayWindow::show_ship_status()
  {

    StatsWindow window6;
    Gtk::Main::run(window6);



  }


  ////////////////////////////////////////////////////////////////////////////////////

                        /* GUI Window to display Combat Info*/

  ////////////////////////////////////////////////////////////////////////////////////






/*


  Need to insert the combat logic in the below window.

*/


  CombatWindow::CombatWindow()
  {

      this->set_title("Combat Control");
    this->set_border_width(5);
    image.set("background_1_moving.gif");
    grid.attach(image,0,0,4,4);

    button1.add_label("Attack");
    button1.signal_pressed().connect(sigc::mem_fun(*this,&CombatWindow::Attack));
    grid.attach(button1,0,4,2,2);


    button2.add_label("Retreat");
    button2.signal_pressed().connect(sigc::mem_fun(*this,&CombatWindow::Retreat));
    grid.attach(button2,2,4,2,2);

    button3.add_label("Divert power to Weapons");
    button3.signal_pressed().connect(sigc::mem_fun(*this,&CombatWindow::Divert_power_to_weapons));
    grid.attach(button3,0,6,2,2);

    button4.add_label("Divert power to shields");
    button4.signal_pressed().connect(sigc::mem_fun(*this,&CombatWindow::Divert_power_to_shields));
    grid.attach(button4,2,6,2,2);

    label1.set_text("Shield Strength");
    label2.set_text("Weapon Damage");
    label3.set_text("Power");


    //note, these will change based on the players actions.
    label4.set_text("20");
    label5.set_text("5");
    label6.set_text("10");

    grid.attach(label1,0,8,1,1);
    grid.attach(label2,1,8,2,1);
    grid.attach(label3,3,8,1,1);

    grid.attach(label4,0,9,1,1);
    grid.attach(label5,1,9,2,1);
    grid.attach(label6,3,9,1,1);






    grid.show_all();

    add(grid);

  }

  CombatWindow::~CombatWindow()
  {}


  void CombatWindow::Retreat()
  {

  }
  void CombatWindow::Attack()
  {

  }

  void CombatWindow::Divert_power_to_shields()
  {

  }

  void CombatWindow::Divert_power_to_weapons()
  {

  }

  ////////////////////////////////////////////////////////////////////////////////////

                        /* GUI Window to display Ship status*/

  ////////////////////////////////////////////////////////////////////////////////////



  StatsWindow::StatsWindow()
  {

      this->set_title("Ship Status");
    this->set_border_width(5);

    label1.set_text("Ship Integrity");
    label2.set_text("Shield Capacity");
    label3.set_text("Attack Damage");
    label4.set_text("Shield Regen");
    label5.set_text("System Break Chance");
    label6.set_text("Power");
    label7.set_text("Power Drain");


    //put ship stats here.
    label8.set_text("..");
    label9.set_text("..");
    label10.set_text("..");
    label11.set_text("..");
    label12.set_text("..");
    label13.set_text("..");
    label14.set_text("..");


    grid.attach(label1,0,0,2,1);
    grid.attach(label2,0,2,2,1);
    grid.attach(label3,0,4,2,1);
    grid.attach(label4,0,6,2,1);
    grid.attach(label5,0,8,2,1);
    grid.attach(label6,0,10,2,1);
    grid.attach(label7,0,12,2,1);

    grid.attach(label8,2,0,2,1);
    grid.attach(label9,2,2,2,1);
    grid.attach(label10,2,4,2,1);
    grid.attach(label11,2,6,2,1);
    grid.attach(label12,2,8,2,1);
    grid.attach(label13,2,10,2,1);
    grid.attach(label14,2,12,2,1);


    grid.show_all();

    add(grid);


  }

  StatsWindow::~StatsWindow()
  {}
