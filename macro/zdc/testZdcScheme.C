void testZdcScheme ()
{
  TStopwatch timer;
  timer.Start();
  gDebug=0;


   gROOT->LoadMacro("$VMCWORKDIR/macro/run/bmnloadlibs.C");
   bmnloadlibs(); // load libraries


  Int_t iVerbose = 2;
  Int_t nEvents = 1;

  TString inFile = "evetest.root";             
  TString parFile = "evetest.root";
  TString outFile = "tmp.root";


  FairRunAna *fRun = new FairRunAna();
  fRun->SetInputFile(inFile);
  fRun->SetOutputFile(outFile);


  FairRuntimeDb *rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo *io1 = new FairParRootFileIo();
  io1->open(parFile.Data());
  rtdb->setFirstInput(io1);

   fRun->Init();

   rtdb->activateParIo(rtdb->getFirstInput());
  BmnZdcGeoPar *p=( BmnZdcGeoPar*) gROOT->FindObject("BmnZdcGeoPar");
  p->print();
  fDigiScheme  = BmnZdcDigiScheme::Instance();
  fDigiScheme->Init(p,0,iVerbose);

  fDigiScheme->Print();

  fDigiScheme->PrintVolume (33,33);	
 	
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  printf("RealTime=%f seconds, CpuTime=%f seconds\n",rtime,ctime);
}