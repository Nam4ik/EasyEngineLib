cmake_minimum_required(VERSION 3.10)  
project(EasyEngine 
    VERSION 1.0 
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_PREFIX /usr/local)

find_package(PocketSphinx   REQUIRED)
find_package(pybind11       REQUIRED)
find_package(cJSON          REQUIRED)
find_package(OpenCV         REQUIRED)
find_package(SDL2           REQUIRED)
find_package(SDL2_image     REQUIRED)
find_package(SDL2_gfx       REQUIRED)
find_package(PortAudio      REQUIRED)
find_package(Threads        REQUIRED)

add_subdirectory(src)

add_executable(Engine  
    src/engine.cpp 
    src/binding.cpp
    src/convert.c
    buf/lib/OpenCV.cpp  # Проверьте путь!
    src/lib/utils.cpp   # Проверьте путь!
)

target_include_directories(Engine PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/buf/lib
)

target_link_libraries(Engine PRIVATE 
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_gfx::SDL2_gfx
    pybind11::pybind11
    cJSON::cJSON
    ${OpenCV_LIBS}
    PortAudio::PortAudio
    Threads::Threads
    PocketSphinx::PocketSphinx
)

include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "EasyEngine")
set(CPACK_PACKAGE_VERSION "1.0")
include(CPack)
