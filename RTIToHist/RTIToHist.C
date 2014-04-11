#include <assert.h>

#include "ConfigHandler.hh"
#include "FileManager.hh"
#include "ObjectManager.hh"

#include <iostream>
#include <string>

#include <TH1.h>
#include <TH2.h>
#include <TFile.h>

#include "RTIData.hh"
#include "RTIReader.hh"

#include "debugging.hh"

//int start = 1337558400;
//int end = 1337644799;

/** Programm that loops only over the RTI files. no ACQt files nessecary! */
int main(int argc, char** argv)
{

  //
  // Usage / Command-Line
  //
  Utilities::ConfigHandler config;
  config.ReadCommandLine(argc, argv);

  config.SetProgramHelpText("RTILoop", "Simple program that loops over the RTI files and dumps complete LiveTime information to a ROOT file.");
  config.AddHelpExample("Loop over RTI files", "--StartUnixTime integer unix time of the first event --EndUnixTime integer unix time of the last event");


  //Default start time
  std::string StartUnixTime = "1307303287";
  config.GetValue("OPTIONS","StartUnixTime",StartUnixTime, "Start unix time for the loop");

  //Default end time
  std::string EndUnixTime = "1385483852";
  config.GetValue("OPTIONS","EndUnixTime",EndUnixTime, "End unix time for the loop");


  // Naming options
  std::string resultdir = "";
  config.GetValue("OPTIONS","resultdir",resultdir,"resultdirecotry for the output");

  std::string suffix = "";
  config.GetValue("OPTIONS","suffix",suffix,"unique suffix for the output");

  std::string prefix = "RTILiveTime";
  config.GetValue("OPTIONS","prefix",prefix,"prefix of the outputfile");

  
  // Apply cuts?
  bool applyCuts = config.GetFlag("OPTIONS","applyCuts","specify if you want to apply the cuts.");




  IO::FileManager fileManager(&config);

  if( !config.PerformChecksAfterOptionParsing() ) return EXIT_FAIL_CONFIG;


  // Get Integer-Start/End points
  int start = atoi(StartUnixTime.c_str());
  int end = atoi(EndUnixTime.c_str());

  if( applyCuts ) std::cout << "applying cuts!" << std::endl;

  Utilities::RTIReader* rti = Utilities::RTIReader::Instance();


  // Define histogram

  // Number of bins = end - start + 1
  int nbins = end-start+1;

  // To use the GetBin convention, apply offset, so that 
  // offset-start = 1
  // offset-end   = offset-start + nbins - 1
  //              =            1 + nbins - 1
  //              =                nbins
  //
  // Off-Set whole diagram by 0.5 to left, so time-stamps are center of bins.
  TH1F* liveTime = new TH1F("liveTime","live time vs time after ams start;time / s;live time", nbins, 0.5, nbins+0.5);


  // Count statistics
  int countPassed = 0;
  int countAll = 0;


  //
  // TRAVERSE
  // Start
  // 
  for( int i = start; i <= end; i++)
  {
    countAll++;
    const Utilities::RTIData* data = rti->GetRTIData(i);
    if (!data)
      continue;

    if( applyCuts ){
      if( !( data->GetTime() >= data->GetRun()) ) continue;
      if( !( data->GetRun() > 0) ) continue;
      if( !( (double)data->GetParticles()/(float)data->GetLevel1Trigger() > 0.07/1600 * data->GetLevel1Trigger()) ) continue;
      if( !( data->GetLiveTime() > 0.5)) continue;
      if( !( (double)data->GetMissedEvents()/(double)data->GetEvents() < 0.1) ) continue;
      if( !( data->GetZenith() < 40. ) ) continue;
    }

    countPassed++;

    // See explanation for offset by start above
    liveTime->SetBinContent(i - start + 1, data->GetLiveTime());
  }
  //
  // End
  //


  // Draw options
  liveTime->SetNdivisions(10);
  liveTime->SetAxisRange(0, 1.0, "Y");
  liveTime->SetMarkerStyle(1);
  liveTime->SetMarkerColor(2);
  liveTime->SetOption("P");




  // Save Root file
  QString name;
  if( !applyCuts) name = QString("noCuts_");
  if(  applyCuts) name = QString("withCuts_");

  std::string filename = Utilities::ObjectManager::MakeStandardRootFileName(resultdir, prefix, suffix);
  QString fname = name;

  fname.append(filename.c_str());

  TFile* f = new TFile(fname.toStdString().c_str(),"RECREATE");

  // Write histogram
  liveTime->Write();

  f->Close();

  // Output statistics
  if( applyCuts ) std::cout << "fraction of passed Events = " << (double)countPassed/(double)countAll << std::endl;

  return EXIT_SUCCESS;
}

