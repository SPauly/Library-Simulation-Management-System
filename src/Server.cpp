#include "Library.h"
#include "LibNet.h"

#include <iostream>

#define log(x) std::cout << x

namespace libnet = lsms::libnet;
namespace libmsg = lsms::libnet::libmsg;

class Server : public libnet::Server_Interface<libmsg::_DefaultMessageType>
{
public:
	Server(uint16_t nPort) : libnet::Server_Interface<libmsg::_DefaultMessageType>(nPort)
	{}

protected:
	virtual bool on_client_connect(std::shared_ptr<libnet::Connection<libmsg::_DefaultMessageType>> client)
	{
		return true;
	}
	
	virtual void on_client_disconnect(std::shared_ptr<libnet::Connection<libmsg::_DefaultMessageType>> client)
	{

	}

	virtual void on_message(std::shared_ptr<libnet::Connection<libmsg::_DefaultMessageType>> client, libmsg::Message<libnet::Connection<libmsg::_DefaultMessageType>>& msg)
	{}

};


void print_welcome()
{
	std::fstream file;
	std::string line;

	try
	{
		file.open((fm::init_workingdir() + "Welcome.txt"), std::ios::in | std::ios::binary);
		if (file.is_open())
		{
			while (!file.eof())
			{
				fm::_getline(file, line);
				std::cout << line << std::endl;
			}
		}
		else
			log("Could not open Welcome Message\n");
	}
	catch (const std::ifstream::failure &e)
	{
	}
	file.close();
};

int main()
{
	Server server(60000);
	server.start_server();
	
	lsms::Library lib{};
	 
	print_welcome();
	std::cin.get();
	system("cls");

	lib.run();

	return 0;
}

