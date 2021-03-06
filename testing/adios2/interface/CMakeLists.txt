#------------------------------------------------------------------------------#
# Distributed under the OSI-approved Apache License, Version 2.0.  See
# accompanying file Copyright.txt for details.
#------------------------------------------------------------------------------#

add_executable(TestADIOSInterfaceWrite TestADIOSInterfaceWrite.cpp)
target_link_libraries(TestADIOSInterfaceWrite adios2 gtest)

add_executable(TestADIOSDefineVariable TestADIOSDefineVariable.cpp)
target_link_libraries(TestADIOSDefineVariable adios2 gtest)

add_executable(TestADIOSDefineAttribute TestADIOSDefineAttribute.cpp)
target_link_libraries(TestADIOSDefineAttribute adios2 gtest)

if(ADIOS2_HAVE_MPI)
  target_link_libraries(TestADIOSInterfaceWrite MPI::MPI_C)
  target_link_libraries(TestADIOSDefineVariable MPI::MPI_C)
  target_link_libraries(TestADIOSDefineAttribute MPI::MPI_C)

  set(extra_test_args EXEC_WRAPPER ${MPIEXEC_COMMAND})
endif()

gtest_add_tests(TARGET TestADIOSInterfaceWrite ${extra_test_args})
gtest_add_tests(TARGET TestADIOSDefineVariable ${extra_test_args})
gtest_add_tests(TARGET TestADIOSDefineAttribute ${extra_test_args})
