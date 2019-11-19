// Filename: RunFmsAnTreeMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Nov 13 17:25:43 2019 (-0500)
// URL: jlab.org/~latif

#include "StRootInclude.h"
#include "RootInclude.h"
#include "BrightStInclude.h"
#include "BrAnMaker/TStFmsRpFilterMaker.h"
#include "BrAnMaker/TStFmsRpTreeMaker.h"

#include <vector>
#include <iostream>
using namespace std;

void RunFmsRpTreeMaker(TString fileList, TString outFile)
{
    //TStRunList::MakeFileList(16072057, 1);
    //fileList = TStar::Config->GetFileList();
    //fileList = "/star/u/kabir/pwg/data/st_fms_16066050_raw_5000002.MuDst.root";

    TFile *f = new TFile(outFile, "RECREATE");
    //1D *massDist = new TH1D("massDist","#pi^{0} invariant mass [GeV]; M_{#pi^{0}} [GeV]",200,0.0,1.0);
    //2D *xyDist = new TH2D("xyDist", "xyDist", 200, -100.0, 100.0, 200, -100.0, 100.0);
    TTree *tree = new TTree("T", "RP + FMS Tree");
    
    vector<Int_t> trigs(9);
    //------- For FMS stream ----------------
    trigs[0] = TStTrigDef::GetTrigId("FMS-JP0");
    trigs[1] = TStTrigDef::GetTrigId("FMS-JP1");
    trigs[2] = TStTrigDef::GetTrigId("FMS-JP2");
    trigs[3] = TStTrigDef::GetTrigId("FMS-sm-bs1");
    trigs[4] = TStTrigDef::GetTrigId("FMS-sm-bs2");
    trigs[5] = TStTrigDef::GetTrigId("FMS-sm-bs3");
    trigs[6] = TStTrigDef::GetTrigId("FMS-lg-bs1");
    trigs[7] = TStTrigDef::GetTrigId("FMS-lg-bs2");
    trigs[8] = TStTrigDef::GetTrigId("FMS-lg-bs3");

    gMessMgr->SetLimit("I", 0);   //Disable StInfo messages including Skipped event message
    gMessMgr->SetLimit("Q", 0);   //Disable StQAInfo messages (includes event processing status)
    
    StChain *chain = new StChain;
    StMuDstMaker *muDstMaker = new StMuDstMaker(0, 0, "", fileList, "", 1000);
    St_db_Maker* dbMk = new St_db_Maker("db", "MySQL:StarDb", "$STAR/StarDb");
    StSpinDbMaker* spindb = new StSpinDbMaker("spinDb");
    StFmsDbMaker* fmsDb = new StFmsDbMaker("fmsDb");

    //Enable new FMS calibration
    //fmsdb->SetAttr("fmsGainCorr","fmsGainCorr-BNL-C");
    
    muDstMaker->SetStatus("*", 0);
    muDstMaker->SetStatus("Fms*", 1);
    muDstMaker->SetStatus("pp2pp*", 1);
    muDstMaker->SetStatus("MuEvent*", 1);

    //-------------- Filter/Skip Events if no RP or FMS BS/JP Trigger----------
    TStFmsRpFilterMaker* filterMaker = new TStFmsRpFilterMaker("");
    for(Int_t i = 0; i < trigs.size(); ++i)
    	filterMaker->addTrigger(trigs[i]); 
    //----------------

    
    StEventMaker* eventMk = new StEventMaker();
    StFmsHitMaker*   fmsHitMk   = new StFmsHitMaker();
    StFmsPointMaker* fmsPointMk = new StFmsPointMaker();
    // FPS
    StFmsFpsMaker * fpsMk = new StFmsFpsMaker();
    
    TStFmsRpTreeMaker *FmsRpTreeMk = new TStFmsRpTreeMaker("TStFmsRpTreeMaker");
    //FmsRpTreeMk->Set1dHist(massDist);
    // FmsRpTreeMk->Set2dHist(xyDist);
    FmsRpTreeMk->SetTrigIDs(trigs);
    FmsRpTreeMk->SetTree(tree);
    FmsRpTreeMk->SetBeamMomentum(100.0);
    
    // mudst reading
    // if 0, get info from StTriggerData from StTriggerDataMaker/StEvent/MuDst
    // and apply new DB; cluster finding/fitting is redone
    // if 1, cluster finding/fitting is NOT redone (i.e., mudst result is read
    // out); however, some recalculations (like point positions) from updated DB values
    // are done
    fmsPointMk->SetReadMuDst(0); // (0)
    fmsHitMk->SetReadMuDst(0);   // (0)
    fpsMk->setReadMuDST(1);      // (1) // (reverse 0<->1 convention as FmsHitMaker and FmsPointMaker)

    // shower shape
    // 0='use lednev params', 1='use zhanwen params', 2='use yuxi params'
    // fmsPointMk->setShowerShapeWithAngle(1); // (1)
  
    // cluster merging over large/small boundary
    // if 1, merges large and small cell clusters (experimental!; default 1 in StFmsPointMaker)
    fmsPointMk->setMergeSmallToLarge(1); // (1)

    // vertex correction
    // if 0, no vertex correction; if 1, use MuDst BBC vertex based on run11 calibration (needs update?)
    fmsPointMk->setVertexZ(1); // (1)

    Int_t nEvents = muDstMaker->chain()->GetEntries();
    cout << "Total Events to be processed: "<< nEvents <<endl;
    
    chain->Init();    
    chain->EventLoop(nEvents);  // Run specified number of events
    chain->Finish();

    f->Write();
    f->Close();

    delete chain;
}
