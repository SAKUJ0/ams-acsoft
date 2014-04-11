/*
 * =====================================================================================
 *
 *       Filename:  Tree.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/11/2014 03:50:59 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "Tree.h"

Tree::Tree(const std::string& fileName, const std::string& treeName) {
   TFile *file = new TFile(fileName.c_str(), "READ");

   TTree *tree;
   tree = (TTree*)file->Get(treeName.c_str());

   this->tree = tree;
}


/********************* 
 * BRANCHES
 * *******************
 */
void Tree::selectBranches()
{
   //Deactivate all branches (used branches activated below)
   tree->SetBranchStatus("*", 0);

   TBranch *branch;

   //Timestamp
   branch = tree->GetBranch("Time");
   branch->SetStatus(1);
   branch->SetAddress(&branchTime);
   
   //ECal energy
   branch = tree->GetBranch("EcalEnergy");
   branch->SetStatus(1);
   branch->SetAddress(&branchEcalEnergy);

   //Rigidity
   branch = tree->GetBranch("TrackerRigidity");
   branch->SetStatus(1);
   branch->SetAddress(&branchTrackerRigidity);

   //Ecal BDT
   branch = tree->GetBranch("EcalBDT");
   branch->SetStatus(1);
   branch->SetAddress(&branchEcalBDT);

}



/********************* 
 * HISTOGRAMS
 * *******************
 */
void Tree::defineHistograms()
{

   Float_t nbins = (EndTime - StartTime)/86400;
   histogramElectronRateHighEnergy = new TH1D("histogramElectronRate", "Electron Rate", nbins, StartTime, EndTime);
   histogramElectronRateHighEnergyRTI = new TH1D("histogramElectronRateRTI", "Electron Rate RTI", nbins, StartTime, EndTime);

}



/******************** 
 * TRAVERSE
 ********************
 */
void Tree::traverseTree()
{

   Int_t treesize = tree->GetEntriesFast();

   // Progress Bar
   Utilities::ProgressBar *progressBar = new Utilities::ProgressBar(treesize);
   progressBar->PrintScale();

   // LiveTime information from RTI
   readLiveTimeFile();

   for (Int_t i = 0; i<treesize; i++)
   {
      tree->GetEntry(i);
      progressBar->Update(i);



      // Electron & High Energy
      if(isElectron() && isEcalEnergyLow())
      {
         histogramElectronRateHighEnergy->Fill(branchTime);

         // StartTime is defined as earliest branchTime available
         // <=> StartTime = min(branchTime)
         //
         // First LiveTime information is in bin with id 1 (0 is underflow).
         // branchTime - StartTime     = 0.
         // branchTime - StartTime + 1 = 1.
         //
         Float_t weight = liveTime->GetBinContent(branchTime - StartTime + 1);
         histogramElectronRateHighEnergyRTI->Fill(branchTime, weight);
      }



   }

}



/******************** 
 * WRITE
 ********************
 */
void Tree::writeHistograms(const std::string& fileName) const
{
   
   TFile* file = new TFile(fileName.c_str(),"RECREATE");

   histogramElectronRateHighEnergy->Write();
   histogramElectronRateHighEnergyRTI->Write();

   file->Close();

}



/******************** 
 * LIVE TIME
 ********************
 */
void Tree::readLiveTimeFile()
{
   TFile *file = new TFile("root/LiveTime_vs_Timestamp.root", "READ");
   liveTime = (TH1F*)file->Get("liveTime");
}



/********************* 
 * CUTS
 * *******************
 */
bool Tree::isElectron() const
{
   return
         branchTrackerRigidity < 0
      && 0.5 < branchEcalEnergy / abs(branchTrackerRigidity)
      && branchEcalEnergy / abs(branchTrackerRigidity) < 10
      && branchEcalBDT > 0.5
      ;
}

bool Tree::isProton() const
{
   return
         branchTrackerRigidity > 0
      && branchEcalBDT < -0.9
      ;
}

bool Tree::isEcalEnergyLow() const
{
   return
         branchEcalEnergy >  8.0
      && branchEcalEnergy < 12.0
      ;
}

bool Tree::isEcalEnergyHigh() const
{
   return
         branchEcalEnergy >  50.0
      && branchEcalEnergy < 100.0
      ;
}
