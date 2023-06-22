#include <cmath>

void TimeCompare( Int_t nrun=1372, Int_t nblock=0) {
    // if full replay : fullreplay = 1;
    // if single event: nevent = event number, replay = 1, fullreplay != 1;
    // if 20k replay: replay = 20k, fullreplay != 1;
    gStyle->SetOptFit(3);
    // --------------------------------------------------------------------------------
    
    TFile *f = new TFile(Form("../ROOTfiles/nps_eel108_%d.root", nrun));
    TChain *t = new TChain("T");
    t->Add(f->GetName(),-1);
    t->SetBranchStatus("*", false); // disable all branches
    t->SetBranchStatus("NPS.cal.fly.adcSampPulseAmp", true);
    t->SetBranchStatus("Ndata.NPS.cal.fly.adcCounter", true);
    t->SetBranchStatus("NPS.cal.fly.adcCounter", true);
    t->SetBranchStatus("NPS.cal.fly.adcSampPulseInt", true);
    t->SetBranchStatus("NPS.cal.fly.adcSampPed", true);
    t->SetBranchStatus("NPS.cal.fly.adcSampPulseTime", true);
    t->SetBranchStatus("NPS.cal.fly.adcPulseTime",true);

    
    // Branch declaration -------------------------------------------------------------
    
    //Int_t NSampWaveForm;
    //t->SetBranchAddress("Ndata.NPS.cal.fly.adcSampWaveform",&NSampWaveForm);
    //Double_t SampWaveForm[300000];
    //t->SetBranchAddress("NPS.cal.fly.adcSampWaveform",&SampWaveForm);
    Int_t NadcCounter;
    t->SetBranchAddress("Ndata.NPS.cal.fly.adcCounter",&NadcCounter);
    Double_t adcCounter[2000];
    t->SetBranchAddress("NPS.cal.fly.adcCounter",&adcCounter);
    Double_t SamplePulsetime[2000];
    t->SetBranchAddress("NPS.cal.fly.adcSampPulseTime",&SamplePulsetime);
    Double_t Pulsetime[2000];
    t->SetBranchAddress("NPS.cal.fly.adcPulseTime",&Pulsetime);
    Double_t Amp[2000];
    t->SetBranchAddress("NPS.cal.fly.adcSampPulseAmp",&Amp);

    // Get some useful variables-------------------------------------------------------

    Int_t binnumber = 120;
    
    //Histogram------------------------------------------------------------------------
    TH1D *TimePulse = new TH1D("TimePulse","Pulse_Timing_Distribution",480,0,480); // X = # of blocks
    TH1D *TimeSample = new TH1D("TimeSample","Sample_Timing_Distribution",480,0,480); // X = # of blocks

    //Filling Histogram
    Long64_t nentries = t->GetEntries();

    cout <<"Total Event Number is "<< nentries << endl;
    for(Int_t i=0; i<nentries; i++){
        t->GetEntry(i);
        cout << "This is 1st iteration " << i << endl;
        for(Int_t j=0; j<NadcCounter; j++){
            if(adcCounter[j] == nblock){
                TimePulse->Fill(Pulsetime[j]);
                TimeSample->Fill(SamplePulsetime[j]);
		cout << "Pulse is " << Pulsetime[j] << "Sample is "<< SamplePulsetime[j] << endl;
            }
        }
    }
    TCanvas *c1 = new TCanvas("c1","",1600,800);
    c1->Divide(2,1);
    c1->cd(1);
    TimePulse->Draw();
    c1->cd(2);
    TimeSample->Draw();

    c1->SaveAs(Form("../TimeComparePlots/TimeCompare_block_%d_run_%d.pdf",nblock,nrun));
}
