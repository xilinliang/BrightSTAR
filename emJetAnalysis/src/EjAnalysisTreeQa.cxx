// Filename: EjAnalysisTreeQa.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Thu Jun 11 13:08:16 2020 (-0400)
// URL: jlab.org/~latif

#include "RootInclude.h"
#include "StRootInclude.h"
#include "BrJetMaker/TStJetCandidate.h"
#include "BrJetMaker/TStJetEvent.h"
#include "BrJetMaker/TStJetSkimEvent.h"
#include <iostream>

using namespace std;

//Use wildcat in the file name to combine many files.
void EjAnalysisTreeQa(TString inFileName, TString outName, TString det)
{
    TChain *ch = new TChain("T");
    ch->Add(inFileName);
    
    TStJetEvent *jetEvent = 0;
    TStJetSkimEvent *skimEvent;
    TStJetCandidate *jet;
    TStJetTower *tower;
    TStJetParticle *particle;
    
    ch->SetBranchAddress("jetEvents", &jetEvent);
    
    TFile *outFile = new TFile(outName, "recreate");
    TH1D *h1nJets_all = new TH1D("h1nJets_all", "Number of Jets from All [TPC/BEMC + EEMC + FMS]", 10, 0, 10);
    TH1D *h1nJets  = new TH1D("h1nJets ", "Number of Jets [FMS or EEMC]; Number of EM-jets per event", 10, 0, 10);
    TH1D *h1spinB = new TH1D("h1spinB", "Blue beam spin; Blue beam spin", 5, -2, 2);
    TH1D *h1spinY = new TH1D("h1spinY", "Yellow beam spin; Yellow beam spin", 5, -2, 2);
    TH1D *h1UnixTime = new TH1D("h1UnixTime", "Event Unix Time; Event Unix Time", 1000000, 1426e6, 1427e6);
    TH1D *h1TrigType = new TH1D("h1TrigType", "Trigger Type; Trigger Type", 10, 0, 10);
    
    TH1D *h1TowerEta = new TH1D("h1TowerEta", "Tower Eta", 200, 0.0, 4.5);
    TH1D *h1TowerPhi = new TH1D("h1TowerPhi", "Tower Phi", 200, -3.2, 3.2);
    TH1D *h1TowerE = new TH1D("h1TowerE", "Tower E", 1000, 0, 100);

    TH1D *h1Eta = new TH1D ("h1Eta", "EM Jet Eta; Jet #eta", 200, 1.0, 5.0);
    TH1D *h1Phi = new TH1D ("h1Phi", "EM Jet Phi; Jet #phi [rad]", 200, -3.3, 3.3);
    TH1D *h1E = new TH1D ("h1E", "EM Jet E; Jet E [GeV]", 200, 0.0, 200.0);
    TH1D *h1JetE_s = new TH1D ("h1JetE_s", "EM Jet E [small cells]; Jet E [GeV]", 200, 0.0, 200.0);
    TH1D *h1JetE_l = new TH1D ("h1JetE_l", "EM Jet E [large cells]; Jet E [GeV]", 200, 0.0, 200.0);    
    TH1D *h1Pt = new TH1D ("h1Pt", "Jet Pt; Jet Pt [GeV/c]", 200, 0.0, 20.0);
    TH1D *h1PtUE = new TH1D ("h1PtUE", "UE corrected Jet Pt; UE corrected Jet Pt [GeV/c]", 200, 0.0, 20.0);
    TH1D *h1dPt = new TH1D ("h1dPt", "UE dPt; UE dPt [GeV/c]", 100, 0.0, 2.0);
    TH1D *h1nPhotonsTow = new TH1D("h1nPhotonsTow", "number of photons in EM jets from tower counts; Number of Photons", 20, 0, 20);
    TH1D *h1nPhotons = new TH1D("h1nPhotons", "number of photons in EM jets from mJets.mNphotons; Number of Photons", 20, 0, 20);
    TH1D *h1vtxZ = new TH1D("h1vtxZ", "Jet vetrex z; Jet vertex z [cm]", 200, -200, 200);

    TH1D *h1EbyPhNo[5];
    for(Int_t i = 0; i < 5; ++i)
	h1EbyPhNo[i] = new TH1D (Form("h1EbyPhNo%i",i), "EM Jet E By Photon Number; Jet E [GeV]", 200, 0.0, 100.0);
    TH1D *h1nJetsPerRun = new TH1D("h1nJetsPerRun", "Number of Jets per Run", 5, 0, 5);

    
    TH2D *h2EvsPt = new TH2D("h2EvsPt", "Eng vs Pt; Pt [GeV/C]; E [GeV]", 100, 0, 20, 100, 0, 100);
    TH2D *h2PtvsE = new TH2D("h2PtvsE", "Pt vs E; E [GeV]; Pt [GeV/c]", 100,  0, 100, 100, 0, 20);
    TH2D *h2nPhVsEng = new TH2D("h2nPhVsEng", "Number of photons vs Eng; E [Gev]; No. of Photons", 100, 0, 100, 20, 0, 20);
    TH2D *h2xy_fms = new TH2D ("h2xy_fms", "Jet Position [FMS]; Jet X [cm]; Jet Y [cm]", 100, -100, 100, 100, -100, 100);
    TH2D *h2xy_eemc = new TH2D ("h2xy_eemc", "Jet Position [EEMC]; Jet X [cm]; Jet Y [cm]", 100, -250, 250, 100, -250, 250);
    TH2D *h2EtaPhi_fms = new TH2D ("h2EtaPhi_fms", "Jet Eta Phi [FMS]; Jet #eta; Jet #phi [rad]", 100, 2.5, 4.5, 100, -3.5, 3.5);
    TH2D *h2EtaPhi_eemc = new TH2D ("h2EtaPhi_eemc", "Jet Eta Phi [EEMC]; Jet #eta; Jet #phi [rad]", 100, 0.8, 2.5, 100, -3.5, 3.5);
    TH2D *h2towEngVsEta = new TH2D("h2towEngVsEta", "Tower Energy vs Eta", 200, 0.0, 5.0, 1000, 0.0, 70);

    Double_t etaMin;
    Double_t etaMax;
    Double_t detZ; //For FMS
    if(det == "fms")
    {
	etaMin = 2.0;
	etaMax = 4.5;
	detZ = 735.; //For FMS

	h1TrigType->GetXaxis()->SetBinLabel(1,"FMS JP0");
	h1TrigType->GetXaxis()->SetBinLabel(2,"FMS JP1");
	h1TrigType->GetXaxis()->SetBinLabel(3,"FMS JP2");
	h1TrigType->GetXaxis()->SetBinLabel(4,"Small BS1");
	h1TrigType->GetXaxis()->SetBinLabel(5,"Small BS2");
	h1TrigType->GetXaxis()->SetBinLabel(6,"Small BS3");
	h1TrigType->GetXaxis()->SetBinLabel(7,"Large BS1");
	h1TrigType->GetXaxis()->SetBinLabel(8,"Large BS2");
	h1TrigType->GetXaxis()->SetBinLabel(9,"Large BS3");	
    }
    else if(det == "eemc")
    {
	etaMin = 1.0;
	etaMax = 2.0;
	detZ = kEEmcZSMD; //For EEMC

	h1TrigType->GetXaxis()->SetBinLabel(1,"EHT0");
	h1TrigType->GetXaxis()->SetBinLabel(2,"JP1");
	h1TrigType->GetXaxis()->SetBinLabel(3,"JP2");
	h1TrigType->GetXaxis()->SetBinLabel(4,"EHT0*EJP1*L2Egamma");
	h1TrigType->GetXaxis()->SetBinLabel(5,"JP2*L2JetHigh");
	h1TrigType->GetXaxis()->SetBinLabel(6,"BHT1*VPDMB-30");
	h1TrigType->GetXaxis()->SetBinLabel(7,"BHT0*BBCMB");
	h1TrigType->GetXaxis()->SetBinLabel(8,"BHT1*BBCMB");
	h1TrigType->GetXaxis()->SetBinLabel(9,"BHT2*BBCMB");		
    }
    else
    {
	cout << "Invalid detector" <<endl;
	return;
    }
    
    
    Double_t jetX, jetY, eta, phi, theta, vtxZ, eng, pt;
    Int_t nJets = 0;
    Int_t totJets = 0;
    
    cout << "Total Entries to be processed: "<< ch->GetEntries() <<endl;

    for(Int_t evt = 0; evt < ch->GetEntries(); ++evt)
    {
	ch->GetEntry(evt);

	if(evt % 1000 == 0)
	    cout << "Events Processed: "<< evt <<endl;
	
	skimEvent = jetEvent->GetEvent();
	
	h1UnixTime->Fill(skimEvent->GetUnixTime());
	h1spinB->Fill(skimEvent->GetSpinB());
	h1spinY->Fill(skimEvent->GetSpinY());
	h1nJets_all->Fill(jetEvent->GetNumberOfJets());
	vtxZ = skimEvent->GetVertexZ();
	
	//Exclude FMS small-bs3 trigger that gives ring of fire issue. But this removes most of high energetic jets.
	// if(det == "fms")
	// {
	//     if(skimEvent->GetTrigFlag(5))
	// 	continue;
	// }
	//Alternative way to reduce ring of fire, require: BBCMult > 2 and TofMult > 2
	
	nJets = 0;	
	for(Int_t j = 0; j <  jetEvent->GetNumberOfJets(); ++j)
	{
	    jet = jetEvent->GetJet(j);
	   	    
	    eta = jet->GetEta();
	    phi = jet->GetPhi();
	    theta =  2 * atan( exp(-eta) );

	    if(eta < etaMin || eta > etaMax) //Conside only EEMC and FMS coverage
		continue;

	    ++nJets;
	    eng = jet->GetE();
	    pt = jet->GetPt();
	    
	    h1nPhotonsTow->Fill(jet->GetNumberOfTowers());
	    h1nPhotons->Fill(jet->GetNphotons());
	    h1Eta->Fill(eta);
	    h1Phi->Fill(phi);
	    h1Pt->Fill(pt);
	    h1dPt->Fill(jet->GetUedPt());
	    h1PtUE->Fill(pt - jet->GetUedPt());
	    h1E->Fill(eng);
	    
	    h2EvsPt->Fill(pt, eng);
	    h2PtvsE->Fill(eng, pt);
	    h2nPhVsEng->Fill(eng, jet->GetNumberOfTowers());

	    jetX = (detZ - vtxZ) * tan(theta) * cos(phi);
	    jetY = (detZ - vtxZ) * tan(theta) * sin(phi);
	    
	    if(det == "fms")  //actual range 2.65 - 3.9
	    {	
		h2xy_fms->Fill(jetX, jetY);
		h2EtaPhi_fms->Fill(eta, phi);

		if(eta > 2.0 && eta < 3.15) //FMS large cells
		    h1JetE_l->Fill(eng); 
		if(eta > 3.15 && eta < 4.0)  //FMS small cells
		    h1JetE_s->Fill(eng);		
	    }
	    
	    if(det == "eemc")   // actual range 1.086 - 2.0
	    {
		h2xy_eemc->Fill(jetX, jetY);
		h2EtaPhi_eemc->Fill(eta, phi);
	    }
	    
	    for(Int_t k = 0; k < jet->GetNumberOfTowers(); ++k)
	    {
		tower = jet->GetTower(k);
	
		h1TowerEta->Fill(tower->GetEta());
		h1TowerPhi->Fill(tower->GetPhi());
		h1TowerE->Fill(tower->GetEnergy());

		h2towEngVsEta->Fill(tower->GetEta(), tower->GetEnergy());
	    }

	    //Particle Branch is for simulated data only
	    // for(Int_t k = 0; k < jet->GetNumberOfParticles(); ++k)
	    // {
	    // 	particle = jet->GetParticle(k);
	
	    // }

	    if(jet->GetNphotons() >= 0 && jet->GetNphotons() < 5)
		h1EbyPhNo[jet->GetNphotons()]->Fill(eng);
	}
	h1nJets->Fill(nJets);
	totJets += nJets;
	
	if(nJets > 0)
	{
	    for(Int_t t = 0; t < 9; ++t)
	    {
		if(skimEvent->GetTrigFlag(t))
		    h1TrigType->Fill(t);
	    }	    
	    h1vtxZ->Fill(vtxZ);
	}
    }

    h1nJetsPerRun->SetBinContent(1, ch->GetEntries());
    h1nJetsPerRun->SetBinContent(2, totJets);
    
    outFile->Write();
    outFile->Close();
    delete ch;
}
