# - Test programs:

# add_executable(test-pad
#   snrs/test/test-pad.cxx
#   )
# target_link_libraries(test-pad snrs)

# add_executable(test-ltd
#   snrs/test/test-ltd.cxx
#   )
# target_link_libraries(test-ltd snrs)

# add_executable(test-fsfs
#   snrs/test/test-fsfs.cxx
#   )
# target_link_libraries(test-fsfs snrs)

# add_executable(test-pad_tessellation
#   snrs/test/test-pad_tessellation.cxx
#   )
# target_link_libraries(test-pad_tessellation snrs)

if(snrs_generate_tests)

  set(snrs_TESTS
    test-sngeom.cxx
    test-mesh_pad_model.cxx
    test-mesh_pad_vg.cxx
    test-mesh_pad_vg_mgr.cxx
    )

  set(snrs_TEST_ENVIRONMENT
    "SNRS_TESTING_DIR=${CMAKE_CURRENT_SOURCE_DIR}"
    "SNRS_RESOURCE_PATH=${PROJECT_SOURCE_DIR}/resources"
    "SNRS_BUILD_DIR=${CMAKE_BINARY_DIR}"
    )
  
  foreach(_testsource ${snrs_TESTS})
    get_filename_component(_testname "${_testsource}" NAME_WE)
    set(_testname "snrs-${_testname}")
    add_executable(${_testname} ${_testsource})
    # Output to testing subdirectory
    set_property(TARGET ${_testname}
      PROPERTY RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY}"
      )
    target_link_libraries(${_testname} snrs)
    add_test(NAME ${_testname} COMMAND ${_testname})
    set_property(TARGET ${_testname}
      APPEND PROPERTY INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
      )
    set_property(TEST ${_testname}
      APPEND PROPERTY ENVIRONMENT ${snrs_TEST_ENVIRONMENT}
      )
    
  endforeach()

  add_test(NAME snrs-test-geomgr COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/test-geomgr.bash)
  set_property(TEST snrs-test-geomgr
    APPEND PROPERTY ENVIRONMENT ${snrs_TEST_ENVIRONMENT}
    )
  
endif(snrs_generate_tests)