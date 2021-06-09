message(STATUS "Building LTD documentation")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "SNRS_DATA_DIR=${SNRS_DATA_DIR}")

add_custom_command(OUTPUT snrs-build-ltd.pdf
  # COMMAND test -L ${CMAKE_CURRENT_SOURCE_DIR}/images && rm -f ${CMAKE_CURRENT_SOURCE_DIR}/images
  COMMAND rm -f ${CMAKE_CURRENT_SOURCE_DIR}/tmpimg
  COMMAND ln -s ${SNRS_DATA_DIR}/geometry/source_foils/ltd/images ${CMAKE_CURRENT_SOURCE_DIR}/tmpimg
  COMMAND make snrs-build-ltd.pdf
  COMMAND rm -f ${CMAKE_CURRENT_SOURCE_DIR}/tmpimg
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/snrs-build-ltd.rst
  VERBATIM
  )

add_custom_target(docBuildLtd ALL
  DEPENDS snrs-build-ltd.pdf
  COMMENT "Build the documentation about the building of LTD datasets"
  VERBATIM
  )

add_dependencies(docBuildLtd snrsBuildLtdDataset)

install(FILES snrs-build-ltd.pdf
  DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/SNRS/documentation/
  )
 
# - end