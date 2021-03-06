/*******************************************************************************
GPU OPTIMIZED MONTE CARLO (GOMC) 2.11
Copyright (C) 2016  GOMC Group
A copy of the GNU General Public License can be found in the COPYRIGHT.txt
along with this program, also can be found at <http://www.gnu.org/licenses/>.
********************************************************************************/
#include "Molecules.h"
#include "Setup.h"
#include "PDBSetup.h" //For init
#include "MolSetup.h" // ""
#include "FFSetup.h"  // ""
#include "Forcefield.h"
#include "VectorLib.h" //for transfer.
#include <algorithm> //For count.

class System;


Molecules::Molecules() : start(NULL), kIndex(NULL), countByKind(NULL),
			 chain(NULL), kinds(NULL), pairEnCorrections(NULL),
			 pairVirCorrections(NULL), printFlag(true) {}

Molecules::~Molecules(void)
{
   delete[] start;
   delete[] kIndex;
   delete[] countByKind;
   delete[] chain;
   delete[] kinds;
   delete[] pairEnCorrections;
   delete[] pairVirCorrections;
}

void Molecules::Init(Setup & setup, Forcefield & forcefield,
		     System & sys)
{
   pdb_setup::Atoms& atoms = setup.pdb.atoms;
   //Molecule kinds arrays/data.
   kindsCount = setup.mol.kindMap.size();
   countByKind = new uint[kindsCount];
   kinds = new MoleculeKind[kindsCount];

   //Molecule instance arrays/data
   count = atoms.startIdxRes.size();
   start = new uint [count+1];
   start = vect::TransferInto<uint>(start, atoms.startIdxRes);
   start[count] = atoms.x.size();
   kIndex = vect::transfer<uint>(atoms.resKinds);
   chain = vect::transfer<char>(atoms.chainLetter);
   for (uint mk = 0 ; mk < kindsCount; mk++)
   {
      countByKind[mk] =
         std::count(atoms.resNames.begin(), atoms.resNames.end(),
		    atoms.resKindNames[mk]);
      kinds[mk].Init(atoms.resKindNames[mk], setup, forcefield, sys);
   }

   if(printFlag)
   {
      //calculating netcharge of all molecule kind
     double netCharge = 0.0;
     for (uint mk = 0 ; mk < kindsCount; mk++)
     {
       netCharge += (countByKind[mk] * kinds[mk].PrintChargeInfo());
     }

     if(abs(netCharge) > 10E-7)
     {
        std::cout<< "================================================"
		 << std::endl << std::endl
		 << "Warning: Sum of the charge in the system is: "
		 << netCharge << std::endl << std::endl
		 <<  "================================================"
		 << std::endl;

	printFlag = false;
     }
   }

   //Pair Correction matrixes
   pairEnCorrections = new double[kindsCount * kindsCount];
   pairVirCorrections = new double[kindsCount * kindsCount];
   for(uint i = 0; i < kindsCount; ++i) {
      for(uint j = i; j < kindsCount; ++j) {
         pairEnCorrections[i*kindsCount + j] = 0.0;
         pairVirCorrections[i*kindsCount +j] = 0.0;
         for(uint pI = 0; pI < kinds[i].NumAtoms(); ++pI) {
            for(uint pJ = 0; pJ < kinds[j].NumAtoms(); ++pJ) {
            pairEnCorrections[i*kindsCount+j] +=
               forcefield.particles->EnergyLRC(kinds[i].AtomKind(pI),
					      kinds[j].AtomKind(pJ));
            pairVirCorrections[i*kindsCount+j] +=
               forcefield.particles->VirialLRC(kinds[i].AtomKind(pI),
					      kinds[j].AtomKind(pJ));
            }
         }
         //set other side of the diagonal
         pairEnCorrections[j*kindsCount + i] =
	    pairEnCorrections[i*kindsCount + j];
         pairVirCorrections[j*kindsCount +i] =
	    pairVirCorrections[i*kindsCount +j];
      }
   }

}
