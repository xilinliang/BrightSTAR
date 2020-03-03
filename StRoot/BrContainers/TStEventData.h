// Filename: TStEventData.h
// Description: Container for Event Data.
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri Dec 20 12:58:02 2019 (-0500)
// URL: jlab.org/~latif


#ifndef TSTEVENTDATA_H
#define TSTEVENTDATA_H

#include <iostream>
#include "TObject.h"


class TStEventData : public TObject
{

public:
    Int_t mRunNumber;			//
    Int_t mEventId;			//
    Short_t mBspin;			//
    Short_t mYspin;			//
    Int_t mTrigFlag;			//
    static const Int_t mMaxTrigs = 30;	//
    Int_t mNtrig;			//
    Int_t mTriggers[mMaxTrigs];		//
    
    Int_t mBbcADCSum[2];		//
    Int_t mBbcADCSumLarge[2];		//
    Int_t mBbcEarliestTDC[2];		//
    Int_t mBbcEarliestTDCLarge[2];	//

    Int_t mZdcADCSum[2];		// 
    Int_t mVpdADCSum[2];		//
    Int_t mTofMultiplicity;		//
    Int_t mNprimaryTracks;              //   
    Int_t mNvertices;                   //   
    Int_t mRanking;                     //   
    Double_t mVx;                       //
    Double_t mVy;                       //
    Double_t mVz;                       //

    //---------
    Int_t GetRunNumber(){return mRunNumber;}							//
    Int_t GetEventId(){return mEventId;}							//
    Short_t GetBlueSpin(){return mBspin;}							//
    Short_t GetYellowSPin(){return mYspin;}							//
    Int_t GetTrigFlag(){return mTrigFlag;}							//
    Int_t GetNtrigs(){return mNtrig;}								//
    Int_t GetTrigger(Int_t index){if(index < mNtrig) return mTriggers[index]; return -1;}	//    
    Int_t GetBbcSumSmall(Int_t ew){if(ew < 2) return mBbcADCSum[ew]; return -1;}		//
    Int_t GetBbcSumLarge(Int_t ew){if(ew < 2) return mBbcADCSumLarge[ew]; return -1;}		//
    Int_t GetEarliestTdcSmall(Int_t ew){if(ew < 2) return mBbcEarliestTDC[ew]; return -1;}	//
    Int_t GetEarliestTdcLarge(Int_t ew){if(ew < 2) return mBbcEarliestTDCLarge[ew]; return -1;}	//

    Int_t GetZdcSum(Int_t ew){if(ew < 2) return mZdcADCSum[ew]; return -1;}			// 
    Int_t GetVpdSum(Int_t ew){if(ew < 2) return mVpdADCSum[ew]; return -1;}			//
    Int_t GetTofMultiplicity(){return mTofMultiplicity;}					//
    Int_t GetNprimaryTracks(){return mNprimaryTracks;}					//
    Int_t GetNvertices(){return mNvertices;}					//
    Int_t GetRanking(){return mRanking;}					//
    Double_t GetVx(){return mVx;}								//
    Double_t GetVy(){return mVy;}								//
    Double_t GetVz(){return mVz;}								//

    //-----------
    void SetRunNumber( Int_t run){mRunNumber = run;}						//
    void SetEventId(Int_t id){mEventId = id;}							//
    void SetBlueSpin(Short_t spin){mBspin = spin;}						//
    void SetYellowSPin(Short_t spin){mYspin = spin;}						//
    void SetTrigFlag(Int_t flag){mTrigFlag = flag;}						//
    void SetNtrigs(Int_t ntrig){mNtrig = ntrig;}						//
    void SetTrigger(Int_t index, Int_t trig){if(index < mMaxTrigs) mTriggers[index] = trig;}	//    
    void SetBbcSumSmall(Int_t ew, Int_t sum){if(ew < 2) mBbcADCSum[ew] = sum;}			//
    void SetBbcSumLarge(Int_t ew, Int_t sum){if(ew < 2) mBbcADCSumLarge[ew] = sum;}		//
    void SetEarliestTdcSmall(Int_t ew, Int_t tdc){if(ew < 2) mBbcEarliestTDC[ew] = tdc;}	//
    void SetEarliestTdcLarge(Int_t ew, Int_t tdc){if(ew < 2) mBbcEarliestTDCLarge[ew] = tdc;}	//

    void SetZdcSum(Int_t ew, Int_t sum){if(ew < 2) mZdcADCSum[ew] = sum;}			// 
    void SetVpdSum(Int_t ew, Int_t sum){if(ew < 2) mVpdADCSum[ew] = sum;}			//
    void SetTofMultiplicity(Int_t mult){mTofMultiplicity = mult;}				//
    void SetNprimaryTracks(Int_t nTracks){mNprimaryTracks = nTracks;}					//
    void SetNvertices(Int_t nVertices){mNvertices = nVertices;}					//
    void SetRanking(Int_t ranking){mRanking = ranking;}					//
    void SetVx(Double_t vx){mVx = vx;}							//
    void SetVy(Double_t vy){mVy = vy;}							//
    void SetVz(Double_t vz){mVz = vz;}							//

    
    TStEventData()
    {
	Reset();	
    }
    ~TStEventData() {}

    void Reset()
    {
	mRunNumber = -1;
	mEventId = -1;
	mBspin = 0;
	mYspin = 0;
	mTrigFlag = -1;
	mNtrig = 0;
	for(Int_t i = 0; i < mMaxTrigs; ++i)
	    mTriggers[i] = 0;
    
	mBbcADCSum[0] = 0;
	mBbcADCSum[1] = 0;
	
	mBbcADCSumLarge[0] = 0;
	mBbcADCSumLarge[1] = 0;
	
	mBbcEarliestTDC[0] = 0;
	mBbcEarliestTDC[1] = 0;
	
	mBbcEarliestTDCLarge[0] = 0;
	mBbcEarliestTDCLarge[1] = 0;

	mZdcADCSum[0] = 0;
	mZdcADCSum[1] = 0;
	
	mVpdADCSum[0] = 0;
	mVpdADCSum[1] = 0;
	mTofMultiplicity = -1;

	mVx = -999.0;
	mVy = -999.0;
	mVz = -999.0;
    }
    
    // Int_t GetYear() const { return (mRunId / 1000000) - 1 + 2000; }
	
    // void SetVertex( Float_t x, Float_t y, Float_t z )
    // {
    // 	this->mPrimaryVertex_mX1 = x;
    // 	this->mPrimaryVertex_mX2 = y;
    // 	this->mPrimaryVertex_mX3 = z;
    // }

    ClassDef( TStEventData, 2 )
};


#endif
