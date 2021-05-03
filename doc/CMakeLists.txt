message(STATUS "Building documentation")
find_program(MakeExecutable make
  DOC "The make executable"
  REQUIRED
  )
message(STATUS "Building documentation")
find_program(Rst2PdfExecutable rst2pdf
  DOC "The rst2pdf executable"
  REQUIRED
  )

add_subdirectory(build-ltd)
add_subdirectory(build-fsf)
