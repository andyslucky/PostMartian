#pragma once
#include <gtkmm.h>
#include <RequestForm.h>
#include <iostream>
#include <memory>
#include <vector>
class MainWindow : public Gtk::Window{
private:
	Gtk::Button* add_request;
	Gtk::Notebook* main_notebook;
	std::vector<Gtk::Frame*> pages;

public:
	MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) : Gtk::Window(cobject){
		refGlade->get_widget("add_request",add_request);
		if(add_request){
			add_request->signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::add_request_clicked));
		}

		refGlade->get_widget("main_notebook",main_notebook);
		
	}
	~MainWindow (){
		for(auto frame : this->pages){
			this->main_notebook->remove_page(*frame);
			delete frame;
		}
		delete add_request;
		delete main_notebook;
	}
	inline static const std::string COMPONENT_ID = "main_window";
	inline static const std::string GLADE_FILE = "./res/postmartianmain.glade";
	inline static MainWindow* load(){
		MainWindow* window = nullptr;
		try{
			auto builder = Gtk::Builder::create_from_file(MainWindow::GLADE_FILE);
			builder->get_widget_derived(MainWindow::COMPONENT_ID,window);
		}catch(const Glib::FileError& ex){
    		std::cerr << "FileError: " << ex.what() << std::endl;
		}
		
		return window;
	}

	void add_request_clicked(){
		Gtk::Frame* box = new Gtk::Frame();
		RequestForm* main_box = nullptr;
		try{
			auto builder = Gtk::Builder::create_from_file(RequestForm::GLADE_FILE);
			builder->get_widget_derived(RequestForm::COMPONENT_ID,main_box);
			if(!main_box){
				delete box;
				return;
			}
			box->add(*main_box);
			main_notebook->append_page(*box);
			main_notebook->show_all_children();
		}catch(const Glib::FileError& ex){
			std::cerr << "FileError: " << ex.what() << std::endl;
		}
	}

};