#include <stdio.h>

#include "../include/command.h"
#include "../include/chess-game.h"
#include "../include/assert-toggle.h"
#include "../include/move-rules.h"

int main(void)
{
    const ChessGame game = {
        .player = WHITE,
        .board = {
            {{WHITE,KING}, {WHITE, ROOK}},
            {{BLACK,QUEEN}}
        }
    };

    ASSERT(isWinForOppPlayer(&game), "win was not true");

}
