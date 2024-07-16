#pragma once

#include "vts_writer.h"

#include <mpi.h>
#include <vtkInformation.h>
#include <vtkMPIController.h>
#include <vtkProgrammableFilter.h>
#include <vtkXMLPStructuredGridWriter.h>

class VTSWriterMPI : public VTSWriter
{
private:
    int local_extent[6];
    int mpi_rank = 0, mpi_size = 1;

    vtkSmartPointer<vtkMPIController>      contr;

    struct Args
    {
        vtkProgrammableFilter* programmable_filter;
        int                    local_extent[6];
    };

public:
    VTSWriterMPI(int Nx, int Ny, int Nz, double h, int argc, char* argv[])
        : VTSWriter(Nx, Ny, Nz, h)
    {
        // Create and Initialize vtkMPIController
        contr = vtkSmartPointer<vtkMPIController>::New();
        contr->Initialize(&argc, &argv, 1);

        MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
        MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

        global_extent[0] = 0;
        global_extent[1] = mpi_size * Nx - 1;
        global_extent[2] = 0;
        global_extent[3] = Ny - 1;
        global_extent[4] = 0;
        global_extent[5] = Nz - 1;

        local_extent[0] = mpi_rank * Nx;
        local_extent[1] = (mpi_rank + 1) * Nx - 1;
        local_extent[2] = 0;
        local_extent[3] = Ny - 1;
        local_extent[4] = 0;
        local_extent[5] = Nz - 1;

        populate_to_position(Nx, Ny, Nz, h, mpi_rank * Nx * h);
    }

    ~VTSWriterMPI()
    {
        delete[] position_x;
        delete[] position_y;
        delete[] position_z;
    }

protected:
    void write_to_file(const std::string& filename) override;
};