#---------------------SET COMPONENT NAME---------------------------------------#
SET(LIB_NAME handler)
#------------------------------------------------------------------------------#
#---------------------SET LOCAL NAME TO LINK COMPONENT-------------------------#
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
SET(STORAGE storage_lib)
set(CMAKE_CXX_STANDARD 20)
#------------------------------------------------------------------------------#
#---------------------SET PROTO FILE-------------------------------------------#
#------------------------------------------------------------------------------#
#-----------------------INCLUDE HEADER-----------------------------------------#
file(GLOB HEADER_LIST CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/include/components/connector/*.hpp")
#------------------------------------------------------------------------------#
#---------------------FIND PACKAGES--------------------------------------------#
find_package(Threads REQUIRED)
#------------------------------------------------------------------------------#
#---------------------GENERATE AUTOCODE----------------------------------------#
#------------------------------------------------------------------------------#
#---------------------COMPILE DEFINES------------------------------------------#
add_definitions(-fcoroutines)
add_definitions(-fno-exceptions)
add_definitions(-fexceptions)
#------------------------------------------------------------------------------#
#---------------------ADD SOURCE-----------------------------------------------#
set(SOURCE  connect.cpp
            non_block_logger.cpp
            handler_base.cpp
            base_task_queue.cpp)
#------------------------------------------------------------------------------#
#---------------------ADD LIBRARY----------------------------------------------#
add_library(${LIB_NAME} ${SOURCE}
        ${HEADER_LIST})
#------------------------------------------------------------------------------#
#--------------------INCLUDE DIRS----------------------------------------------#
include_directories(${})
target_include_directories(${LIB_NAME}
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include/components)
#---------------------LINK COMPONENTS------------------------------------------#
#------------------------------------------------------------------------------#
target_link_libraries(${LIB_NAME} PRIVATE ${storage_lib})
#------------------------------------------------------------------------------#
target_compile_features(${LIB_NAME} PUBLIC cxx_std_20)
source_group(TREE "../include" PREFIX "Header Files" FILES ${HEADER_LIST})
