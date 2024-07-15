#include "io/vts_writer_mpi.h"

int main(int argc, char* argv[])
{
    int mpi_rank = 0, mpi_size = 1;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    constexpr int    Nx = 8;
    constexpr int    Ny = 8;
    constexpr int    Nz = 8;
    constexpr double H  = 0.01;

    FluidGrid3D fluid_grid = CREATE_GRID3D(Nx, Ny, Nz, H);

    VTSWriterMPI writer(Nx, Ny, Nz, H, argc, argv);

    writer.write(fluid_grid, "test_output");

    MPI_Finalize();

    return 0;
}