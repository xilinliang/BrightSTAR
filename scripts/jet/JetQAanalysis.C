// #include "StRootInclude.h"
// #include "RootInclude.h"
// #include "cppInclude.h"


void JetQAanalysis(
            Int_t nentries,
	    TString jetfile,
	    TString skimfile,
	    TString outfile
    )
{
    cout << "nentries = " << nentries << endl;
    cout << "jetfile  = " << jetfile  << endl;
    cout << "skimfile = " << skimfile << endl;
    cout << "outfile = " << outfile << endl;

    // Load libraries
    // gSystem->Load("StJetEvent");
    // gSystem->Load("StJetSkimEvent");

    // Open jet & skim files
    TChain* jetChain = new TChain("jet");
    TChain* skimChain = new TChain("jetSkimTree");

    Int_t nFiles_jet, nFiles_skim;
    nFiles_jet = jetChain->Add(jetfile);
    nFiles_skim = skimChain->Add(skimfile);

    Int_t nEvents = jetChain->GetEntries();
    if(nentries == -1 || nentries > nEvents)
	nentries = nEvents;

    cout << "Number of files added: "<<nFiles_jet <<endl;
    cout << "Total Entries to be processed: "<< nentries <<endl;
    //Check if added files are sorted properly which is crucial for skimChain vs jetChain synchronization
    jetChain->ls();
    skimChain->ls();
    // Set jet buffer
    StJetEvent* jetEvent = 0;
    jetChain->SetBranchAddress("AntiKtR060NHits12",&jetEvent);

    // Set skim buffer
    StJetSkimEvent* skimEvent = 0;
    skimChain->SetBranchAddress("skimEventBranch",&skimEvent);


    TH1D *h1nJets = new TH1D ("h1nJets", "Number of Jets", 10, -1, 9);
    TH1D *h1JetEta = new TH1D ("h1JetEta", "Jet Eta", 100, -1.5, 5.0);
    TH1D *h1JetPhi = new TH1D ("h1JetPhi", "Jet Phi", 100, -3.1, 3.1);

    
    //----- Event loop ----------
    //Loads entries from Jet Tree and Skim Tree simultaneously
    for (int iEntry = 0; iEntry < nentries; ++iEntry)
    {
	if (jetChain->GetEvent(iEntry) <= 0 || skimChain->GetEvent(iEntry) <= 0) break;

	// Should not be null
	assert(jetEvent && skimEvent);

	// Enforce event synchronization
	assert(jetEvent->runId() == skimEvent->runId() && jetEvent->eventId() == skimEvent->eventId());

	if (iEntry % 1000 == 0) cout << iEntry << endl;

	if(jetEvent->numberOfJets() == 0)
	    continue;

	StJetVertex* vertex = jetEvent->vertex();
	if (!vertex) continue;
	if(vertex->ranking() < 0) continue;
	// if(fabs(vertex->position().z()) > 80.0) continue;
	
	h1nJets->Fill(jetEvent->numberOfJets());
	for(Int_t i = 0; i < jetEvent->numberOfJets(); ++i)
	{
	    h1JetEta->Fill(jetEvent->jet(i)->eta());
	    h1JetPhi->Fill(jetEvent->jet(i)->phi());
	}

	
    } // End event loop

    TCanvas *c1 = new TCanvas();
    h1nJets->Draw();
    TCanvas *c2 = new TCanvas();
    h1JetEta->Draw();
    TCanvas *c3 = new TCanvas();
    h1JetPhi->Draw();
}
