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
 *  PhSp.cpp
 *
 *  Created on: Oct 21, 2016
 *      Author: Henry Schreiner, Antonio Augusto Alves Junior
 */

#include <iostream>
#include <assert.h>
#include <time.h>
#include <vector>
#include <memory>
#include <chrono>

//this lib
#include <hydra/Types.h>
#include <hydra/Vector4R.h>
#include <hydra/PhaseSpace.h>
#include <hydra/Containers.h>
#include <hydra/Evaluate.h>
#include <hydra/Function.h>
#include <hydra/FunctorArithmetic.h>
#include <hydra/FunctionWrapper.h>
#include <hydra/Copy.h>

#include <CLI/CLI.hpp>
#include <CLI/Timer.hpp>


/**
 * @file
 * @example PhSp.cpp
 * @brief This is a simple file that runs a hard-coded example, Lambda_c -> pKpi. This
 * is intendend to be as simple as possible.
 *
 * Usage:
 *
 *./PhSp
**/


// These two types are used too often to write "hydra::" every time
using hydra::GInt_t;
using hydra::GReal_t;

GInt_t main(int argc, char** argv) {
    size_t  nentries       = 10000000;
    GReal_t mother_mass    = 2.28646;
    GReal_t daughter1_mass = 1.007276466879;
    GReal_t daughter2_mass = .13957018;
    GReal_t daughter3_mass = .493677;

    CLI::App app{"PhSp example in Hydra"};
    app.add_option("nentries,-n,--nentries", nentries, "Number of entries", true);
    app.add_option("-m,--mother", mother_mass, "Number of entries", true);
    app.add_option("--daughter1", daughter1_mass, "Mass of daughter 1", true);
    app.add_option("--daughter2", daughter2_mass, "Mass of daughter 2", true);
    app.add_option("--daughter3", daughter3_mass, "Mass of daughter 3", true);

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    // Masses of the particles
    hydra::Vector4R mother(mother_mass, 0.0, 0.0, 0.0);
    std::vector<GReal_t> masses_daughters{ daughter1_mass, daughter2_mass, daughter3_mass };

    // Create PhaseSpace object 
    hydra::PhaseSpace<3> phsp(mother.mass(), masses_daughters);

    // Prepare an event list
    hydra::Events<3, hydra::device> event_list(nentries);

    // Fill the event list
    {
        CLI::AutoTimer timer {"P -> A B C"};
        phsp.Generate(mother, event_list.begin(), event_list.end());
    }
        #if __NVCC__
    {
        CLI::AutoTimer timer {"cudaDeviceSynchronize()"}; 
        cudaDeviceSynchronize();
    }
        #endif

    // Copy the events to the host
    CLI::Timer time_host("Copying to host");
    hydra::Events<3, hydra::host> event_list_host(event_list);
    std::cout << time_host << std::endl;

    return 0;
}
