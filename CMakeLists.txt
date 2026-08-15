cmake_minimum_required(VERSION 3.10)

project(chess-minimal)

add_executable(
    main

    src/board-output.c
    src/board.c
    src/chess-game.c
    src/command.c
    src/int-vec-2d.c
    src/io-plus.c
    src/main.c
    src/move-rules.c
    src/piece-traits.c
    src/piece.c

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
)
