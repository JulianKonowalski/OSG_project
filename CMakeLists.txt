cmake_minimum_required(VERSION 3.21)
project(osg_application LANGUAGES CXX)

include(ExternalProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

set(OSG_SRC_DIR ${CMAKE_SOURCE_DIR}/3rdparty/OpenSceneGraph)
set(OSG_INC_DIR ${OSG_SRC_DIR}/include)
set(OSG_BUILD_INC_DIR ${CMAKE_BINARY_DIR}/3rdparty/OpenSceneGraph/include)

set(LIBJPG_SRC_DIR ${CMAKE_SOURCE_DIR}/3rdparty/libjpg)

ExternalProject_Add(libjpeg
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rdparty/libjpeg
  BINARY_DIR ${CMAKE_BINARY_DIR}/libjpeg
  INSTALL_DIR ${CMAKE_BINARY_DIR}/libjpeg
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/libjpeg
    -DENABLE_SHARED=ON
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON
  BUILD_BYPRODUCTS ${CMAKE_BINARY_DIR}/libjpeg.a
  STEP_TARGETS build install
)
set(JPEG_INCLUDE_DIR ${CMAKE_BINARY_DIR}/libjpeg/include)
set(JPEG_LIBRARY ${CMAKE_BINARY_DIR}/libjpeg/libjpeg.a)

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

target_compile_definitions(${PROJECT_NAME} PRIVATE 
  ASSET_DIR="${CMAKE_SOURCE_DIR}/assets"
)

if(WIN32)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${CMAKE_BINARY_DIR}/3rdparty/OpenSceneGraph/bin"
      "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    COMMENT "Copying OSG DLLs next to executable"
  )
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${CMAKE_BINARY_DIR}/libjpeg/bin"
      "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    COMMENT "Copying LIBJPEG DLLs next to executable"
  )
endif()