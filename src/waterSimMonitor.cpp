/*
 * =======================================================================
 * 
 * 	Author: Evan Smith
 * 	Date  : 6/13/2017
 *
 * 	waterSimMonitor.cpp
 *
 * =======================================================================
 */

#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>
#include <pv/pvAccess.h>
#include <pv/pvaClient.h>
#include <pv/pvData.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;


int main (int argc, char **argv)
{
	bool verbosity(false);
	bool debug(false);
	
	// Handle executable flag.
	if (argc > 1 && argv[1] != NULL) {
		string arg(argv[1]);
		if (arg == "-v") {
		
			verbosity = true;		
	
	/* Help flag */
		} else if (arg == "-h") {
			
			cout << "Help -- executable flags -- only one accepted at a time\n"
			     << "\t-v (verbose. prints ouput.)\n"
				 << "\t-d (debug. prints debug information)\n"
				 << "\t-h (help. prints help information)\n";
			return 0;
	
	/* Debug flag */
		} else if (arg == "-d") {
		
			debug = true;
		
	/* Error */
		} else {
			
			cout << "Unrecognized option: '" << arg
			     << "'. ('waterSimMonitor -h' for help.)\n"; 
			return 1;
		
		}
	}
	
	cout << "waterSim Monitor\n";
	
	string channel_name("waterSim");

	try {
	
		PvaClientPtr pvaClient = PvaClient::get("pva");
		
		cout << "debug : " << (debug ? "true" : "false") << endl;
		
		if (debug) PvaClient::setDebug(true);
	
		// Have simulation thread in this program?

		// Monitor stuff and print level values.
		PvaClientChannelPtr channel = pvaClient->channel(channel_name);
		
		if (!channel) {
			cerr << "Channel '" << channel_name << "' failed to connect.\n";
			return 1;
		}

		PvaClientMonitorPtr monitor = channel->monitor("");
		
		while(true) {
			
			monitor->waitEvent();
		
			PvaClientMonitorDataPtr monitorData = monitor->getData();
			
			cout << "Changed: " << *monitorData->getChangedBitSet() << endl;

			monitor->releaseEvent();
		}


	} catch (std::runtime_error e) {	
		
		cerr << "exception: " << e.what() << endl;
		return -1;
	
	}

	return 0;
}

