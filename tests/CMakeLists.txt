# Testing library
FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v2.9.1
)
FetchContent_MakeAvailable(catch2)

set(LIB_CONFIG_LOADER config_load)
set(SOCKET_CLIENT socket_client)
set(TEST_NAME testlib)

if(CMAKE_HOST_WIN32)
    set(libconfig "libconfig")
else()
    set(libconfig "config")
endif()

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
include_directories(
        ../include
        ${GTEST_INCLUDE_DIRS}
)

set(SOURCE db_test.cpp)

add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_DEBUG_LIBS         OFF)
set(Boost_USE_RELEASE_LIBS       ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

find_package(Boost)
if(Boost_FOUND)
    message(boost found)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

# Tests need to be added as executables first
add_executable(${TEST_NAME} ${SOURCE})

# I'm using C++17 in the test
target_compile_features(${TEST_NAME} PRIVATE cxx_std_17)


target_link_libraries(${TEST_NAME} ${Boost_LIBRARIES})
target_link_libraries(${TEST_NAME} ${GTEST_LIBRARIES} Threads::Threads)
#[[target_link_libraries(${TEST_NAME} storage_lib)]]
#[[target_link_libraries(${TEST_NAME} connector)]]
target_link_libraries(${TEST_NAME} ${libconfig}++)
add_test(NAME testlibtest COMMAND testlib) # Command can be a target
