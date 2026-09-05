cmake_minimum_required(VERSION 3.15)

project(shess)

add_executable(
    main

    src/main.c
    src/board-output.c
    src/board.c
    src/chess-game.c
    src/command.c
    src/int-vec-2d.c
    src/io-plus.c
    src/move-rules.c
    src/piece-traits.c
    src/piece.c
    src/board-theme.c
    src/settings.c
    src/game-state.c
)

add_executable(
    test

    test/test.c
    src/board-output.c
    src/board.c
    src/chess-game.c
    src/command.c
    src/int-vec-2d.c
    src/io-plus.c
    src/move-rules.c
    src/piece-traits.c
    src/piece.c
    src/board-theme.c
    src/settings.c
    src/game-state.c
)

target_compile_options( main PRIVATE
    -Wall
    -Werror
    -Wextra
    -g
    -O2
    -std=c99
)

target_compile_options( test PRIVATE
    -Wall
    -Werror
    -Wextra
    -g
    -O2
    -std=c99
)
