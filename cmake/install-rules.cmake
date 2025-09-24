if(PROJECT_IS_TOP_LEVEL)
  set(
      CMAKE_INSTALL_INCLUDEDIR "include/tasty-${PROJECT_VERSION}"
      CACHE STRING ""
  )
  set_property(CACHE CMAKE_INSTALL_INCLUDEDIR PROPERTY TYPE PATH)
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package tasty)

install(
    DIRECTORY
    include/
    "${PROJECT_BINARY_DIR}/export/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT tasty_Development
)

install(
    TARGETS tasty_tasty
    EXPORT tastyTargets
    RUNTIME #
    COMPONENT tasty_Runtime
    LIBRARY #
    COMPONENT tasty_Runtime
    NAMELINK_COMPONENT tasty_Development
    ARCHIVE #
    COMPONENT tasty_Development
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    tasty_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${package}"
    CACHE STRING "CMake package config location relative to the install prefix"
)
set_property(CACHE tasty_INSTALL_CMAKEDIR PROPERTY TYPE PATH)
mark_as_advanced(tasty_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${tasty_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT tasty_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${tasty_INSTALL_CMAKEDIR}"
    COMPONENT tasty_Development
)

install(
    EXPORT tastyTargets
    NAMESPACE tasty::
    DESTINATION "${tasty_INSTALL_CMAKEDIR}"
    COMPONENT tasty_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
