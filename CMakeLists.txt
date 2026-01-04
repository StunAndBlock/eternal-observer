cmake_minimum_required(VERSION 3.14)
project(ethernal-observer LANGUAGES CXX)


add_definitions(-DUNICODE -D_UNICODE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()


# Рекурсивно собираем все исходники из src/
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)

# Добавляем исполняемый файл
add_executable(${PROJECT_NAME} WIN32 ${SOURCES})

# Указываем include директории
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/internal       # свои заголовки
         ${CMAKE_CURRENT_SOURCE_DIR}/include/external  
)

target_link_libraries(${PROJECT_NAME}
    comctl32
    shell32
)


if(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
        LINK_FLAGS "/SUBSYSTEM:WINDOWS"
    )
endif()