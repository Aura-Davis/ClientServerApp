# CMakeList.txt : CMake project for ClientServerApp, include source and define
# project specific logic here.
#

# Add source to this project's executable.
cmake_minimum_required(VERSION 3.19)
project(Networking)
set(CMAKE_CXX_STANDARD 20)
find_package(Boost 1.87.0 REQUIRED)
file(GLOB SOURCES src/* src/*/* src/*/*/*)
add_library (${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
	PUBLIC
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		${Boost_INCLUDE_DIRS}

	PRIVATE
		
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE
		${Boost_LIBRARIES}
)
# TODO: Add tests and install targets if needed.
