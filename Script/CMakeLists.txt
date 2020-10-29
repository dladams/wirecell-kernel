# myproj/Script/CMakeLists.txt

# Build setup file for this package.
string(TOUPPER ${PROJECT_NAME} PROJECT_UPNAME)
configure_file(setup.sh.in setup.sh)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/setup.sh DESTINATION .)

# Install scripts.
install(
PROGRAMS
  wirecell-kernel-help
  wckPlot
DESTINATION bin
)

# Intall project test.
#install(PROGRAMS test DESTINATION test)
