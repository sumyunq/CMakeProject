include_directories ("${PROJECT_SOURCE_DIR}/../src/FileOperator/")
aux_source_directory(../src/FileOperator/ SOU_FILE)

add_executable(test1 test_FileOperator.cc ${SOU_FILE})

