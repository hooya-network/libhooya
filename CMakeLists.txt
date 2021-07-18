cmake_minimum_required(VERSION 3.5)

project(libhooya VERSION 0.1.0
	DESCRIPTION ""
	LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -pedantic -Wstrict-aliasing -Werror)

execute_process(
	COMMAND git rev-parse --is-inside-work-tree
	OUTPUT_VARIABLE GIT_IS_INSIDE_WORK_TREE
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(PROJECT_VERSION_FULL "${PROJECT_VERSION}")

if (${GIT_IS_INSIDE_WORK_TREE})
	# git commit hash
	execute_process(
		COMMAND git log -1 --format=%h
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		OUTPUT_VARIABLE GIT_COMMIT_HASH
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	# git tag
	execute_process(
		COMMAND git describe --exact-match --tags
		OUTPUT_VARIABLE GIT_TAG ERROR_QUIET
	)

	# git diff
	execute_process(
		COMMAND git diff --quiet --exit-code
		RESULTS_VARIABLE GIT_DIFF_RESULT
	)

	# Change prerelease if this is a tagged commit
	set(PROJECT_VERSION_PRERELEASE "wip")
	if (NOT ${GIT_TAG} EQUAL "")
		set(PROJECT_VERSION_PRERELEASE "release")
	endif()

	set(PROJECT_VERSION_FULL "${PROJECT_VERSION_FULL}-${PROJECT_VERSION_PRERELEASE}")
	set(PROJECT_VERSION_FULL "${PROJECT_VERSION_FULL}+${GIT_COMMIT_HASH}")

	# indicate if anything's different between HEAD and current tracked files
	if (${GIT_DIFF_RESULT})
		set(PROJECT_VERSION_FULL "${PROJECT_VERSION_FULL}-dirty")
	endif()

endif()

configure_file(
	${CMAKE_SOURCE_DIR}/versioning.hh.in
	${CMAKE_BINARY_DIR}/versioning.hh
)

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
	pipeline/Uplink.cc
	pipeline/NetMessage.cc
	pipeline/NetIn.cc
	pipeline/NetOut.cc
)

target_include_directories(pipeline
	PUBLIC pipeline/include
	PRIVATE pipeline
)

target_link_libraries(pipeline
	PRIVATE -pthread sock
)

# --- `test` ---
find_package(GTest)
if (${GTEST_FOUND})
	add_executable(unit-test
		test/pipeline/FIFOTest.cc
		test/pipeline/UplinkTest.cc
		test/sock/DGramTest.cc
		test/sock/RemoteTest.cc
	)

	set_target_properties(unit-test
		PROPERTIES EXCLUDE_FROM_ALL 1
	)

	target_link_libraries(unit-test
		GTest::gtest GTest::gtest_main
		pipeline
		sock
	)

	target_include_directories(sock
		PUBLIC test/include
	)

	gtest_discover_tests(unit-test)

	add_custom_target(build-test)
	add_dependencies(build-test unit-test)

	add_custom_target(check
		COMMAND ./unit-test
		DEPENDS unit-test
	)
endif()

# libhooya.so
install(TARGETS hooya LIBRARY DESTINATION lib)

# Public headers
install(
	DIRECTORY sock/include/ pipeline/include/
	DESTINATION include
)
