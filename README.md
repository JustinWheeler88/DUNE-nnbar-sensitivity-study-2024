This repository contains software to obtain the sensitivity of the DUNE far detector to neutron-antineutron oscillation $\left(n\rightarrow\bar{n}\right)$ without taking into account systematic uncertainty. 
The analysis begins with data files stored in the .root format for simulated neutron-antineutron (signal) and atmospheric neutrino (background) events within a Liquid Argon Time Projection Chamber (LArTPC). 
This code does not perform the reconstruction of events such as the classification of track-like and shower-like objects, but begins with the reconstructed events and calorimetry data. 

This code was written for my analysis which uses 100k simulated signal and background events. The simulations were performed using GENIE 3.4.0 (model: hn BR) as the generator. 
The simulation software used is DUNEsw: v09_85_00d00 within the far detector horizontal drift module with dimensions 1x2x6 m. 
Additionally, modeling of the atmospheric neutrino events was performed with a larger than realistic flux of low-energy neutrinos, thus there is a reweighting scheme included. This can be ommited. 
These macros can be applied to any data files, though some editing of variables may be necessary.

The overall workflow is as follows:
- calcFeatures.C - Begins with data files for signal and background events. This macro calculates feature variables per event. Output: atm/nnbar_featurevars_nocut.root
- calcWeights.C - Begins with data files for signal and background events as well as larger sample of 200k events including weights for background. Output: atm/nnbar_weights_nocut.root
- cutFeatures.C - Applies pre-cuts to data, Output: atm/nnbar_featurevars_cut.root, atm/nnbar_weights_cut.root, and atm/nnbar efficiency and deficiency due to cuts
- classfication.ipynb - Boosted Decision Tree classification of signal and background. Output: 90% C.L. free $n\rightarrow\bar{n}$ oscillation lifetime at DUNE TDR background rate and exposure without systematic uncertainty analysis.
