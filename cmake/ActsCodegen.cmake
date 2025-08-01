include_guard(GLOBAL)

message(STATUS "Configuring codegen: preparing uv")

find_program(uv_exe uv)

set(_uv_version "0.7.19")
set(_base_url
    "https://github.com/astral-sh/uv/releases/download/${_uv_version}"
)

if(uv_exe STREQUAL "uv_exe-NOTFOUND")
    message(STATUS "uv not found, installing it")

    if(NOT APPLE AND NOT UNIX)
        message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
    endif()

    if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
        if(APPLE)
            set(UV_NAME "${_base_url}/uv-x86_64-apple-darwin.tar.gz")
            set(UV_HASH
                "SHA256=698d24883fd441960fb4bc153b7030b89517a295502017ff3fdbba2fb0a0aa67"
            )
        elseif(UNIX)
            set(UV_URL "${_base_url}/uv-x86_64-unknown-linux-musl.tar.gz")
            set(UV_HASH
                "SHA256=6236ed00a7442ab2c0f56f807d5a3331f3fb5c7640a357482fbc8492682641b2"
            )
        endif()
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "(arm)|(ARM)|(aarch64)")
        if(APPLE)
            set(UV_URL "${_base_url}/uv-aarch64-apple-darwin.tar.gz")
            set(UV_HASH
                "SHA256=698d24883fd441960fb4bc153b7030b89517a295502017ff3fdbba2fb0a0aa67"
            )
        elseif(UNIX)
            set(UV_URL "${_base_url}/uv-aarch64-unknown-linux-musl.tar.gz")
            set(UV_HASH
                "SHA256=e83c7c6d86c8e7456078c736a72550ce20222df8083f9317fc58cd49422ce5eb"
            )
        endif()
    else()
        message(
            FATAL_ERROR
            "Unsupported architecture: ${CMAKE_SYSTEM_PROCESSOR}"
        )
    endif()

    message(STATUS "Downloading uv from ${UV_URL}")
    set(UV_DIR "${CMAKE_BINARY_DIR}/uv")
    file(DOWNLOAD ${UV_URL} ${UV_DIR}/uv.tar.gz EXPECTED_HASH ${UV_HASH})

    file(ARCHIVE_EXTRACT INPUT ${UV_DIR}/uv.tar.gz DESTINATION ${UV_DIR})

    file(REMOVE ${UV_DIR}/uv.tar.gz)

    file(GLOB uv_extracted ${UV_DIR}/uv*)
    message(STATUS "Extracted uv: ${uv_extracted}")

    find_program(uv_exe uv PATHS ${uv_extracted} REQUIRED NO_DEFAULT_PATH)
endif()

message(STATUS "Found uv: ${uv_exe}")

execute_process(
    COMMAND ${uv_exe} --version
    OUTPUT_VARIABLE uv_version
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "uv version: ${uv_version}")

function(acts_code_generation)
    set(options ISOLATED)
    set(oneValueArgs TARGET PYTHON PYTHON_VERSION OUTPUT)
    set(multiValueArgs DEPENDS WITH_REQUIREMENTS WITH)
    cmake_parse_arguments(
        PARSE_ARGV
        0
        ARGS
        "${options}"
        "${oneValueArgs}"
        "${multiValueArgs}"
    )

    if(NOT DEFINED ARGS_PYTHON_VERSION)
        set(ARGS_PYTHON_VERSION "3.13")
    endif()

    if(NOT DEFINED ARGS_PYTHON)
        message(SEND_ERROR "No python script specified")
        return()
    endif()

    if(NOT EXISTS ${ARGS_PYTHON})
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${ARGS_PYTHON})
            set(ARGS_PYTHON ${CMAKE_CURRENT_SOURCE_DIR}/${ARGS_PYTHON})
        else()
            message(SEND_ERROR "Python script not found: ${ARGS_PYTHON}")
            return()
        endif()
    endif()

    set(_arg_isolated "")
    if(ARGS_ISOLATED)
        set(_arg_isolated "--isolated")
    endif()

    set(_depends "${ARGS_PYTHON}")
    set(_with_args "")
    foreach(_requirement ${ARGS_WITH_REQUIREMENTS})
        list(APPEND _depends ${_requirement})
        list(APPEND _with_args "--with-requirements;${_requirement}")
    endforeach()

    foreach(_requirement ${ARGS_WITH})
        list(APPEND _with_args "--with;${_requirement}")
        if(IS_DIRECTORY ${_requirement})
            file(GLOB_RECURSE _depends_py ${_requirement}/*)
            list(APPEND _depends ${_depends_py})
        endif()
    endforeach()

    get_filename_component(_output_name ${ARGS_OUTPUT} NAME)

    set(_codegen_root ${CMAKE_CURRENT_BINARY_DIR}/codegen/${ARGS_TARGET})
    set(_output_file ${_codegen_root}/${ARGS_OUTPUT})

    get_filename_component(_output_dir ${_output_file} DIRECTORY)
    file(MAKE_DIRECTORY ${_output_dir})

    add_custom_command(
        OUTPUT ${_output_file}
        COMMAND
            env -i UV_NO_CACHE=1 ${uv_exe} run --quiet --python
            ${ARGS_PYTHON_VERSION} --no-project ${_arg_isolated} ${_with_args}
            ${ARGS_PYTHON} ${_output_file}
        DEPENDS ${_depends}
        COMMENT "Generating ${ARGS_OUTPUT}"
        VERBATIM
    )

    add_custom_target(${ARGS_TARGET}_Internal DEPENDS ${_output_file})
    add_library(${ARGS_TARGET} INTERFACE)
    add_library(Acts::${ARGS_TARGET} ALIAS ${ARGS_TARGET})
    target_include_directories(${ARGS_TARGET} INTERFACE ${_codegen_root})

    add_dependencies(${ARGS_TARGET} ${ARGS_TARGET}_Internal)
endfunction()
