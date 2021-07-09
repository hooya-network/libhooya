cmake_minimum_required(VERSION 3.5)

project(libhooya VERSION 0.0.0
	DESCRIPTION ""
	LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -pedantic -Wstrict-aliasing -Werror)

add_library(hooya SHARED
	$<TARGET_OBJECTS:sock>
	$<TARGET_OBJECTS:pipeline>
)

# --- `net` ---
add_library(sock OBJECT
	sock/DGram.cc
	sock/Remote.cc
	sock/udpS.cc
	sock/Exception.cc
)

target_include_directories(sock
	PUBLIC sock/include
	PRIVATE sock
)

# --- `pipeline` ---
add_library(pipeline OBJECT
	pipeline/EgressFeed.cc
	pipeline/IngressFeed.cc
	pipeline/CountingSemaphore.cc
	pipeline/ingressCache.cc
	pipeline/ingressCacheEntry.cc
	pipeline/NetMessage.cc
	pipeline/NetIn.cc
	pipeline/NetOut.cc
)

target_include_directories(pipeline
	PUBLIC pipeline/include
	PRIVATE pipeline
)

target_link_libraries(pipeline
	PRIVATE -pthread
)

# libhooya.so
install(TARGETS hooya LIBRARY DESTINATION lib)

# Public headers
install(
	DIRECTORY sock/include/ pipeline/include/
	DESTINATION include
)
