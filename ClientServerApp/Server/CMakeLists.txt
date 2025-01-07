# CMakeList.txt : CMake project for ClientServerApp, include source and define
# project specific logic here.
#

# Add source to this project's executable.
cmake_minimum_required(VERSION 3.19)
project(Server)
set(CMAKE_CXX_STANDARD 20)
add_executable (${PROJECT_NAME} "Server.cpp" "Server.h")

target_include_directories(${PROJECT_NAME}
	PUBLIC
		Networking
)

target_link_libraries(${PROJECT_NAME}
	PUBLIC
		Networking
)
# TODO: Add tests and install targets if needed.
