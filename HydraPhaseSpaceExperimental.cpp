/*----------------------------------------------------------------------------
 *
 *   Copyright (C) 2016 Antonio Augusto Alves Junior
 *
 *   This file is part of Hydra Data Analysis Framework.
 *
 *   Hydra is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Hydra is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Hydra.  If not, see <http://www.gnu.org/licenses/>.
 *
 *---------------------------------------------------------------------------*/
/*
 * HydraPhaseSpaceExample.cu
 *
 *  Created on: Sep 22, 2016
 *      Author: Antonio Augusto Alves Junior
 *      Modified: Henry Fredrick Schreiner III
 */


#include <iostream>
#include <assert.h>
#include <time.h>
#include <vector>
#include <array>
#include <chrono>

//command line
#include <CLI/CLI.hpp>
#include <CLI/Timer.hpp>

//this lib
#include <hydra/Types.h>
#include <hydra/experimental/Vector4R.h>
#include <hydra/experimental/PhaseSpace.h>
#include <hydra/experimental/Events.h>
#include <hydra/experimental/Chain.h>
#include <hydra/Evaluate.h>
#include <hydra/Function.h>
#include <hydra/FunctorArithmetic.h>
#include <hydra/FunctionWrapper.h>
#include <hydra/Copy.h>

using hydra::GReal_t;

GInt_t main(int argc, char** argv) {

    CLI::App app{"Command line arguments for HydraRandomExample"};

	size_t  nentries = 5e6;
    app.add_option("-n,--number-of-events", nentries, "Number of events", true);

	GReal_t mother_mass = 0;
	app.add_option("-P,--parent-mass", mother_mass, "Mass of mother particle")
        ->required();

	GReal_t daughter1_mass = 0;
	app.add_option("-A,--daughter-A-mass", daughter1_mass
				"Mass of daughter particle 'A' [P -> A B [C -> a b]]")
        ->required();

	GReal_t daughter2_mass = 0;
	app.add_option("-B,--daughter-B-mass", daughter2_mass
				"Mass of daughter particle 'B' [P -> A B [C -> a b]]")
        ->required();

	GReal_t daughter3_mass = 0;
	app.add_option("-C,--daughter-C-mass", daughter3_mass
				"Mass of daughter particle 'C' [P -> A B [C -> a b]]")
        ->required();


	GReal_t gand_daughter1_mass = 0;
	app.add_option("-a,--grand-daughter-a-mass", gand_daughter1_mass
				"Mass of grand-daughter particle 'a' [P -> A B [C -> a b]]")
        ->required();

	GReal_t gand_daughter2_mass = 0;
	app.add_option("-b,--grand-daughter-b-mass", gand_daughter2_mass
				"Mass of grand-daughter particle 'b' [P -> A B [C -> a b]]")
        ->required();


    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e){
        return app.exit(e);
    }

    CLI::AutoTimer total_timer{"Total time taken: "};

	//----------------
	// P-> A B C
	//----------------
    
	hydra::experimental::Vector4R P(mother_mass, 0.0, 0.0, 0.0);
	GReal_t massesP[3]{daughter1_mass, daughter2_mass, daughter3_mass };

	// Create PhaseSpace object for B0-> K pi J/psi
	hydra::experimental::PhaseSpace<3> phsp_P(mother_mass, massesP);

	hydra::experimental::Events<3, device> P2ABC_Events_d(nentries);

    {
        CLI::AutoTimer timer {"P -> A B C", CLI::Timer::Big};
	    phsp_P.Generate(P, P2ABC_Events_d.begin(), P2ABC_Events_d.end());
    }

	for(size_t i=0; i<10; i++ ){
        std::cout << P2ABC_Events_d[i] << std::endl;
	}


   //----------------
   // C-> a b
   //----------------
	GReal_t massesC[2]{gand_daughter1_mass, gand_daughter2_mass };
	// Create PhaseSpace object for J/psi->mu+ mu-
	hydra::experimental::PhaseSpace<2> phsp_C(daughter1_mass , massesC);
	hydra::experimental::Events<2, device> C2ab_Events_d(nentries);


    {
        CLI::AutoTimer timer{"C -> a b"};
	    phsp_C.Generate( P2ABC_Events_d.DaughtersBegin(0), P2ABC_Events_d.DaughtersEnd(0)
			, C2ab_Events_d.begin());
    }

	for( size_t i=0; i<10; i++ ){
        std::cout << C2ab_Events_d[i] << std::endl;
	}

    std::cout << P2ABC_Events_d.GetNEvents() << std::endl;

	typedef hydra::experimental::Events<3, device> event3_t;
	typedef hydra::experimental::Events<2, device> event2_t;
	hydra::experimental::Chain<event3_t, event2_t> chain(std::move(P2ABC_Events_d), std::move(C2ab_Events_d));

	for(auto row:chain ){
        std::cout << row << std::endl;
	}



	return 0;
}