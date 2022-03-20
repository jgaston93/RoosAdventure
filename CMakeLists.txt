cmake_minimum_required(VERSION 3.10)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(SDL2_INCLUDE_DIR C:/Users/jgast/Documents/Source/SDL2-2.0.18/x86_64-w64-mingw32/include)
set(SDL2_LIB_DIR  C:/Users/jgast/Documents/Source/SDL2-2.0.18/x86_64-w64-mingw32/lib)

# set the project name
project(RoosAdventure)

# add the executable
add_executable(RoosAdventure src/GameUtilityFunctions.cpp
                                src/CollisionCheckingFunctions.cpp
                                src/BedMazeLevelFunctions.cpp
                                src/BedroomLevelFunctions.cpp
                                src/ComputerLevelFunctions.cpp
                                src/DoorLevelFunctions.cpp
                                src/LivingRoomLevelFunctions.cpp
                                src/OfficeLevelFunctions.cpp
                                src/OutsideLevelFunctions.cpp
                                src/KitchenLevelFunctions.cpp
                                src/BottomKitchenLevelFunctions.cpp
                                src/HallwayLevelFunctions.cpp
                                src/main.cpp)

target_include_directories(RoosAdventure PUBLIC inc
                                                ${SDL2_INCLUDE_DIR})

target_link_directories(RoosAdventure PUBLIC ${SDL2_LIB_DIR})

target_link_libraries(RoosAdventure PUBLIC -lmingw32
                                            -lSDL2main
                                            -lSDL2
                                            -lSDL2_image
                                            -mwindows)

add_custom_command(TARGET RoosAdventure POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${PROJECT_SOURCE_DIR}/lib/libgcc_s_seh-1.dll"
                    "${PROJECT_SOURCE_DIR}/lib/libjpeg-9.dll"
                    "${PROJECT_SOURCE_DIR}/lib/libpng16-16.dll"
                    "${PROJECT_SOURCE_DIR}/lib/libstdc++-6.dll"
                    "${PROJECT_SOURCE_DIR}/lib/libtiff-5.dll"
                    "${PROJECT_SOURCE_DIR}/lib/libwebp-7.dll"
                    "${PROJECT_SOURCE_DIR}/lib/libwinpthread-1.dll"
                    "${PROJECT_SOURCE_DIR}/lib/SDL2_image.dll"
                    "${PROJECT_SOURCE_DIR}/lib/SDL2.dll"
                    "${PROJECT_SOURCE_DIR}/lib/zlib1.dll"
                    $<TARGET_FILE_DIR:RoosAdventure>)