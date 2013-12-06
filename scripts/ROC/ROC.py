#!/usr/bin/python

import sys, getopt, warnings, os, re
import itertools, copy, math



def parse_options(argv):
    
    opts, args = getopt.getopt(argv[1:], "hi:o:",
                                    ["help",
                                     "input-file",
				                     "output-file"])


    # Default working dir and config file
    sInputFileName   = ""
    sOutputFileName  = ""

    # Basic options
    for option, value in opts:
        if option in ("-h", "--help"):
            print "-i input-file -o output-file"
            sys.exit(0)
        if option in ("-i", "--input-file"):
            sInputFileName  = value
        if option in ("-o", "--output-dir"):
            sOutputFileName = value

    if ( sInputFileName  == ""  )  :
        print "please specify input filename"
        sys.exit(1)

    if ( sOutputFileName == "") :
        sOutputFileName = os.path.splitext(sInputFileName)[0] + ".roc"
    
    return [sInputFileName, sOutputFileName]

## Get file(s) list in working dir with specific file extension
def get_file_list_with_ext(working_dir, file_ext):

    # define sipros file extension 
    file_list = []

    # working directory
    if os.path.exists(working_dir):
        for file_name in os.listdir(working_dir):

            # check the file extension
            if file_name.endswith(file_ext):
                file_path_name = working_dir + file_name
                file_list.append(file_path_name)

       # if len(file_list) == 0:
        #    print >> sys.stderr, "\nCannot open %s file(s)." % (file_ext)
            # die("Program exit!")
	 #   sys.exit(0)
        file_list = sorted(file_list)

    else:
        print >> sys.stderr, "\nCannot open working directory", working_dir
        sys.exit(1)

    return file_list

def ReadScore(sInputFileName) :
    input_file = open(sInputFileName)
    iCaseNum = 0
    bRankOne = False
    bRealHit = False
    dRealHit_Scores_list = []
    dWrongHit_Scores_list= []
    for each_line in input_file :
        each_line = each_line.strip()
        if (each_line == ""):
            continue
        if (each_line.startswith(">")):
            iCaseNum += 1
            bRankOne = True
            if each_line.startswith(">1\t"):
                bRealHit = True
            if (len(each_line.split("\t")) > 2) :
                score_idx = 2
            else :
                score_idx = 1
        elif (bRankOne) :
            dCurrentScore = float(each_line.split("\t")[score_idx])
            bRankOne = False
            if (bRealHit) :
                bRealHit = False
                dRealHit_Scores_list.append(dCurrentScore)
            else :
                dWrongHit_Scores_list.append(dCurrentScore)
   #         print dCurrentScore
    input_file.close()
   # print iCaseNum
    return dRealHit_Scores_list, dWrongHit_Scores_list, iCaseNum

def CalculateSensitivity(dScore_list, dThreshold, iCaseNum):
    iPositive_case_above_threshold = 0
    for each_score in dScore_list:
        if each_score > dThreshold :
            iPositive_case_above_threshold += 1
    current_sensitivity = float(iPositive_case_above_threshold) / float(len(dScore_list))
    return current_sensitivity, iPositive_case_above_threshold

def CalculateFDR(dScore_list, dThreshold, iPositive_case_above_threshold):
    iFalse_case_above_threshold = 0
    for each_score in dScore_list:
        if each_score > dThreshold :
            iFalse_case_above_threshold += 1
    iDiscovered = iFalse_case_above_threshold+iPositive_case_above_threshold
    if (iDiscovered > 0) :
        current_fdr = float(iFalse_case_above_threshold)/float(iDiscovered)
    else :
        current_fdr = 0
    return current_fdr

def CalculateROC(dRealHit_Scores_list, dWrongHit_Scores_list, iCaseNum) :
    curve_point = []
    dAllScore_list = [-100] +  dRealHit_Scores_list + dWrongHit_Scores_list
    dAllScore_list.sort()
    for each_threshold in dAllScore_list :
        current_sensitivity, iPositive_case_above_threshold = CalculateSensitivity(dRealHit_Scores_list, each_threshold, iCaseNum)
        current_fdr = CalculateFDR(dWrongHit_Scores_list, each_threshold, iPositive_case_above_threshold)     
        curve_point.append([current_sensitivity, current_fdr])
    return curve_point


def WriteOutput(curve_point, sOutputFileName) :
    output_file = open(sOutputFileName, "w")
    for each_point in curve_point :
        output_file.write(str(each_point[0])+" "+str(each_point[1])+"\n")

    output_file.close()


def main(argv=None):

    # try to get arguments and error handling
    if argv is None:
        argv = sys.argv
       		 # parse options
        [sInputFileName, sOutputFileName] = parse_options(argv)      
        dRealHit_Scores_list, dWrongHit_Scores_list, iCaseNum = ReadScore(sInputFileName)
        curve_point = CalculateROC(dRealHit_Scores_list, dWrongHit_Scores_list, iCaseNum)
        WriteOutput(curve_point, sOutputFileName)


## If this program runs as standalone, then go to main.
if __name__ == "__main__":
    main()


