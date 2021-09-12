cmake_minimum_required(VERSION 2.6)

project(issheSocks C)

set(issheSocksClient
    src/isshe_socks_client.c)

set(libs
    event)

ADD_DEFINITIONS(-Wall -O2 --std=gnu99 -Wmissing-declarations)

# 是否是MacOS
if(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD" OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    include_directories( /usr/local/include )
    link_directories( /usr/local/lib )
endif()

add_executable(isshe_socks_client ${issheSocksClient})
target_link_libraries(isshe_socks_client ${link_flag} ${libs})

install(TARGETS isshe_socks_client
        RUNTIME DESTINATION bin
)
