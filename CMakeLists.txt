CMAKE_MINIMUM_REQUIRED(VERSION 3.8)

project(SurfaceDrawing C CXX)
set(CMAKE_CXX_STANDARD 17)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

IF( WIN32 )
    SET( WINDOWS 1 )
ELSEIF( APPLE )
    SET(MACOSX 1)
ELSEIF( UNIX )
    SET(LINUX 1)
ELSEIF( MINGW )
    SET(WINDOWS 1)
ELSEIF( CYGWIN )
    SET(WINDOWS 1)
ELSE()
    MESSAGE("WHAT IS THIS PLATFORM ?")
    RETURN()
ENDIF()

if( LINUX )
    set(${PROJECT_NAME}_DEFINITIONS LINUX)
elseif(WINDOWS)
    set(${PROJECT_NAME}_DEFINITIONS WINDOWS)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Network)

##################################################

function(extract_include_dirs header_files result_varname)
set(header_directories "")
foreach(header in ${header_files})
get_filename_component(tmp ${header} DIRECTORY)
list( APPEND header_directories ${tmp})
endforeach(header in ${header_files})
list(REMOVE_DUPLICATES header_directories)

set(${result_varname} ${header_directories} PARENT_SCOPE)
endfunction(extract_include_dirs)

# =====================================================================================================
# =====================================================================================================
# =====================================================================================================
# =====================================================================================================

file(GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/SurfaceDrawing/*.cpp")
file(GLOB_RECURSE ${PROJECT_NAME}_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/SurfaceDrawing/*.h*")

extract_include_dirs("${${PROJECT_NAME}_HEADER_FILES}" ${PROJECT_NAME}_INCLUDE_DIRECTORIES)

set( ${PROJECT_NAME}_INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} ${${PROJECT_NAME}_INCLUDE_DIRECTORIES})

set_source_files_properties(${PROJECT_NAME}_HEADER_FILES PROPERTIES HEADER_FILE_ONLY TRUE)
add_executable(${PROJECT_NAME}_CLIENT ${${PROJECT_NAME}_SRC_FILES} ${${PROJECT_NAME}_HEADER_FILES})
target_compile_definitions(${PROJECT_NAME}_CLIENT PUBLIC ${${PROJECT_NAME}_DEFINITIONS} CLIENTSIDE)
target_include_directories(${PROJECT_NAME}_CLIENT PUBLIC ${${PROJECT_NAME}_INCLUDE_DIRECTORIES})
target_link_libraries(${PROJECT_NAME}_CLIENT Qt5::Core Qt5::Widgets Qt5::Network)

add_executable(${PROJECT_NAME}_SERVER ${${PROJECT_NAME}_SRC_FILES} ${${PROJECT_NAME}_HEADER_FILES})
target_compile_definitions(${PROJECT_NAME}_SERVER PUBLIC ${${PROJECT_NAME}_DEFINITIONS} SERVERSIDE)
target_include_directories(${PROJECT_NAME}_SERVER PUBLIC ${${PROJECT_NAME}_INCLUDE_DIRECTORIES})
target_link_libraries(${PROJECT_NAME}_SERVER Qt5::Core Qt5::Widgets Qt5::Network)
