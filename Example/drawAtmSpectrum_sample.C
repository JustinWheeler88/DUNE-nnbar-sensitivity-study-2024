// drawAtmSpectrum.C draws energy distribution for weighted and unweighted background

// Written by: Justin Wheeler at Fermilab
// Written for the DUNE collaboration as a part of the Science Undergraduate Laboratory Internship (SULI) program
// Date: 8/6/2024

#include <iostream>
#include <cmath> 

void drawAtmSpectrum_sample(){
	
	// User input for background data file
	std::string inputFileName = "atm_output_sample.root";
	std::string inputTreeName = "ana";

	gROOT->ProcessLine(".x duneStyle_6to4_sample.C");

	float pi = TMath::Pi();

	// Read data file
	TFile *outfile = TFile::Open(inputFileName.c_str());
	TTree *outtree = (TTree*)outfile->Get(inputTreeName.c_str());

	// Read weights
	TFile* atmweight_file = TFile::Open("atm_weights_nocut_sample.root");
	TTree* atmweight = (TTree*)atmweight_file->Get("weight");

	Int_t geant_list_size;
	int max_listsize = outtree->GetMaximum("geant_list_size");

	Int_t no_primaries;
	Int_t process_primary[max_listsize];
	Int_t pdg[max_listsize];
	Float_t nu_energy[max_listsize];

	outtree->SetBranchAddress("geant_list_size", &geant_list_size);
	outtree->SetBranchAddress("no_primaries", &no_primaries);
	outtree->SetBranchAddress("process_primary", &process_primary);
	outtree->SetBranchAddress("pdg", &pdg);
	outtree->SetBranchAddress("Eng", &nu_energy);

	Double_t weight;
	atmweight->SetBranchAddress("Weight", &weight);

	// Create histogram
	TH1D *ht_atm1 = new TH1D("nu_eng","nu_eng",100,0,10);
	TH1D *ht_atm2 = new TH1D("nu_eng","nu_eng",100,0,10);

	Long64_t nentries = outtree->GetEntries();
	for(Long64_t en = 0; en < nentries; en++) {
		outtree->GetEntry(en);
		atmweight->GetEntry(en);
		for (int g = 0; g < no_primaries; g++){
			if ((std::abs(pdg[g]) == 12 || std::abs(pdg[g]) == 14 || std::abs(pdg[g]) == 16)) {
				ht_atm1->Fill(nu_energy[g]);
				ht_atm2->Fill(nu_energy[g], weight);
				break;
			}
		}
	}

	TCanvas* c = new TCanvas();
	c->Update();

	ht_atm1->SetLineColor(TColor::GetColor("#D81B60")); // Colorblind color pallete for red
	ht_atm1->Draw("hist same");
	ht_atm1->SetXTitle("Atmospheric nu E [GeV])");
	ht_atm1->SetYTitle("Weighted entries");
	ht_atm1->GetYaxis()->SetRangeUser(1, ht_atm1->GetMaximum()*1.2);
	
	ht_atm2->SetLineColor(TColor::GetColor("#1E88E5")); // Colorblind color pallete for red
	ht_atm2->Draw("SAME");
	ht_atm2->Scale(ht_atm1->Integral()/ht_atm2->Integral());


	TLegend *legend = new TLegend(0.45, 0.75, 0.6, 0.85);
	legend->AddEntry(ht_atm1, "Unweighted", "l");
	legend->AddEntry(ht_atm2, "Weighted", "l");
	legend->SetTextSize(0.05);
	legend->Draw();

	// Save here

}
