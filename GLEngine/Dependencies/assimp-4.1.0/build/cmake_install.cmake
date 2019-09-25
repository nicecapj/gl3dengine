# Install script for directory: C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Project")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/build/Debug/assimp-vc140-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/build/Release/assimp-vc140-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/build/MinSizeRel/assimp-vc140-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/build/RelWithDebInfo/assimp-vc140-mt.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/assimp/anim.h;/assimp/ai_assert.h;/assimp/camera.h;/assimp/color4.h;/assimp/color4.inl;/assimp/config.h;/assimp/defs.h;/assimp/Defines.h;/assimp/cfileio.h;/assimp/light.h;/assimp/material.h;/assimp/material.inl;/assimp/matrix3x3.h;/assimp/matrix3x3.inl;/assimp/matrix4x4.h;/assimp/matrix4x4.inl;/assimp/mesh.h;/assimp/postprocess.h;/assimp/quaternion.h;/assimp/quaternion.inl;/assimp/scene.h;/assimp/metadata.h;/assimp/texture.h;/assimp/types.h;/assimp/vector2.h;/assimp/vector2.inl;/assimp/vector3.h;/assimp/vector3.inl;/assimp/version.h;/assimp/cimport.h;/assimp/importerdesc.h;/assimp/Importer.hpp;/assimp/DefaultLogger.hpp;/assimp/ProgressHandler.hpp;/assimp/IOStream.hpp;/assimp/IOSystem.hpp;/assimp/Logger.hpp;/assimp/LogStream.hpp;/assimp/NullLogger.hpp;/assimp/cexport.h;/assimp/Exporter.hpp;/assimp/DefaultIOStream.h;/assimp/DefaultIOSystem.h;/assimp/SceneCombiner.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/assimp" TYPE FILE FILES
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/anim.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/ai_assert.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/camera.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/color4.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/color4.inl"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/build/../include/assimp/config.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/defs.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/Defines.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/cfileio.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/light.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/material.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/material.inl"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/matrix3x3.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/matrix3x3.inl"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/matrix4x4.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/matrix4x4.inl"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/mesh.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/postprocess.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/quaternion.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/quaternion.inl"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/scene.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/metadata.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/texture.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/types.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/vector2.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/vector2.inl"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/vector3.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/vector3.inl"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/version.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/cimport.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/importerdesc.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/Importer.hpp"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/DefaultLogger.hpp"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/ProgressHandler.hpp"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/IOStream.hpp"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/IOSystem.hpp"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/Logger.hpp"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/LogStream.hpp"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/NullLogger.hpp"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/cexport.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/Exporter.hpp"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/DefaultIOStream.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/DefaultIOSystem.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/SceneCombiner.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/assimp/Compiler/pushpack1.h;/assimp/Compiler/poppack1.h;/assimp/Compiler/pstdint.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/assimp/Compiler" TYPE FILE FILES
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/Compiler/pushpack1.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/Compiler/poppack1.h"
    "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/workUnreal/gl3dengine/GLEngine/Dependencies/assimp-4.1.0/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
