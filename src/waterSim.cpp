/*
 * =============================================================
 * 	Author: Evan Smith
 * 	Date  : 6/5/2017
 *
 * 	waterSim.cpp
 *
 *	Source file that implements the creation of the normative
 *	type database.
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

static void createScalarRecord(
	PVDatabasePtr const &master,
	ScalarType scalarType,
	string const &recordNamePrefix)
{
	string recordName = recordNamePrefix;
	
	NTScalarBuilderPtr ntScalarBuilder = NTScalar::createBuilder();
	
	// Create the pvStructure to be inserted into the record.
	PVStructurePtr pvStructure = ntScalarBuilder->
		value(scalarType)->
		addAlarm()->
		addTimeStamp()->
		createPVStructure();

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
	createScalarRecord(master, pvDouble, "waterSim");
	
	return master;
}

