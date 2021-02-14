#include <assert.h>
#include <stdio.h>
#include <sys/time.h>

#include "attacks.h"
#include "bits.h"
#include "board.h"
#include "movegen.h"
#include "types.h"

long perft(int depth) {
  long nodes = 0;

  moves_t moveList[1];
  generateMoves(moveList);

  if (depth == 1)
    return moveList->count;

  for (int i = 0; i < moveList->count; i++) {
    move_t m = moveList->moves[i];

    makeMove(m);
    nodes += perft(depth - 1);
    undoMove(m);
  }

  return nodes;
}

long perftStart(char *fen, int depth) {
  long nodes = 0;

  parseFen(fen);
  printf("Running performance test...\n");
  printBoard();

  struct timeval stop, start;
  gettimeofday(&start, NULL);

  moves_t moveList[1];
  generateMoves(moveList);

  for (int i = 0; i < moveList->count; i++) {
    move_t m = moveList->moves[i];
    makeMove(m);
    long results = perft(depth - 1);
    printf("%s%s%c: %ld\n", idxToCord[moveStart(m)], idxToCord[moveEnd(m)], movePromo(m) ? pieceChars[movePromo(m)] : ' ', results);
    nodes += results;
    undoMove(m);
  }

  gettimeofday(&stop, NULL);
  printf("Got result of %ld took %.3fms\n", nodes, ((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec) / 1000.0);
  return nodes;
}

int main() {
  initAttacks();

  // 722.711ms
  long nodes = perftStart("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 6);
  assert(nodes == 119060324);

  // 949.001ms
  nodes = perftStart("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -", 5);
  assert(nodes == 193690690);

  // 115.488ms
  nodes = perftStart("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -", 6);
  assert(nodes == 11030083);

  // 93.014ms
  nodes = perftStart("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 5);
  assert(nodes == 15833292);

  // 474.586ms
  nodes = perftStart("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 5);
  assert(nodes == 89941194);

  // 818.587ms
  nodes = perftStart("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 5);
  assert(nodes == 164075551);
}
