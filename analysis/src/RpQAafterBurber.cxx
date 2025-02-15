// Filename: RpQAafterBurner.cxx
// Description: Look at various RP variable distributions.
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon May 20 15:04:13 2019 (-0400)
// URL: jlab.org/~latif

/*

Afterburner works only if inplemented in the maker class. Check Maker class implementation example.

*/


#include "RootInclude.h"
#include "TStRunList.h"
#include "TStar.h"
#include "StRootInclude.h"

// RP afterburner
#include "StMuRpsUtil/StMuRpsUtil.h"
#include "StMuRpsUtil/StMuRpsCollection2.h"

void RpQAafterBurner(Int_t runNo, Int_t maxFiles)
{
    TStopwatch st;
    st.Start();
    Int_t nFiles = TStRunList::MakeFileList(runNo, maxFiles);
    TString fileList = TStar::Config->GetFileList();
    TStar::ExitIfInvalid(fileList);
    
    const Int_t nPlots = 13;
    TH1F *h1[nPlots];

    TString fileName = "RpQAafterBurner.root";
    TFile *file = new TFile(fileName, "recreate", "RP Track variable distributions from run" + (TString) runNo);
    h1[0] = new TH1F("h1nPlanes", "number of planes; number of planes", 100, 0.0, 0.0);
    h1[1] = new TH1F("h1nRP", "number of RP; number of RP", 100, 0.0, 0.0);
    h1[2] = new TH1F("h1BrID", "branch ID; branch ID", 100, 0.0, 0.0);
    h1[3] = new TH1F("h1nTracks", "Number of Tracks per event [ with nTracks > 0 ]; Number of Tracks", 500, 0.0, 500.0);
    h1[4] = new TH1F("h1Theta", "#theta; #theta [mRad]", 200, 0.0, 0.0);
    h1[5] = new TH1F("h1Theta_x", "#theta_{x}; #theta_{x} [mRad]", 200, 0.0, 0.0);
    h1[6] = new TH1F("h1Theta_y", "#theta_{y}; #theta_{y} [mRad]", 200, 0.0, 0.0);
    h1[7] = new TH1F("h1Eta", "#eta; #eta", 100, 0.0, 0.0);
    h1[8] = new TH1F("h1Pt", "Pt [GeV/c]; Pt [GeV/c]", 100, 0.0, 0.0);
    h1[9] = new TH1F("h1p", "p [GeV/c]; p [GeV/c]", 100, 0.0, 200.0);
    h1[10] = new TH1F("h1xi", "#xi; #xi", 100, 0.0, 0.0);
    h1[11] = new TH1F("h1tNeg", "-t [(GeV/c)^{2}]; -t [(GeV/c)^{2}]", 100, 0.0, 0.0);
    h1[12] = new TH1F("h1Phi", "#phi [mRad]; #phi [mRad]", 100, 0.0, 0.0);
        
    //-------------------------------------------------------    
    StChain *chain = new StChain;
    StMuDstMaker *muDstMaker = new StMuDstMaker(0, 0, "", fileList, "", nFiles);
    muDstMaker->SetStatus("*",0);
    muDstMaker->SetStatus("pp2pp*",1);
    
    StMuDst *mDst = muDstMaker->muDst();   // Get StMuDst
    //-----
    StMuRpsUtil* mAfterburner = new StMuRpsUtil(muDstMaker); // RP afterburner
    //-----
    //StMuRpsCollection *rpsMuColl; //no afterburner
    StMuRpsCollection2 *rpsMuColl;   //2 for afterburner
    StMuRpsTrackPoint  *rpsTrkPoint;
    StMuRpsTrack *rpsTrk;
    
    TChain *ch = muDstMaker->chain();
    int iEvent = 0;
    chain->Init();
    
    int nEvents = ch->GetEntries();
    cout << "Total events to be processed: "<< nEvents <<endl;
    for (int iev = 0; iev < nEvents; iev++)
    {
	chain->Clear();
	int iret = chain->Make(iev);

	//rpsMuColl = mDst->RpsCollection(); // Without afterburner
	
	//------ Using afterburner ----
	mAfterburner->updateVertex(0.000415, 0.000455, 0.0); // specific to run 15 pp200 trans !!!
	rpsMuColl = mAfterburner->process(); // executes afterburner 
	//------
		
	if(!rpsMuColl)
	{
	    cout<<"No RP data for this event"<<endl;
	    continue;
	}
	h1[0]->Fill(rpsMuColl->numberOfPlanes());
	h1[1]->Fill(rpsMuColl->numberOfRomanPots());
	if(rpsMuColl->numberOfTracks() > 0)
	{
	    h1[3]->Fill(rpsMuColl->numberOfTracks());
	    for(Int_t i = 0; i < rpsMuColl->numberOfTracks(); ++i)
	    {
		h1[2]->Fill(rpsMuColl->track(i)->branch());	
		h1[4]->Fill(1000.0*rpsMuColl->track(i)->theta());	
		h1[5]->Fill(1000.0*rpsMuColl->track(i)->theta(0));	
		h1[6]->Fill(1000.0*rpsMuColl->track(i)->theta(1));	
		h1[7]->Fill(rpsMuColl->track(i)->eta());
		h1[8]->Fill(rpsMuColl->track(i)->pt());		
		h1[9]->Fill(rpsMuColl->track(i)->p());		
		h1[10]->Fill(rpsMuColl->track(i)->xi(100.0)); // Beam momentum is approximate		
		h1[11]->Fill(-1.0*rpsMuColl->track(i)->t(100.0));		
		h1[12]->Fill(rpsMuColl->track(i)->phi());		
	    }
	}
	//--- afterburner -----
	mAfterburner->clear(); // Critical!!!
	if(!(iev%1000))
	    cout<<"Events processed:" << iev <<endl;
    }
    chain->Finish();

    TCanvas *c1[nPlots];
    for(Int_t i = 0; i < nPlots; ++i)
	c1[i] = new TCanvas();

    for(Int_t i = 0; i < nPlots; ++i)
    {
	c1[i]->cd();
	h1[i]->Draw();
    }
    
    file->cd();
    for(Int_t i = 0; i < nPlots; ++i)
    {
	h1[i]->Write();
	c1[i]->Write();
    }
    
    file->Close();
    st.Stop();
    st.Print();
}			
