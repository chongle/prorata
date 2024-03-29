
#include "projectInfo.h"


ProjectInfo::ProjectInfo()
{
	// constructor
}

ProjectInfo::~ProjectInfo()
{
	int i;
	for(i = 0; i < vpProteomeInfo.size(); ++i )
	{
		delete vpProteomeInfo[i];
	}

	for(i = 0; i < vpProteinCombined.size(); ++i )
	{
		delete vpProteinCombined[i];
	}

}


bool ProjectInfo::process( string sProRataCombineXMLfilename )
{
	
	cout << "start setup templateProteinCombined .. " << endl; // DEBUG	
	if( !setupTemplateProteinCombined( sProRataCombineXMLfilename ) )
	{
		cout << "ERROR: there are some problem with ProRataCombine.xml!" << endl;
		return false;
	}
	
	cout << "start compileLocusList .. " << endl; // DEBUG	
	if( !compileLocusList() )
	{
		cout << "ERROR: cannot creat a list of locuses from the QPR files!" << endl;
		return false;
	}

	cout << "start runQuantification .. " << endl; // DEBUG	
	for( int i = 0; i < vsLocusList.size(); ++i )
	{
		cout << "Running quantification for locus " << vsLocusList[i] << endl;
		// creat new ProteinCombined instances from templateProteinCombined using copy constructor
		ProteinCombined * pCurrentProteinCombined = new ProteinCombined( templateProteinCombined );
		if( !pCurrentProteinCombined->runQuantification( vsLocusList[i], vsDescriptionList[i] ) )
		{
			cout << "WARNING: some problem in quantifying gene " << vsLocusList[i] << endl;	
		}
		vpProteinCombined.push_back( pCurrentProteinCombined );
	}

	LessProteinCombined lessProteinCombinedSort( "locus" );
	sort( vpProteinCombined.begin(), vpProteinCombined.end(), lessProteinCombinedSort );
	
	return true;
}

bool ProjectInfo::writeFileTAB( string sTabFilename )
{
	
	// write the flat-file table for the protein quantification results
	string sCompleteFilename = sTabFilename;
	ofstream fStreamTro( sCompleteFilename.c_str() );
	if( !fStreamTro )
	{
		cout << "ERROR: cannot write the TRO file " << endl;
		return false;
	}

	fStreamTro << "Protein Quantification Results by ProRata " << ProRataConfig::getProRataVersion() <<  endl;
	fStreamTro << endl;
	fStreamTro << "Configurations" << endl;
	fStreamTro << endl;

	vector< ProteinDirectComparison > vCurrentDirectComparison;
	vector< ProteinIndirectComparison > vCurrentIndirectComparison;
	vector< ProteinReplicate > vCurrentReplicate;

	int i;
	int j;
	int k;
	bool bValidityTemp;
	double dLog2RatioTemp;
	double dLowerLimitCITemp;
	double dUpperLimitCITemp;
	int iQuantifiedPeptidesTemp;

	// this is the default string for Not-Available in Excel
	// in R, use na.string = "#N/A" option in read.table()
	string sNAstring = "#N/A";
	
	// print column name 
	string sName;
			fStreamTro << "locus"  << '\t' ;
			vCurrentDirectComparison.clear();
			vCurrentDirectComparison = templateProteinCombined.getProteinDirectComparisonVector();
			vCurrentIndirectComparison.clear();
			vCurrentIndirectComparison = templateProteinCombined.getProteinIndirectComparisonVector();
			for( j = 0; j < vCurrentDirectComparison.size(); ++j )
			{
				vCurrentReplicate.clear();
				vCurrentReplicate = vCurrentDirectComparison[j].getProteinReplicateVector();
				for( k = 0; k < vCurrentReplicate.size(); ++k )
				{
					sName = vCurrentDirectComparison[j].getName() + "_" + vCurrentReplicate[k].getName();
						fStreamTro << sName + "_log2ratio" << '\t'
							<< sName + "_lowerCI" << '\t'
							<< sName + "_upperCI" << '\t' 
							<< sName + "_QuantifiedPeptides" << '\t';  
				}
				sName = vCurrentDirectComparison[j].getName();
				fStreamTro << sName + "_log2ratio" << '\t'
					<< sName + "_lowerCI" << '\t'
					<< sName + "_upperCI" << '\t'
					<< sName + "_QuantifiedPeptides" << '\t';  
				
			}
			for( j = 0; j < vCurrentIndirectComparison.size(); ++j )
			{
				sName = vCurrentIndirectComparison[j].getName();
				fStreamTro << sName + "_log2ratio" << '\t'
					<< sName + "_lowerCI" << '\t'
					<< sName + "_upperCI" << '\t' ;			

			}
			fStreamTro << "description"  << endl;

	
	// print the results
	for( i = 0; i < vpProteinCombined.size(); ++i )
	{
		if(vpProteinCombined[i]->getValidity() )
		{
			fStreamTro << vpProteinCombined[i]->getLocus()  << '\t' ;
			vCurrentDirectComparison.clear();
			vCurrentDirectComparison = vpProteinCombined[i]->getProteinDirectComparisonVector();
			vCurrentIndirectComparison.clear();
			vCurrentIndirectComparison = vpProteinCombined[i]->getProteinIndirectComparisonVector();
			for( j = 0; j < vCurrentDirectComparison.size(); ++j )
			{
				vCurrentReplicate.clear();
				vCurrentReplicate = vCurrentDirectComparison[j].getProteinReplicateVector();
				for( k = 0; k < vCurrentReplicate.size(); ++k )
				{
					vCurrentReplicate[k].getEstimates(bValidityTemp, dLog2RatioTemp, dLowerLimitCITemp, dUpperLimitCITemp);
					iQuantifiedPeptidesTemp = vCurrentReplicate[k].getQuantifiedPeptides();
					if( bValidityTemp )
					{
						fStreamTro << dLog2RatioTemp << '\t'
							<< dLowerLimitCITemp << '\t'
							<< dUpperLimitCITemp << '\t'
							<< iQuantifiedPeptidesTemp << '\t';
					}
					else
					{
						fStreamTro << sNAstring << '\t'
							<< sNAstring << '\t'
							<< sNAstring << '\t'
							<< sNAstring << '\t';
					}
				}
				vCurrentDirectComparison[j].getEstimates(bValidityTemp, dLog2RatioTemp, dLowerLimitCITemp, dUpperLimitCITemp);
				iQuantifiedPeptidesTemp = vCurrentDirectComparison[j].getQuantifiedPeptides();
				if( bValidityTemp )
				{
					fStreamTro << dLog2RatioTemp << '\t'
						<< dLowerLimitCITemp << '\t'
						<< dUpperLimitCITemp << '\t'
						<< iQuantifiedPeptidesTemp << '\t';
				}
				else
				{
					fStreamTro << sNAstring << '\t'
						<< sNAstring << '\t'
						<< sNAstring << '\t'
						<< sNAstring << '\t';
				}				
			}
			for( j = 0; j < vCurrentIndirectComparison.size(); ++j )
			{
				vCurrentIndirectComparison[j].getEstimates(bValidityTemp, dLog2RatioTemp, dLowerLimitCITemp, dUpperLimitCITemp);
				if( bValidityTemp )
				{
					fStreamTro << dLog2RatioTemp << '\t'
						<< dLowerLimitCITemp << '\t'
						<< dUpperLimitCITemp << '\t';
				}
				else
				{
					fStreamTro << sNAstring << '\t'
						<< sNAstring << '\t'
						<< sNAstring << '\t';
				}				

			}
			fStreamTro << vpProteinCombined[i]->getDescription()  << endl;

		}
	}
	fStreamTro.close();
}

bool ProjectInfo::writeFileXML( string sXMLFilename )
{
	cout << "Called to cread XML file " << sXMLFilename << endl;
	cout << "To be implemented! " << endl;
}

bool ProjectInfo::compileLocusList( )
{
	vsLocusList.clear();
	
	vector< string > vsCurrentLocusList;
	vector< string > vsCurrentDescriptionList;

	int i = 0;
	int j = 0;
	int k = 0;
	bool bPresent = false;
	for( i = 0; i < vpProteomeInfo.size(); ++i )
	{
		// retrieve the locus list from a ProteomeInfo
		vsCurrentLocusList.clear();
		vsCurrentDescriptionList.clear();
		vpProteomeInfo[i]->getLocusDescriptionList( vsCurrentLocusList, vsCurrentDescriptionList );
		for( j = 0; j < vsCurrentLocusList.size(); ++j )
		{
			// determine if this locus has been present in the vsLocusList
			// if not, then add it to vsLocusList
			bPresent = false;
			for( k = 0; k < vsLocusList.size(); ++k )
			{
				if( vsLocusList[k] == vsCurrentLocusList[j] )
				{
					bPresent = true;
					break;
				}
			}
			if(!bPresent)
			{
				vsLocusList.push_back( vsCurrentLocusList[j] );
				vsDescriptionList.push_back( vsCurrentDescriptionList[j] );
			}
		}
	}

	// sort vsLocusList and vsDescriptionList
	vector<string> vsLocusListSort = vsLocusList;
	vector<string> vsDescriptionListSort = vsDescriptionList;

	sort( vsLocusList.begin(), vsLocusList.end() );
	for( i = 0; i < vsLocusList.size(); ++i )
	{
		for( j = 0; j < vsLocusListSort.size(); ++j )
		{
			if(vsLocusList[i]==vsLocusListSort[j])
			{
				// overwrite
				vsDescriptionList[i] = vsDescriptionListSort[j];
				break;
			}
		}
	}

}

bool ProjectInfo::setupTemplateProteinCombined( string sProRataCombineXMLfilename )
{
	sCombineXMLfilename = sProRataCombineXMLfilename;
	
	// Creat a TinyXML document 
	TiXmlDocument txdCombineFile;

	// Try loading the file.
	if ( ! ( txdCombineFile.LoadFile( sCombineXMLfilename.c_str() ) ) )
	{
		cout << "ERROR! Loading Configuration file" << endl;
		return false;
	}

	// push back the element name in the hierarchical order
	// the top level goes first and the leaf node goes last
	vector<string> vsTagList;	
	
	string sTemp;
	istringstream issStream;

	

	/*
	 *  setup templateProteinCombined
	 */
	
	string sCurrentFilenameQPR;
	
	TiXmlNode * txnRoot = NULL;

	// a text pointer for retrieving the text inside an element
	TiXmlText * txsText = NULL;
	TiXmlNode * txnSubNode = NULL;		
	
	txnRoot = txdCombineFile.FirstChild( "PRORATA_COMBINE" );
	if ( ! txnRoot )
	{
		cout << "ERROR: cannot find PRORATA_COMBINE node in ProRataCombine.xml!" << endl;
		return false;
	}

	TiXmlNode * txnDirect = txnRoot->FirstChild( "COMBINE_REPLICATES" );
	if ( ! txnDirect )
	{
		cout << "ERROR: cannot find COMBINE_REPLICATES node in ProRataCombine.xml!" << endl;
		return false;
	}
	else
	{
		// Extract the CONFIG elements inside <COMBINE_REPLICATES>
		vsTagList.clear();
		vsTagList.push_back( "PRORATA_COMBINE" );
		vsTagList.push_back( "COMBINE_REPLICATES" );
		vsTagList.push_back( "CONFIG" );
		vsTagList.push_back( "VALID_REPLICATE_CI_NUMBER_CUTOFF" );
		sTemp = getValue( txdCombineFile, vsTagList );
		issStream.clear();
		issStream.str( sTemp );
		int iValidReplicateCI;
		issStream >> iValidReplicateCI;
		ProteinDirectComparison::setValidReplicateCICutoff( iValidReplicateCI );

		vsTagList[3] = "MIN_PEPTIDE_NUMBER";
		sTemp = getValue( txdCombineFile, vsTagList );
		issStream.clear();
		issStream.str( sTemp );
		int iMinPeptideNumber;
		issStream >> iMinPeptideNumber;
		ProteinDirectComparison::setMinimumPeptideNumber( iMinPeptideNumber );
		
		vsTagList[3] = "MAX_CI_WIDTH";
		sTemp = getValue( txdCombineFile, vsTagList );
		issStream.clear();
		issStream.str( sTemp );
		double dMaxCIwidth;
		issStream >> dMaxCIwidth;
		ProteinDirectComparison::setMaxCIwidth( dMaxCIwidth );
		
		vsTagList[3] = "LN_LIKELIHOOD_CUTOFF_OFFSET";
		sTemp = getValue( txdCombineFile, vsTagList );
		issStream.clear();
		issStream.str( sTemp );
		double dLnLikelihoodCutoffOffset;
		issStream >> dLnLikelihoodCutoffOffset;
		ProteinDirectComparison::setLnLikehoodCutOffset( dLnLikelihoodCutoffOffset );
	}
	

	
	// loop thru all DIRECT_COMPARISON elements	
	for( txnDirect = txnDirect->FirstChild( "DIRECT_COMPARISON" ); txnDirect; txnDirect = txnDirect->NextSibling( "DIRECT_COMPARISON" )  )
	{
		ProteinDirectComparison currentDirectComparison;
		
		// set NAME
		txnSubNode = txnDirect->FirstChild( "NAME" );
		if ( ! txnSubNode )
		{
			cout << "ERROR: cannot find NAME node in ProRataCombine.xml!" << endl;
			return false;
		}
		txsText = txnSubNode->FirstChild()->ToText();
		currentDirectComparison.setName( txsText->Value() );

		// set NUMERATOR
		txnSubNode = txnDirect->FirstChild( "NUMERATOR" );
		if ( ! txnSubNode )
		{
			cout << "ERROR: cannot find NUMERATOR node in ProRataCombine.xml!" << endl;
			return false;
		}
		txsText = txnSubNode->FirstChild()->ToText();
		currentDirectComparison.setNumerator( txsText->Value() );

		// set DENOMINATOR
		txnSubNode = txnDirect->FirstChild( "DENOMINATOR" );
		if ( ! txnSubNode )
		{
			cout << "ERROR: cannot find DENOMINATOR node in ProRataCombine.xml!" << endl;
			return false;
		}
		txsText = txnSubNode->FirstChild()->ToText();
		currentDirectComparison.setDenominator( txsText->Value() );
		
		TiXmlNode * txnReplicateNode = NULL;		
		// loop thru all REPLIPCATE elements
		for( txnReplicateNode = txnDirect->FirstChild("REPLIPCATE"); txnReplicateNode; 
				txnReplicateNode = txnReplicateNode->NextSibling("REPLIPCATE") )
		{
			ProteinReplicate currentProteinReplicate;
			
			// set NAME
			txnSubNode = txnReplicateNode->FirstChild( "NAME" );
			if ( ! txnSubNode )
			{
				cout << "ERROR: cannot find NAME node in ProRataCombine.xml!" << endl;
				return false;
			}
			txsText = txnSubNode->FirstChild()->ToText();
			currentProteinReplicate.setName( txsText->Value() );
			
			// get QPR filename
			txnSubNode = txnReplicateNode->FirstChild( "QPR_FILE" );
			if ( ! txnSubNode )
			{
				cout << "ERROR: cannot find QPR_FILE node in ProRataCombine.xml!" << endl;
				return false;
			}
			txsText = txnSubNode->FirstChild()->ToText();
			sCurrentFilenameQPR = txsText->Value();

			cout << "Loading QPR file: " << sCurrentFilenameQPR << endl;
			// load QPR file
			ProteomeInfo * pCurrentProteomeInfo = new ProteomeInfo;
			if( !pCurrentProteomeInfo->readFileQPR( sCurrentFilenameQPR ) )
			{
				cout << "ERROR: cannot load QPR file " << sCurrentFilenameQPR << endl;
				return false;
			}

			vpProteomeInfo.push_back( pCurrentProteomeInfo );
			currentProteinReplicate.setProteomeInfo( pCurrentProteomeInfo );
			
			if( !currentDirectComparison.addReplicate( currentProteinReplicate ) )
			{
				cout << "ERROR: cannot process the replicate from QPR file " << sCurrentFilenameQPR << endl;
				return false;
			}
		}

		if( !templateProteinCombined.addDirectComparison(currentDirectComparison) )
		{
			cout << "ERROR: cannot process the direct comparison " << currentDirectComparison.getName() << endl;
			return false;
		}
	}


	// determine if there is an indirect comparison in this project
	
	TiXmlNode * txnIndirect = txnRoot->FirstChild( "COMBINE_DIRECT_COMPARISONS" );
	if ( txnIndirect )
	{
		// load CONFIG for COMBINE_DIRECT_COMPARISONS
		vsTagList.clear();
		vsTagList.push_back( "PRORATA_COMBINE" );
		vsTagList.push_back( "COMBINE_DIRECT_COMPARISONS" );
		vsTagList.push_back( "CONFIG" );
		vsTagList.push_back( "MAX_CI_WIDTH" );
		sTemp = getValue( txdCombineFile, vsTagList );
		issStream.clear();
		issStream.str( sTemp );
		double dMaxCIwidthIndirect;
		issStream >> dMaxCIwidthIndirect;
		ProteinIndirectComparison::setMaxCIwidth( dMaxCIwidthIndirect );	

		vsTagList[3] = "LN_LIKELIHOOD_CUTOFF_OFFSET";
		sTemp = getValue( txdCombineFile, vsTagList );
		issStream.clear();
		issStream.str( sTemp );
		double dLnLikelihoodCutoffOffsetIndirect;
		issStream >> dLnLikelihoodCutoffOffsetIndirect;
		ProteinIndirectComparison::setLnLikehoodCutOffset( dLnLikelihoodCutoffOffsetIndirect );

		// loop thru all INDIRECT_COMPARISON elements
		for( txnIndirect = txnIndirect->FirstChild( "INDIRECT_COMPARISON" ); txnIndirect; txnIndirect = txnIndirect->NextSibling( "INDIRECT_COMPARISON" )  )
		{
			ProteinIndirectComparison currentIndirectComparison;
			
			// set NAME
			txnSubNode = txnIndirect->FirstChild( "NAME" );
			if ( ! txnSubNode )
			{
				cout << "ERROR: cannot find NAME node in ProRataCombine.xml!" << endl;
				return false;
			}
			txsText = txnSubNode->FirstChild()->ToText();
			currentIndirectComparison.setName( txsText->Value() );

			// set NUMERATOR
			txnSubNode = txnIndirect->FirstChild( "NUMERATOR" );
			if ( ! txnSubNode )
			{
				cout << "ERROR: cannot find NUMERATOR node in ProRataCombine.xml!" << endl;
				return false;
			}
			txsText = txnSubNode->FirstChild()->ToText();
			currentIndirectComparison.setNumerator( txsText->Value() );

			// set DENOMINATOR
			txnSubNode = txnIndirect->FirstChild( "DENOMINATOR" );
			if ( ! txnSubNode )
			{
				cout << "ERROR: cannot find DENOMINATOR node in ProRataCombine.xml!" << endl;
				return false;
			}
			txsText = txnSubNode->FirstChild()->ToText();
			currentIndirectComparison.setDenominator( txsText->Value() );

			// set DIRECT_COMPARISON_NAME_A 
			txnSubNode = txnIndirect->FirstChild( "DIRECT_COMPARISON_NAME_A" );
			if ( ! txnSubNode )
			{
				cout << "ERROR: cannot find DIRECT_COMPARISON_NAME_A node in ProRataCombine.xml!" << endl;
				return false;
			}
			txsText = txnSubNode->FirstChild()->ToText();
			currentIndirectComparison.setDirectComparisonName0( txsText->Value() );

			// set DIRECT_COMPARISON_NAME_B
			txnSubNode = txnIndirect->FirstChild( "DIRECT_COMPARISON_NAME_B" );
			if ( ! txnSubNode )
			{
				cout << "ERROR: cannot find DIRECT_COMPARISON_NAME_B node in ProRataCombine.xml!" << endl;
				return false;
			}
			txsText = txnSubNode->FirstChild()->ToText();
			currentIndirectComparison.setDirectComparisonName1( txsText->Value() );
			
			if( !templateProteinCombined.addIndirectComparison(currentIndirectComparison) )
			{
				cout << "ERROR: cannot add indirect comparison" << endl;
				return false;
			}
		}
	}
	
	// If everything goes fine return 0.
	return true;

}

/////////////////////////////////////////////////////
// this function is copied from proRataConfig.cpp  //
/////////////////////////////////////////////////////

/*
 * An utility method to safely extract information from an XML tag.
 * the text inside an XML element is extracted and pasted together if separated
 * by comments or elements.
 * the element is reached by giving a vector of the element name in the order
 * of their hierarchy. Arbitrary number of level can be reached  
 */

string ProjectInfo::getValue( TiXmlDocument &txdDoc, 
		const vector<string> &vsTagList )
{
	// Check to see if the provided XML node is valid.
	// If yes, extract the value from the node return it.
	// If no, return emply string.

	// creat a pointer to a node
	TiXmlNode * txnTemp = NULL;

	// check if the tree path is not empty
	if ( vsTagList.size() < 1 )
		return string("");
	
	// iterator for the input vsTagList
	vector<string>::const_iterator itrTagListItr;
	itrTagListItr = vsTagList.begin();

	// move the pointer to txddoc's first child with the specified tag name
	txnTemp = txdDoc.FirstChild( (*itrTagListItr ).c_str() );

	// check if this element exists
	if ( ! txnTemp )
	{
		cout << "ERROR: TAG\"" << (*itrTagListItr) << 
			"\" not found in the configuration file." << endl;
		return string("");
	}

	itrTagListItr++;

	// move the pointer down the hierarchial tree of elements
	for( ; itrTagListItr != vsTagList.end(); itrTagListItr++ )
	{

		txnTemp = txnTemp->FirstChild( (*itrTagListItr ).c_str() );

		if ( ! txnTemp )
		{
			cout << "ERROR: TAG\"" << (*itrTagListItr) << 
				"\" not found in the configuration file." << endl;
			return string("");
		}

	}

	// move the iterator back to point it to the last element name
	itrTagListItr--;

	/*
	 * inside the pointed element, there could be a mixture of
	 * text nodes, comment nodes and element nodes
	 * loop thru every nodes and for each text nodes, retrieve their text
	 * concatenate the text together and return them
	 */
	
	TiXmlText *txs;
	string sTemp = "";
	
	// point txnTemp to the child nodes and loop thru every child node
	for( txnTemp = txnTemp->FirstChild(); txnTemp; txnTemp = txnTemp->NextSibling() )
	{
		// if this node is pointing to a node of type TEXT, which equals 4 in enum NodeType
		if( txnTemp->Type() == 4 )
		{
			// cast txnTemp to a text node
			txs = txnTemp->ToText();
			// get txnTemp's value and then append it to sTemp
			if( txs )
				sTemp.append( txs->Value() );
		}
	}

	return sTemp;
}

