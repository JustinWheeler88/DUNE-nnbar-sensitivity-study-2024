// drawPID.C draws histograms for various variables with truth-level particles identified

// Written by: Justin Wheeler at Fermilab
// Written for the DUNE collaboration as a part of the Science Undergraduate Laboratory Internship (SULI) program
// Date: 8/6/2024

#include <iostream>
#include <string>
#include <vector>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TLegend.h>

void drawPID(const char* file_ids[], size_t file_id_sz) {
    
    // User input for data file name  
    int num_files = 2;
    std::string inputFileName[2] = {"/some/backgroundfile/name", "/some/signalfile/name"};  // Input .root file names here
    std::string inputTreeName[2] = {"some_tree_name", "some_tree_name"};                    // Input tree names here
    
    // Identify signal and background feature files
    std::string fileIdentifier[2] = {"atm", "nnbar"};

    gROOT->ProcessLine(".x duneStyle_square.C");

    // Define arrays for variables, units, bins, lower and upper limits
    const int nvars = 5;
    TString htname[] = {"trkpidpida_pandoraTrack", "trkpidchipr_pandoraTrack", "trkpidchimu_pandoraTrack", "trkpidchipi_pandoraTrack", "trklen_pandoraTrack"};
    TString axname[] = {"PIDA", "Chi^2 (proton)", "Chi^2 (muon)", "Chi^2 (pion)", "Track Length [cm]"};

    int nbin[] = {60, 200, 80, 80, 50};
    float low[] = {0, 0, 0, 0, 0};
    float up[] = {30, 200, 80, 80, 200};

    // Define histograms for each variable and PDG category
    TH1F *ht_p[nvars], *ht_pi[nvars], *ht_mu[nvars], *ht_other[nvars];

    for (int i = 0; i < nvars; ++i) {
        ht_p[i] = new TH1F("hist_"+htname[i]+"_p", axname[i]+" - Proton", nbin[i], low[i], up[i]);
        ht_pi[i] = new TH1F("hist_"+htname[i]+"_pi", axname[i]+" - Pion", nbin[i], low[i], up[i]);
        ht_mu[i] = new TH1F("hist_"+htname[i]+"_mu", axname[i]+" - Muon", nbin[i], low[i], up[i]);
        ht_other[i] = new TH1F("hist_"+htname[i]+"_other", axname[i]+" - Other", nbin[i], low[i], up[i]);
    }

    // Loop over each file
    for (int file_i = 0; file_i < num_files; file_i++){
        // Read data file
        TFile *outfile = TFile::Open(inputFileName[file_i].c_str());
        TTree *outtree = (TTree*)outfile->Get(inputTreeName[file_i].c_str());

        // Define vars for indexing data file
        Short_t kPFP;
        int maxPFP = outtree->GetMaximum("nPFParticles");

        Short_t ktrk;
        int max_ktrk = outtree->GetMaximum("ntracks_pandoraTrack");
        Short_t is_trk[maxPFP];
        Short_t trk_bestplane[max_ktrk];
        Float_t trk_length[max_ktrk];
        Int_t trk_pdgtruth[max_ktrk][3];
        Float_t trk_pid_vars[nvars][max_ktrk][3];  // nvars variables, each with max_ktrk tracks and 3 planes

        // Set Branch addresses for data file
        outtree->SetBranchAddress("nPFParticles", &kPFP);
        outtree->SetBranchAddress("ntracks_pandoraTrack", &ktrk);
        outtree->SetBranchAddress("pfp_isTrack", &is_trk);
        outtree->SetBranchAddress("trkpidbestplane_pandoraTrack", &trk_bestplane);
        outtree->SetBranchAddress("trklen_pandoraTrack", &trk_length);
        outtree->SetBranchAddress("trkpdgtruth_pandoraTrack", &trk_pdgtruth);
        for (int j = 0; j < nvars - 1; ++j) {
            outtree->SetBranchAddress(htname[j], &trk_pid_vars[j]);
        }

        // Loop over all events
        Long64_t nentries = outtree->GetEntries();
        for (Long64_t en = 0; en < nentries; en++) {
            outtree->GetEntry(en);

            // Loop over all panda flow particles in event
            for (int ipart = 0; ipart < ktrk; ipart++) {
                if ((is_trk[ipart] > 0.5) && (is_trk[ipart] < 1.5) && (ipart <= ktrk)) {
                    Short_t plane = (trk_pdgtruth[ipart][2] > -99990) ? 2 : trk_bestplane[ipart];
                    Short_t pdg = trk_pdgtruth[ipart][plane];

                    // Fill histograms
                    for (int j = 0; j < nvars; ++j) {
                        float var_value = (j < nvars - 1) ? trk_pid_vars[j][ipart][plane] : trk_length[ipart]; // For track length
                        if (pdg == 2212) {
                            ht_p[j]->Fill(var_value);
                        } else if (std::abs(pdg) == 211) {
                            ht_pi[j]->Fill(var_value);
                        } else if (std::abs(pdg) == 13) {
                            ht_mu[j]->Fill(var_value);
                        } else {
                            ht_other[j]->Fill(var_value);
                        }
                    }
                }
            }
        }
    }

    // Draw histograms
    for (int i = 0; i < nvars; ++i) {
        TCanvas* c = new TCanvas("c_"+htname[i], axname[i]+" Histogram", 800, 600);

        Float_t min_ht_val = std::min({ht_p[i]->GetMinimum(), ht_pi[i]->GetMinimum(), ht_mu[i]->GetMinimum(), ht_other[i]->GetMinimum()});
        Float_t max_ht_val = std::max({ht_p[i]->GetMaximum(), ht_pi[i]->GetMaximum(), ht_mu[i]->GetMaximum(), ht_other[i]->GetMaximum()});

        ht_p[i]->SetLineColor(TColor::GetColor("#D81B60")); // Red
        ht_p[i]->SetLineWidth(4);
        ht_p[i]->Draw();
        ht_p[i]->SetXTitle(axname[i]);
        ht_p[i]->GetYaxis()->SetRangeUser(min_ht_val,max_ht_val*1.2);

        ht_pi[i]->SetLineColor(TColor::GetColor("#004D40")); // Green
        ht_pi[i]->SetLineWidth(4);
        ht_pi[i]->Draw("SAME");

        ht_mu[i]->SetLineColor(TColor::GetColor("#FFC107")); // Yellow
        ht_mu[i]->SetLineWidth(4);
        ht_mu[i]->Draw("SAME");

        ht_other[i]->SetLineColor(TColor::GetColor("#1E88E5")); // Blue
        ht_other[i]->SetLineWidth(4);
        ht_other[i]->Draw("SAME");

        TLegend* legend = new TLegend(0.6, 0.65, 0.75, 0.85);
        legend->AddEntry(ht_p[i], "Proton", "l");
        legend->AddEntry(ht_pi[i], "Pion", "l");
        legend->AddEntry(ht_mu[i], "Muon", "l");
        legend->AddEntry(ht_other[i], "Other", "l");
        legend->SetTextSize(0.05);
        legend->Draw();

        // Save plots here
    }
}