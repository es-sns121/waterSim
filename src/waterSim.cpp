/*
 * =============================================================
 * 	Author: Evan Smith
 * 	Date  : 6/5/2017
 *
 * 	waterSim.cpp
 *
 *	Source file that implements the creation of the database.
 *
 * =============================================================
 */

#define epicsExportSharedSymbols
#include <pv/waterSim.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <pv/channelProviderLocal.h>
#include <pv/ntfield.h>
#include <pv/ntscalar.h>
#include <pv/serverContext.h>
#include <pv/standardField.h>
#include <pv/standardPVField.h>

using namespace std;
using std::tr1::static_pointer_cast;

using namespace epics::nt;
using namespace epics::pvAccess;
using namespace epics::pvData;
using namespace epics::pvDatabase;

// Defined locally in pv/waterSim.h
using namespace epics::waterSim;

// Global definitions of singleton builder classes
static FieldCreatePtr         fieldCreate = getFieldCreate();
static StandardFieldPtr     standardField = getStandardField();
static PVDataCreatePtr       pvDataCreate = getPVDataCreate();
static StandardPVFieldPtr standardPVField = getStandardPVField();

PVStructurePtr createAlarmLimit()
{
	
	StructureConstPtr top = fieldCreate->createFieldBuilder()->
		
		add("active", pvBoolean)->
		add("lowAlarmLimit", pvDouble)->
		add("lowWarningLimit", pvDouble)->
		add("highWarningLimit", pvDouble)->
		add("highAlarmLimit", pvDouble)->
		add("lowAlarmSeverity", pvInt)->
		add("lowWarningSeverity", pvInt)->
		add("highWarningSeverity", pvInt)->
		add("highAlarmSeverity", pvInt)->
		add("hysteresis", pvDouble)->
		
		createStructure();

	PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);

	return pvStructure;
}

WaterSimRecordPtr WaterSimRecord::create(const string & recordName)
{
	StructureConstPtr top = fieldCreate->createFieldBuilder()->
		
		add("water_level", pvDouble)->
		add("pump_status", pvBoolean)->
		add("pump_rate", pvDouble)->
		add("outflow", pvDouble)->
		
		add("highAlarm", standardField->alarm())->
		add("lowAlarm", standardField->alarm())->
		
		add("timeStamp", standardField->timeStamp())->
		add("display", standardField->display())->
		add("alarmLimit", createAlarmLimit()->getField())->

		createStructure();

	PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);

	WaterSimRecordPtr pvRecord( new WaterSimRecord(recordName, pvStructure) );

	pvRecord->initPvt();

	return pvRecord;

}

// Creates and adds records to database.
PVDatabasePtr WaterSim::create()
{

	// Get the database hosted by the local provider.
	PVDatabasePtr master = PVDatabase::getMaster();
	
	string recordName("waterSim");

	PVRecordPtr pvRecord = WaterSimRecord::create(recordName);

	bool result = master->addRecord(pvRecord);
	if (!result) cerr << "Failed to add record " << recordName << " to database\n";
	
	return master;
}

WaterSimRecord::WaterSimRecord(const string & recordName,
				   const PVStructurePtr & pvStructure)
		: PVRecord(recordName, pvStructure)
{
}

void WaterSimRecord::initPvt() {
	
	initPVRecord();

	PVStructurePtr pvStructure = getPVStructure();
	
	alarmLimit = createAlarmLimit();

	waterLevel = pvStructure->getSubField<PVDouble>("water_level");
	waterLevel->put(187.0);
	
	outflow = pvStructure->getSubField<PVDouble>("outflow");
	pumpRate = pvStructure->getSubField<PVDouble>("pump_rate");
	pumpStatus = pvStructure->getSubField<PVBoolean>("pump_status");
	
	PVFieldPtr pvField = pvStructure->getSubField("highAlarm");
	
	pvHighAlarm.attach(pvField);
	highAlarm.setMessage("no alarm");
	highAlarm.setSeverity(noAlarm);
	pvHighAlarm.set(highAlarm);

	pvField = pvStructure->getSubField("lowAlarm");
	
	pvLowAlarm.attach(pvField);
	lowAlarm.setMessage("no alarm");
	lowAlarm.setSeverity(noAlarm);
	pvLowAlarm.set(lowAlarm);

	alarmLimit = pvStructure->getSubField<PVStructure>("alarmLimit");
	alarmLimit->getSubField<PVDouble>("highWarningLimit")->put(188.0);
	alarmLimit->getSubField<PVDouble>("lowWarningLimit")->put(184.0);
}

void WaterSimRecord::process()
{
	// Based on the outflow, set the new water level.
	double newLevel = waterLevel->get();
	newLevel -= outflow->get();	
	newLevel += pumpRate->get();

	cout << "newLevel: " << newLevel << endl;

	if (newLevel < alarmLimit->getSubField<PVDouble>("lowWarningLimit")->get()) {
		pumpStatus->put(true);
		pumpRate->put(1.2);
	}

	if (newLevel > alarmLimit->getSubField<PVDouble>("highWarningLimit")->get()) {
		pumpStatus->put(false);
		pumpRate->put(0.0);
	}
	
	waterLevel->put(newLevel);

	
}
