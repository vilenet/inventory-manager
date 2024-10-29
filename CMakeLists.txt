cmake_minimum_required(VERSION 3.10)
project(InventoryManager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories("${CMAKE_SOURCE_DIR}/include")

# Path to PDCurses
include_directories("C:/msys64/mingw64/include")
link_directories("C:/msys64/mingw64/lib")

add_executable(inventory-manager
    src/main.cpp
    src/database.cpp
    src/inventory_manager.cpp
    src/ui.cpp)

target_include_directories(inventory-manager PRIVATE "${CMAKE_SOURCE_DIR}/sqlite")

target_link_libraries(inventory-manager PRIVATE "${CMAKE_SOURCE_DIR}/sqlite/sqlite3.dll" pdcurses)

set_target_properties(inventory-manager PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Copy sqlite3.dll to the bin folder after build
add_custom_command(TARGET inventory-manager POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/sqlite/sqlite3.dll" 
    "$<TARGET_FILE_DIR:inventory-manager>"
)
