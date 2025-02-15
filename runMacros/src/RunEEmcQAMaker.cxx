// Filename: RunEEmcQAMaker.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Jun 23 21:23:37 2019 (-0400)
// URL: jlab.org/~latif

#include "StRootInclude.h"
#include "BrEEmcMaker/TStEEmcQAMaker.h"
#include "TChain.h"

void RunEEmcQAMaker(TString InputFileList, TString outputfile, Int_t nEntries)
{
    if(InputFileList == "")
	InputFileList = "root://xrdstar.rcf.bnl.gov:1095//home/starlib/home/starreco/reco/production_pp200trans_2015/ReversedFullField/P16id/2015/090/16090026/st_physics_16090026_raw_5500035.MuDst.root";	

    StChain* chain = new StChain;
    StMuDstMaker* muDstMaker = new StMuDstMaker(0, 0, "", InputFileList, ".", 1000, "MuDst");
    StMuDbReader* muDB = StMuDbReader::instance();

    St_db_Maker* dbMaker = new St_db_Maker("StarDb", "MySQL:StarDb");     // STAR database
    StEEmcDbMaker* eemcDb = new StEEmcDbMaker;     // EEMC database

    // Barrel ADC to energy maker
    //StEmcADCtoEMaker* adc2e = new StEmcADCtoEMaker;
    //adc2e->saveAllStEvent(true);

    TStEEmcQAMaker* EEmcDistMaker = new TStEEmcQAMaker(muDstMaker);
    EEmcDistMaker->setOutFile(outputfile);
    TChain *ch = muDstMaker->chain();
    
    chain->Init();
    if(nEntries == -1)
	nEntries = ch->GetEntries();
      
    // Event loop
    for (int iEvent = 1; iEvent <= nEntries; ++iEvent)
    {
	chain->Clear();
	int status = chain->Make(iEvent);
	if(iEvent%100 == 0)
	    cout << "Events processed:"<<iEvent<<endl;
    }
}
