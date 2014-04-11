/*
 * =====================================================================================
 *
 *       Filename:  Tree.h
 *
 *    Description:  LeptonAnalysis tool
 *
 *        Version:  1.0
 *        Created:  04/11/2014 03:39:21 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "ProgressBar.hh"

#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TFile.h>

#include <cmath>
#include <iostream>
using std::abs;

class Tree {

   public:
      // Constructor
      Tree(const std::string& fileName, const std::string& treeName);

      // Sets private branches and histograms below
      void selectBranches();
      void defineHistograms();

      // Traverses tree
      void traverseTree();

      // Write histograms to file
      void writeHistograms(const std::string& fileName) const;


   private:
      TTree *tree;

      // Branches
      Int_t branchTime;
      Float_t branchEcalEnergy;
      Float_t branchTrackerRigidity;
      Float_t branchEcalBDT;

      // Histograms
      TH1D* histogramElectronRateHighEnergy;
      TH1D* histogramElectronRateHighEnergyRTI;

      // Cuts
      bool isElectron() const;
      bool isProton() const;
      bool isEcalEnergyLow() const;
      bool isEcalEnergyHigh() const;

      static const int StartTime = 1307303287;
      static const int EndTime = 1385483852;

      TH1F *liveTime;
      void readLiveTimeFile();

};
