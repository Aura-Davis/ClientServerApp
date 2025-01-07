# CMakeList.txt : CMake project for ClientServerApp, include source and define
# project specific logic here.
#

# Add source to this project's executable.
cmake_minimum_required(VERSION 3.19)
project(ClientServerApp)
add_subdirectory(Networking)
add_subdirectory(Client)
add_subdirectory(Server)


# TODO: Add tests and install targets if needed.
