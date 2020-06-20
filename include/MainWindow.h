#pragma once
#include <gtkmm.h>
#include <iostream>
#include <fstream>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/URI.h>
#include <Poco/StreamCopier.h>
#include <HttpRequest.h>
#include <istream>
#include <memory>
#include <sstream>
#include <unordered_map>
class MainWindow : public Gtk::Window{
private:
	Gtk::Entry* addr_bar;
	Gtk::TextView* content_view;
	Gtk::Button* send_button;
	Gtk::ComboBox* method_box;
	Gtk::Notebook* content_notebook;
	int content_notebook_page = 0;
	Gtk::Label* message_label;
	Gtk::Button* add_header;
	Gtk::Button* delete_header;
	Gtk::TreeView* header_tree;
	Glib::RefPtr<Gtk::ListStore> liststore2;

public:
	MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) : Gtk::Window(cobject){
		refGlade->get_widget("send_button",send_button);
		if(send_button){
			send_button->signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::send_request_clicked));
		}
		
		refGlade->get_widget("addr_bar",addr_bar);
		
		
		refGlade->get_widget("content_view",content_view);
		
		refGlade->get_widget("method_box",method_box);

		refGlade->get_widget("content_notebook",content_notebook);
		refGlade->get_widget("message_label",message_label);
		refGlade->get_widget("add_header",add_header);
		if(add_header){
			add_header->signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::add_header_clicked));
		}
		refGlade->get_widget("delete_header",delete_header);
		if(delete_header){
			delete_header->signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::delete_header_clicked));
		}
		refGlade->get_widget("header_tree",header_tree);
		this->liststore2 = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(
    		refGlade->get_object("liststore2")
		);
	}
	~MainWindow (){
		delete addr_bar;
		delete content_view;
		delete send_button;
		delete method_box;
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
	void show_message(const char* message){
		this->show_message(std::string(message));
	}

	void show_message(std::string message){
		message_label->set_text(Glib::ustring("😥️ "+message));
		if(content_notebook_page > 0){
			this->content_notebook->prev_page();
			content_notebook_page = 0;
		}
	}
	void add_header_clicked(){
		liststore2->append();
	}

	void delete_header_clicked(){

	}

	void send_request_clicked(){
		if(content_notebook_page != 1){
			content_notebook_page = 1;
			this->content_notebook->next_page();
		}
		Poco::URI uri(addr_bar->get_text());
		auto method = this->method_box->get_entry_text();
		if(method == ""){
			this->show_message("No method selected");
			return;
		}
		std::unordered_map<std::string, std::string> headers;
		try{
			HttpRequest request(method,uri,headers);
			request.sendRequest([=](Poco::Net::HTTPResponse& response,std::istream& bodyStream){
				std::stringstream os;
				Poco::StreamCopier::copyStream(bodyStream,os);
				this->content_view->get_buffer()->set_text(Glib::ustring(os.str()));
			});
			
		}catch(const std::exception& e){
			this->show_message(e.what());
		}
		
	}

};