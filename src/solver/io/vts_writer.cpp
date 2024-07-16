#include "vts_writer.h"

void VTSWriter::write(FluidGrid3D& grid, const std::string& filename)
{
    populate_to_grid(grid);

    write_to_file(filename);
}

void VTSWriter::populate_to_position(int    Nx,
                                     int    Ny,
                                     int    Nz,
                                     double h,
                                     double offset_x,
                                     double offset_y,
                                     double offset_z)
{
    assert(Nx > 0);
    assert(Ny > 0);
    assert(Nz > 0);
    assert(h > 0.0);

    position_x = new double[Nx];
    position_y = new double[Ny];
    position_z = new double[Nz];

    for (int i = 0; i < Nx; i++)
    {
        position_x[i] = offset_x + i * h + 0.5 * h;
    }

    for (int j = 0; j < Ny; j++)
    {
        position_y[j] = offset_y + j * h + 0.5 * h;
    }

    for (int k = 0; k < Nz; k++)
    {
        position_z[k] = offset_z + k * h + 0.5 * h;
    }

    points = vtkSmartPointer<vtkPoints>::New();

    // Paraview need Fortran order
    for (int k = 0; k < Nz; k++)
    {
        for (int j = 0; j < Ny; j++)
        {
            for (int i = 0; i < Nx; i++)
            {
                double x = position_x[i];
                double y = position_y[j];
                double z = position_z[k];
                points->InsertNextPoint(x, y, z);
            }
        }
    }
}

void VTSWriter::populate_to_grid(FluidGrid3D& grid)
{
    EXPOSE_GRID3D_PARA(grid);

    auto& u = grid.u;
    auto& v = grid.v;
    auto& w = grid.w;
    auto& p = grid.p;

    vtk_structured_grid = vtkSmartPointer<vtkStructuredGrid>::New();

    velocity = vtkSmartPointer<vtkDoubleArray>::New();
    velocity->SetName("Velocity");
    velocity->SetNumberOfComponents(3);
    velocity->SetNumberOfTuples(Nx * Ny * Nz);

    pressure = vtkSmartPointer<vtkDoubleArray>::New();
    pressure->SetName("Pressure");
    pressure->SetNumberOfComponents(1);
    pressure->SetNumberOfTuples(Nx * Ny * Nz);

    std::size_t idx = 0;

    std::cout << "Populating data for vts." << std::endl;

    // Paraview need Fortran order
    for (int k = 0; k < Nz; k++)
    {
        for (int j = 0; j < Ny; j++)
        {
            for (int i = 0; i < Nx; i++)
            {
                double uc = (u(i + usi - 1, j + usj, k + usk) + u(i + usi, j + usj, k + usk)) / 2.0;
                double vc = (v(i + vsi, j + vsj - 1, k + vsk) + v(i + vsi, j + vsj, k + vsk)) / 2.0;
                double wc = (w(i + wsi, j + wsj, k + wsk - 1) + w(i + wsi, j + wsj, k + wsk)) / 2.0;
                double pc = p(i, j, k);

                velocity->SetTuple3(k * Ny * Nx + j * Nx + i, uc, vc, wc);
                pressure->SetTuple1(k * Ny * Nx + j * Nx + i, pc);
                idx++;
            }
        }
    }
}

void VTSWriter::write_to_file(const std::string& filename)
{
    vtk_structured_grid->SetExtent(global_extent);
    vtk_structured_grid->SetPoints(points);

    vtk_structured_grid->GetPointData()->AddArray(velocity);
    vtk_structured_grid->GetPointData()->AddArray(pressure);

    std::cout << "Writing vts: " << filename + ".vts" << std::endl;

    vtkSmartPointer<vtkXMLStructuredGridWriter> writer = vtkSmartPointer<vtkXMLStructuredGridWriter>::New();
    writer->SetDataModeToBinary();
    writer->SetFileName((filename + ".vts").c_str());
    writer->SetInputData(vtk_structured_grid);
    writer->Write();

    if (writer->GetErrorCode() != vtkErrorCode::NoError)
    {
        std::cout << "Writing to file failed: " << filename + ".vts" << std::endl;
    }
}