cmake_minimum_required(VERSION 3.1)

#set project name and language
project(vf-tcp LANGUAGES CXX)

#Automoc and RCC ON. Needed in QT projects
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
#C++ 11 Flag
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


option(useSystemd "Using libsystemd" ON)

#Set project Version
set(VfTcp_VERSION_MAJOR "0")
set(VfTcp_VERSION_MINOR "0")
set(VfTcp_VERSION_PATCH "1")

set(PROJECT_VERSION "${VfTcp_VERSION_MAJOR}.${VfTcp_VERSION_MINOR}.${VfTcp_VERSION_PATCH}")

#Adding cmake functions like feature_summary()
include(FeatureSummary)
include(GNUInstallDirs)

#Find dependecies
find_package(Qt5 COMPONENTS Core Network CONFIG REQUIRED)


if(${useSystemd})
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(Systemd REQUIRED libsystemd)
endif()



#sum up project Files 
file(GLOB SOURCES 
    ${PROJECT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${PROJECT_SOURCE_DIR}/*.h
    )

file(GLOB RESOURCES 
    ${PROJECT_SOURCE_DIR}/*.qrc
    )

#create library 
add_library(VfTcp SHARED
    ${SOURCES}
    ${HEADER}
    ${RESOURCES}
    )



target_link_libraries(VfTcp 
    PRIVATE
    Qt5::Core
    Qt5::Network
    )


if(${Systemd_FOUND})
    target_link_libraries(VfTcp 
        PRIVATE
        systemd
    )
endif()


# announce headers - target perspective
target_include_directories(VfTcp
    PUBLIC
    $<INSTALL_INTERFACE:include/VfTcp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

#set target Version
set_target_properties(VfTcp PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfTcp PROPERTIES SOVERSION ${VfTcp_VERSION_MAJOR})

message(STATUS "Project Version: " ${PROJECT_VERSION})

#generate export header
include(GenerateExportHeader)
generate_export_header(VfTcp)

#install export header
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vftcp_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfTcp
    )



# install library
install(TARGETS VfTcp
    EXPORT VfTcpExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

# configure *ConfigVersion.cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    VfTcpConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMinorVersion
    )

# configure *Config.cmake
configure_file(VfTcpConfig.cmake.in VfTcpConfig.cmake @ONLY)

message(STATUS "BINDIR: " ${CMAKE_CURRENT_BINARY_DIR})

# install *Config(Version).cmake
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/VfTcpConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/VfTcpConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VfTcp
    )

# install targets cmake-files
install(EXPORT VfTcpExport
    FILE VfTcpTargets.cmake
    NAMESPACE VeinMeta::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VfTcp
    )

# install public headers
install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfTcp
    )

# spawn out some info on configuration
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)



