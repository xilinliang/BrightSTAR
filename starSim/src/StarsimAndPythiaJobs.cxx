// Filename: StarsimAndPythiaJobs.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Jan 15 18:47:02 2020 (-0500)
// URL: jlab.org/~latif

// Make sure to change 3 places nummbered below

#include <iostream>
#include "RootInclude.h"
#include "BrightStInclude.h"

using namespace std;

void StarsimAndPythiaJobs(Int_t nJobs, Int_t nEvents)
{
    //-----------0. Change condor config to copy LHAPDF data ----------
    // This job submission requires copying LHAPDF data. Change condor configuration for this
    
    //------- 1.Change Here Function Name ------------
    TString funcName = "FmsSimRunStarsimAndBfc";
    TString jobName = "FmsSim";
    //--------------------------------------
        
    cout << "Total number of jobs to be processed: "<< nJobs <<endl;

    //------------ Loop for jobs --------------------------
    for (Int_t i = 0; i < nJobs; ++i)
    {
	TString argList;
	TString currentJobName;
	
	currentJobName = jobName;
	currentJobName += i;
	
	argList = "(";
	argList += i;
	argList += ",";
	argList += nEvents;
	argList += ")";
	
	//---------------------------------------------------------------------
	
	cout << (funcName + argList)<<endl;

	TStScheduler::SubmitGenericJob(funcName + argList, currentJobName);
	//break;
    }
}
