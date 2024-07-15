#pragma once

#include "field3.hpp"

struct FluidGrid3D
{
    const int Nx;
    const int Ny;
    const int Nz;

    const int Nux_inner; // inner field dimension on x of velocity u
    const int Nuy_inner; // inner field dimension on y of velocity u
    const int Nuz_inner; // inner field dimension on z of velocity u

    const int Nvx_inner; // inner field dimension on x of velocity v
    const int Nvy_inner; // inner field dimension on y of velocity v
    const int Nvz_inner; // inner field dimension on z of velocity v

    const int Nwx_inner; // inner field dimension on x of velocity w
    const int Nwy_inner; // inner field dimension on y of velocity w
    const int Nwz_inner; // inner field dimension on z of velocity w

    const int usi; // inner field start position x of velocity u
    const int usj; // inner field start position y of velocity u
    const int usk; // inner field start position z of velocity u

    const int vsi; // inner field start position x of velocity v
    const int vsj; // inner field start position y of velocity v
    const int vsk; // inner field start position z of velocity v

    const int wsi; // inner field start position x of velocity w
    const int wsj; // inner field start position y of velocity w
    const int wsk; // inner field start position z of velocity w

    const double h;

    field3 u;
    field3 v;
    field3 w;
    field3 p;
};

#define CREATE_GRID3D(Nx, Ny, Nz, h)                             \
    {                                                            \
        Nx,                                      /* Nx */        \
            Ny,                                  /* Ny */        \
            Nz,                                  /* Nz */        \
            Nx - 1,                              /* Nux_inner */ \
            Ny,                                  /* Nuy_inner */ \
            Nz,                                  /* Nuz_inner */ \
            Nx,                                  /* Nvx_inner */ \
            Ny - 1,                              /* Nvy_inner */ \
            Nz,                                  /* Nvz_inner */ \
            Nx,                                  /* Nwx_inner */ \
            Ny,                                  /* Nwy_inner */ \
            Nz - 1,                              /* Nwz_inner */ \
            1,                                   /* usi */       \
            1,                                   /* usj */       \
            1,                                   /* usk */       \
            1,                                   /* vsi */       \
            1,                                   /* vsj */       \
            1,                                   /* vsk */       \
            1,                                   /* wsi */       \
            1,                                   /* wsj */       \
            1,                                   /* wsk */       \
            h,                                   /* h */         \
            field3(Nx + 1, Ny + 2, Nz + 2, "u"), /* u */         \
            field3(Nx + 2, Ny + 1, Nz + 2, "v"), /* v */         \
            field3(Nx + 2, Ny + 2, Nz + 1, "w"), /* w */         \
            field3(Nx, Ny, Nz, "p")              /* p */         \
    }

#define EXPOSE_GRID3D_PARA(grid)             \
    const int    Nx        = grid.Nx;        \
    const int    Ny        = grid.Ny;        \
    const int    Nz        = grid.Nz;        \
    const int    Nux_inner = grid.Nux_inner; \
    const int    Nuy_inner = grid.Nuy_inner; \
    const int    Nuz_inner = grid.Nuz_inner; \
    const int    Nvx_inner = grid.Nvx_inner; \
    const int    Nvy_inner = grid.Nvy_inner; \
    const int    Nvz_inner = grid.Nvz_inner; \
    const int    Nwx_inner = grid.Nwx_inner; \
    const int    Nwy_inner = grid.Nwy_inner; \
    const int    Nwz_inner = grid.Nwz_inner; \
    const int    usi       = grid.usi;       \
    const int    usj       = grid.usj;       \
    const int    usk       = grid.usk;       \
    const int    vsi       = grid.vsi;       \
    const int    vsj       = grid.vsj;       \
    const int    vsk       = grid.vsk;       \
    const int    wsi       = grid.wsi;       \
    const int    wsj       = grid.wsj;       \
    const int    wsk       = grid.wsk;       \
    const double h         = grid.h;
