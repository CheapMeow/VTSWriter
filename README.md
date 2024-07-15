## Usage

The repository is used to demonstrate a VTS writer, which uses `vtkXMLPStructuredGridWriter` to support MPI.

Most of code comes from [https://stackoverflow.com/questions/24123432/composing-vtk-file-from-multiple-mpi-outputs](https://stackoverflow.com/questions/24123432/composing-vtk-file-from-multiple-mpi-outputs)

## Build

The repository depends on VTK, you should have compiled VTK and configed `VTK_DIR` in your `.bashrc`.

Another dependency is MPI. You should add `mpicxx` and `mpirun` to your `.bashrc`.

To build this repository:

```shell
source build.sh
```

Run program:

```shell
mpirun -np 2 ./test
```

Debug program:

```shell
mpirun -np 2 -gtool "gdb-oneapi:0,1=attach" ./test
```