if(${PACKAGE_GRIDGEN})
  file(GLOB PACKAGE_GRIDGEN_SOURCE "*.cc")
  file(GLOB PACKAGE_GRIDGEN_HEADER "*.h")

  add_library(package_gridgen STATIC
          ${PACKAGE_GRIDGEN_SOURCE}
          ${PACKAGE_GRIDGEN_HEADER}
          )
endif()