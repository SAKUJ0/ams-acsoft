/*
 * =====================================================================================
 *
 *       Filename:  TimeAnalysis.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/11/2014 03:57:19 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "Tree.h"

int main(int argc, char** argv)
{
   (void)argc; (void)argv;

   Tree *tree = new Tree("root/LeptonAnalysis_ISS_AfterSelection.root", "LeptonAnalysis");

   tree->selectBranches();
   tree->defineHistograms();

   tree->traverseTree();

   tree->writeHistograms("root/TimeAnalysis_ISS_AfterSelection_Histograms.root");

   return 0;
}
