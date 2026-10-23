cmake_minimum_required(VERSION 3.30.5)
project(OpenCvLearning VERSION 0.1.0 LANGUAGES CXX)

if(NOT CMAKE_VERSION VERSION_EQUAL "3.30.5")
    message(FATAL_ERROR "This learning baseline requires CMake 3.30.5")
endif()
if(NOT WIN32 OR NOT MSVC OR NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Use MSVC x64 for the first learning baseline")
endif()
if(NOT CMAKE_CXX_COMPILER MATCHES "14[.]44[.]35207")
    message(FATAL_ERROR "Select the locked MSVC 14.44.35207 toolchain")
endif()
if(NOT CMAKE_CXX_COMPILER_ARCHITECTURE_ID STREQUAL "x64")
    message(FATAL_ERROR "Select the x64 compiler target, not ARM64 or x86")
endif()
if(NOT CMAKE_SYSTEM_PROCESSOR)
    # Some Windows environments omit this; compiler architecture is verified above.
    set(CMAKE_SYSTEM_PROCESSOR AMD64)
endif()
if(NOT CMAKE_BUILD_TYPE MATCHES "^(Debug|Release)$")
    message(FATAL_ERROR "Select Debug or Release")
endif()
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
find_package(Qt6 6.8.3 EXACT REQUIRED COMPONENTS Core Gui Widgets Test)
find_package(OpenCV 5.0.0 EXACT REQUIRED COMPONENTS core imgproc imgcodecs)
find_package(spdlog 1.17.0 EXACT CONFIG REQUIRED
    PATHS "${PROJECT_SOURCE_DIR}/build/deps/vcpkg_installed/cvlab-x64-windows-static-md/share/spdlog"
    NO_DEFAULT_PATH)
include(CTest)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)
target_compile_options(project_options INTERFACE /W4 /WX /permissive- /Zc:__cplusplus /utf-8 /EHsc)
target_compile_definitions(project_options INTERFACE NOMINMAX WIN32_LEAN_AND_MEAN)
target_include_directories(project_options INTERFACE "${PROJECT_SOURCE_DIR}/src")

add_library(core STATIC src/core/diagnostics/Logger.h src/core/diagnostics/Logger.cpp)
target_link_libraries(core PUBLIC project_options PRIVATE spdlog::spdlog)

add_library(business STATIC src/business/models/ImageProperties.h src/business/services/ImageInspection.h src/business/services/ImageInspection.cpp)
target_link_libraries(business PUBLIC core opencv_core)
target_include_directories(business SYSTEM PUBLIC ${OpenCV_INCLUDE_DIRS})

add_library(presentation STATIC src/presentation/widgets/ImageConversion.h src/presentation/widgets/ImageConversion.cpp
    src/presentation/widgets/MainWindow.h src/presentation/widgets/MainWindow.cpp src/presentation/widgets/MainWindow.ui)
set_target_properties(presentation PROPERTIES AUTOMOC ON AUTOUIC ON)
target_link_libraries(presentation PUBLIC business Qt6::Widgets PRIVATE opencv_imgproc)

add_library(application_runtime STATIC src/app/ApplicationRuntime.h src/app/ApplicationRuntime.cpp)
target_link_libraries(application_runtime PUBLIC core Qt6::Core)
add_executable(environment_probe src/app/EnvironmentMain.cpp)
target_link_libraries(environment_probe PRIVATE application_runtime business)
add_executable(mat_demo src/app/MatDemoMain.cpp)
target_link_libraries(mat_demo PRIVATE application_runtime business)
add_executable(vision_viewer WIN32 src/app/ViewerMain.cpp resources/learning.qrc)
set_target_properties(vision_viewer PROPERTIES AUTORCC ON)
target_link_libraries(vision_viewer PRIVATE application_runtime presentation)
install(TARGETS environment_probe mat_demo vision_viewer RUNTIME DESTINATION .)

# Offline course experiments share the fixed foundation dependencies.
add_executable(foundation_labs src/app/FoundationLabs.cpp)
add_executable(advanced_labs src/app/AdvancedLabs.cpp)
add_executable(engineering_labs src/app/EngineeringLabs.cpp)
foreach(lab_target IN ITEMS foundation_labs advanced_labs engineering_labs)
    target_link_libraries(${lab_target} PRIVATE application_runtime business opencv_imgproc opencv_imgcodecs opencv_geometry)
endforeach()
install(TARGETS foundation_labs advanced_labs engineering_labs RUNTIME DESTINATION .)

if(BUILD_TESTING)
    foreach(lesson RANGE 3 30)
        if(lesson LESS 14)
            set(lab_target foundation_labs)
        elseif(lesson LESS 24)
            set(lab_target advanced_labs)
        else()
            set(lab_target engineering_labs)
        endif()
        add_test(NAME course_${lesson} COMMAND ${lab_target} ${lesson} "${CMAKE_BINARY_DIR}/course-output/${lesson}")
        set_tests_properties(course_${lesson} PROPERTIES
            ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:$<TARGET_FILE_DIR:Qt6::Core>;PATH=path_list_prepend:$<TARGET_FILE_DIR:opencv_core>"
            TIMEOUT 45)
    endforeach()
    add_executable(logger_tests tests/unit/TestLogger.cpp)
    set_target_properties(logger_tests PROPERTIES AUTOMOC ON)
    target_link_libraries(logger_tests PRIVATE core Qt6::Test)
    add_test(NAME logger_contracts COMMAND logger_tests -o logger-results.txt,txt -o -,txt)
    set_tests_properties(logger_contracts PROPERTIES
        ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:$<TARGET_FILE_DIR:Qt6::Core>" TIMEOUT 30)
    add_executable(learning_tests tests/unit/TestLearning.cpp)
    set_target_properties(learning_tests PROPERTIES AUTOMOC ON)
    target_link_libraries(learning_tests PRIVATE presentation Qt6::Test Qt6::Widgets)
    add_test(NAME learning_contracts COMMAND learning_tests -o learning-results.txt,txt -o -,txt)
    set_tests_properties(learning_contracts PROPERTIES
        ENVIRONMENT "QT_QPA_PLATFORM=offscreen"
        ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:$<TARGET_FILE_DIR:Qt6::Core>;PATH=path_list_prepend:$<TARGET_FILE_DIR:opencv_core>"
        TIMEOUT 30)
    add_executable(window_tests tests/unit/TestWindow.cpp resources/learning.qrc)
    set_target_properties(window_tests PROPERTIES AUTOMOC ON AUTORCC ON)
    target_link_libraries(window_tests PRIVATE presentation Qt6::Test)
    add_test(NAME window_contracts COMMAND window_tests -o window-results.txt,txt -o -,txt)
    set_tests_properties(window_contracts PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen;QT_QPA_FONTDIR=$ENV{SystemRoot}/Fonts"
        ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:$<TARGET_FILE_DIR:Qt6::Core>;PATH=path_list_prepend:$<TARGET_FILE_DIR:opencv_core>" TIMEOUT 30)
    add_library(header_checks OBJECT tests/unit/HeaderLogger.cpp tests/unit/HeaderProperties.cpp tests/unit/HeaderInspection.cpp tests/unit/HeaderConversion.cpp tests/unit/HeaderWindow.cpp tests/unit/HeaderRuntime.cpp)
    target_link_libraries(header_checks PRIVATE presentation)
    add_test(NAME environment_smoke COMMAND environment_probe)
    add_test(NAME mat_smoke COMMAND mat_demo)
    set_tests_properties(environment_smoke mat_smoke PROPERTIES
        ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:$<TARGET_FILE_DIR:Qt6::Core>;PATH=path_list_prepend:$<TARGET_FILE_DIR:opencv_core>" TIMEOUT 30)
    find_package(Python3 3.9 REQUIRED COMPONENTS Interpreter)
    set(reference_answers
        solutions/0002-image-mat/README.md
        solutions/0014-calibration/README.md solutions/0015-stereo-pose/README.md
        solutions/0016-video-time/README.md solutions/0017-tracking/README.md
        solutions/0018-optics-lighting/README.md solutions/0019-subpixel/README.md
        solutions/0020-defects/README.md solutions/0021-classification/README.md
        solutions/0022-dnn-contract/README.md solutions/0023-detection-segmentation/README.md
        solutions/0024-performance-memory/README.md solutions/0027-source-comparison/README.md)
    add_custom_command(OUTPUT "${CMAKE_BINARY_DIR}/generated/ReferenceAnswers.cpp"
        COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/scripts/ExtractReferenceChecks.py" "${PROJECT_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/generated/ReferenceAnswers.cpp"
        DEPENDS scripts/ExtractReferenceChecks.py docs/course-catalog.json ${reference_answers}
        VERBATIM)
    add_executable(reference_answer_tests EXCLUDE_FROM_ALL "${CMAKE_BINARY_DIR}/generated/ReferenceAnswers.cpp")
    target_link_libraries(reference_answer_tests PRIVATE business opencv_imgproc)
    foreach(lab_target IN ITEMS foundation_labs advanced_labs engineering_labs)
        add_test(NAME ${lab_target}_reject_invalid COMMAND ${lab_target} 99 "${CMAKE_BINARY_DIR}/invalid-course-output")
        set_tests_properties(${lab_target}_reject_invalid PROPERTIES WILL_FAIL TRUE TIMEOUT 10
            ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:$<TARGET_FILE_DIR:Qt6::Core>;PATH=path_list_prepend:$<TARGET_FILE_DIR:opencv_core>")
    endforeach()
    add_test(NAME lesson_documents COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/scripts/VerifyLessons.py")
endif()
