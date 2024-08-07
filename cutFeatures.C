// cutFeatures.C performs precuts on feature variable files

// Written by: Justin Wheeler at Fermilab
// Written for the DUNE collaboration as a part of the Science Undergraduate Laboratory Internship (SULI) program
// Date: 8/6/2024

#include <iostream>
#include <string>
#include <TFile.h>
#include <TTree.h>

void cutFeatures() {

    // Uncut feature variable file names
    int num_files = 2;    

    // Identify signal and background feature files
    std::string fileIdentifier[2] = {"atm", "nnbar"};

    for (int file_i = 0; file_i < num_files; file_i++){
        
        // Read uncut feature var and weight files
        TFile *varfile = TFile::Open(Form("%s_featurevars_nocut.root",fileIdentifier[file_i].c_str()));
        TTree *vartree = (TTree*)varfile->Get("feats");

        TFile *wfile = TFile::Open(Form("%s_weights_nocut.root",fileIdentifier[file_i].c_str()));
        TTree *wtree = (TTree*)wfile->Get("weight");
            
        // Create new files with trees for feature vars and weights
        TFile *cut_varfile = TFile::Open(Form("%s_featurevars_cut.root",fileIdentifier[file_i].c_str()), "RECREATE");
        TTree *cut_vartree = new TTree("ana", Form("%s feature vars with pre-cuts applied",fileIdentifier[file_i].c_str()));
        
        TFile *cut_wfile = TFile::Open(Form("%s_weights_cut.root",fileIdentifier[file_i].c_str()), "RECREATE");
        TTree *cut_wtree = new TTree("ana", Form("%s normalized weights with pre-cuts applied",fileIdentifier[file_i].c_str()));

        // Define feature vars for uncut file
        Short_t num_particles;
        Short_t num_showers;
        Short_t num_tracks;
        Short_t num_p;
        Short_t num_mu;
        Float_t trk_vis_eng;
        Float_t shwr_vis_eng;
        Float_t tot_vis_eng;
        Float_t tot_momentum;
        Float_t inv_mass;
        Float_t sphericity;
        Float_t aplanarity;
        Float_t FW0;
        Float_t FW1;
        Float_t FW2;
        vartree->SetBranchAddress("num_particles", &num_particles);
        vartree->SetBranchAddress("num_showers", &num_showers);
        vartree->SetBranchAddress("num_tracks", &num_tracks); 
        vartree->SetBranchAddress("num_p", &num_p);
        vartree->SetBranchAddress("num_mu", &num_mu);
        vartree->SetBranchAddress("trk_eng", &trk_vis_eng);
        vartree->SetBranchAddress("shwr_eng", &shwr_vis_eng);
        vartree->SetBranchAddress("visible_energy", &tot_vis_eng);
        vartree->SetBranchAddress("tot_momentum", &tot_momentum);
        vartree->SetBranchAddress("invariant_mass", &inv_mass);
        vartree->SetBranchAddress("sphericity", &sphericity);
        vartree->SetBranchAddress("aplanarity", &aplanarity);
        vartree->SetBranchAddress("FW0", &FW0);
        vartree->SetBranchAddress("FW1", &FW1);
        vartree->SetBranchAddress("FW2", &FW2);

        // Define weight for uncut file
        Double_t weight;
        wtree->SetBranchAddress("Weight", &weight);

        // Create pre-cut var file branches
        Short_t cut_num_particles;
        Short_t cut_num_showers;
        Short_t cut_num_tracks;
        Short_t cut_num_p;
        Short_t cut_num_mu;
        Float_t cut_trk_vis_eng;
        Float_t cut_shwr_vis_eng;
        Float_t cut_tot_vis_eng;
        Float_t cut_tot_momentum;
        Float_t cut_inv_mass;
        Float_t cut_sphericity;
        Float_t cut_aplanarity;
        Float_t cut_FW0;
        Float_t cut_FW1;
        Float_t cut_FW2;
        cut_vartree->Branch("num_particles", &cut_num_particles);
        cut_vartree->Branch("num_showers", &cut_num_showers);
        cut_vartree->Branch("num_tracks", &cut_num_tracks); 
        cut_vartree->Branch("num_p", &cut_num_p);
        cut_vartree->Branch("num_mu", &cut_num_mu);
        cut_vartree->Branch("trk_eng", &cut_trk_vis_eng);
        cut_vartree->Branch("shwr_eng", &cut_shwr_vis_eng);
        cut_vartree->Branch("visible_energy", &cut_tot_vis_eng);
        cut_vartree->Branch("tot_momentum", &cut_tot_momentum);
        cut_vartree->Branch("invariant_mass", &cut_inv_mass);
        cut_vartree->Branch("sphericity", &cut_sphericity);
        cut_vartree->Branch("aplanarity", &cut_aplanarity);
        cut_vartree->Branch("FW0", &cut_FW0);
        cut_vartree->Branch("FW1", &cut_FW1);
        cut_vartree->Branch("FW2", &cut_FW2);

        // Create pre-cut weight file branches
        Double_t cut_weight;
        cut_wtree->Branch("Weight", &cut_weight);

        // Define misc vars
        Float_t cutvals = 0;
        Float_t keptvals = 0;

        // Loop over all events
        Long64_t nentries = vartree->GetEntries();
        for(Long64_t en = 0; en < nentries; en+=1) {
            vartree->GetEntry(en);
            wtree->GetEntry(en);

            if (num_particles <= 1) {
                cutvals++; 
                continue;
            }
            if (tot_momentum >= 980) {
                cutvals++;
                continue;
            }
            if (tot_vis_eng > 1800) {
                cutvals++;
                continue;
            }

            cut_num_particles   = num_particles;
            cut_num_showers     = num_showers;
            cut_num_tracks      = num_tracks;
            cut_num_p           = num_p;
            cut_num_mu          = num_mu;
            cut_trk_vis_eng     = trk_vis_eng;
            cut_shwr_vis_eng    = shwr_vis_eng;
            cut_tot_vis_eng     = tot_vis_eng;
            cut_tot_momentum    = tot_momentum;
            cut_inv_mass        = inv_mass;
            cut_sphericity      = sphericity;
            cut_aplanarity      = aplanarity;
            cut_FW0             = FW0;
            cut_FW1             = FW1;
            cut_FW2             = FW2;

            cut_weight = weight;

            cut_vartree->Fill();
            cut_wtree->Fill();
            keptvals++;
        }

        // Normalize weights
        if (fileIdentifier[file_i] == "atm") {
            Long64_t ncut_entries = cut_wtree->GetEntries();
            for (Long64_t cut_en = 0; cut_en < ncut_entries; cut_en++) {
                cut_wtree->GetEntry(cut_en);
                cut_weight *= ((double)nentries / (double)ncut_entries);
            }
        }


        // Write the new tree to the file
        cut_varfile->Write();
        cut_wfile->Write();
        cout << fileIdentifier[file_i] << " kept: " << keptvals/(cutvals+keptvals) << " cut: " << cutvals/(keptvals+cutvals) << endl;
    }
}