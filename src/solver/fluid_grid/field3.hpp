#pragma once

#include <cassert>
#include <string>

class field3
{
protected:
    unsigned int Nx, Ny, Nz;
    unsigned int allocated_size;
    std::string  name;

public:
    double* value = nullptr;

    field3() {}

    field3(unsigned int _Nx, unsigned int _Ny, unsigned int _Nz, const std::string& _name)
        : Nx(_Nx)
        , Ny(_Ny)
        , Nz(_Nz)
        , name(_name)
    {
        allocated_size = Nx * Ny * Nz;
        value          = new double[allocated_size];

        for (unsigned int i = 0; i < allocated_size; i++)
            value[i] = 0.;
    }

    ~field3() { delete[] value; }

    double& operator()(unsigned int i, unsigned int j, unsigned int k) { return value[i * Ny * Nz + j * Nz + k]; }
};