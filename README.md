# waterSim

This example provides an EPICS V4 database that hosts an EPICS V4 PVRecord. 
The record holds a normative type pvData structure. The records purpose
is to hold data relevant to a water tower. This includes water level stats,
valve status, and pump status.
The client program will connect to the database and then perform some
demonstrations on how to interact with the record.

## Building

If a proper RELEASE.local file exists two directory levels above **waterSim**,
or in **waterSim/configure/RELEASE.local** then just type:

    make

It can also be built by:

    cp configure/ExampleRELEASE.local configure/RELEASE.local
    edit file configure/RELEASE.local
    make

## To start the database as a standalone main

    > pwd
    /home/Epics/EPICS-CPP-4.5.0/waterSim/
    > bin/$EPICS_HOST_ARCH/waterSimMain

## OR
    > pwd
    /home/Epics/EPICS-CPP-4.5.0/waterSim/
	> serverRunner

## To start the client program

    > pwd
    /home/Epics/EPICS-CPP-4.5.0/waterSim/
    > bin/$EPICS_HOST_ARCH/waterSimclient

## OR
    > pwd
    /home/Epics/EPICS-CPP-4.5.0/waterSim/
	> clientRunner

## waterSim/src/pv

This directory has the following files:

     waterSim.h
  

## waterSim/src

This directory has the following files:

* waterSimClient.cpp  
Code for a client program that uses pvaClient to access the record
hosted on the database.

* waterSim.cpp 
Code that creates a PVRecord.    

* waterSimMain.cpp
Code that allows the PVRecord to be available via a standalone main program.

