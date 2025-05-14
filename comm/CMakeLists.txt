cmake_minimum_required(VERSION 3.15)
project(PCAN_Test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add include directory for our header and the external PCAN header
include_directories(include)

# Add the executable target
add_executable(pcan_test
    src/main.cpp
    src/pcan_uds_handler.cpp
)

# Tell the executable where to find the include files
target_include_directories(pcan_test PUBLIC include)

# For Windows, ensure the linker can find necessary libraries if needed (though we load dynamically)
if(WIN32)
    # Add necessary system libraries (e.g., for LoadLibrary, GetProcAddress)
    # MinGW usually links these automatically, but being explicit can sometimes help.
    # target_link_libraries(pcan_test PRIVATE kernel32)
endif()

# --- Installation Step (Optional but Good Practice) ---
# This allows 'cmake --install .' after building

# Define install location for the executable
install(TARGETS pcan_test
    RUNTIME DESTINATION bin # Installs .exe to <prefix>/bin
)

# Copy the PCAN-UDS.dll to the install directory next to the executable
install(FILES ${CMAKE_SOURCE_DIR}/lib/PCAN-UDS.dll
    DESTINATION bin # Installs DLL to <prefix>/bin
    COMPONENT Runtime # Only install DLL when installing the runtime component
)


message(STATUS "PCAN Test executable configured.")
message(STATUS "Source files: src/main.cpp src/pcan_uds_handler.cpp")
message(STATUS "Include directory: ${CMAKE_SOURCE_DIR}/include")
message(STATUS "To build, navigate to your build directory and run 'cmake --build .' ")
message(STATUS "The executable 'pcan_test.exe' will be in the build/bin directory.")
message(STATUS "Ensure PCAN-UDS.dll is either in the same directory as pcan_test.exe or in the system PATH.") 