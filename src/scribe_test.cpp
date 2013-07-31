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

using namespace std;
using namespace boost;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace scribe::thrift;

#define G_Tools_Version "1.0.0"
const char* g_msg = "N";

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
    unsigned long data_size = 0;	
    unsigned long run_times = 0;	
	int next_option;
	const char* const short_options = "h:p:c:s:vt:";
    const struct option long_options[] = { 
        { "host",     0, NULL, 'h' },
        { "port",     0, NULL, 'p' },
        { "category", 0, NULL, 'c' },
        { "size", 0, NULL, 's' },
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
            case 's':
                data_size = strtoul(optarg, NULL, 0); 
                break;
			case 'p':
				port = strtoul(optarg, NULL, 0);
				break;
            case 't':
                run_times = strtoul(optarg, NULL, 0);
                break;
		}		
	}
	cout << argv[0] <<" started  ==> host:"<<host<<" port:"<< port <<" category:"<< category << " data_size:"<< 
        data_size<<" run times:"<< run_times<<endl;
    if(0 == port || category.empty() || host.empty() || 0 == data_size || 0 == run_times){
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
	    // create a log entry and push it to vector
        std:string msg_str;
        for(int i = 0; i < data_size; i++){
            msg_str.append(g_msg);
        }
        int count = 0;
        while(true){
            LogEntry le;
	    
            le.message = msg_str;
	        le.category = category;

	        std::vector<LogEntry> messages;
	        messages.push_back(le);
	        // write to scribe server
	        int ret = sbclient->Log(messages);
            if(0 != ret){
                printf("hit try later\n");
            }
            ++count;
            if(count == run_times){
                break;
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
