#  ===================== waterSim Makefile  =====================

TOP=..

include $(TOP)/configure/CONFIG

waterSimSRC = $(TOP)/src

# Includes
INC += pv/waterSim.h

# Lib
LIBRARY += waterSim
LIBSRCS += waterSim.cpp
waterSim_LIBS += pvaClient pvDatabase pvAccess nt pvData Com

# Server
PROD_HOST += Server 
Server_SRCS += waterSimMain.cpp
Server_LIBS += waterSim
Server_LIBS += pvaClient pvDatabase pvAccess nt pvData Com

# Client
PROD_HOST += Client
Client_SRCS += waterSimClient.cpp
Client_LIBS += pvaClient pvAccess nt pvData ca Com

# Monitor
PROD_HOST += Monitor
Monitor_SRCS += waterSimMonitor.cpp
Monitor_LIBS += pvaClient pvAccess nt pvData ca Com

# Shared Library ABI version.
SHRLIB_VERSION ?= 1.0

include $(TOP)/configure/RULES

#  ================================================================
