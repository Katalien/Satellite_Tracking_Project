# CMakeList.txt: проект CMake для Satellite_Tracking_Project
cmake_minimum_required (VERSION 3.8)

project ("Satellite_Tracking_Project")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (Satellite_Tracking_Project "User.cpp"
                                           "Antenna.cpp" "Antenna.hpp"
                                           "ComPort.cpp" "ComPort.hpp"
                                           "Handle.cpp" "Handle.hpp"
                                           "Program.cpp" "Program.hpp"
                                           "Satellite.cpp" "Satellite.hpp" 
                                           "SatTrackInterface.cpp" "SatTrackInterface.hpp"
                                           "TleData.cpp" "TleData.hpp")

add_library(spg4 STATIC IMPORTED)
set_target_properties(spg4 PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libsgp4/lib/sgp4s.lib)
set_target_properties(spg4 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/libsgp4/include/SGP4)
target_link_libraries(Satellite_Tracking_Project spg4)

add_library(curl STATIC IMPORTED)
set_target_properties(curl PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/curl/lib/libcurl-d_imp.lib)
set_target_properties(curl PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/curl/include/curl)
target_link_libraries(Satellite_Tracking_Project curl)
