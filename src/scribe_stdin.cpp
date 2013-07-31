/***************************************************************************
 * 
 * Copyright (c) 2013 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
#include <unistd.h> 
#include <iostream>
#include <getopt.h>
// scribe headers
#include "scribe.h" // this header is in the gen-cpp directory
// thrift headers
#include <protocol/TBinaryProtocol.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>
#include <transport/TZlibTransport.h>

using namespace std;
using namespace boost;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace scribe::thrift;

#define G_Tools_Version "1.0.0"

void print_usage(const char* program_name) {
	cout << "Scribe tools made by 0xmalloc@gmail.com" <<endl;
	cout << "Version :" << G_Tools_Version << endl;
	cout << "Usage: " << program_name << " [-h host] [-p port] [-c category]" << endl;
	cout << "Usage: " << program_name << " [-v help] [--help help]" << endl;
}

int main(int argc, char** argv)
{
	if(argc <= 1){
       print_usage(argv[0]);
       exit(0);
    }
    std::string category; // the log category
	std::string host;    // the ip host
	unsigned int port = 0; // the port
	int next_option;
	const char* const short_options = "h:p:c:v";
    const struct option long_options[] = { 
        { "host",     0, NULL, 'h' },
        { "port",     0, NULL, 'p' },
        { "category", 0, NULL, 'c' },
        { "help",     0, NULL, 'v' },
    };

	while (0 < (next_option = getopt_long(argc, argv, short_options, long_options, NULL))) {
		switch (next_option) {
			default:
			case 'v':
				print_usage(argv[0]);
				exit(0);
			case 'h':
				host = optarg;
				break;
			case 'c':
				category = optarg;
				break;
			case 'p':
				port = strtoul(optarg, NULL, 0);
				break;
		}		
	}
	cout << argv[0] <<" started  ==> host:"<<host<<" port:"<< port <<" category:"<< category <<endl;
    if(0 == port || category.empty() || host.empty()){
        cout << "Parameters Error, use "<<argv[0] <<" --help to see the help info"<<endl;
        exit(0);
    }

	shared_ptr<TTransport> socket(new TSocket(host, port));
	shared_ptr<TTransport> transport(new TFramedTransport(socket));
	//shared_ptr<TZlibTransport> zlib(new TZlibTransport(socket));
    //shared_ptr<TTransport> transport(new TBufferedTransport(zlib));
	shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	// use boost shared pointer
	shared_ptr<scribeClient> sbclient(new scribeClient(protocol));
    try
	{
	    transport->open();
        std::string message;
        while(std::cin){
            getline(std::cin, message);
            
            LogEntry le;
            le.message = message;
	        le.category = category;

	        std::vector<LogEntry> messages;
	        messages.push_back(le);
	        // write to scribe server
	        int ret = sbclient->Log(messages);
            if(0 != ret){
                printf("hit try later\n");
            }
        }
	    // close the transport
	    transport->close();
	}
    catch(TException &tx)
	{
	    printf("ERROR: %s\n",tx.what());
	}
}





















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
