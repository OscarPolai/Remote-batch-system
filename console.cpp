#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <fstream>
#include <boost/asio.hpp>
//#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>



boost::asio::io_service ioservice;
//boost::asio::ip::tcp::resolver resolver{ioservice};
//boost::asio::ip::tcp::socket tcp_socket{ioservice};

//std::array<char, 4096> bytes;
bool has_client[5];
std::string Host[5], Port[5], FileName[5];

std::vector<std::string> input;
std::ifstream in;

void read_handler(const boost::system::error_code &ec, std::size_t bytes_transferred);
void do_read();


void parse_string(std::string query_string){

    std::string temp;
    std::replace(query_string.begin(), query_string.end(), '&', ' ');
	std::istringstream iss(query_string);

    if (query_string.length() == 0) return;

    for(int i=0;i<5;++i){
        iss >> temp; 
        if (temp.length() != 0) Host[i] = temp.substr(3, temp.length() - 1);
        iss >> temp;
        if (temp.length() != 0) Port[i] = temp.substr(3, temp.length() - 1);
        iss >> temp;
		if (temp.length() != 0) FileName[i] = temp.substr(3, temp.length() - 1);

        if(Host[i].length() != 0 && Port[i].length() != 0 && FileName[i].length())  has_client[i]=true;
    }

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
/*
void write_handler(const boost::system::error_code &ec, std::size_t bytes_transferred){

    if(!ec) tcp_socket.async_read_some(boost::asio::buffer(bytes), read_handler);
}

void do_write(){
    std::string cmd, html_cmd;

    //cmd = input[0];
    
    

    if (in.is_open()){

		getline(in, cmd);
	}
    cmd += "\n";

    //input.erase(input.begin(), input.begin() + 1);
    
    //cmd = "ls\n";

    html_cmd = cmd;
	boost::algorithm::replace_all(html_cmd, "\n", "&NewLine;");
	boost::algorithm::replace_all(html_cmd, "\r", "");
	boost::algorithm::replace_all(html_cmd, "\"", "&quot;");
	html_cmd = std::string("<script>document.getElementById(\"") + std::string("s") + std::to_string(1) + "\").innerHTML += \"<b>" + html_cmd + "</b>\";</script>";
	//fflush(stdout);
    std::cout << html_cmd << std::endl;
    //fflush(stdout);


    boost::asio::async_write(tcp_socket, boost::asio::buffer(cmd, cmd.length()), read_handler);

}

void read_handler(const boost::system::error_code &ec, std::size_t bytes_transferred){

    if(!ec){

        std::string str = bytes.data();
        boost::algorithm::replace_all(str, "\n", "&NewLine;");
		boost::algorithm::replace_all(str, "\r", "");
		boost::algorithm::replace_all(str, "\"", "&quot;");			
        std::string html_str = std::string("<script>document.getElementById(\"") + std::string("s") + std::to_string(1)+"\").innerHTML += \"" + str + "\";</script>";
		//fflush(stdout);
        std::cout << html_str <<std::endl;
        //fflush(stdout);
        //std::cout.write(bytes.data(), bytes_transferred);
        bytes.fill('\0');

        if(str.find("% ") != std::string::npos){ // read %
            
            do_write(); // prepare next
			
		}else{  //get result
            
            do_read();
            //tcp_socket.async_read_some(boost::asio::buffer(bytes), read_handler);
            
		}
    } 

}

void do_read(){

    bytes.fill('\0');
    tcp_socket.async_read_some(boost::asio::buffer(bytes), read_handler);
}

void connect_handler(const boost::system::error_code &ec){

    if(!ec) do_read();
    //if(!ec) tcp_socket.async_read_some(boost::asio::buffer(bytes), read_handler);   
}

void resolve_handler(const boost::system::error_code &ec, boost::asio::ip::tcp::resolver::iterator it){

    if(!ec) tcp_socket.async_connect(*it, connect_handler);
}

std::vector<std::string> get_file(){

    std::ifstream in;
    std::vector<std::string> res;
    std::string tmp;
	
    in.open("../test_case/t1.txt");

	if (in.is_open()){

		while(getline(in, tmp)){

            res.push_back(tmp + "\n");
		}	
	}

    return res;
}
*/


class ShellSession : 
    public std::enable_shared_from_this<ShellSession>{

    private:
        boost::asio::ip::tcp::socket tcp_socket;
        boost::asio::ip::tcp::resolver resolver;
        std::array<char, 4096> bytes;
        std::ifstream in;
        int num;

    public:
        ShellSession(int n): tcp_socket(ioservice), resolver(ioservice), num(n)
        {
            in.open("./test_case/" + FileName[num]);
        }

        void start(){ do_resolve();}

    private:
        void do_resolve(){
            auto self(shared_from_this());
            boost::asio::ip::tcp::resolver::query q{ Host[num], Port[num]};
            boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(q);
            boost::asio::ip::tcp::endpoint endpoint = *iter;
            tcp_socket.connect(endpoint);
            do_read();  
        }
        
        void do_read(){

            auto self(shared_from_this());
            bytes.fill('\0');
            tcp_socket.async_read_some(boost::asio::buffer(bytes), 
                [this, self](boost::system::error_code ec, size_t length){

                    if(!ec){

                        std::string str = bytes.data();
                        boost::algorithm::replace_all(str, "\n", "&NewLine;");
                        boost::algorithm::replace_all(str, "\r", "");
                        boost::algorithm::replace_all(str, "\"", "&quot;");			
                        std::string html_str = std::string("<script>document.getElementById(\"") + std::string("s") + std::to_string(num)+"\").innerHTML += \"" + str + "\";</script>";
                        fflush(stdout);
                        std::cout << html_str <<std::endl;
                        //fflush(stdout);
                        //std::cout.write(bytes.data(), bytes_transferred);
                        bytes.fill('\0');

                        if(str.find("% ") != std::string::npos){ // read %
                            
                            do_write(); // prepare next
                            
                        }else{  //get result
                            
                            do_read();
                            //tcp_socket.async_read_some(boost::asio::buffer(bytes), read_handler);  
                        }
                    }
                });
        }

        void do_write(){
            std::string cmd, html_cmd;
            auto self(shared_from_this());
            //cmd = input[0];
            
            if (in.is_open()){

                getline(in, cmd);
            }
            cmd += "\n";

            //input.erase(input.begin(), input.begin() + 1);
            
            //cmd = "ls\n";

            html_cmd = cmd;
            boost::algorithm::replace_all(html_cmd, "\n", "&NewLine;");
            boost::algorithm::replace_all(html_cmd, "\r", "");
            boost::algorithm::replace_all(html_cmd, "\"", "&quot;");
            html_cmd = std::string("<script>document.getElementById(\"") + std::string("s") + std::to_string(num) + "\").innerHTML += \"<b>" + html_cmd + "</b>\";</script>";
            fflush(stdout);
            std::cout << html_cmd << std::endl;
            //fflush(stdout);

            //boost::asio::async_write(tcp_socket, boost::asio::buffer(cmd, cmd.length()), read_handler);

            boost::asio::async_write(tcp_socket, boost::asio::buffer(cmd, cmd.length()),
                [this, self](boost::system::error_code ec, std::size_t /*length*/)
                {
                    if (!ec)   do_read();
                });

        }
};

int main(){

    std::string query_string = getenv("QUERY_STRING");
    parse_string(query_string);

    std::cout << "HTTP/1.1 200 OK\r\n";
    std::cout << query_string;

    send_inihtml();

    
    for(int i=0;i<5;++i){

        if(has_client[i] == true)std::make_shared<ShellSession>(i)->start();
    }
    
    ioservice.run();

    return 0;
}