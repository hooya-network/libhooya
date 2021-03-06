cmake_minimum_required(VERSION 3.12)

project(libhooya VERSION 0.1.0
	DESCRIPTION ""
	LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(PkgConfig REQUIRED)

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

add_library(hooya SHARED)
target_link_libraries(hooya PUBLIC
	pipeline
	sock
	crypto
	net
	img
)

# --- `crypto` ---
pkg_check_modules(BOTAN2 REQUIRED botan-2)
add_library(crypto OBJECT
	crypto/Keccak.cc
	crypto/CID.cc
	crypto/bin2.cc
)

target_link_libraries(crypto
	PRIVATE ${BOTAN2_LIBRARIES}
)

target_include_directories(crypto
	PUBLIC crypto/include
	PRIVATE crypto ${BOTAN2_INCLUDE_DIRS}
)

# --- `sock` ---
add_library(sock OBJECT
	sock/DGram.cc
	sock/Remote.cc
	sock/udpS.cc
	sock/connection.cc
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
	pipeline/Exception.cc
	pipeline/NetDataCache.cc
)

target_include_directories(pipeline
	PUBLIC pipeline/include
	PRIVATE pipeline
)

target_link_libraries(pipeline
	PRIVATE -pthread sock net crypto
)

# --- `img` ---
find_package(OpenCV REQUIRED)
add_library(img OBJECT
	img/Exception.cc
	img/Image.cc
)
option(WITH_OPENCV_HIGHGUI OFF)
if (${WITH_OPENCV_HIGHGUI})
	target_compile_definitions(img
		PUBLIC USE_OPENCV_HIGHGUI
	)
endif()

target_include_directories(img
	PUBLIC img/include
	PRIVATE img ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(img
	PRIVATE ${OpenCV_LIBS}
)

# --- `net` ---
add_library(net OBJECT
	net/NetData.cc
)

target_include_directories(net
	PUBLIC net/include
	PRIVATE net
)

# --- `test` ---
find_package(GTest)
if (${GTEST_FOUND})
	enable_testing()
	add_executable(unit-test
		test/pipeline/FIFOTest.cc
		test/pipeline/UplinkTest.cc
		test/pipeline/NetInTest.cc
		test/sock/DGramTest.cc
		test/sock/RemoteTest.cc
		test/net/NetDataTest.cc
		test/crypto/KeccakTest.cc
		test/crypto/CIDTest.cc
		test/img/ImageTest.cc
	)

	set_target_properties(unit-test
		PROPERTIES EXCLUDE_FROM_ALL 1)

	gtest_discover_tests(
		unit-test
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test
	)

	target_link_libraries(unit-test
		GTest::gtest GTest::gtest_main
		hooya
	)

	target_include_directories(unit-test
		PUBLIC test/include
	)

	add_custom_target(build-test)
	add_dependencies(build-test unit-test)

	add_custom_target(check
		COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
		DEPENDS unit-test
	)
endif()

# --- `doc` ---
find_package(Doxygen)
if (${DOXYGEN_FOUND})
	set(DOXYGEN_INDEX ${CMAKE_CURRENT_BINARY_DIR}/html/index.html)
	set(DOXYGEN_README_MD ${PROJECT_SOURCE_DIR}/README.md)
	set(DOXYGEN_FOOTER ${PROJECT_SOURCE_DIR}/doc/doxygen/footer.html)

	set(CRYPTO_DIR ${PROJECT_SOURCE_DIR}/crypto)
	set(NET_DIR ${PROJECT_SOURCE_DIR}/net)
	set(IMG_DIR ${PROJECT_SOURCE_DIR}/img)
	set(PIPELINE_DIR ${PROJECT_SOURCE_DIR}/pipeline)
	set(SOCK_DIR ${PROJECT_SOURCE_DIR}/sock)

	configure_file(doc/doxygen/Doxyfile.in
		${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

	add_custom_target(doc
		DEPENDS README.md ${CRYPTO_DIR} ${NET_DIR} ${PIPELINE_DIR} ${SOCK_DIR}
		${DOXYGEN_FOOTER} ${DOXYGEN_README_MD}
		COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating docs"
	)
	set_target_properties(doc PROPERTIES EXCLUDE_FROM_ALL 1)
endif()

# libhooya.so
install(TARGETS hooya LIBRARY)

# Public headers
install(DIRECTORY
	crypto/include/
	pipeline/include/
	net/include/
	sock/include/
	img/include/
	DESTINATION include
)
