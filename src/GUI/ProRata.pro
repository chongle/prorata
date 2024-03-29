######################################################################
# Automatically generated by qmake (2.00a) Wed Aug 24 09:25:10 2005
######################################################################

TEMPLATE = app
DEPENDPATH += .
win32:DEFINES    += QT_DLL QWT_DLL
INCLUDEPATH += . ../qwt/include ../../include
RC_FILE = ProRata.rc
QT += xml

LIBS += -lqtassistantclient -L..\..\lib -L..\Common\ -L..\PRatio -L..\SicForma -lqwt5 -lm \
..\PRatio\lubksb.o \
..\PRatio\ludcmp.o \
..\PRatio\nrutil.o \
..\PRatio\pca.o \
..\PRatio\pcaUtility.o \
..\PRatio\peakPicker.o \
..\PRatio\peptideInfo.o \
..\PRatio\peptideRatio.o \
..\PRatio\proteinInfo.o \
..\PRatio\proteinRatio.o \
..\PRatio\proteomeInfo.o \
..\PRatio\savgol.o \
..\PRatio\smoother.o \
..\Common\chromatogram.o \
..\Common\directoryStructure.o \
..\Common\getopt.o \
..\Common\proRataConfig.o \
..\Common\proRataParameters.o \
..\Common\tinystr.o \
..\Common\tinyxml.o \
..\Common\tinyxmlerror.o \
..\Common\tinyxmlparser.o \
..\SicForma\base64.o  \
..\SicForma\dtaSelectReader.o \
..\SicForma\idData.o \
..\SicForma\isotopologue.o \
..\SicForma\msData.o \
..\SicForma\mzReader.o \
..\SicForma\ramp.o \
..\SicForma\sicInfo.o  \
..\..\lib\wsock32.lib

#../Libparsifal/temp/lib/libparsifal.a


# Input
HEADERS += helpIcons.h \
	   proRataSplash.h \
           notAvailableImage.h \
           proRataChromatogramGraph.h \
           proRataExec.h \
           proRataExecDialog.h \
           proRataGraph.h \
           proRataGraphDialog.h \
           proRataGraphPane.h \
           proRataImage.h \
           proRataImagePane.h \
           proRataLikelihoodGraph.h \
           proRataSequenceCoverageGraph.h \
           fastaData.h \
           proRataMainWidget.h \
           proRataMainWindow.h \
#           proRataMassSpectrumGraph.h \
           proRataMassSpectrum.h \
           proRataTandemMassSpectrum.h \
           proRataMassSpectrumData.h \
           proRataPCAGraph.h \
           proRataPeptideTable.h \
           proRataPeptideTuple.h \
           proRataPPCGraph.h \
           proRataProteinTable.h \
           proRataProteinTuple.h \
           proRataSearchPane.h \
#           proRataSearchDock.h \
           proRataTable.h \
           proRataTablePane.h \
           proRataTextArea.h \
           proRataTextPane.h \
           proRataXmlProcessor.h \
	   proRataConfigBrowser.h \
	   proRataDTASelect.h \
	   proRataPreProcess.h \
	   proRataPreProcessAbstract.h \ 
	   proRataRaw2MzXMLBrowser.h \
	   proRataWorkingDirBrowser.h \
	   proRataCfgSICExt.h \
	   proRataConfigDialog.h \
	   proRataIsotop.h \
	   delegate.h \
	   proRataMerge.h \
	   proRataQuant.h \
	   proRataExportImage.h \
	   proRataSplashScreen.h \
           titleLabel.h
SOURCES += mainMainWidget.cpp \
           proRataChromatogramGraph.cpp \
           proRataExec.cpp \
           proRataExecDialog.cpp \
           proRataGraph.cpp \
           proRataGraphDialog.cpp \
           proRataGraphPane.cpp \
           proRataImage.cpp \
           proRataImagePane.cpp \
           proRataLikelihoodGraph.cpp \
           fastaData.cpp \
           proRataSequenceCoverageGraph.cpp \
           proRataMainWidget.cpp \
           proRataMainWindow.cpp \
#           proRataMassSpectrumGraph.cpp \
           proRataMassSpectrumData.cpp \
           proRataMassSpectrum.cpp \
           proRataTandemMassSpectrum.cpp \
           proRataPCAGraph.cpp \
           proRataPeptideTable.cpp \
           proRataPPCGraph.cpp \
           proRataProteinTable.cpp \
           proRataSearchPane.cpp \
#           proRataSearchDock.cpp \
           proRataTable.cpp \
           proRataTablePane.cpp \
           proRataTextArea.cpp \
           proRataTextPane.cpp \
           proRataXmlProcessor.cpp \
	   proRataConfigBrowser.cpp \
	   proRataDTASelect.cpp \
	   proRataPreProcess.cpp \
	   proRataPreProcessAbstract.cpp \ 
	   proRataRaw2MzXMLBrowser.cpp \
	   proRataWorkingDirBrowser.cpp \
	   proRataCfgSICExt.cpp \
	   proRataConfigDialog.cpp \
	   proRataIsotop.cpp \
	   delegate.cpp \
	   proRataMerge.cpp \
	   proRataQuant.cpp \
  	   proRataExportImage.cpp \
	   proRataSplashScreen.cpp \
           titleLabel.cpp
