SOURCES = RTIToHist.C


HEADERS =

include($$(ACROOTSOFTWARE)/aux/ACbinary.prf)

TARGET = RTIToHist

LIBS += $$ROOTLIBS -lacsoft

