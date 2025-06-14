cmake_minimum_required(VERSION 3.16)
project(minipromptGPT)

# Définir le standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options de compilation pour le debug et les warnings
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Inclure FetchContent pour télécharger les dépendances
include(FetchContent)

# Télécharger nlohmann/json
FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

# Définir les sources (tous dans le même dossier)
set(SOURCES
        main.cpp
        PromptManager.cpp
)

set(HEADERS
        PromptManager.h
)

# Créer l'exécutable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Lier les bibliothèques
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

# Définir le répertoire d'inclusion
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Copier le fichier de configuration JSON s'il existe
if(EXISTS "${CMAKE_SOURCE_DIR}/prompts.json")
    configure_file(
            "${CMAKE_SOURCE_DIR}/prompts.json"
            "${CMAKE_BINARY_DIR}/prompts.json"
            COPYONLY
    )
endif()

# Options pour différents compilateurs
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Afficher des informations de configuration
message(STATUS "Configuration terminée pour ${PROJECT_NAME}")
message(STATUS "Standard C++ : ${CMAKE_CXX_STANDARD}")
message(STATUS "Type de build : ${CMAKE_BUILD_TYPE}")

# Optionnel : Ajouter des tests si on veut aller plus loin
option(BUILD_TESTS "Construire les tests unitaires" OFF)

if(BUILD_TESTS)
    # Ici on pourrait ajouter des tests avec Google Test
    message(STATUS "Tests activés")
endif()