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

    include/assert-toggle.h
    include/board-output.h
    include/board.h
    include/chess-game.h
    include/command.h
    include/int-vec-2d.h
    include/io-plus.h
    include/move-rules.h
    include/piece-traits.h
    include/piece.h
    include/board-theme.h
    include/settings.h
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

    include/assert-toggle.h
    include/board-output.h
    include/board.h
    include/chess-game.h
    include/command.h
    include/int-vec-2d.h
    include/io-plus.h
    include/move-rules.h
    include/piece-traits.h
    include/piece.h
    include/board-theme.h
    include/settings.h
)

target_compile_options( main PRIVATE
    -Wall
    -Werror
    -Wextra
    -O2
    -std=c99
)
