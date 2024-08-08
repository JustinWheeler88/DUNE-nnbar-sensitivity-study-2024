// calcFeatures.C calculates the feature variables for each event in a .root data file
// There are no events cut from the output feature variable file
// The eigenvalue calculation will send MakeSchurr alerts for events with 0 reconstructed particles
// These alerts can be ignored as events with 0 reco particles will be cut in future steps

// Written by: Justin Wheeler at Fermilab
// Written for the DUNE collaboration as a part of the Science Undergraduate Laboratory Internship (SULI) program
// Date: 8/6/2024

#include <iostream>
#include <string>
#include <TFile.h>
#include <TTree.h>

void calcFeatures_sample() {

    // User input for data file name  
    int num_files = 2;
    std::string inputFileName[2] = {"atm_output_sample.root", "nnbar_output_sample.root"};  // Input .root file names here
    std::string inputTreeName[2] = {"ana", "ana"};                    // Input tree names here
    
    // Identify signal and background feature files
    std::string fileIdentifier[2] = {"atm", "nnbar"};

    for (int file_i = 0; file_i < num_files; file_i++){
        // Read data file
        TFile *outfile = TFile::Open(inputFileName[file_i].c_str());
        TTree *outtree = (TTree*)outfile->Get(inputTreeName[file_i].c_str());
            
        // Create new file for calculated feature variables
        TFile *varfile = new TFile(Form("%s_featurevars_nocut_sample.root", fileIdentifier[file_i].c_str()), "RECREATE");
        TTree *vartree = new TTree("feats", Form("%s feature vars no cuts applied", fileIdentifier[file_i].c_str()));

        // Define feature vars
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

        // Add branches for feature vars
        vartree->Branch("num_particles", &num_particles);
        vartree->Branch("num_showers", &num_showers);
        vartree->Branch("num_tracks", &num_tracks); 
        vartree->Branch("num_p", &num_p);
        vartree->Branch("num_mu", &num_mu);
        vartree->Branch("trk_eng", &trk_vis_eng);
        vartree->Branch("shwr_eng", &shwr_vis_eng);
        vartree->Branch("visible_energy", &tot_vis_eng);
        vartree->Branch("tot_momentum", &tot_momentum);
        vartree->Branch("invariant_mass", &inv_mass);
        vartree->Branch("sphericity", &sphericity);
        vartree->Branch("aplanarity", &aplanarity);
        vartree->Branch("FW0", &FW0);
        vartree->Branch("FW1", &FW1);
        vartree->Branch("FW2", &FW2);

        // Define vars for indexing data file
        Short_t kPFP;
        int maxPFP = outtree->GetMaximum("nPFParticles");
        
        Short_t ktrk;
        int max_ktrk = outtree->GetMaximum("ntracks_pandoraTrack");
        Short_t is_trk[maxPFP];
        Short_t trk_bestplane[max_ktrk];
        Float_t trk_length[max_ktrk];
        Float_t trk_pida[max_ktrk][3];
        Short_t ktrkhits[max_ktrk][3];
        Float_t trk_thetayz[max_ktrk];
        Float_t trk_dedx_reco[max_ktrk][3][2000];
        Float_t trk_momrange_reco[max_ktrk];
        Float_t trk_start_xhat[max_ktrk];
        Float_t trk_start_yhat[max_ktrk];
        Float_t trk_start_zhat[max_ktrk];
        Float_t trk_xyz[max_ktrk][3][2000][3]; // [track][plane][wire][x,y,z]

        Short_t kshwr;
        int max_kshwr = outtree->GetMaximum("nshowers_pandoraShower");
        Short_t is_shwr[maxPFP];
        Short_t shwr_bestplane[max_kshwr];
        Float_t shwr_totEng_reco[max_kshwr][3]; 
        Float_t shwr_start_xhat[max_kshwr];
        Float_t shwr_start_yhat[max_kshwr];
        Float_t shwr_start_zhat[max_kshwr];

        // Set Branch addresses for data file
        outtree->SetBranchAddress("nPFParticles", &kPFP);

        outtree->SetBranchAddress("ntracks_pandoraTrack", &ktrk);
        outtree->SetBranchAddress("pfp_isTrack", &is_trk);
        outtree->SetBranchAddress("trkpidbestplane_pandoraTrack", &trk_bestplane);
        outtree->SetBranchAddress("ntrkhits_pandoraTrack", &ktrkhits);
        outtree->SetBranchAddress("trklen_pandoraTrack", &trk_length);
        outtree->SetBranchAddress("trkpidpida_pandoraTrack", &trk_pida);
        outtree->SetBranchAddress("trkthetayz_pandoraTrack", &trk_thetayz);
        outtree->SetBranchAddress("trkdedx_pandoraTrack", &trk_dedx_reco);
        outtree->SetBranchAddress("trkmomrange_pandoraTrack", &trk_momrange_reco);
        outtree->SetBranchAddress("trkstartdcosx_pandoraTrack", &trk_start_xhat);
        outtree->SetBranchAddress("trkstartdcosy_pandoraTrack", &trk_start_yhat);
        outtree->SetBranchAddress("trkstartdcosz_pandoraTrack", &trk_start_zhat);
        outtree->SetBranchAddress("trkxyz_pandoraTrack", &trk_xyz);

        outtree->SetBranchAddress("nshowers_pandoraShower", &kshwr);
        outtree->SetBranchAddress("pfp_isShower", &is_shwr);
        outtree->SetBranchAddress("shwr_bestplane_pandoraShower", &shwr_bestplane);
        outtree->SetBranchAddress("shwr_totEng_pandoraShower", &shwr_totEng_reco);
        outtree->SetBranchAddress("shwr_startdcosx_pandoraShower", &shwr_start_xhat);
        outtree->SetBranchAddress("shwr_startdcosy_pandoraShower", &shwr_start_yhat);
        outtree->SetBranchAddress("shwr_startdcosz_pandoraShower", &shwr_start_zhat);

        // Define Misc. Vars
        Float_t dr, dx, dy, dz;
        Float_t p_vec[3];
        Float_t tot_p_vec[3];
        TMatrixD Spher(3, 3);
        Float_t sum_p_sqrd;
        Short_t plane;
        Float_t p_i_holder;
        Float_t p_j_holder;
        Float_t dcos_i_holder[3];
        Float_t dcos_j_holder[3];
        Float_t cos_theta_ij;
        Float_t m_p = 938.2720894;  // MeV
        Float_t m_mu = 105.6583755; // MeV
        Float_t mass_holder;
        Float_t tot_Eng;

        // Loop over all events
        Long64_t nentries = outtree->GetEntries();
        for(Long64_t en = 0; en < nentries; en+=1) {
            outtree->GetEntry(en);

            num_particles = 0;
            num_showers = 0;
            num_tracks = 0;
            num_p = 0;
            num_mu = 0;

            trk_vis_eng = 0;
            shwr_vis_eng = 0;
            tot_vis_eng = 0;
            tot_momentum = 0;
            inv_mass = 0;

            sphericity = 0;
            aplanarity = 0;
            FW0 = 0;
            FW1 = 0;
            FW2 = 0;

            tot_p_vec[0] = 0;
            tot_p_vec[1] = 0;
            tot_p_vec[2] = 0;

            sum_p_sqrd = 0;

            Spher(0, 0) = 0.0f; Spher(0, 1) = 0.0f; Spher(0, 2) = 0.0f;
            Spher(1, 0) = 0.0f; Spher(1, 1) = 0.0f; Spher(1, 2) = 0.0f;
            Spher(2, 0) = 0.0f; Spher(2, 1) = 0.0f; Spher(2, 2) = 0.0f;

            tot_Eng = 0;
            mass_holder = 0;

            // Loop over all particles in event
            for (int ipart = 0; ipart < std::max(ktrk, kshwr); ipart+=1) {                     
                
                p_i_holder = 0;
                p_j_holder = 0;
                dcos_i_holder[0]=0;dcos_i_holder[1]=0;dcos_i_holder[2]=0;
                dcos_j_holder[0]=0;dcos_j_holder[1]=0;dcos_j_holder[2]=0;

                p_vec[0] = 0;
                p_vec[1] = 0;
                p_vec[2] = 0;
                
                if((is_trk[ipart] > 0.5) && (is_trk[ipart] < 1.5) && (ipart < ktrk)){
                    // Cut poorly constructed tracks
                    if (trk_momrange_reco[ipart] < 0) continue;
                    if ((std::pow(trk_start_xhat[ipart],2)+std::pow(trk_start_yhat[ipart],2)+std::pow(trk_start_zhat[ipart],2))>1.1) continue;
                    
                    // Multiplicity
                    num_tracks += 1;
                    plane = (trk_pida[ipart][2] > 0) ? 2 : trk_bestplane[ipart]; 
                    if (trk_pida[ipart][plane] > 10) {
                        num_p++;
                        mass_holder = m_p;
                    } else { 
                        num_mu++;
                        mass_holder = m_mu;
                    }

                    // Kinematics
                    plane = (trk_dedx_reco[ipart][2][0] > 0) ? 2 : trk_bestplane[ipart]; 
                    for(int iwire = 1; iwire<ktrkhits[ipart][plane]; iwire++){
                        //cout << trk_dedx_reco[ipart][plane][iwire] << endl;
                        if(trk_dedx_reco[ipart][plane][iwire] > 100) continue;

                        dx = trk_xyz[ipart][plane][iwire][0] - trk_xyz[ipart][plane][iwire-1][0];
                        dy = trk_xyz[ipart][plane][iwire][1] - trk_xyz[ipart][plane][iwire-1][1];
                        dz = trk_xyz[ipart][plane][iwire][2] - trk_xyz[ipart][plane][iwire-1][2];
                        dr = std::sqrt(std::pow(dx,2) + std::pow(dy,2) + std::pow(dz,2));

                        trk_vis_eng += trk_dedx_reco[ipart][plane][iwire] * dr;
                    }
                    
                    p_vec[0] = trk_momrange_reco[ipart] * 1e3 * trk_start_xhat[ipart]; // MeV
                    p_vec[1] = trk_momrange_reco[ipart] * 1e3 * trk_start_yhat[ipart];
                    p_vec[2] = trk_momrange_reco[ipart] * 1e3 * trk_start_zhat[ipart];
                    tot_p_vec[0] += p_vec[0]; tot_p_vec[1] += p_vec[1]; tot_p_vec[2] += p_vec[2];

                    tot_Eng += std::sqrt(std::pow(mass_holder,2) + std::pow(trk_momrange_reco[ipart]*1e3, 2));
                    
                    // Event Shape
                    Spher(0,0) += p_vec[0]*p_vec[0]; Spher(0,1) += p_vec[0]*p_vec[1]; Spher(0,2) += p_vec[0]*p_vec[2];
                    Spher(1,0) += p_vec[1]*p_vec[0]; Spher(1,1) += p_vec[1]*p_vec[1]; Spher(1,2) += p_vec[1]*p_vec[2];
                    Spher(2,0) += p_vec[2]*p_vec[0]; Spher(2,1) += p_vec[2]*p_vec[1]; Spher(2,2) += p_vec[2]*p_vec[2];
                    sum_p_sqrd += std::pow(trk_momrange_reco[ipart]*1e3, 2);

                    p_i_holder = trk_momrange_reco[ipart] * 1e3;
                    dcos_i_holder[0]=trk_start_xhat[ipart];dcos_i_holder[1]=trk_start_yhat[ipart];dcos_i_holder[2]=trk_start_zhat[ipart];

                } else if((is_shwr[ipart] > 0.5) && (is_shwr[ipart] < 1.5) && (ipart < kshwr)) {
                    // Cut poorly constructed showers
                    plane = (shwr_totEng_reco[ipart][2] > 0) ? 2 : shwr_bestplane[ipart];                 
                    if (shwr_totEng_reco[ipart][plane] < 0) continue;
                    if ((std::pow(shwr_start_xhat[ipart],2)+std::pow(shwr_start_yhat[ipart],2)+std::pow(shwr_start_zhat[ipart],2))>1.1) continue;

                    // Multiplicity
                    num_showers += 1;
                    
                    // Kinematics
                    shwr_vis_eng += shwr_totEng_reco[ipart][plane];
                    
                    p_vec[0] = shwr_totEng_reco[ipart][plane] * shwr_start_xhat[ipart]; // MeV
                    p_vec[1] = shwr_totEng_reco[ipart][plane] * shwr_start_yhat[ipart];
                    p_vec[2] = shwr_totEng_reco[ipart][plane] * shwr_start_zhat[ipart];
                    tot_p_vec[0] += p_vec[0]; tot_p_vec[1] += p_vec[1]; tot_p_vec[2] += p_vec[2];

                    tot_Eng += std::abs(shwr_totEng_reco[ipart][plane]);

                    // Event Shape
                    Spher(0,0) += p_vec[0]*p_vec[0]; Spher(0,1) += p_vec[0]*p_vec[1]; Spher(0,2) += p_vec[0]*p_vec[2];
                    Spher(1,0) += p_vec[1]*p_vec[0]; Spher(1,1) += p_vec[1]*p_vec[1]; Spher(1,2) += p_vec[1]*p_vec[2];
                    Spher(2,0) += p_vec[2]*p_vec[0]; Spher(2,1) += p_vec[2]*p_vec[1]; Spher(2,2) += p_vec[2]*p_vec[2];
                    sum_p_sqrd += std::pow(shwr_totEng_reco[ipart][plane],2);

                    p_i_holder = shwr_totEng_reco[ipart][plane];
                    dcos_i_holder[0]=shwr_start_xhat[ipart];dcos_i_holder[1]=shwr_start_yhat[ipart];dcos_i_holder[2]=shwr_start_zhat[ipart];
                }

                // Fox-Wolfram Moment calculation loop
                for (int jpart=0; jpart < std::max(ktrk, kshwr); jpart++) {
                    if ((is_trk[jpart] > 0.5) && (is_trk[jpart] < 1.5) && (jpart < ktrk)) {
                        if (trk_momrange_reco[jpart] < 0) continue;
                        if ((std::pow(trk_start_xhat[jpart],2)+std::pow(trk_start_yhat[jpart],2)+std::pow(trk_start_zhat[jpart],2))>1.1) continue;
                        p_j_holder = trk_momrange_reco[jpart] * 1e3;
                        dcos_j_holder[0]=trk_start_xhat[jpart];dcos_j_holder[1]=trk_start_yhat[jpart];dcos_j_holder[2]=trk_start_zhat[jpart];
                    } else if((is_shwr[jpart] > 0.5) && (is_shwr[jpart] < 1.5) && (jpart < kshwr)) {
                        plane = (shwr_totEng_reco[jpart][2] > 0) ? 2 : shwr_bestplane[jpart];
                        if (shwr_totEng_reco[jpart][plane] < 0) continue;
                        if ((std::pow(shwr_start_xhat[jpart],2)+std::pow(shwr_start_yhat[jpart],2)+std::pow(shwr_start_zhat[jpart],2))>1.1) continue;
                        p_j_holder = shwr_totEng_reco[jpart][plane];
                        dcos_j_holder[0]=shwr_start_xhat[jpart];dcos_j_holder[1]=shwr_start_yhat[jpart];dcos_j_holder[2]=shwr_start_zhat[jpart];
                    }

                    cos_theta_ij = dcos_i_holder[0]*dcos_j_holder[0] + 
                                    dcos_i_holder[1]*dcos_j_holder[1] + 
                                    dcos_i_holder[2]*dcos_j_holder[2];
                    FW0 += p_i_holder * p_j_holder;
                    FW1 += p_i_holder * p_j_holder * (cos_theta_ij); // x
                    FW2 += p_i_holder * p_j_holder * (0.5*((3*std::pow(cos_theta_ij,2))-1)); // 1/2*(3x^2-1)
                }
            }

            // Add calculated variables to tree
            num_particles = num_tracks + num_showers;

            tot_vis_eng = shwr_vis_eng + trk_vis_eng;
            
            tot_momentum = std::sqrt(std::pow(tot_p_vec[0],2) + std::pow(tot_p_vec[1],2) + std::pow(tot_p_vec[2],2));
            
            inv_mass = std::sqrt((std::pow(tot_Eng,2) - std::pow(tot_momentum,2)));

            Spher *= 1/sum_p_sqrd;
            TMatrixDEigen eigen(Spher);
            TVectorD eigenvalues = eigen.GetEigenValuesRe();
            sphericity = 3/2 * (eigenvalues[1] + eigenvalues[2]);
            aplanarity = 3/2 * eigenvalues[2];

            FW0 /= std::pow(tot_vis_eng,2);
            FW1 /= std::pow(tot_vis_eng,2);
            FW2 /= std::pow(tot_vis_eng,2);

            // Fill Tree
            vartree->Fill();

        }

        // Write the new tree to the file
        varfile->Write();
    }
}
