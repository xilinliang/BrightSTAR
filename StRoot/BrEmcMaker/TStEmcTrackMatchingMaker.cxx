// Filename: TStEmcTrackMatchingMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:37:54 2019 (-0400)
// URL: jlab.org/~latif


#include "TStEmcTrackMatchingMaker.h"
#include "StEvent/StEvent.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StEvent/StEmcCollection.h"
#include "StEmcUtil/geometry/StEmcGeom.h"
#include "StEmcUtil/projection/StEmcPosition.h"
#include "StEvent/StEventTypes.h"
#include "TMath.h"

ClassImp(TStEmcTrackMatchingMaker)

//_____________________________________________________________________________ 
TStEmcTrackMatchingMaker::TStEmcTrackMatchingMaker(const char *name):StMaker(name)
{

}

//_____________________________________________________________________________ 
TStEmcTrackMatchingMaker::~TStEmcTrackMatchingMaker()
{
    //delete mTraits;
}


//_____________________________________________________________________________ 
Int_t TStEmcTrackMatchingMaker::Init()
{
    mTraits = new TStTpcTofPidTraits;     
    //
    return kStOK;
}


//_____________________________________________________________________________
Int_t TStEmcTrackMatchingMaker::Make()
{
    mMuDst = (StMuDst*)GetInputDS("MuDst");
    //mEvent = (StEvent*)GetInputDS("StEvent");
    if(!mMuDst)
    {
	cout << "TStEmcTrackMatchingMaker::Make - No StMuDst event!" <<endl;
	return kStFatal;
    }
    ResetPidTraits();    
    //StEvent *stEvent = mMuDst->createStEvent();
    //stEvent->statistics();
    //mEvent->statistics();

    //MatchToTracks(stEvent);
    //delete stEvent;  //Must delete to avoid nasty memory leak
    //stEvent = 0;
    MatchToTracks();
    return kStOK;
}

Int_t TStEmcTrackMatchingMaker::MatchToTracks(StEvent *event)
{
    if(!event)
        return 0;
    mEmcCollection = mMuDst->emcCollection();
    if(!mEmcCollection)
    {
	cout<<"No EMC data for this event"<<endl;
	return kStSkip;
    }
    
    StSPtrVecEmcPoint &mEmcPoints = mEmcCollection->barrelPoints();
    
    //cout << "------> No of points for matching: "<< mEmcPoints.size() <<endl;

    StEmcPosition emcPosition;
    Float_t field = 0.5;
    StEventSummary *evtSummary = event->summary();
    if (evtSummary)
        field = evtSummary->magneticField()/10;

    Int_t nR = mEmcPoints.size();
    StEmcGeom* geom = StEmcGeom::instance("bemc");
    if(nR>0)
    {
        LOG_DEBUG << "Matching to tracks... NP = " << nR << endm;
        StSPtrVecTrackNode& tracks = event->trackNodes();
        Int_t nTracks =  tracks.size();
        StThreeVectorD momentum,position;
        for(Int_t t=0;t<nTracks;t++)
        {
            StTrack *track = tracks[t]->track(0);
            if(track)
            {
                if(track->geometry())
                {
                    Bool_t tok = emcPosition.trackOnEmc(&position,&momentum,
                                                       track,(Double_t)field,
                                                       (Double_t)geom->Radius());
                    if(tok)
                    {
                        Float_t eta = position.pseudoRapidity();
                        Float_t phi = position.phi();
                        if(fabs(eta)<1)
                        {
                            StEmcPoint *cl;

                            for(Int_t i=0; i<nR; i++)
                            {
                                cl = mEmcPoints[i];
                                if(cl)
                                {
                                    StThreeVectorF pos = cl->position();
                                    Float_t etaP = pos.pseudoRapidity();
                                    Float_t phiP = pos.phi();
                                    Float_t D = sqrt(cl->deltaEta()*cl->deltaEta()+cl->deltaPhi()*cl->deltaPhi());
                                    Float_t d = sqrt((eta-etaP)*(eta-etaP)+(phi-phiP)*(phi-phiP));
                                    if(d<D)
                                    {
                                        cl->setDeltaEta(eta-etaP);
                                        cl->setDeltaPhi(phi-phiP);
                                    }

                                    StThreeVectorF err = cl->positionError();
                                    Float_t etaE = err.pseudoRapidity();
                                    Float_t phiE = err.phi();

                                    Float_t dPhi = fabs(phi-phiP);
                                    if (dPhi>TMath::Pi())
                                        dPhi=2.0*TMath::Pi()-dPhi;

                                    if(fabs(eta-etaP)<fabs(etaE) && dPhi<fabs(phiE))
                                    {
                                        Int_t Category = cl->quality();
                                        Category = Category | 16;
                                        cl->setQuality(Category);
                                        cl->addTrack(track);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    return 0;
}    


Int_t TStEmcTrackMatchingMaker::MatchToTracks()
{
    mEmcCollection = mMuDst->emcCollection();
    if(!mEmcCollection)
    {
	cout<<"No EMC data for this event"<<endl;
	return kStSkip;
    }
    
    StSPtrVecEmcPoint &mEmcPoints = mEmcCollection->barrelPoints();
    
    //cout << "------> No of points for matching: "<< mEmcPoints.size() <<endl;
    StMuTrack *muTrack;
    StEmcPosition emcPosition;
    Float_t field = 0.5;
    StEventSummary evtSummary = mMuDst->event()->eventSummary();
    if (fabs(evtSummary.magneticField()) < 100)
        field = evtSummary.magneticField()/10;

    Int_t nR = mEmcPoints.size();
    StEmcGeom* geom = StEmcGeom::instance("bemc");
    if(nR>0)
    {
        LOG_DEBUG << "Matching to PRIMARY tracks... NP = " << nR << endm;
        
        Int_t nTracks =  mMuDst->numberOfPrimaryTracks();
        StThreeVectorD momentum,position;
        for(Int_t t=0;t<nTracks;t++)
        {
	    muTrack = mMuDst->primaryTracks(t);
            StTrack *track = mMuDst->createStTrack(muTrack);
            if(track)
            {
                if(track->geometry())
                {
                    Bool_t tok = emcPosition.trackOnEmc(&position,&momentum,
                                                       track,(Double_t)field,
                                                       (Double_t)geom->Radius());
                    if(tok)
                    {
                        Float_t eta = position.pseudoRapidity();
                        Float_t phi = position.phi();
                        if(fabs(eta)<1)
                        {
                            StEmcPoint *cl;

                            for(Int_t i=0; i<nR; i++)
                            {
				Int_t nMatchedTracks = 0;
                                cl = mEmcPoints[i];
                                if(cl)
                                {
                                    StThreeVectorF pos = cl->position();
                                    Float_t etaP = pos.pseudoRapidity();
                                    Float_t phiP = pos.phi();
                                    Float_t D = sqrt(cl->deltaEta()*cl->deltaEta()+cl->deltaPhi()*cl->deltaPhi());

                                    Float_t d = sqrt((eta-etaP)*(eta-etaP)+(phi-phiP)*(phi-phiP));
                                    if(d<D)
                                    {
                                        cl->setDeltaEta(eta-etaP);
                                        cl->setDeltaPhi(phi-phiP);
                                    }

                                    StThreeVectorF err = cl->positionError();
                                    Float_t etaE = err.pseudoRapidity();
                                    Float_t phiE = err.phi();

                                    Float_t dPhi = fabs(phi-phiP);
                                    if (dPhi>TMath::Pi())
                                        dPhi=2.0*TMath::Pi()-dPhi;

                                    if(fabs(eta-etaP)<fabs(etaE) && dPhi<fabs(phiE))
                                    {
                                        Int_t Category = cl->quality();
                                        Category = Category | 16;
                                        cl->setQuality(Category);
                                        cl->addTrack(track);

					//--------- Save PID trait information ----------------------
					mTraits->q[i][nMatchedTracks] = muTrack->charge();
					mTraits->p[i][nMatchedTracks] = muTrack->p().mag();
					mTraits->pt[i][nMatchedTracks] = muTrack->pt();
					mTraits->dca[i][nMatchedTracks] = muTrack->dca().z();
					mTraits->beta[i][nMatchedTracks] = muTrack->btofPidTraits().beta(); //<------- This should be revisited
					++nMatchedTracks;

					//--------------- Energy from each detector --------------
					//Ideally we would alsolike to save energy from Tower, preshower, SMDs
					//Currently Preshower energy is not set in the reconstruction which is the most relevent part here
					// cout <<"Total energy: "<< cl->energy()<<endl; //Total energy is the same as the tower energy
					// cout <<"Energy in EMC Tower: "<< cl->energyInDetector(kBarrelEmcTowerId)<<endl;
					// cout <<"Pre-shower: "<< cl->energyInDetector(kBarrelEmcPreShowerId)<<endl;
					// cout <<"SMD phi: "<< cl->energyInDetector(kBarrelSmdPhiStripId) <<endl;
					// cout <<"SMD eta: "<< cl->energyInDetector(kBarrelSmdEtaStripId) <<endl;					
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    return 0;
}    


//_____________________________________________________________________________
Int_t TStEmcTrackMatchingMaker::Finish()
{
    //Write histograms to root file etc.

    return kStOK;
}

//_____________________________________________________________________________
void TStEmcTrackMatchingMaker::ResetPidTraits()
{
    memset(mTraits->q, -999, mMaxPoints*mMaxTracks*sizeof(Int_t));
    memset(mTraits->p, -1, mMaxPoints*mMaxTracks*sizeof(Double_t));
    memset(mTraits->pt, -1, mMaxPoints*mMaxTracks*sizeof(Double_t));
    memset(mTraits->dca, -1, mMaxPoints*mMaxTracks*sizeof(Double_t));
    memset(mTraits->beta, -1, mMaxPoints*mMaxTracks*sizeof(Double_t));
}
