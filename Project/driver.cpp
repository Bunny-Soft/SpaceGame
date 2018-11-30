#include "mainheader.h"
#include <gtkmm.h>
#include <iostream>



int main(int argc, char* argv[])
{

  Gtk::Main app(argc,argv);
  MainWindow window;
  Gtk::Main::run(window);

  if(check == 1)
  {
    Gtk::Main app(argc,argv);
    ShipWindow window2;
    Gtk::Main::run(window2);
  }

  if(ship_type > 0)
  {

    Gtk::Main app(argc,argv);
      PlayWindow window4;
      Gtk::Main::run(window4);

  }






  return 0;
}
