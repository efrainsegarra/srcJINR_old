
using namespace std;

void Preparation_TOF700(char *fname="bmn_run0362.root") {
    gROOT->LoadMacro("$VMCWORKDIR/macro/run/bmnloadlibs.C");
    bmnloadlibs();
    /////////////////////////////////////////////////////////////////////////////////////
    int RUN;
    sscanf(&fname[strlen(fname) - 8], "%d", &RUN);

    const char *mapping;
    if(RUN < 470) mapping="TOF700_map_Feb20_Mar11.txt";
    else mapping="TOF700_map_Mar11.txt";

    BmnTof2Raw2Digit TOF2(mapping, fname);
    TOF2.print();
    if (RUN < 470)
    {
	TOF2.SetWcut(1700);
	TOF2.SetWmax(3700);
	TOF2.SetWT0min(260);
	TOF2.SetWT0max(560);
	TOF2.SetLeadMinMax(1,-400, -250);
	TOF2.SetLeadMinMax(2,-300, -150);
	TOF2.SetLeadMinMax(3,-400, -50);
	TOF2.SetLeadMinMax(4,-120, +120);
    }
    else
    {
	TOF2.SetWcut(1700);
	TOF2.SetWmax(3700);
	TOF2.SetWT0min(640);
	TOF2.SetWT0max(710);
	TOF2.SetLeadMinMax(1,-350, -150);
	TOF2.SetLeadMinMax(2,-350, -150);
	TOF2.SetLeadMinMax(3,-350, +50);
	TOF2.SetLeadMinMax(4,-200, +200);
    }
    cout << "Process RUN:  " << RUN << endl;

    /////////////////////////////////////////////////////////////////////////////////////
    TFile *_f_in = new TFile(fname, "READ");
    TTree *_t_in = (TTree *) _f_in->Get("BMN_RAW");
    TClonesArray *t0_raw   = new TClonesArray("BmnTDCDigit");
    TClonesArray *sync_raw = new TClonesArray("BmnSyncDigit");
    TClonesArray *tof2_raw = new TClonesArray("BmnTDCDigit");
    _t_in->SetBranchAddress("bmn_t0",    &t0_raw);
    _t_in->SetBranchAddress("bmn_sync",  &sync_raw);
    _t_in->SetBranchAddress("bmn_tof700",&tof2_raw);
    /////////////////////////////////////////////////////////////////////////////////////

    for (int ev = 0; ev < _t_in->GetEntries(); ev++) {

        if ((ev % 10000) == 0) printf("%d\n", ev);

        sync_raw->Clear();
	t0_raw->Clear();

        _t_in->GetEntry(ev);

        TOF2.fillPreparation(tof2_raw, sync_raw, t0_raw);

    }
    /////////////////////////////////////////////////////////////////////////////////////
   
    _f_in->Close();

    TOF2.drawprep();
}

void select_hist()
{
  //example of macro called when a mouse event occurs in a pad.
  // Example:
  // Root > TFile f("hsimple.root");
  // Root > hpxpy.Draw();
  // Root > c1.AddExec("exsel","select_hist()");
  // When moving the mouse in the canvas, a second canvas shows the
  // projection along X of the bin corresponding to the Y position
  // of the mouse. The resulting histogram is fitted with a gaussian.
  // A "dynamic" line shows the current bin position in Y.
  // This more elaborated example can be used as a starting point
  // to develop more powerful interactive applications exploiting CINT
  // as a development engine.
  //Author: Rene Brun

  int event = gPad->GetEvent();
  if (event != 11) return;

  TObject *select = gPad->GetSelected();
  if(!select) return;
  if (!(select->InheritsFrom("TH1"))) return;

  //create or set the new canvas c2
  TCanvas *csingle = 0;
  if ((csingle = (TCanvas*)gROOT->GetListOfCanvases()->FindObject("csingle")));
  else    csingle = new TCanvas("csingle");
  csingle->cd();

  //draw slice corresponding to mouse position
  if (!(select->InheritsFrom("TH1F")))
    {
      TH1F *h = (TH1F *)select;
      h->Draw();
    }
  else if (!(select->InheritsFrom("TH2F")))
    {
      TH2F *h = (TH2F *)select;
      h->Draw();
    }
  else if (!(select->InheritsFrom("TProfile")))
    {
      TProfile *h = (TProfile *)select;
      h->Draw();
    }
  else if (!(select->InheritsFrom("TH1D")))
    {
      TH1D *h = (TH1D *)select;
      h->Draw();
    }
  else if (!(select->InheritsFrom("TH2D")))
    {
      TH2D *h = (TH2D *)select;
      h->Draw();
    }
  else if (!(select->InheritsFrom("TProfile2D")))
    {
      TProfile2D *h = (TProfile2D *)select;
      h->Draw();
    }
  else return;
  csingle->Update();
}