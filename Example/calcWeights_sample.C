// calcWeights.C takes a data file calculated that is a subset of a larger data file and finds the weights associated with each event
// The original analysis was performed with unweighted background events. 
// If all events are weighted as 1 or contain their own weights this macro is unnecessary

// Written by: Justin Wheeler at Fermilab
// Written for the DUNE collaboration as a part of the Science Undergraduate Laboratory Internship (SULI) program
// Date: 8/6/2024

#include <iostream>
#include <string>
#include <TFile.h>
#include <TTree.h>

void calcWeights_sample() {

    // User input
    // User input for data file name  
    int num_files = 2;
    std::string inputFileName[2] = {"atm_output_sample.root", "nnbar_output_sample.root"};  // Input .root file names here
    std::string inputTreeName[2] = {"ana", "ana"};                    // Input tree names here

    // Identify signal and background feature files
    std::string fileIdentifier[2] = {"atm", "nnbar"};

    // User input for larger file
    std::string largerFileName = "atm_weights_sample.root";
    std::string largerFileTree = "weight";

    for (int file_i = 0; file_i < num_files; file_i++){
        // Read data file
        TFile *outfile = TFile::Open(inputFileName[file_i].c_str());
        TTree *outtree = (TTree*)outfile->Get(inputTreeName[file_i].c_str());
    
        // Read larger output file with weights
        TFile *mergefile = TFile::Open(largerFileName.c_str());
        TTree *mergetree = (TTree*)mergefile->Get(largerFileTree.c_str());
        
        // Create new files for atm weights
        TFile *wfile = new TFile(Form("%s_weights_nocut_sample.root",fileIdentifier[file_i].c_str()), "RECREATE");
        TTree *wtree = new TTree("weight", Form("%s normalized weights no cut applied", fileIdentifier[file_i].c_str()));

        // Define variable for weights in atm weight file
        Double_t weight;
        wtree->Branch("Weight", &weight);    
        
        // Define vars for indexing data file
        Short_t nnuvtx;
        int max_knuvtx = outtree->GetMaximum("nnuvtx");
        Float_t nuvtxx[max_knuvtx];
        Float_t nuvtxy[max_knuvtx];
        Float_t nuvtxz[max_knuvtx];
        
        outtree->SetBranchAddress("nnuvtx", &nnuvtx);
        if (fileIdentifier[file_i] == "atm") {
            outtree->SetBranchAddress("nuvtxx_truth", &nuvtxx);
            outtree->SetBranchAddress("nuvtxy_truth", &nuvtxy);
            outtree->SetBranchAddress("nuvtxz_truth", &nuvtxz);
        } else {
            outtree->SetBranchAddress("nuvtxx", &nuvtxx);
            outtree->SetBranchAddress("nuvtxy", &nuvtxy);
            outtree->SetBranchAddress("nuvtxz", &nuvtxz);
        }

        // vars to index mergefile
        Short_t mcnnuvtx;
        Float_t mcnuvtxx;
        Float_t mcnuvtxy;
        Float_t mcnuvtxz;
        Double_t mcweight;

        mergetree->SetBranchAddress("mc.nuvtxx", &mcnuvtxx);
        mergetree->SetBranchAddress("mc.nuvtxy", &mcnuvtxy);
        mergetree->SetBranchAddress("mc.nuvtxz", &mcnuvtxz);
        mergetree->SetBranchAddress("weight", &mcweight);

        // Define Misc. Vars
        Long64_t nweights = mergetree->GetEntries();
        Double_t minweight;
        Float_t mindist;
        Float_t dist;

        // Loop over all events
        Long64_t nentries = outtree->GetEntries();
        for(Long64_t en = 0; en < nentries; en+=1) {
            outtree->GetEntry(en);

            minweight = 9.0e9;
            mindist = 9.0e9;
            if (fileIdentifier[file_i] == "atm") {
                // Find minimum 3D distance in nu vertex between files
                for(Long64_t wen = 0; wen < nweights; wen++) {
                    mergetree->GetEntry(wen);
                    dist = std::sqrt(pow(nuvtxx[0]-mcnuvtxx,2) + pow(nuvtxy[0]-mcnuvtxy,2) + pow(nuvtxz[0]-mcnuvtxz,2));
                    if (dist < mindist) {
                        mindist = dist;
                        minweight = mcweight;
                    }
                    if (dist < 1e-4) continue;
                }
                weight = minweight * (nweights / nentries);
            } else {
                weight = 1;
            }
            
            wtree->Fill();
        }

        // Write the new tree to the file
        wfile->Write();
    }
}
