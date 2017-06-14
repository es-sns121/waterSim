#  ===================== waterSim Makefile  =====================

TOP=..

include $(TOP)/configure/CONFIG

#USR_CXXFLAGS += -std=c++0x 
waterSimSRC = $(TOP)/src

# Includes
INC += pv/waterSim.h

# Lib
LIBRARY += waterSim
LIBSRCS += waterSim.cpp
waterSim_LIBS += pvaClient pvDatabase pvAccess nt pvData Com

# Server
PROD_HOST += waterSimMain
waterSimMain_SRCS += waterSimMain.cpp
waterSimMain_LIBS += waterSim
waterSimMain_LIBS += pvaClient pvDatabase pvAccess nt pvData Com

# Client
PROD_HOST += waterSimClient
waterSimClient_SRCS += waterSimClient.cpp
waterSimClient_LIBS += pvaClient pvAccess nt pvData ca Com

# Shared Library ABI version.
SHRLIB_VERSION ?= 1.0

include $(TOP)/configure/RULES

#  ================================================================
