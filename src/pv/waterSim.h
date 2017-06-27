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

#include <pv/ntfield.h>
#include <pv/pvAlarm.h>

namespace epics { namespace waterSim {
	
	class epicsShareClass WaterSim 
	{
		public:
			static epics::pvDatabase::PVDatabasePtr create();
	
	}; /* class WaterSim */
	
	class WaterSimRecord;
	typedef std::tr1::shared_ptr<WaterSimRecord> WaterSimRecordPtr;
	
	class epicsShareClass WaterSimRecord : 
		public epics::pvDatabase::PVRecord 
	{
		public:
			POINTER_DEFINITIONS(WaterSim);
			
			static WaterSimRecordPtr create(const std::string & recordName);
			
			virtual ~WaterSimRecord() {}
			
			virtual void process();

		private:
			WaterSimRecord(
				const std::string & recordName, 
				epics::pvData::PVStructurePtr const & pvStructure);
			
			void initPvt();

			epics::pvData::PVAlarm pvHighAlarm;
			epics::pvData::Alarm   highAlarm;
			epics::pvData::PVAlarm pvLowAlarm;
			epics::pvData::Alarm   lowAlarm;
			
			epics::pvData::PVStructurePtr alarmLimit;

			epics::pvData::PVDoublePtr  waterLevel;
			epics::pvData::PVDoublePtr  outflow;
			epics::pvData::PVBooleanPtr pumpStatus;
			epics::pvData::PVDoublePtr  pumpRate;
	
	}; /* class WaterSimRecord */

}} /* namespace epics, waterSim */


#endif /* WATERSIM_H */
