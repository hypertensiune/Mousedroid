cmake_minimum_required(VERSION 3.26)
include(CMakePrintHelpers)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# If find_package doesn't find wxWidgets, try settings the LIB_DIR or (and) the ROOT_DIR manually
if(WIN32)
    set(wxWidgets_LIB_DIR $ENV{WXWIN}/lib/gcc_dll)
    # set(wxWidgets_ROOT_DIR $ENV{WXWIN})
endif()

find_package(wxWidgets REQUIRED net gl core base adv)

project(Mousedroid)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

if(WIN32)

    set(CMAKE_CXX_FLAGS "-Wl,-subsystem,windows")
    set(SRC_FILES ${SRC_FILES} src/input/WindowsInputManager.cpp resource.o)
    set(ASIO_PATH $ENV{ASIO})
    set(LIBS ws2_32 wsock32)

else()

    set(SRC_FILES ${SRC_FILES} src/input/LinuxInputManager.cpp)

endif()

add_executable(Mousedroid ${SRC_FILES})

if(wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})
    target_link_libraries(Mousedroid ${wxWidgets_LIBRARIES})
endif()

target_link_libraries(Mousedroid ${LIBS})

target_include_directories(Mousedroid PRIVATE ./include ${ASIO_PATH}/include)
