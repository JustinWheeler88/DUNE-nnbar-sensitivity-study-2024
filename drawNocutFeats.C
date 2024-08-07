// drawNocutFeats.C makes histograms for all feature variables without precuts
// These histograms are not weighted

// Written by: Justin Wheeler at Fermilab
// Written for the DUNE collaboration as a part of the Science Undergraduate Laboratory Internship (SULI) program
// Date: 8/6/2024

#include <iostream>
#include <cmath> 

void drawNocutFeats(){
	gROOT->ProcessLine(".x duneStyle_square.C");
	// tempStyle();

	float pi = TMath::Pi();

	// Read in .root files to TChain
    TChain *tc_atm = new TChain("feats");
    tc_atm->Add("atm_featurevars_nocut.root");

    TChain *tc_nnbar = new TChain("feats");
    tc_nnbar->Add("nnbar_featurevars_nocut.root");

	// TFile* atmweight_file = TFile::Open("atm_featurevarsweights.root");
	// TTree* atmweight = (TTree*)atmweight_file->Get("ana");

	// TFile* nnbarweight_file = TFile::Open("nnbar_featurevarsweights.root");
	// TTree* nnbarweight = (TTree*)nnbarweight_file->Get("ana");

	// Create lists of names of variables, units, and ...
	const int nh=16;
   	TString htname[nh]={"num_particles","num_particles","num_showers","num_tracks",
						"num_p","num_mu",
                        "trk_eng","shwr_eng","visible_energy",
						"tot_momentum","sphericity","aplanarity",
						"FW0","FW1","FW2",
						"invariant_mass"};

	TString axname[nh]={"","# of particles","# of showers","# of tracks",
						"# of protons","# of muons",
						"Track visible energy","Shower visible energy","Total visible energy",
						"Total momentum","Sphericity","Aplanarity",
						"0th F-W moment","1st F-W moment","2nd F-W moment",
						"Invariant mass"};

   	TString unit[nh]={"","","","",
						"","",
						"[MeV]","[MeV]","[MeV]",
						"[MeV/c]","","",
						"","","",
						"[MeV/c^{2}]"};

	TString yscale[nh]={"","","","",
						"","",
						"log","log","log",
						"","","",
						"","","",
						""};
	
	// Path for Variables
	// float var[nh];

	// Create histogram
	TH1D *ht_atm[nh];
	TH1D *ht_nnbar[nh];
	int nbin[nh]={21,21,12,21,
                    9,9,
					50, 40,40,
					50,20,40,
					30,40,40,
					70}; // number of bins per variable

	float low[nh]={-0.5,-0.5,-0.5,-0.5,
					-.5,-.5,
					0,0,0,
					0,0,0,
					0,0,0,
					0}; // min val per variable

	float up[nh]={20.5,20.5,11.5,20.5,
					8.5,8.5,
					5000,4000,4000,
					1500,1,1,
					4,4,4,
					7e3}; // max val per variable

	// Fill Histograms
	for(int i=0;i<nh;i++){

		ht_atm[i]=new TH1D(htname[i]+"_atm",htname[i]+"_atm",nbin[i],low[i],up[i]);
		ht_nnbar[i]=new TH1D(htname[i]+"_nnbar",htname[i]+"_nnbar",nbin[i],low[i],up[i]);

        TString drawCommand = htname[i] + ">>" + htname[i]+"_atm";
        tc_atm->Draw(drawCommand, "", "goff");

        TString drawCommand2 = htname[i] + ">>" + htname[i]+"_nnbar";
        tc_nnbar->Draw(drawCommand2, "", "goff");
	}

	for(int i=0;i<nh;i++){
		TCanvas* c = new TCanvas();

		if ((string)yscale[i] == "log") {
			c->SetLogy();
		}
		c->Update();

		ht_atm[i]->SetLineColor(TColor::GetColor("#D81B60")); // Colorblind color pallete for red
		ht_atm[i]->SetLineWidth(4.0);
		ht_atm[i]->Draw("SAME");
		ht_atm[i]->SetXTitle(axname[i]+" "+unit[i]);
		ht_atm[i]->GetYaxis()->SetRangeUser(1, TMath::Max(ht_nnbar[i]->GetMaximum()*1.2 ,ht_atm[i]->GetMaximum()*1.2));

		ht_nnbar[i]->SetLineColor(TColor::GetColor("#1E88E5")); // Blue
		ht_nnbar[i]->SetLineWidth(4.0);
		ht_nnbar[i]->Draw("SAME");
		
		TLegend *legend = new TLegend(0.45, 0.75, 0.6, 0.85);
		legend->AddEntry(ht_nnbar[i], "Signal", "l");
		legend->AddEntry(ht_atm[i], "Background", "l");
		legend->SetTextSize(0.05);
		legend->Draw();

    	// Save here
	}
}
