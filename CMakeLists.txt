cmake_minimum_required(VERSION 3.15)
project(Rash LANGUAGES CXX)

# Zwingt CMake, die compile_commands.json für Neovim/clangd zu generieren
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Nutzen von modernem C++ (z.B. C++20, kannst du bei Bedarf auf 17 ändern)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Alle Quelldateien definieren
set(SOURCES
    src/main.cpp
    src/shell.cpp
    src/builtin.cpp
)

# Das ausführbare Programm (Executable) definieren
add_executable(rash ${SOURCES})

# Den "inc"-Ordner für Header-Dateien bekannt machen
target_include_directories(rash PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)

# Sinnvolle Compiler-Warnungen aktivieren
if(MSVC)
  target_compile_options(rash PRIVATE /W4)
else()
  target_compile_options(rash PRIVATE -Wall -Wextra -Wpedantic)
endif()

