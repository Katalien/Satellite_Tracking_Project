# CMakeList.txt: проект CMake для Satellite_Tracking_Project; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

project ("Satellite_Tracking_Project")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (Satellite_Tracking_Project "User.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Satellite_Tracking_Project PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
