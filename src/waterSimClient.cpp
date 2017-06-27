/*
 * =======================================================================
 * 
 * 	Author: Evan Smith
 * 	Date  : 6/13/2017
 *
 * 	waterSimClient.cpp
 *
 * =======================================================================
 */

#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>
#include <time.h>
#include <pv/pvAccess.h>
#include <pv/pvaClient.h>
#include <pv/pvData.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

double genOutflowValue () {
	static bool first_call = true;

	if (first_call) {
		first_call = false;
		srand(time(NULL));
	}

	double outflow(0.0);

	outflow = ((rand() % 10000) / 10000.0);

	return outflow;
}

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
			     << "'. ('waterSimClient -h' for help.)\n"; 
			return 1;
		
		}
	}
	
	cout << "waterSim Client\n";
	
	string channel_name("waterSim");

	try {
	
		PvaClientPtr pvaClient = PvaClient::get("pva");
		
		cout << "debug : " << (debug ? "true" : "false") << endl;
		
		if (debug) PvaClient::setDebug(true);
	
	/* Connect the channel to the record. */
		
		PvaClientChannelPtr channel = pvaClient->channel(channel_name);
	
		if (!channel) {
			cerr << "Channel '" << channel_name << "' failed to connect.\n";
			return 1;
		}

	/* Alter the data on the record. */
		
		PvaClientPutPtr put = channel->createPut("");
		PvaClientPutDataPtr putData = put->getData();
		PVStructurePtr pvStructure = putData->getPVStructure();
		
		PVDoublePtr outflow = pvStructure->getSubField<PVDouble>("outflow");
		
		if (!outflow) {
			cerr << "outflow NULL\n";
			return -1;
		}
		
		double outflow_value = 0;
		
		for (size_t i = 0; i < 96; ++i) {
			
			outflow_value = genOutflowValue();
			
			if (verbosity) { 
				cout << setprecision(4) << fixed
					 << setw(10) << "outflow: " << setw(6) << outflow_value << "\n";
			}
			outflow->put(outflow_value);
			put->put();
		
			sleep(2.0);
		}
		

	} catch (std::runtime_error e) {	
		
		cerr << "exception: " << e.what() << endl;
		return -1;
	
	}

	return 0;
}

