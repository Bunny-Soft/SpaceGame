#include "mainheader.h"
#include <iostream>

int check = 0;
int ship_type =0;

////////////////////////////////////////////////////////////////////////////////////

                      /* GUI Window to display the Main menu */

////////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow()
{

  image.set("yummy.jpg");
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

  ship1.set("yummy.jpg");
  grid.attach(ship1,0,0,2,2);

  button1.add_label("Crusier");
  button1.signal_pressed().connect(sigc::mem_fun(*this,&ShipWindow::Selected_Cruiser));
  grid.attach(button1,0,3,2,3);

  ship2.set("yummy.jpg");
  grid.attach(ship2,2,0,2,2);

  button2.add_label("Bulwark");
  button2.signal_pressed().connect(sigc::mem_fun(*this,&ShipWindow::Selected_Bulwark));
  grid.attach(button2,2,3,2,3);

  ship3.set("yummy.jpg");
  grid.attach(ship3,4,0,2,2);

  button3.add_label("Mining");
  button3.signal_pressed().connect(sigc::mem_fun(*this,&ShipWindow::Selected_Mining));
  grid.attach(button3,4,3,2,3);

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

}

TutorialWindow::~TutorialWindow()
{}



  PlayWindow::PlayWindow()
  {

    ship1.set("yummy.jpg");
    grid.attach(ship1,0,0,4,4);

    button2.add_label("Generate Power");
    button2.signal_pressed().connect(sigc::mem_fun(*this,&PlayWindow::Generate_power));
    grid.attach(button2,0,4,2,2);


    button3.add_label("Combat");
    button3.signal_pressed().connect(sigc::mem_fun(*this,&PlayWindow::Combat_Status));
    grid.attach(button3,2,4,2,2);

    button4.add_label("Repair");
    button4.signal_pressed().connect(sigc::mem_fun(*this,&PlayWindow::Repair));
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

  }


  void PlayWindow::Repair()
  {

  }

  void PlayWindow::Generate_power()
  {

  }

  void PlayWindow::show_ship_status()
  {

  }
