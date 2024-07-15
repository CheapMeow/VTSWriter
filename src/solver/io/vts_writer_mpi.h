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
    vtkSmartPointer<vtkProgrammableFilter> programmable_filter;

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

        local_extent[0] = mpi_rank * Nx;
        local_extent[1] = (mpi_rank + 1) * Nx;
        local_extent[2] = 0;
        local_extent[3] = Ny;
        local_extent[4] = 0;
        local_extent[5] = Nz;

        // Create a vtkProgrammableFilter
        programmable_filter = vtkSmartPointer<vtkProgrammableFilter>::New();

        // Initialize an instance of Args
        Args args;
        args.programmable_filter = programmable_filter;
        for (int i = 0; i < 6; ++i)
            args.local_extent[i] = local_extent[i];

        programmable_filter->SetExecuteMethod(
            [](void* arg) {
                Args*              args       = reinterpret_cast<Args*>(arg);
                auto               info       = args->programmable_filter->GetOutputInformation(0);
                auto               output_tmp = args->programmable_filter->GetOutput();
                auto               input_tmp  = args->programmable_filter->GetInput();
                vtkStructuredGrid* output     = dynamic_cast<vtkStructuredGrid*>(output_tmp);
                vtkStructuredGrid* input      = dynamic_cast<vtkStructuredGrid*>(input_tmp);
                output->ShallowCopy(input);
                output->SetExtent(args->local_extent);
            },
            &args);

        populate_to_position(Nx, Ny, Nz, h, mpi_rank * Nx * h);
    }

    ~VTSWriterMPI()
    {
        delete[] position_x;
        delete[] position_y;
        delete[] position_z;

        contr->Finalize();
    }

protected:
    void write_to_file(const std::string& filename) override;
};