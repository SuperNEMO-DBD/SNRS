message(STATUS "Building LTD documentation")

add_custom_command(OUTPUT snrs-build-ltd.pdf
  # COMMAND test -L ${CMAKE_CURRENT_SOURCE_DIR}/images && rm -f ${CMAKE_CURRENT_SOURCE_DIR}/images
  COMMAND ln -s ${SNRS_DATA_DIR}/geometry/source_foils/ltd/images ${CMAKE_CURRENT_SOURCE_DIR}/images
  COMMAND make snrs-build-ltd.pdf
  COMMAND rm -f ${CMAKE_CURRENT_SOURCE_DIR}/images
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