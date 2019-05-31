#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(MM)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")

#Find GSL
find_package(gsl REQUIRED)
include_directories(${GSL_INCLUDE_DIR})
#link_directories(${GSL_LIBRARY_DIR})

#Find ROOT
find_package(ROOT REQUIRED)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")
include_directories(${ROOT_INCLUDE_DIR})
link_directories(${ROOT_LIBRARY_DIR})

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${Geant4_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc ${PROJECT_SOURCE_DIR}/src/jsoncpp.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh ${PROJECT_SOURCE_DIR}/include/json/*.h)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(mmSim mmSim.cc ${sources} ${headers})
target_link_libraries(mmSim ${Geant4_LIBRARIES} ${GSL_LIBRARIES} ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B5. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(_SCRIPTS
  config.json
  gui.mac
  icons.mac
  init.mac
  init_vis.mac
  run.mac
  vis.mac
  )

foreach(_script ${_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#
#----------------------------------------------------------------------------
# Add program to the project targets
# (this avoids the need of typing the program name after make)
#
add_custom_target(MM DEPENDS mmSim)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS mmSim DESTINATION bin)
