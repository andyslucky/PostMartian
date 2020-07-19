#include <gtkmm.h>
#include <iostream>
#include <HttpRequest.h>
#include <memory>

#define move_cstr(Type) Type(Type&& rhs)

#define derived_move_cstr(Type,Super) Type(Type&& rhs) : Super((Super&&)rhs)

class RequestForm : public Gtk::Box{
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
	derived_move_cstr(RequestForm,Gtk::Box){}

	RequestForm(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) : Gtk::Box(cobject){
		refGlade->get_widget("send_button",send_button);
		if(send_button){
			send_button->signal_clicked().connect(sigc::mem_fun(*this,&RequestForm::send_request_clicked));
		}
		
		refGlade->get_widget("addr_bar",addr_bar);
		
		
		refGlade->get_widget("content_view",content_view);
		
		refGlade->get_widget("method_box",method_box);

		refGlade->get_widget("content_notebook",content_notebook);
		refGlade->get_widget("message_label",message_label);
		refGlade->get_widget("add_header",add_header);
		if(add_header){
			add_header->signal_clicked().connect(sigc::mem_fun(*this,&RequestForm::add_header_clicked));
		}
		refGlade->get_widget("delete_header",delete_header);
		if(delete_header){
			delete_header->signal_clicked().connect(sigc::mem_fun(*this,&RequestForm::delete_header_clicked));
		}
		refGlade->get_widget("header_tree",header_tree);
		this->liststore2 = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(
			refGlade->get_object("liststore2")
			);
	}

	~RequestForm(){

	}

	inline static const std::string COMPONENT_ID = "main_box";
	
	inline static const std::string GLADE_FILE = "./res/requestform.glade";
	
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