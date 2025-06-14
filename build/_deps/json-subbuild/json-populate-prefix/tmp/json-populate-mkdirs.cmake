# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/Users/ridabelaqziz/Desktop/Revision2025/minipromptGPT/build/_deps/json-src")
  file(MAKE_DIRECTORY "/Users/ridabelaqziz/Desktop/Revision2025/minipromptGPT/build/_deps/json-src")
endif()
file(MAKE_DIRECTORY
  "/Users/ridabelaqziz/Desktop/Revision2025/minipromptGPT/build/_deps/json-build"
  "/Users/ridabelaqziz/Desktop/Revision2025/minipromptGPT/build/_deps/json-subbuild/json-populate-prefix"
  "/Users/ridabelaqziz/Desktop/Revision2025/minipromptGPT/build/_deps/json-subbuild/json-populate-prefix/tmp"
  "/Users/ridabelaqziz/Desktop/Revision2025/minipromptGPT/build/_deps/json-subbuild/json-populate-prefix/src/json-populate-stamp"
  "/Users/ridabelaqziz/Desktop/Revision2025/minipromptGPT/build/_deps/json-subbuild/json-populate-prefix/src"
  "/Users/ridabelaqziz/Desktop/Revision2025/minipromptGPT/build/_deps/json-subbuild/json-populate-prefix/src/json-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/ridabelaqziz/Desktop/Revision2025/minipromptGPT/build/_deps/json-subbuild/json-populate-prefix/src/json-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/ridabelaqziz/Desktop/Revision2025/minipromptGPT/build/_deps/json-subbuild/json-populate-prefix/src/json-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
