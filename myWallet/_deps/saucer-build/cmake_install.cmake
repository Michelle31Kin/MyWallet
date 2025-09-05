# Install script for directory: /home/enam/data/UG MS ENAM/L100/SECOND SEMESTER/LECTURES/DCIT 104/SEMESTER PROJECT/myWallet/_deps/saucer-src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/enam/data/UG MS ENAM/L100/SECOND SEMESTER/LECTURES/DCIT 104/SEMESTER PROJECT/myWallet/_deps/lockpp-build/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/enam/data/UG MS ENAM/L100/SECOND SEMESTER/LECTURES/DCIT 104/SEMESTER PROJECT/myWallet/_deps/boost_callable_traits-build/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/enam/data/UG MS ENAM/L100/SECOND SEMESTER/LECTURES/DCIT 104/SEMESTER PROJECT/myWallet/_deps/ereignis-build/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/enam/data/UG MS ENAM/L100/SECOND SEMESTER/LECTURES/DCIT 104/SEMESTER PROJECT/myWallet/_deps/fmt-build/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/enam/data/UG MS ENAM/L100/SECOND SEMESTER/LECTURES/DCIT 104/SEMESTER PROJECT/myWallet/_deps/flagpp-build/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/enam/data/UG MS ENAM/L100/SECOND SEMESTER/LECTURES/DCIT 104/SEMESTER PROJECT/myWallet/_deps/rebind-build/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/enam/data/UG MS ENAM/L100/SECOND SEMESTER/LECTURES/DCIT 104/SEMESTER PROJECT/myWallet/_deps/poolparty-build/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/enam/data/UG MS ENAM/L100/SECOND SEMESTER/LECTURES/DCIT 104/SEMESTER PROJECT/myWallet/_deps/eraser-build/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/enam/data/UG MS ENAM/L100/SECOND SEMESTER/LECTURES/DCIT 104/SEMESTER PROJECT/myWallet/_deps/glaze-build/cmake_install.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/enam/data/UG MS ENAM/L100/SECOND SEMESTER/LECTURES/DCIT 104/SEMESTER PROJECT/myWallet/_deps/saucer-build/libsaucer.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/enam/data/UG MS ENAM/L100/SECOND SEMESTER/LECTURES/DCIT 104/SEMESTER PROJECT/myWallet/_deps/saucer-src/include/saucer")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/home/enam/data/UG MS ENAM/L100/SECOND SEMESTER/LECTURES/DCIT 104/SEMESTER PROJECT/myWallet/_deps/saucer-build/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
