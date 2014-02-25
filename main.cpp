/* n dimensional Fast Marching example with the main functions used */

#include <iostream>
#include <cmath>
#include <chrono>
#include <array>
#include <string>

#include "fmdata/fmcell.h"
#include "ndgridmap/ndgridmap.hpp"
#include "console/console.h"
#include "fmm/fastmarching.hpp"
#include "io/maploader.hpp"
#include "io/gridplotter.hpp"
#include "io/gridwriter.hpp"

using namespace std;
using namespace std::chrono;

int main(int argc, const char ** argv)
{
	constexpr int ndims = 2; // Setting two dimensions.
	
	console::info("Parsing input arguments.");
	string filename;
	console::parseArguments(argc,argv, "-map", filename);
	
	
	console::info("Creating grid from image.");
	nDGridMap<FMCell, ndims> grid;
	MapLoader::loadMapFromImg(filename.c_str(), grid);
	
	console::info("Showing the grid.");
	GridPlotter::plotMap(grid, 0);
	GridPlotter::plotMap(grid);
	
	
	console::info("Testing Fast Marching Method.");
	
		time_point<std::chrono::system_clock> start, end; // Time measuring.

	vector<int> init_points;
	array<int,ndims> coords = {150, 200};
	int idx;
	grid.coord2idx(coords, idx);
	grid.showIdx(coords);
	
	init_points.push_back(idx); // Setting the initial point.
	
	FastMarching<FMCell, ndims> fmm;
	fmm.setEnvironment(&grid);
		start = system_clock::now();
	fmm.setInitialPoints(init_points);
	fmm.computeFM();
		end = system_clock::now();
		double time_elapsed = duration_cast<milliseconds>(end-start).count();
		cout << "\tElapsed FM time: " << time_elapsed << " ms" << endl;
		
	console::info("Plotting the results ");
	GridPlotter::plotArrivalTimes(grid);
	
	
	console::info("Now using all black points as wave sources");
	init_points.clear();
	MapLoader::loadMapFromImg(filename.c_str(), grid, init_points); // This is the only thing that changes.
	
	FastMarching<FMCell, ndims> fmm2;
	fmm2.setEnvironment(&grid);
		start = system_clock::now();
	fmm2.setInitialPoints(init_points);
	fmm2.computeFM();
		end = system_clock::now();
		 time_elapsed = duration_cast<milliseconds>(end-start).count();
		cout << "\tElapsed FM time: " << time_elapsed << " ms" << endl;
		
	console::info("Plotting the results ");
	GridPlotter::plotArrivalTimes(grid);
	
	
	
	console::info("Saving into file test_fm.txt");
	GridWriter::saveGridValues("test_fm.txt", grid);	

    return 0;
}
