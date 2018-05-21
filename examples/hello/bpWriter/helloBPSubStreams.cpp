/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * helloBPSubStreams.cpp
 *
 *  Created on: Feb 21, 2018
 *      Author: William F Godoy godoywf@ornl.gov
 */

#include <ios>      //std::ios_base::failure
#include <iostream> //std::cout
#include <mpi.h>
#include <stdexcept> //std::invalid_argument std::exception
#include <vector>

#include <adios2.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /** Application variable */
    std::vector<float> myFloats = {0, 1}; //, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> myInts = {0, -1, -2, -3, -4, -5, -6, -7, -8, -9};
    const std::size_t Nx = myFloats.size();

    const std::string myString("Hello Variable String from rank " +
                               std::to_string(rank));

    try
    {
        /** ADIOS class factory of IO class objects, DebugON is recommended */
        adios2::ADIOS adios(MPI_COMM_WORLD, adios2::DebugON);

        /*** IO class object: settings and factory of Settings: Variables,
         * Parameters, Transports, and Execution: Engines */
        adios2::IO &bpIO = adios.DeclareIO("BPFile_N2M");
        bpIO.SetParameter("SubStreams", "2");

        /** global array : name, { shape (total) }, { start (local) }, {
         * count
         * (local) }, all are constant dimensions */
        adios2::Variable<float> &bpFloats = bpIO.DefineVariable<float>(
            "bpFloats", {size * Nx}, {rank * Nx}, {Nx}, adios2::ConstantDims);

        //        adios2::Variable<int> &bpInts = bpIO.DefineVariable<int>(
        //            "bpInts", {size * Nx}, {rank * Nx}, {Nx},
        //            adios2::ConstantDims);
        //
        //        adios2::Variable<std::string> &bpString =
        //            bpIO.DefineVariable<std::string>("bpString");

        adios2::Attribute<int> &attribute =
            bpIO.DefineAttribute<int>("attrINT", -1);

        /** Engine derived class, spawned to start IO operations */
        adios2::Engine &bpFileWriter =
            bpIO.Open("myVector_cpp.bp", adios2::Mode::Write);

        for (unsigned int t = 0; t < 2; ++t)
        {
            bpFileWriter.BeginStep();

            // bpFileWriter.PutDeferred(bpInts, myInts.data());

            myFloats[0] = static_cast<float>(t);
            myFloats[1] = static_cast<float>(rank);
            bpFileWriter.PutDeferred<float>(bpFloats, myFloats.data());

            // bpFileWriter.PutDeferred(bpString, myString);
            bpFileWriter.EndStep();
        }

        /** Create bp file, engine becomes unreachable after this*/
        bpFileWriter.Close();
    }
    catch (std::invalid_argument &e)
    {
        std::cout << "Invalid argument exception, STOPPING PROGRAM from rank "
                  << rank << "\n";
        std::cout << e.what() << "\n";
    }
    catch (std::ios_base::failure &e)
    {
        std::cout << "IO System base failure exception, STOPPING PROGRAM "
                     "from rank "
                  << rank << "\n";
        std::cout << e.what() << "\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Exception, STOPPING PROGRAM from rank " << rank << "\n";
        std::cout << e.what() << "\n";
    }

    MPI_Finalize();

    return 0;
}
