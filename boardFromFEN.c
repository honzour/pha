// Modified source code from Etheral
// https://github.com/AndyGrant/Ethereal/blob/master/src/board.c

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "cotyvar.h"

enum { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

enum {
  COLOUR_NB = 2,
  RANK_NB = 8, FILE_NB = 8,
  PIECE_NB = 6,
  CONT_NB = 2
};

const char* PieceLabel[COLOUR_NB] = { "PNBRQK", "pnbrqk" };

static inline int makePiece(int type, int colour) {
  assert(0 <= type && type < PIECE_NB);
  assert(0 <= colour && colour <= COLOUR_NB);
  if (colour == 0)
    return type + 1;
  else
    return -(type + 1);
}

static void setSquare(TSch squares, int colour, int piece, int sq) {
  assert(0 <= colour && colour < COLOUR_NB);
  assert(0 <= piece && piece < PIECE_NB);

  squares[sq] = makePiece(piece, colour);
}

int square(int rank, int file) {
  assert(0 <= rank && rank < RANK_NB);
  assert(0 <= file && file < FILE_NB);
  return 2 * 10 + 1 + rank * 10 + file;
}

static int stringToSquare(char* str) {
  return str[0] == '-' ? 0 : square(str[1] - '1', str[0] - 'a');
}

cfunkce void boardFromFEN(TPozice* board, const char* fen) {
  int sq = 9 * 10 + 1;
  char ch;
  char* str = _strdup(fen), * strPos = NULL;
  char* token = strtok_s(str, " ", &strPos);

  memcpy(board, &PrazdnePostaveni, sizeof(TPozice));

  // Piece placement
  while ((ch = *token++)) {
    if (isdigit(ch))
      sq += ch - '0';
    else if (ch == '/')
      sq -= 8 + 10;
    else {
      const int colour = islower(ch) > 0;
      const char* piece = strchr(PieceLabel[colour], ch);

      if (piece)
      {
        setSquare(board->sch, colour, piece - PieceLabel[colour], sq);
        sq++;
      }
    }
  }

  // Turn of play
  token = strtok_s(NULL, " ", &strPos);
  board->bily = token[0] == 'w' ? 1 : 0;

  // Castling rights
  token = strtok_s(NULL, " ", &strPos);

  while ((ch = *token++)) {
    if (ch == 'K') 
      board->roch |= 1;
    if (ch == 'Q') 
      board->roch |= 2;
    if (ch == 'k') 
      board->roch |= 4;
    if (ch == 'q') 
      board->roch |= 8;
  }

  // En passant square
  int epSquare = stringToSquare(strtok_s(NULL, " ", &strPos));
  int pawnSquare;
  if (board->bily)
    pawnSquare = epSquare - 10;
  else
    pawnSquare = epSquare + 10;

  board->mimoch = pawnSquare;

  // Half & Full Move Counters
  int halfMoveCounter = atoi(strtok_s(NULL, " ", &strPos));
  board->quietMoveCount = halfMoveCounter;
//  board->fullMoveCounter = atoi(strtok_s(NULL, " ", &strPos));

  free(str);
}