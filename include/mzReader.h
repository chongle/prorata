#ifndef MZREADER_H
#define MZREADER_H

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <deque>
#include <stdio.h>
#include <stdlib.h>
#include "ramp.h"
#include "base64.h"

using namespace std;

// the number of MassSpectrum objects that should be 
// saved in the deqMassSpecBuffer
#define BUFFER_SIZE 200

/*
 * a class for holding mass spectrum data
 * this is only used in mzReader for read buffering
 */
class MassSpectrum
{
	public:
		unsigned long int iScan;
		vector<float> vfMass;
		vector<float> vfInten;
};

/*
 * a reader for mzXML files and mzData files
 * created based on readmzXML.c
 * it is essentially a C++ wrapper for RAMP
 * To compile with Microsoft Visual C++, add wsock32.lib
 * into object/library modules ( Project-> Settings -> Link tab )
 */

class mzReader
{
	public:
		mzReader();
		
		// close the file if it is opened
		~mzReader();

		/*
		 * set the filename szXMLFile
		 * Read the file
		 * Get the Index Offset
		 * Read the index
		 * Return false if the file doesn't exist 
		 * or has a incorrect extension name
		 */ 
		bool setFilename( string sFilename );

		// get the first scan number
		unsigned long int getAnalysisFirstScan()
		{	return iAnalysisFirstScan; 	}

		// get the last scan number
		unsigned long int getAnalysisLastScan()
		{	return iAnalysisLastScan; 	}

		/*
		 * get the header information for a scan
		 * precursor m/z is set to be 0 for the full scans
		 */
		bool getHeaderInfo(unsigned long int iScan, 
				int * piMSLevel, double * pdPrecursorMZ, 
				int * piPeaksCount, double * pdRetentionTime);

		/*
		 * get the peak list for a scan
		 * getHeaderInfo should be called prior to getPeak to retrieve iPeaksCount
		 * vfMass: m/z column; vfInten: intensity column
		 */
		bool getPeaks( unsigned long int iScan, int iPeaksCount, 
				vector<float> & vfMass, vector<float> & vfInten );

		/*
		 * the read-buffered version of getPeaks
		 * the inquired MS scan will be first searched in the deqMassSpecBuffer
		 * if found, then it will be returned. this saves the time for calling RAMP functions and accessing the file
		 * if not found, the getPeaks() function will be called and this MS scan will be push into
		 * deqMassSpecBuffer and the the oldest MS scan in deqMassSpecBuffer will be poped out
		 */
		
		bool getPeaksBuffered( unsigned long int iScan, int iPeaksCount, 
				vector<float> & vfMass, vector<float> & vfInten );


	private:

		/* 
		 * Mass spec data reading Buffer
		 * the deque is used for saving the new MassSpectrum with push_back
		 * and deleting the oldest MassSpectrum with pop_front
		 */
		deque< MassSpectrum > deqMassSpecBuffer;
		deque< MassSpectrum >::iterator iterBuffer;
		
		// input filename
		string szXMLFile;

		/*
		 * internal variables used by RAMP
		 */

		RAMPFILE  *pFI;

		off_t  indexOffset;
/*
 * this is for Visual C++ compilation 
#if _WIN32
		__int64  *pScanIndex;
#else
		off_t  *pScanIndex;
#endif
*/
		// this is for Linux and MinGW compilation
		off_t  *pScanIndex;
		/*
		 *  a structure used to store the header information
		 *  it is define in RAMP.h
		 */ 
		ScanHeaderStruct scanHeader;

		int iAnalysisFirstScan;
		int iAnalysisLastScan;
};

#endif //MZREADER_H

