SOURCES = Tree.cpp TimeAnalysis.cpp

HEADERS = Tree.h

include($$(ACROOTSOFTWARE)/aux/ACbinary.prf)

TARGET = TimeAnalysis

LIBS += $$ROOTLIBS -lacsoft

