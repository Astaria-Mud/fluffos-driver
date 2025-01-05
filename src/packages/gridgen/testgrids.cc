// testgrids.c
#include "base/package_api.h"
#include <cstdio>
#include "packages/gridgen/gridgen.h"

#ifndef __GG_GRID__
#include "packages/gridgen/grid.cc"
#endif
#ifndef __GG_MAZE__
#include "packages/gridgen/maze.cc"
#endif
#ifndef __GG_DUNGEON__
#include "packages/gridgen/dungeon.cc"
#endif
#ifndef __GG_MINES__
#include "packages/gridgen/mines.cc"
#endif
#ifndef __GG_CAVERNS__
#include "packages/gridgen/caverns.cc"
#endif
#ifndef __GG_POINT_POOL__
#include "packages/gridgen/pointpool.cc"
#endif

#define TEST_SHAPES    -1 // Testing of shape primitives

#define TEST_CAVERNS    0
#define TEST_MAZE       1
#define TEST_DUNGEON    2
#define TEST_MINES      3

// Uncomment the test you want to run below.

//#define TEST            TEST_SHAPES
//#define TEST            TEST_CAVERNS
#define TEST            TEST_MAZE
//#define TEST            TEST_DUNGEON
//#define TEST            TEST_MINES

static char MapChars[] =
  {
    ' ', // NOTHING
    'C', // OPEN
    'o', // TUNNEL
    '+', // DEBUG_OVERLAP_SYM
  };

static void dumpGrid (Grid grid)
{
  int i, j;
  for (i = 0; i < grid.height; i++)
    {
    for (j = 0; j < grid.width; j++)
      printf ("%c", MapChars[grid.buf[i][j]]);
    printf ("\n");
    }
} // dumpGrid

int main (int argc, char** argv)
{
  Point2d tmp;
  Grid grid;
  int result;

  grid.height = grid.width = 50;
  _initGrid (&grid);
  srand (time (nullptr));

#if TEST != TEST_SHAPES
  while (1) // Check for any 'stuck' cases; if this loop ever ends, bad case.
    {
#if TEST == TEST_CAVERNS
      result = _genCaverns (&grid, nullptr, 15, 20);
#elif TEST == TEST_MAZE
      //tmp.x = tmp.y = 4;
      result = _genMaze (&grid, nullptr);
#elif TEST == TEST_DUNGEON
      result = _genDungeon (&grid, nullptr, 15, 30);
#elif TEST == TEST_MINES
      result = _genMines (&grid, nullptr, 35, 55);
#endif
      if (0 == result)
	{
	  printf ("** Cannot place geometry within this grid.\n");
	  return 1;
	}
      else if (-1 == result)
	{
	  printf ("** Point stack size exceeded.\n");
	  return 1;
	}
      else if (-2 == result)
	{
	  printf ("** Bad parameters specified to generation routine\n");
	  return 1;
	}
      dumpGrid (grid);
      _deinitGrid (&grid);
      _initGrid (&grid);
    }
#else // test shapes
  tmp.x = tmp.y = 25;
  _drawRectangle (&grid, tmp, 15, 15, OPEN, 0); // nonfilled rectangle
  tmp.x = tmp.y = 5;
  _drawRectangle (&grid, tmp, 7, 7, OPEN, 1); // filled rectangle
  dumpGrid (grid);
#endif

  if (!_deinitGrid (&grid))
    {
      printf ("** Error trying to deinit grid.\n");
      return 1;
    }
  
  return 0;
} // main
