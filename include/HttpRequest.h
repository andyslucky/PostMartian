#include <Poco/Net/HTTPMessage.h>
#include <Poco/Net/HTTPSession.h>
#include <istream>
#include <unordered_map>
#include <functional>
#include <memory>
#include <Poco/URI.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPClientSession.h>
using namespace Poco::Net;
class HttpRequest{
	private:
		std::shared_ptr<HTTPSession> session;
		std::shared_ptr<Poco::Net::HTTPRequest> request;
		std::string body;
	public:
		HttpRequest(std::string method,Poco::URI uri, std::unordered_map<std::string, std::string> headers, std::string body){
			this->body = body;
			if(uri.getScheme() == "https"){
				this->session = std::shared_ptr<HTTPSession>(new HTTPSClientSession(uri.getHost(),uri.getPort()));
			}else{
				this->session = std::shared_ptr<HTTPSession>(new HTTPClientSession(uri.getHost(),uri.getPort()));
			}
			std::string path(uri.getPathAndQuery());
			if (path.empty()) 
				path = "/";
			this->request = std::shared_ptr<Poco::Net::HTTPRequest>(new Poco::Net::HTTPRequest(method,path,HTTPMessage::HTTP_1_1));

			for(auto keypair : headers){
				this->request->add(keypair.first, keypair.second);
			}
			
		}

		HttpRequest(std::string method,Poco::URI uri, std::unordered_map<std::string,std::string> headers) : HttpRequest(method,uri,headers,""){

		}

		void sendRequest(std::function<void(HTTPResponse&, std::istream&)> responseHandler){
			auto rawSession = this->session.get();
			auto http_session = dynamic_cast<HTTPClientSession*>(rawSession);
			auto https_session = dynamic_cast<HTTPSClientSession*>(rawSession);
			HTTPResponse response;
			if(http_session != nullptr){
				http_session->sendRequest(*this->request) << this->body;
				std::istream& str = http_session->receiveResponse(response);
				responseHandler(response,str);
			}else if(https_session != nullptr){
				https_session->sendRequest(*this->request) << this->body;
				std::istream& str = https_session->receiveResponse(response);
				responseHandler(response,str);
			}

		}


};