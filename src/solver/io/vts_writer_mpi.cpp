#include "vts_writer_mpi.h"

void VTSWriterMPI::write_to_file(const std::string& filename)
{
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