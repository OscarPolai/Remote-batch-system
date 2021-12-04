#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
bool has_client[5];
std::string Host[5], Port[5], FileName[5];

void parse_string(std::string query_string){

    for(int i=0;i<5;++i){
        has_client[i]=false;
    }
    has_client[0] = true; has_client[1] = true;

    Host[0] = "nplinux1.cs.nctu.edu.tw";
    Host[1] = "nplinux1.cs.nctu.edu.tw";

    Port[0] = "9999";
    Port[1] = "9998";

    FileName[0] = "t1.txt";
    FileName[1] = "t3.txt";
}




void send_inihtml(){

    std::string contentType;
	std::string initHTML;

    contentType = "Content-type: text/html\r\n\r\n";

    initHTML =
		"<!DOCTYPE html>\
		<html lang=\"en\">\
  			<head>\
    			<meta charset=\"UTF-8\" />\
    			<title>NP Project 3 Console hihi</title>\
    			<link\
      				rel=\"stylesheet\"\
      				href=\"https://cdn.jsdelivr.net/npm/bootstrap@4.5.3/dist/css/bootstrap.min.css\"\
      				integrity=\"sha384-TX8t27EcRE3e/ihU7zmQxVncDAy5uIKz4rEkgIXeMed4M0jlfIDPvg6uqKI2xXr2\"\
      				crossorigin=\"anonymous\"\
    			/>\
    			<link\
      				href=\"https://fonts.googleapis.com/css?family=Source+Code+Pro\"\
      				rel=\"stylesheet\"\
    			/>\
    			<link\
      				rel=\"icon\"\
      				type=\"image/png\"\
      				href=\"https://cdn0.iconfinder.com/data/icons/small-n-flat/24/678068-terminal-512.png\"\
    			/>\
    			<style>\
      				* {\
        				font-family: 'Source Code Pro', monospace;\
        				font-size: 1rem !important;\
      				}\
      				body {\
        				background-color: #212529;\
      				}\
      				pre {\
        				color: #cccccc;\
      				}\
      				b {\
        				color: #01b468;\
      				}\
    			</style>\
  			</head>\
  			<body>\
    			<table class=\"table table-dark table-bordered\">\
      				<thead>\
        				<tr>";


    for(int i=0;i<5;++i){

        if(has_client[i] == true){
            
            initHTML += "<th scope=\"col\">" + Host[i] + ":" + Port[i] + "</th>";
        }
    }

    initHTML += 
                        "</tr>\
                    </thead>\
                    <tbody>\
                        <tr>";

        
    for(int i=0;i<5;++i){

        if(has_client[i] == true){

            initHTML += "<td><pre id = \"s" + std::to_string(i) + "\" class=\"mb-0\"></pre></td>";
        }
    }


    initHTML += 

                        "</tr>\
                    </tbody>\
                </table>\
            </body>\
        </html>";


    std::cout<<contentType;
    std::cout<<initHTML;
}

int main(){

    std::string query_string = getenv("QUERY_STRING");
    parse_string(query_string);

    std::cout << "HTTP/1.1 200 OK\r\n";
    std::cout << query_string;

    send_inihtml();

    return 0;
}