
#ifdef __CINT__

#pragma link C++ function JobPrototype;                   // Job prototyping and testing

//RP
#pragma link C++ function RpReadDemo;                     // Read RP data
#pragma link C++ function RpQA;                           // Various RP distributions
#pragma link C++ function RpQAafterBurner;                // Various RP distributions with afterburner enabled

//EEMC
#pragma link C++ function EEmcReadDemo;                   // Read EEMC data
#pragma link C++ function EEmcReadDemo2;                  // Read EEMC data
#pragma link C++ function EEmcQA;                         // Various EEMC distributions
#pragma link C++ function EEmcEvent;                      // Various EEMC distributions

//FMS
#pragma link C++ function FmsQA;                          // FMS QA for cell status
#pragma link C++ function FmsQAusingMaker;                // FMS QA for cell status using maker
#pragma link C++ function FmsCellStatus;                  // FMS cell status list and plot
#pragma link C++ function FmsPi0InvMass1;                 // FMS Pi0 Invariant mass directly from MuDST files.
#pragma link C++ function FmsPi0MakerEx;                  // FMS Pi0 Invariant mass with on-the-fly reconstruction.

//EMC
#pragma link C++ function EmcQA;                          // EMC QA 
#pragma link C++ class EmcTreeReader;                     // EMC Tree Reader Class (Auto generated from root file) 
#pragma link C++ function EmcPi0Analysis;                 // EMC Pi0 Analysis using Emc Tree and reader

//TPC
#pragma link C++ function TpcTrackQA;                     // TPC track properties

//TPC + BTOF
#pragma link C++ class PidTreeReader;                     // Charged particle PID tree reader class (auto generated)
#pragma link C++ function PidEpEm;                        // e+,e- invariant mass from charged particle pid tree
#pragma link C++ function PidKpKm;                        // K+,K- invariant mass from charged particle pid tree
#pragma link C++ function PidLambda;                      // Lambda invariant mass of pr,pi- combination from charged particle pid tree 
#pragma link C++ function PidPiPpiM;                      // pi+,pi- invariant mass from charged particle pid tree
#pragma link C++ function PidKmPiP;                       // K-,pi+ invariant mass from charged particle pid tree

//TOF
#pragma link C++ function TofData;                        // Access BTOF data 

//Jets
#pragma link C++ function JetQA;                          // Jet Data Quality Check

#endif

