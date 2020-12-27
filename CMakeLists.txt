cmake_minimum_required(VERSION 3.14)
project(newCpuRender LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)

include(cmake/lidanBase.cmake)
include(cmake/LidanBuild.cmake)


SET(LOG 2)
SET(ROOT_DIR ${CMAKE_SOURCE_DIR})
set(CONDIF_DIR "${CMAKE_SOURCE_DIR}/")

include(ProcessorCount)
ProcessorCount(processor_N)
if(NOT processor_N EQUAL 0)
    set(CTEST_BUILD_FLAGS -j${processor_N})
    set(ctest_test_args ${ctest_test_args} PARALLEL_LEVEL ${N})
endif()


configure_file (
        "${CMAKE_CURRENT_SOURCE_DIR}/config/config.h.in"
        "${CMAKE_CURRENT_BINARY_DIR}/config/config.h"
)

if(APPLE)
    if(CMAKE_C_COMPILER_ID MATCHES "Clang")
        set(OpenMP_C"${CMAKE_C_COMPILER}")
        set(OpenMP_C_FLAGS"-fopenmp=libomp -Wno-unused-command-line-argument")
        set(OpenMP_C_LIB_NAMES"libomp""libgomp""libiomp5")
        set(OpenMP_libomp_LIBRARY ${OpenMP_C_LIB_NAMES})
        set(OpenMP_libgomp_LIBRARY ${OpenMP_C_LIB_NAMES})
        set(OpenMP_libiomp5_LIBRARY ${OpenMP_C_LIB_NAMES})
    endif()
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(OpenMP_CXX"${CMAKE_CXX_COMPILER}")
        set(OpenMP_CXX_FLAGS"-fopenmp=libomp -Wno-unused-command-line-argument")
        set(OpenMP_CXX_LIB_NAMES"libomp""libgomp""libiomp5")
        set(OpenMP_libomp_LIBRARY ${OpenMP_CXX_LIB_NAMES})
        set(OpenMP_libgomp_LIBRARY ${OpenMP_CXX_LIB_NAMES})
        set(OpenMP_libiomp5_LIBRARY ${OpenMP_CXX_LIB_NAMES})
    endif()
endif()


ADD_SUBSRC(renderdir render)
ADD_SUBSRC(utildir util)
ADD_SUBSRC(geomdir geom)
ADD_SUBSRC(datatypedir datatype)
ADD_SUBSRC(IOdir IO )
#ADD_SUBSRC(testDatadir testData)


add_subdirectory(external/glm)
add_subdirectory(external/cxxopts)
#add_subdirectory(external/googletest)

include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/render/"
        "${CMAKE_CURRENT_SOURCE_DIR}/util/"
        "${CMAKE_CURRENT_SOURCE_DIR}/geom/"
        "${CMAKE_CURRENT_SOURCE_DIR}/datatype/"
        "${CMAKE_CURRENT_SOURCE_DIR}/IO/"
        "${CMAKE_CURRENT_SOURCE_DIR}/external/glm/"
        "${CMAKE_CURRENT_SOURCE_DIR}/external/include/"
#        "${CMAKE_CURRENT_SOURCE_DIR}/external/googletest/googletest/include"
#        "${CMAKE_CURRENT_SOURCE_DIR}/external/googletest/googlemock/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/external/cxxopts/include"
        "${CMAKE_CURRENT_BINARY_DIR}/config/"
        )

IF (WIN32)
    include_directories(external/SDL/lib-vc2019/include/)
    link_directories(external/SDL/lib-vc2019/lib/x64)
    message(STATUS "using windows external")
else()
    include_directories(/usr/local/Cellar/sdl2/2.0.9_1/include/SDL2)
    link_directories(/usr/local/Cellar/sdl2/2.0.9_1/lib)
    message(STATUS "using mac/linux external")
endif()


add_executable(newCpuRender ${IOdir} ${datatypedir} ${geomdir}  ${renderdir} ${utildir} main.cpp )
target_link_libraries(newCpuRender SDL2)