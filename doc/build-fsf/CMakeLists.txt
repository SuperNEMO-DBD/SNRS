message(STATUS "Building FSF documentation")

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/snrs-build-fsf.pdf
  COMMAND rm -f ${CMAKE_CURRENT_SOURCE_DIR}/tmpimg
  COMMAND ln -s ${SNRS_DATA_DIR}/geometry/source_foils/fsf/images ${CMAKE_CURRENT_SOURCE_DIR}/tmpimg
  COMMAND make snrs-build-fsf.pdf
  COMMAND rm -f ${CMAKE_CURRENT_SOURCE_DIR}/tmpimg
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/snrs-build-fsf.rst
  VERBATIM
  )

add_custom_target(docBuildFsf ALL
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/snrs-build-fsf.pdf
  COMMENT "Build the documentation about the building of FSF datasets"
  VERBATIM
  )

add_dependencies(docBuildFsf snrsBuildFsfDataset)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/snrs-build-fsf.pdf
  DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/SNRS/documentation/
  )
 
# - end