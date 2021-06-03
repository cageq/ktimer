cmake_minimum_required(VERSION 3.13)

project (heaptimer VERSION 0.6 )
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


if(APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unused-function -g  ")
elseif(UNIX)
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread -g -O2  -fPIC -Wno-unused-function ")
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread -g   -Wno-unused-function -fvisibility=hidden  ")
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread -g -static-libgcc  -static-libstdc++  -Wl,-Bstatic -lc -Wl,-Bdynamic   -Wno-unused-function -fvisibility=hidden  ")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread -g  -Wno-unused-function -fvisibility=hidden   ")
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread -g -static-libgcc  -static-libstdc++  -Wl,-Bstatic -lc -Wl,-Bdynamic  -Wl,-rpath,bin   -Wno-unused-function -fvisibility=hidden  ")
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-rpath ${CMAKE_CURRENT_SOURCE_DIR}/libs/ssl/lib/ ")
	link_directories("libs/t2sdk/bin/linux64")
	link_directories("libs/ssl/lib")
elseif(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D NDEBUG")
endif()


include_directories(".")
include_directories("include")
include_directories("src") 

 


file(GLOB SRCS 
	src/test.cpp	
	)
add_executable(test ${SRCS} ) 
