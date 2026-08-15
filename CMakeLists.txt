cmake_minimum_required(VERSION 3.10)

project(chess-minimal)

add_executable(
    main

    board-output.c
    board.c
    chess-game.c
    command.c
    int-vec-2d.c
    io-plus.c
    main.c
    move-rules.c
    piece-traits.c
    piece.c

    assert-toggle.h
    board-output.h
    board.h
    chess-game.h
    command.h
    int-vec-2d.h
    io-plus.h
    move-rules.h
    piece-traits.h
    piece.h
)
