# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/enam/data/UG MS ENAM/L100/SECOND SEMESTER/LECTURES/DCIT 104/SEMESTER PROJECT/output/_deps/boost_callable_traits-src")
  file(MAKE_DIRECTORY "/home/enam/data/UG MS ENAM/L100/SECOND SEMESTER/LECTURES/DCIT 104/SEMESTER PROJECT/output/_deps/boost_callable_traits-src")
endif()
file(MAKE_DIRECTORY
  "/home/enam/data/UG MS ENAM/L100/SECOND SEMESTER/LECTURES/DCIT 104/SEMESTER PROJECT/output/_deps/boost_callable_traits-build"
  "/home/enam/data/UG MS ENAM/L100/SECOND SEMESTER/LECTURES/DCIT 104/SEMESTER PROJECT/output/_deps/boost_callable_traits-subbuild/boost_callable_traits-populate-prefix"
  "/home/enam/data/UG MS ENAM/L100/SECOND SEMESTER/LECTURES/DCIT 104/SEMESTER PROJECT/output/_deps/boost_callable_traits-subbuild/boost_callable_traits-populate-prefix/tmp"
  "/home/enam/data/UG MS ENAM/L100/SECOND SEMESTER/LECTURES/DCIT 104/SEMESTER PROJECT/output/_deps/boost_callable_traits-subbuild/boost_callable_traits-populate-prefix/src/boost_callable_traits-populate-stamp"
  "/home/enam/data/UG MS ENAM/L100/SECOND SEMESTER/LECTURES/DCIT 104/SEMESTER PROJECT/output/_deps/boost_callable_traits-subbuild/boost_callable_traits-populate-prefix/src"
  "/home/enam/data/UG MS ENAM/L100/SECOND SEMESTER/LECTURES/DCIT 104/SEMESTER PROJECT/output/_deps/boost_callable_traits-subbuild/boost_callable_traits-populate-prefix/src/boost_callable_traits-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/enam/data/UG MS ENAM/L100/SECOND SEMESTER/LECTURES/DCIT 104/SEMESTER PROJECT/output/_deps/boost_callable_traits-subbuild/boost_callable_traits-populate-prefix/src/boost_callable_traits-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/enam/data/UG MS ENAM/L100/SECOND SEMESTER/LECTURES/DCIT 104/SEMESTER PROJECT/output/_deps/boost_callable_traits-subbuild/boost_callable_traits-populate-prefix/src/boost_callable_traits-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
