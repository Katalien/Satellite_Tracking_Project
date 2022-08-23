# CMakeList.txt: проект CMake для Satellite_Tracking_Project
cmake_minimum_required (VERSION 3.8)

project ("Satellite_Tracking_Project")

include_directories(libsgp4)

add_subdirectory(libsgp4)

# Добавьте источник в исполняемый файл этого проекта.
add_executable (Satellite_Tracking_Project "User.cpp"
                                           "Antenna.cpp" "Antenna.hpp"
                                           "ComPort.cpp" "ComPort.hpp"
                                           "Handle.cpp" "Handle.hpp"
                                           "Program.cpp" "Program.hpp"
                                           "Satellite.cpp" "Satellite.hpp" 
                                           "SatTrackInterface.cpp" "SatTrackInterface.hpp"
                                           "TleData.cpp" "TleData.hpp" "curl/curl.h" "curl/curlver.h" "curl/easy.h" "curl/header.h" "curl/mprintf.h" "curl/multi.h" "curl/options.h" "curl/stdcheaders.h" "curl/system.h" "curl/typecheck-gcc.h" "curl/urlapi.h")

target_link_libraries(Satellite_Tracking_Project sgp4)
