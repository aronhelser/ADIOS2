#------------------------------------------------------------------------------#
# Distributed under the OSI-approved Apache License, Version 2.0.  See
# accompanying file Copyright.txt for details.
#------------------------------------------------------------------------------#

add_subdirectory(bp)
add_subdirectory(skeleton)

if(ADIOS2_HAVE_HDF5)
  add_subdirectory(hdf5)
endif()

if(ADIOS2_HAVE_DataMan)
  add_subdirectory(dataman)
endif()

if(ADIOS2_HAVE_SST)
  add_subdirectory(sst)
endif()

if(ADIOS2_HAVE_MPI)
  add_subdirectory(insitumpi)
  add_subdirectory(staging-common)
endif()

