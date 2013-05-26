cmake_minimum_required (VERSION 2.6)

# possible values: Release, Debug, RelWithDebugInfo
# set(CMAKE_BUILD_TYPE RelWithDebugInfo)

project(LART)
set(PROJECT_VERSION "0.0.1")

set(ARCHIVE_NAME ${CMAKE_PROJECT_NAME}-${PROJECT_VERSION})
add_custom_target(dist
    COMMAND git archive --prefix=${ARCHIVE_NAME}/ HEAD
        | bzip2 > ${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.tar.bz2
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_definitions(-DPROJECT_VERSION=\"${PROJECT_VERSION}\")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread -std=c++0x -fPIC")

if(CMAKE_BUILD_TYPE MATCHES DEBUG)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -march=native -msse -mfpmath=sse")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -msse -mfpmath=sse")
	add_definitions("-DNDEBUG")
endif()

SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


#
# BUILD DEPENDENCIES:
#

find_package(PkgConfig REQUIRED)

pkg_check_modules(JACK jack)

# BOOST
#set(Boost_USE_STATIC_LIBS)
#find_package(Boost COMPONENTS program_options timer filesystem system REQUIRED)


#
# BUILD TARGETS
#


include_directories(${PROJECT_SOURCE_DIR})

add_library(lart lart/heap.cc)
target_link_libraries(lart ${JACK_LIBRARIES})

add_executable(example example.cc)
target_link_libraries(example lart)

#
# INSTALL
#

install(TARGETS lart RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install(FILES lart/heap.h lart/junk.h lart/ringbuffer.h DESTINATION include/lart)

