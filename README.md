# Spectral Modifier [in progress]
This is adapted from [amrex-tutorials' SWFFT/Poisson](https://github.com/AMReX-Codes/amrex-tutorials/tree/main/ExampleCodes/SWFFT/SWFFT_poisson) to apply a given power spectrum in Fourier space (such as [this one](https://github.com/YL-codehub/SpectralModifier/blob/704220db80c44bbb18e01ebff62b7b56e1f65ef2/test_SpectralModifier.cpp#L17)) to a 3D Gaussian random draw.

Aimed at amrex users wanting an efficient stochastic source with a given spectrum.

Current compilation settings are these of Cambridge's CSD3.
Run e.g. with:
```
mpirun -n 8 ./main3d.intel-llvm.MPI.ex inputs
```