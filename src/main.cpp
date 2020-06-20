#include <iostream>
#include <gtkmm.h>
#include <MainWindow.h>
using namespace std;
int main(int argc, char* argv[]){
  auto app = Gtk::Application::create(argc,argv,"com.test");
  auto window = MainWindow::load();
  return app->run(*window);

}
