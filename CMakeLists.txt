cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("hadouken")

include_directories(
    C:/Code/vktr/io.js/src
    C:/Code/vktr/io.js/deps/cares/include
    C:/Code/vktr/io.js/deps/uv/include
    C:/Code/vktr/io.js/deps/v8/include
    C:/Libs/boost_1_58_0
    C:/Libs/libtorrent-rasterbar-1.0.5/include
)

link_directories(
    C:/Code/vktr/io.js/Release/lib
    C:/Code/vktr/io.js/build/Release/lib
    C:/Libs/boost_1_58_0/stage/lib
    C:/Libs/libtorrent-rasterbar-1.0.5/bin/msvc-12.0/release/address-model-64/boost-source/link-static/runtime-link-static/threading-multi
)

set(
    IOJS_LIBS
    cares.lib
    http_parser.lib
    iojs.lib
    iphlpapi.lib
    libuv.lib
    openssl.lib
    psapi.lib
    userenv.lib
    v8_base.lib
    v8_libbase.lib
    v8_libplatform.lib
    v8_nosnapshot.lib
    winmm.lib
    ws2_32.lib
    zlib.lib
)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

add_definitions(
    -DBOOST_ASIO_SEPARATE_COMPILATION
)

add_executable(
    hadouken
    src/libtorrent
    src/lt/fingerprint
    src/lt/session
    src/main
)

target_link_libraries(
    hadouken
    ${IOJS_LIBS}
    libtorrent.lib
)
