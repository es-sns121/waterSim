#ifndef WATERSIM_H
#define WATERSIM_H

#ifdef epicsExportSharedSymbols
#	define  waterSimEpicsExportSharedSymbols
#	undef   epicsExportSharedSymbols
#endif

#include <pv/pvDatabase.h>

#ifdef waterSimEpicsExportSharedSymbols
#	define epicsExportSharedSymbols  
#	undef  waterSimEpicsExportSharedSymbols
#endif

#include <shareLib.h>

namespace epics { namespace waterSim {

	class epicsShareClass WaterSim {
		public:
			static epics::pvDatabase::PVDatabasePtr create();
	};
	
}}


#endif /* WATERSIM_H */
