# myproj/Data/CMakeLists.txt

# Install data files.
install(
  DIRECTORY .
  DESTINATION data
  FILES_MATCHING
    PATTERN "*.txt"
    PATTERN "*.root"
    PATTERN "*.json"
    PATTERN "CMakeLists.txt" EXCLUDE
)
