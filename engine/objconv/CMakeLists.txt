##
# CMakeLists.txt
#
# Vega Strike - Space Simulation, Combat and Trading
# Copyright (C) 2001-2025 The Vega Strike Contributors:
# Project creator: Daniel Horn
# Original development team: As listed in the AUTHORS file
# Current development team: Roy Falk, Benjamen R. Meyer, Stephen G. Tuggy
#
#
# https://github.com/vegastrike/Vega-Strike-Engine-Source
#
# This file is part of Vega Strike.
#
# Vega Strike is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Vega Strike is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Vega Strike.  If not, see <https://www.gnu.org/licenses/>.
#


#SET_PROPERTY(DIRECTORY PROPERTY INCLUDE_DIRECTORIES "")
INCLUDE_DIRECTORIES("..")
SET(TRISORT_SOURCES trisort.cpp)
ADD_EXECUTABLE(vs-buildenv-trisort ${TRISORT_SOURCES})
TARGET_INCLUDE_DIRECTORIES(vs-buildenv-trisort SYSTEM PRIVATE ${VSE_TST_INCLUDES})
TARGET_INCLUDE_DIRECTORIES(vs-buildenv-trisort PRIVATE
        # VS engine headers
        ${Vega_Strike_SOURCE_DIR}
        ${Vega_Strike_SOURCE_DIR}/engine
        ${Vega_Strike_SOURCE_DIR}/engine/src
        # Library Headers
        ${Vega_Strike_SOURCE_DIR}/libraries
        # CMake Artifacts
        ${Vega_Strike_BINARY_DIR}
        ${Vega_Strike_BINARY_DIR}/src
        ${Vega_Strike_BINARY_DIR}/engine
        ${Vega_Strike_BINARY_DIR}/engine/src
)

SET_TARGET_PROPERTIES(vs-buildenv-trisort PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
SET_PROPERTY(TARGET vs-buildenv-trisort PROPERTY CXX_STANDARD 14)
SET_PROPERTY(TARGET vs-buildenv-trisort PROPERTY CXX_STANDARD_REQUIRED TRUE)
SET_PROPERTY(TARGET vs-buildenv-trisort PROPERTY CXX_EXTENSIONS ON)

TARGET_COMPILE_DEFINITIONS(vs-buildenv-trisort PUBLIC "BOOST_ALL_DYN_LINK" "$<$<CONFIG:Debug>:BOOST_DEBUG_PYTHON>")
IF (WIN32)
    TARGET_COMPILE_DEFINITIONS(vs-buildenv-trisort PUBLIC BOOST_USE_WINAPI_VERSION=0x0A00)
    TARGET_COMPILE_DEFINITIONS(vs-buildenv-trisort PUBLIC _WIN32_WINNT=0x0A00)
    TARGET_COMPILE_DEFINITIONS(vs-buildenv-trisort PUBLIC WINVER=0x0A00)
    TARGET_COMPILE_DEFINITIONS(vs-buildenv-trisort PUBLIC "$<$<CONFIG:Debug>:Py_DEBUG>")
ENDIF()

SET(ASTEROID_SOURCES asteroidgen.cpp)
ADD_EXECUTABLE(vs-buildenv-asteroidgen ${ASTEROID_SOURCES})
TARGET_INCLUDE_DIRECTORIES(vs-buildenv-asteroidgen SYSTEM PRIVATE ${VSE_TST_INCLUDES})
TARGET_INCLUDE_DIRECTORIES(vs-buildenv-asteroidgen PRIVATE
        # VS engine headers
        ${Vega_Strike_SOURCE_DIR}
        ${Vega_Strike_SOURCE_DIR}/engine
        ${Vega_Strike_SOURCE_DIR}/engine/src
        # Library Headers
        ${Vega_Strike_SOURCE_DIR}/libraries
        # CMake Artifacts
        ${Vega_Strike_BINARY_DIR}
        ${Vega_Strike_BINARY_DIR}/src
        ${Vega_Strike_BINARY_DIR}/engine
        ${Vega_Strike_BINARY_DIR}/engine/src
)

SET_TARGET_PROPERTIES(vs-buildenv-asteroidgen PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
SET_PROPERTY(TARGET vs-buildenv-asteroidgen PROPERTY CXX_STANDARD 14)
SET_PROPERTY(TARGET vs-buildenv-asteroidgen PROPERTY CXX_STANDARD_REQUIRED TRUE)
SET_PROPERTY(TARGET vs-buildenv-asteroidgen PROPERTY CXX_EXTENSIONS ON)

TARGET_COMPILE_DEFINITIONS(vs-buildenv-asteroidgen PUBLIC "BOOST_ALL_DYN_LINK" "$<$<CONFIG:Debug>:BOOST_DEBUG_PYTHON>")
IF (WIN32)
    TARGET_COMPILE_DEFINITIONS(vs-buildenv-asteroidgen PUBLIC BOOST_USE_WINAPI_VERSION=0x0A00)
    TARGET_COMPILE_DEFINITIONS(vs-buildenv-asteroidgen PUBLIC _WIN32_WINNT=0x0A00)
    TARGET_COMPILE_DEFINITIONS(vs-buildenv-asteroidgen PUBLIC WINVER=0x0A00)
    TARGET_COMPILE_DEFINITIONS(vs-buildenv-asteroidgen PUBLIC "$<$<CONFIG:Debug>:Py_DEBUG>")
ENDIF()


SET(REPLACE_SOURCES replace.cpp)
ADD_EXECUTABLE(vs-buildenv-replace ${REPLACE_SOURCES})
TARGET_INCLUDE_DIRECTORIES(vs-buildenv-replace SYSTEM PRIVATE ${VSE_TST_INCLUDES})
TARGET_INCLUDE_DIRECTORIES(vs-buildenv-replace PRIVATE
        # VS engine headers
        ${Vega_Strike_SOURCE_DIR}
        ${Vega_Strike_SOURCE_DIR}/engine
        ${Vega_Strike_SOURCE_DIR}/engine/src
        # Library Headers
        ${Vega_Strike_SOURCE_DIR}/libraries
        # CMake Artifacts
        ${Vega_Strike_BINARY_DIR}
        ${Vega_Strike_BINARY_DIR}/src
        ${Vega_Strike_BINARY_DIR}/engine
        ${Vega_Strike_BINARY_DIR}/engine/src
)

SET_TARGET_PROPERTIES(vs-buildenv-replace PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
SET_PROPERTY(TARGET vs-buildenv-replace PROPERTY CXX_STANDARD 14)
SET_PROPERTY(TARGET vs-buildenv-replace PROPERTY CXX_STANDARD_REQUIRED TRUE)
SET_PROPERTY(TARGET vs-buildenv-replace PROPERTY CXX_EXTENSIONS ON)

TARGET_COMPILE_DEFINITIONS(vs-buildenv-replace PUBLIC "BOOST_ALL_DYN_LINK" "$<$<CONFIG:Debug>:BOOST_DEBUG_PYTHON>")
IF (WIN32)
    TARGET_COMPILE_DEFINITIONS(vs-buildenv-replace PUBLIC BOOST_USE_WINAPI_VERSION=0x0A00)
    TARGET_COMPILE_DEFINITIONS(vs-buildenv-replace PUBLIC _WIN32_WINNT=0x0A00)
    TARGET_COMPILE_DEFINITIONS(vs-buildenv-replace PUBLIC WINVER=0x0A00)
    TARGET_COMPILE_DEFINITIONS(vs-buildenv-replace PUBLIC "$<$<CONFIG:Debug>:Py_DEBUG>")
ENDIF()


#find Expat
FIND_PACKAGE(EXPAT REQUIRED)
IF (EXPAT_FOUND)
    SET(MSH_INCLUDES ${MSH_INCLUDES} ${EXPAT_INCLUDE_DIRS})
    SET(MSH_LIBS ${MSH_LIBS} ${EXPAT_LIBRARIES})
ELSE (EXPAT_FOUND)
    MESSAGE(WARNING "We can't find expat for XML crap")
ENDIF (EXPAT_FOUND)

#ogre already looked for by parent
IF (OGRE_FOUND)
    SET(MSH_INCLUDES ${MSH_INCLUDES} ${OGRE_INCLUDE_DIRS})
    SET(MSH_LIBS ${MSH_LIBS} ${OGRE_LIBRARIES})
    MESSAGE(STATUS "Compiling vega-meshtool with OgreMesh support")
ELSE (OGRE_FOUND)
    MESSAGE(STATUS "Compiling vega-meshtool without OgreMesh support: Ogre not found")
ENDIF (OGRE_FOUND)

SET(MESHER_SOURCES
    mesher/Converter.cpp
    mesher/from_BFXM.cpp
    mesher/from_obj.cpp
    mesher/main.cpp
    mesher/to_obj.cpp
    mesher/to_OgreMesh.cpp
    mesher/to_BFXM.cpp
    mesher/Modules/Convert.cpp
    mesher/Modules/OldSyntax.cpp
    mesher/Modules/Dims.cpp
    mesher/Modules/BFXM_to_Wavefront.cpp
    mesher/Modules/BFXM_to_XMesh.cpp
    mesher/Modules/XMesh_to_BFXM.cpp
    mesher/Modules/XMesh_to_Ogre.cpp
    mesher/Modules/Wavefront_to_BFXM.cpp
    ${Vega_Strike_SOURCE_DIR}/libraries/root_generic/xml_support.cpp
)

INCLUDE_DIRECTORIES(${MSH_INCLUDES} mesher)
ADD_EXECUTABLE(vega-meshtool ${MESHER_SOURCES})
TARGET_INCLUDE_DIRECTORIES(vega-meshtool SYSTEM PRIVATE ${VSE_TST_INCLUDES})
TARGET_INCLUDE_DIRECTORIES(vega-meshtool PRIVATE
        # VS engine headers
        ${Vega_Strike_SOURCE_DIR}
        ${Vega_Strike_SOURCE_DIR}/engine
        ${Vega_Strike_SOURCE_DIR}/engine/src
        # Library Headers
        ${Vega_Strike_SOURCE_DIR}/libraries
        # CMake Artifacts
        ${Vega_Strike_BINARY_DIR}
        ${Vega_Strike_BINARY_DIR}/src
        ${Vega_Strike_BINARY_DIR}/engine
        ${Vega_Strike_BINARY_DIR}/engine/src
)

SET_TARGET_PROPERTIES(vega-meshtool PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
SET_PROPERTY(TARGET vega-meshtool PROPERTY CXX_STANDARD 14)
SET_PROPERTY(TARGET vega-meshtool PROPERTY CXX_STANDARD_REQUIRED TRUE)
SET_PROPERTY(TARGET vega-meshtool PROPERTY CXX_EXTENSIONS ON)

TARGET_COMPILE_DEFINITIONS(vega-meshtool PUBLIC "BOOST_ALL_DYN_LINK" "$<$<CONFIG:Debug>:BOOST_DEBUG_PYTHON>")
IF (WIN32)
    TARGET_COMPILE_DEFINITIONS(vega-meshtool PUBLIC BOOST_USE_WINAPI_VERSION=0x0A00)
    TARGET_COMPILE_DEFINITIONS(vega-meshtool PUBLIC _WIN32_WINNT=0x0A00)
    TARGET_COMPILE_DEFINITIONS(vega-meshtool PUBLIC WINVER=0x0A00)
    TARGET_COMPILE_DEFINITIONS(vega-meshtool PUBLIC "$<$<CONFIG:Debug>:Py_DEBUG>")
ENDIF()

# OGRE already looked for by parent
# USE_SYSTEM_BOOST already set by main script
IF (OGRE_FOUND AND NOT USE_SYSTEM_BOOST)
    # Ogre usage in mesher requires boost::system
    ADD_DEPENDENCIES(vega-meshtool boost_system)
    SET(MSH_LIBS ${MSH_LIBS} boost_system)
ENDIF (OGRE_FOUND AND NOT USE_SYSTEM_BOOST)

TARGET_LINK_LIBRARIES(vega-meshtool ${MSH_LIBS})
INSTALL(TARGETS vega-meshtool DESTINATION bin)
