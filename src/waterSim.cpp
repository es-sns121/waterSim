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

// Located in local pv directory.
#include <pv/waterSim.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <pv/standardField.h>
#include <pv/standardPVField.h>
#include <pv/channelProviderLocal.h>
#include <pv/serverContext.h>
#include <pv/ntscalar.h>

using namespace std;
using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::nt;
using namespace epics::pvDatabase;
using namespace epics::pvAccess;
// Defined locally in pv/waterSim.h
using namespace epics::waterSim;

// Global definitions of singleton builder classes
static FieldCreatePtr         fieldCreate = getFieldCreate();
static StandardFieldPtr     standardField = getStandardField();
static PVDataCreatePtr       pvDataCreate = getPVDataCreate();
static StandardPVFieldPtr standardPVField = getStandardPVField();

static void createRecord(
	PVDatabasePtr const &master,
	ScalarType scalarType,
	string const &recordName)
{
	StructureConstPtr top = fieldCreate->createFieldBuilder()->
		add("water_level", pvDouble)->
		add("pump_status", pvBoolean)->
		add("pump_rate", pvDouble)->
		add("outflow", pvDouble)->
		add("alarm", standardField->alarm())->
		add("timeStamp", standardField->timeStamp())->
		createStructure();

	PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);

	// Create the record and attempt to add it to the database.	
	PVRecordPtr pvRecord = PVRecord::create(recordName, pvStructure);
	
	bool result = master->addRecord(pvRecord);
	if (!result) cerr << "Failed to add record " << recordName << " to database\n";

	return ;
}

// Creates and adds records to database.
PVDatabasePtr WaterSim::create()
{

	// Get the database hosted by the local provider.
	PVDatabasePtr master = PVDatabase::getMaster();
	
	// Create string and string array records.	
	createRecord(master, pvDouble, "waterSim");
	
	return master;
}

