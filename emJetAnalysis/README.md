EM-jet A_N Analysis
================================

- The relevant directory for EM-jet analysis is: `BrightSTAR/emJetAnalysis`

- My analysis has four steps. Step-1 uses MuDst to generate NanoJetTree. Step-2 uses NanoJetTree from step-1 to gegerate hitogram in root file. Step-3 uses histograms in root file from Step-2 to calculate A_N and plot them. Step-4 uses the output from step-3 to add systematic error bars or other style to the plots.


- Step-1: Generate DST files (EM-jet Trees)
  - The corresponding script is: `emJetAnalysis/src/EjRunEmJetTreeMaker.cxx`
  - It relies on two makers named `StJetMaker2015.cxx` and `TStNanoJetTreeMaker.cxx` under `StRoot` to produce Nano jet trees.


- Step-2: Generate Binned Histograms Required for `A_N` calculation
  - The corresponding script is: `emJetAnalysis/src/EjCreateBinnedHist.cxx`
  - There is also an extended version of the script thaty is used to extended my original analysis integrating over energy and photon multiplicity bins, also to compare with Zhanwen's result.


- Step-3: Calculate A_N and plot them
  - The corresponding script is: `emJetAnalysis/src/EjCalculateAN.cxx` 

- Step-4: Replot the A_N results with systematic error bars shown
  - The corresponding script is `emJetAnalysis/src/EjReplotAn.cxx`

- Nano Jet Tree file location: `/gpfs/mnt/gpfs01/star/pwg/kabir/dst/emJet`

- Regarding how to run the root ananlysis, the instruction can be found at the top level (of BrightSTAR) README  file titled "Quick Start".


