acts_add_library(
    PluginFpeMonitoring
    SHARED
    src/FpeMonitor.cpp
    ACTS_INCLUDE_FOLDER include/Acts
)
target_include_directories(
    ActsPluginFpeMonitoring
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(ActsPluginFpeMonitoring PUBLIC Acts::Core)

# Fpe flags
set(_fpe_options "")

include(CheckCXXSourceCompiles)

find_library(dl_LIBRARY dl)
find_package(Backtrace)
find_program(addr2line_EXECUTABLE addr2line)
if(APPLE)
    list(APPEND _fpe_options -D_GNU_SOURCE)
else()
    if(dl_LIBRARY)
        target_link_libraries(ActsPluginFpeMonitoring PUBLIC ${dl_LIBRARY})

        set(_backtrace_setup_complete FALSE)

        find_path(
            boost_stacktrace_include
            NAMES "boost/stacktrace.hpp"
            REQUIRED
        )
        if(Backtrace_FOUND)
            # check if we need to link against bracktrace or not
            set(backtrace_include "")

            file(
                WRITE
                "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/backtrace.cpp"
                "
        #include <boost/stacktrace.hpp>
        int main() {}
        "
            )

            message(CHECK_START "Does backtrace work with the default include")

            try_compile(
                _backtrace_default_header
                "${CMAKE_BINARY_DIR}"
                "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/backtrace.cpp"
                LINK_LIBRARIES ${dl_LIBRARY}
                CMAKE_FLAGS "-DINCLUDE_DIRECTORIES=${boost_stacktrace_include}"
                COMPILE_DEFINITIONS -DBOOST_STACKTRACE_USE_BACKTRACE
                OUTPUT_VARIABLE __OUTPUT
            )

            if(_backtrace_default_header)
                message(CHECK_PASS "yes")
            else()
                message(CHECK_FAIL "no")

                file(GLOB hints "/usr/lib/gcc/*/*/include")
                find_file(backtrace_header NAMES "backtrace.h" HINTS ${hints})

                if(${backtrace_header} STREQUAL "backtrace_header-NOTFOUND")
                    message(STATUS "Could not find backtrace header file")
                else()
                    set(backtrace_include
                        "-DBOOST_STACKTRACE_BACKTRACE_INCLUDE_FILE=\"${backtrace_header}\""
                    )

                    file(
                        WRITE
                        "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/backtrace.cpp"
                        "
            #include <boost/stacktrace.hpp>
            int main() {}
            "
                    )

                    message(
                        CHECK_START
                        "Does backtrace work with explicit include"
                    )

                    try_compile(
                        _backtrace_explicit_header
                        "${CMAKE_BINARY_DIR}"
                        "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/backtrace.cpp"
                        LINK_LIBRARIES ${dl_LIBRARY}
                        CMAKE_FLAGS
                            "-DINCLUDE_DIRECTORIES=${boost_stacktrace_include}"
                        COMPILE_DEFINITIONS
                            -DBOOST_STACKTRACE_USE_BACKTRACE
                            ${backtrace_include}
                        OUTPUT_VARIABLE __OUTPUT
                    )

                    if(_backtrace_explicit_header)
                        message(CHECK_PASS "yes")
                        list(APPEND _fpe_options "${backtrace_include}")
                    else()
                        message(CHECK_FAIL "no")
                    endif()
                endif()
            endif()

            file(
                WRITE
                "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/backtrace.cpp"
                "#include <boost/stacktrace.hpp>\n"
                "#include <iostream>\n"
                "int main() { std::cout << boost::stacktrace::stacktrace(); }\n"
            )

            message(CHECK_START "Does backtrace work without linker flag")
            try_compile(
                _backtrace_nolink
                "${CMAKE_BINARY_DIR}"
                "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/backtrace.cpp"
                LINK_LIBRARIES ${dl_LIBRARY}
                CMAKE_FLAGS "-DINCLUDE_DIRECTORIES=${boost_stacktrace_include}"
                COMPILE_DEFINITIONS
                    -DBOOST_STACKTRACE_USE_BACKTRACE
                    ${backtrace_include}
                OUTPUT_VARIABLE __OUTPUT
            )

            if(_backtrace_nolink)
                message(CHECK_PASS "yes")
                set(_backtrace_setup_complete TRUE)
            else()
                message(CHECK_FAIL "no")

                file(
                    WRITE
                    "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/backtrace.cpp"
                    "#include <boost/stacktrace.hpp>\n"
                    "#include <iostream>\n"
                    "int main() { std::cout << boost::stacktrace::stacktrace(); }\n"
                )

                message(CHECK_START "Does backtrace work with linker flag")
                try_compile(
                    _backtrace_link
                    "${CMAKE_BINARY_DIR}"
                    "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/backtrace.cpp"
                    LINK_LIBRARIES backtrace ${dl_LIBRARY}
                    CMAKE_FLAGS
                        "-DINCLUDE_DIRECTORIES=${boost_stacktrace_include}"
                    COMPILE_DEFINITIONS
                        -DBOOST_STACKTRACE_USE_BACKTRACE
                        ${backtrace_include}
                    OUTPUT_VARIABLE __OUTPUT
                )

                if(_backtrace_link)
                    message(CHECK_PASS "yes")
                    list(APPEND _fpe_options -DBOOST_STACKTRACE_USE_BACKTRACE)
                    target_link_libraries(
                        ActsPluginFpeMonitoring
                        PUBLIC backtrace
                    )

                    set(_backtrace_setup_complete TRUE)
                else()
                    message(CHECK_FAIL "no")
                endif()
            endif()
        endif()

        if(NOT _backtrace_setup_complete)
            message(STATUS "Unable to set up stacktrace setup: use noop")
            list(APPEND _fpe_options -BOOST_STACKTRACE_USE_NOOP)
        endif()
    endif()
endif()

target_compile_options(ActsPluginFpeMonitoring PUBLIC "${_fpe_options}")
