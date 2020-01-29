cmake_minimum_required(VERSION 3.5)

project(QEU4AchivementManager LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Core Quick Network REQUIRED)

file (GLOB CppSrc
        src/*.cpp
		src/*.hpp
)

include_directories("submodules/expected/include")

if(ANDROID)
    add_library(QEU4AchivementManager SHARED
      ${CppSrc}
      qml.qrc
      images.qrc
    )
else()
    add_executable(QEU4AchivementManager
      ${CppSrc}
      qml.qrc
      images.qrc
    )
endif()

target_compile_definitions(QEU4AchivementManager
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(QEU4AchivementManager
  PRIVATE Qt5::Core Qt5::Quick)

  
# copy the required librairies
get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

message(${_qt_bin_dir})

if(WIN32)
	add_custom_command(TARGET QEU4AchivementManager POST_BUILD
	    COMMAND "${CMAKE_COMMAND}" -E
	        env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
	            --verbose 0
	            --no-compiler-runtime
				 --qmldir ${PROJECT_SOURCE_DIR}/qml
	            \"$<TARGET_FILE:QEU4AchivementManager>\"
	    COMMENT "Deploying Qt..."
	)
endif()
