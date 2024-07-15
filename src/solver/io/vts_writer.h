#pragma once

#include "fluid_grid/fluid_grid_3d.h"

#include <cassert>

#include <vtkDoubleArray.h>
#include <vtkErrorCode.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkStructuredGrid.h>
#include <vtkXMLStructuredGridWriter.h>

class VTSWriter
{
protected:
    int global_extent[6];

    double* position_x = nullptr;
    double* position_y = nullptr;
    double* position_z = nullptr;

    vtkSmartPointer<vtkPoints>         points;
    vtkSmartPointer<vtkDoubleArray>    velocity;
    vtkSmartPointer<vtkDoubleArray>    pressure;
    vtkSmartPointer<vtkStructuredGrid> vtk_structured_grid;

public:
    VTSWriter(int Nx, int Ny, int Nz, double h)
    {
        global_extent[0] = 0;
        global_extent[1] = Nx;
        global_extent[2] = 0;
        global_extent[3] = Ny;
        global_extent[4] = 0;
        global_extent[5] = Nz;
        populate_to_position(Nx, Ny, Nz, h);
    }

    ~VTSWriter()
    {
        delete[] position_x;
        delete[] position_y;
        delete[] position_z;
    }

    void write(FluidGrid3D& grid, const std::string& filename);

protected:
    void populate_to_position(int    Nx,
                              int    Ny,
                              int    Nz,
                              double h,
                              double offset_x = 0.0,
                              double offset_y = 0.0,
                              double offset_z = 0.0);

    void populate_to_grid(FluidGrid3D& grid);

    virtual void write_to_file(const std::string& filename);
};