Condor in BrightSTAR
============================

- From this directory, only `condor.config` file is read from BrightSTAR scheduler and all other files are generated based on this configuration.

- The files `condor.job`, `condor.sh`, `jobMacro.C` are just reference here. These are automatically generated for each job. You can use them if you do not want to submit job directly from root prompt.

- There is a more direct approach that does not require intermediate shell script or macro. This is the preferred approach and has been implemented in BrightSTAR. 
