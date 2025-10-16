cmake_minimum_required(VERSION 3.21)
project(osg_application LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

set(OSG_SRC_DIR ${CMAKE_SOURCE_DIR}/3rdparty/OpenSceneGraph)
set(OSG_INC_DIR ${OSG_SRC_DIR}/include)
set(OSG_BUILD_INC_DIR ${CMAKE_BINARY_DIR}/3rdparty/OpenSceneGraph/include)
add_subdirectory(${OSG_SRC_DIR})

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE 
  ${OSG_INC_DIR}
  ${OSG_BUILD_INC_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
  osg
  osgDB
  osgUtil
  osgViewer
)

if(WIN32)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${CMAKE_BINARY_DIR}/3rdparty/OpenSceneGraph/bin"
      "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    COMMENT "Copying OSG DLLs next to executable"
  )
endif()