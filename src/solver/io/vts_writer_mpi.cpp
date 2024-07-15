#include "vts_writer_mpi.h"

void VTSWriterMPI::write_to_file(const std::string& filename)
{
    // Create a vtkProgrammableFilter
    vtkSmartPointer<vtkProgrammableFilter> programmable_filter = vtkSmartPointer<vtkProgrammableFilter>::New();

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

    programmable_filter->SetInputData(vtk_structured_grid);

    vtk_structured_grid->SetExtent(global_extent);
    vtk_structured_grid->SetPoints(points);

    vtk_structured_grid->GetPointData()->AddArray(velocity);
    vtk_structured_grid->GetPointData()->AddArray(pressure);

    std::cout << "Writing vts: " << filename + ".vts" << std::endl;

    // Create the parallel writer and call some functions
    auto parallel_writer = vtkSmartPointer<vtkXMLPStructuredGridWriter>::New();
    parallel_writer->SetInputConnection(programmable_filter->GetOutputPort());
    parallel_writer->SetController(contr);
    parallel_writer->SetFileName((filename + ".pvts").c_str());
    parallel_writer->SetNumberOfPieces(mpi_size);
    parallel_writer->SetStartPiece(mpi_rank);
    parallel_writer->SetEndPiece(mpi_rank);
    parallel_writer->SetDataModeToAppended();
    parallel_writer->SetCompressorTypeToNone();
    parallel_writer->EncodeAppendedDataOff();
    parallel_writer->Update();
    parallel_writer->Write();

    if (parallel_writer->GetErrorCode() != vtkErrorCode::NoError)
    {
        std::cout << "Writing to file failed: " << filename + ".vts" << std::endl;
    }
}