#include <AMReX_MultiFab.H>
#include <AMReX_Geometry.H>
#include <AMReX_REAL.H>
#include <AMReX_PlotFileUtil.H>
#include <AMReX_ParmParse.H>
// #include <AMReX_VisMF.H>
#include <random>
#include <iostream>
#include "SpectralModifier.hpp" 

using namespace amrex;

// run with mpirun -n 8 ./main*.ex inputs


// Example of function.
double myFunction(double k) {
    double hubble = 1.0e-5;
    if (k<= hubble) {
        return hubble*pow(k,-1.5)/sqrt(2.0);
    } else{
        return 0.0;
        }
}

int main(int argc, char* argv[])
{
    amrex::Initialize(argc,argv);

    // Define problem domain
    int n_cell;  // Number of cells in each dimension
    int max_grid_size;
    int verbose;  // Verbosity level
    Real dx;

    // Read the params

    {
        ParmParse pp;
        pp.query("n_cell", n_cell);
        pp.query("max_grid_size", max_grid_size);
        pp.query("dx", dx);  
        pp.query("verbose", verbose);
    }

    Box domain(IntVect(0, 0, 0), IntVect(n_cell-1, n_cell-1, n_cell-1));
    
    Real x_hi = n_cell*dx;
    Real y_hi = n_cell*dx;
    Real z_hi = n_cell*dx;
    RealBox real_box({0.0,0.0,0.0}, {x_hi,y_hi,z_hi}); // Build a box by giving the coordinates of the diagonal points (ie lengths of box)

    Array<int,3> is_periodic({1,1,1}); // Periodic in all directions
    Geometry geom(domain, &real_box, CoordSys::cartesian, is_periodic.data());

    // Define MultiFab
    int ncomp = 1;  // Number of components
    int ngrow = 0;  // Number of ghost cells

    BoxArray ba(domain);
    ba.maxSize(max_grid_size);  // Adjust this as needed
    DistributionMapping dm(ba);

    MultiFab input(ba, dm, ncomp, ngrow);

    // // Instantiate your SpectralModifier class
    SpectralModifier spectral_modifier(input, geom, verbose);

    // Fill input MultiFab with random noise
    std::random_device rd;
    std::mt19937 gen(rd());
    spectral_modifier.FillInputWithRandomNoise(gen);

    // Here you would call the appropriate method on spectral_modifier
    // to process the input and fill the output
    MultiFab output = spectral_modifier.apply_func(myFunction);

    Vector<std::string> varnames = {"var0"};
    WriteSingleLevelPlotfile("plt_00000",input, varnames, geom, 0, 0.0);
    WriteSingleLevelPlotfile("plt_00001",output, varnames, geom, 1, 0.0);
    amrex::Finalize();
    return 0;
}
