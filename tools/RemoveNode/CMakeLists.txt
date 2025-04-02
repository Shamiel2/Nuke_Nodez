cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(RemoveNode)

set(CMAKE_MODULE_PATH "CMake;${CMAKE_MODULE_PATH}")
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
set(DCMAKE_PREFIX_PATH "$NUKE_ROOT")
set(Qt5_DIR "./Qt/lib/cmake/Qt5") # Add the path to the QT5 binaries eg. D:../../Qt/lib/cmake/Qt5
find_package(Nuke REQUIRED)


function(add_new_plugin PLUGIN_NAME)
    add_nuke_plugin(${PLUGIN_NAME} ${ARGN})
    target_compile_definitions(${PLUGIN_NAME} PRIVATE FN_EXAMPLE_PLUGIN)
endfunction()


find_package(Qt5 COMPONENTS Core Gui Widgets)
if (Qt5_FOUND)
    set(CMAKE_AUTOMOC ON)
    add_nuke_plugin(RemoveNode RemoveNode.cpp)
    target_sources(RemoveNode PRIVATE RemoveNode.moc.h)
    target_link_libraries(RemoveNode PRIVATE Qt::Core Qt::Gui Qt::Widgets)

else()
    message(STATUS "Qt5 libraries were not found, skipping the Qt examples")
endif()

