all:
	g++ console.cpp -o console.cgi -I /usr/local/include -L /usr/local/lib -std=c++11 -Wall -pedantic -pthread -lboost_system
	g++ http_server.cpp -o http_server  -I /usr/local/include -L /usr/local/lib -std=c++11 -Wall -pedantic -pthread -lboost_system

clean:

	rm -f console.cgi
	rm -f http_server
