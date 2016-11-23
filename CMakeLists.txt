cmake_minimum_required(VERSION 2.8.7)

project(VkClient)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

option(BUILD_SHARED_LIBS "Build shared instead of static library" OFF)
option(BUILD_TESTS "Build tests for vkclient" ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)

add_library(${PROJECT_NAME} STATIC ${CMAKE_CURRENT_SOURCE_DIR}/sources/client.cpp)

find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)
find_package(Threads REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

if(THREADS_HAVE_PTHREAD_ARG)
	set_property(TARGET ${PROJECT_NAME} PROPERTY COMPILE_OPTIONS "-pthread")
	set_property(TARGET ${PROJECT_NAME} PROPERTY INTERFACE_COMPILE_OPTIONS "-pthread")
endif()

if(CMAKE_THREAD_LIBS_INIT)
	target_link_libraries(${PROJECT_NAME} "${CMAKE_THREAD_LIBS_INIT}")
endif()

if(UNIX)
	set(DEPENDS_LIBRARIES ${OPENSSL_LIBRARIES} ${CURL_LIBRARY})
elseif(MSVC)
	set(DEPENDS_LIBRARIES ${OPENSSL_LIBRARIES} ${CURL_LIBRARY} ws2_32)
endif()

add_executable(Examples ${CMAKE_CURRENT_SOURCE_DIR}/examples/main.cpp)

target_link_libraries(Examples ${PROJECT_NAME} ${DEPENDS_LIBRARIES})

if(BUILD_TESTS)
	enable_testing()
	file(GLOB ${PROJECT_NAME}_TEST_SOURCES tests/*.cpp)
	add_executable(check ${${PROJECT_NAME}_TEST_SOURCES})
	target_link_libraries(check ${PROJECT_NAME} ${DEPENDS_LIBRARIES})
	add_test(NAME check COMMAND check "-s" "-r" "compact" "--use-colour" "yes")
endif()
