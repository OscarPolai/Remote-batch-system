//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2021 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class session
  : public std::enable_shared_from_this<session>
{
public:
  session(tcp::socket socket)
    : socket_(std::move(socket))
  {
  }

  void start()
  {
    do_read();
  }

private:
  
  void parse_header(char* data){    
    
    std::string tmp(data);
    std::istringstream is(tmp);

    is >> REQUEST_METHOD >> REQUEST_URI >> SERVER_PROTOCOL >> tmp >> HTTP_HOST;

    std::istringstream iss(REQUEST_URI);  // set iss is REQUEST_URI ( ./console.cgi?h0=nplinux1.cs.nctu.edu.tw&p0...)

    
    getline(iss, REQUEST_URI, '?');  // overwrite REQUEST_URI from ? in the iss string.
	  getline(iss, QUERY_STRING); 

    SERVER_ADDR = socket_.local_endpoint().address().to_string();
		REMOTE_ADDR = socket_.remote_endpoint().address().to_string();

		SERVER_PORT = std::to_string(socket_.local_endpoint().port());
		REMOTE_PORT = std::to_string(socket_.remote_endpoint().port());

    setenv("REQUEST_URI", REQUEST_URI.c_str(), 1);
    setenv("REQUEST_METHOD", REQUEST_METHOD.c_str(), 1);
    setenv("HTTP_HOST", HTTP_HOST.c_str(), 1);
    setenv("QUERY_STRING", QUERY_STRING.c_str(), 1);
    setenv("SERVER_ADDR", SERVER_ADDR.c_str(), 1);
    setenv("REMOTE_ADDR", REMOTE_ADDR.c_str(), 1);

  }

  void do_read()
  {
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
          if (!ec)
          {

            parse_header(data_);
            //do_write(length);
            pid_t pid;
            pid = fork();
            //setenv("REQUEST_METHOD", "GET", 1);
            if(pid < 0){
              std::cerr << "Fork error\n";
            }
            else if(pid == 0){

              int fd = socket_.native_handle();
              dup2(fd, STDIN_FILENO);
              dup2(fd, STDOUT_FILENO);
              dup2(STDOUT_FILENO, STDERR_FILENO);
              
              // execute cgi program            
              std::string path = "./printenv.cgi";
              std::cout << "HTTP/1.1 200 OK\r\n";
              
              //std::string REQUEST_URI = getenv( REQUEST_URI.c_str());
             
              //std::cout<<  REQUEST_URI << std::endl;
              execl(path.c_str(), "printenv", NULL);
              std::cerr << "ERROR: exec error" << std::endl;
              exit(0);
            }

            socket_.close();
          }
        });
  }

  void do_write(std::size_t length)
  {
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            do_read();
          }
        });
  }

  tcp::socket socket_;
  enum { max_length = 1024 };
  char data_[max_length];
  std::string REQUEST_METHOD, REQUEST_URI, SERVER_PROTOCOL, HTTP_HOST, QUERY_STRING, SERVER_ADDR, REMOTE_ADDR, SERVER_PORT, REMOTE_PORT;
};

class server
{
public:
  server(boost::asio::io_context& io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
  {
    do_accept();
  }

private:
  void do_accept()
  {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
          if (!ec)
          {
            std::make_shared<session>(std::move(socket))->start();
          }

          do_accept();
        });
  }

  tcp::acceptor acceptor_;
};

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: async_tcp_echo_server <port>\n";
      return 1;
    }

    boost::asio::io_context io_context;

    server s(io_context, std::atoi(argv[1]));

    std::cout<<"ASIO server start"<<std::endl;

    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}