#
# deep_fake
#

cmake_minimum_required(VERSION 3.12)

include(cmake/tools/guards.cmake)

#
# Project configuration
#

project(
    deep_fake
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)

#
# Add executable
#

if(CONAN_EXPORTED)
    include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    include("${CMAKE_SOURCE_DIR}/cmake/conan.cmake")
    conan_cmake_run(CONANFILE conanfile.txt BASIC_SETUP BUILD missing)
endif()

file(GLOB_RECURSE PROJECT_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE PROJECT_HEADERS "${CMAKE_SOURCE_DIR}/include/*.hpp")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})

# Lib needs its header files, and users of the library must also see these (PUBLIC).
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PUBLIC ${CONAN_LIBS})

#
# Generate preprocessor variable for all model
#

file(GLOB_RECURSE MODELS_PATH "${CMAKE_SOURCE_DIR}/assets/models/*.dat")

foreach(model_path ${MODELS_PATH})
    string(REPLACE ".dat" "" new_name ${model_path})
    get_filename_component(filename ${new_name} NAME)
    string(TOUPPER ${filename} filename)
    add_compile_definitions(${filename}="${model_path}")
endforeach()