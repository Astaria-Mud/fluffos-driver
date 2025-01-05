// mines.c
#define __GG_MINES__

#include "base/package_api.h"

#include "packages/gridgen/gridgen.h"
#ifndef __GG_GRID__
#include "packages/gridgen/grid.cc"
#endif
#ifndef __GG_LINE__
#include "packages/gridgen/line.cc"
#endif

#define MAX_NUM_SHAFTS 500

/*---------------------------------------------------------------------------
  _genMines()

  Generates a mines terrain within a grid.  Returns 1 on success or -3 on
  failure.  Failure can occur if corridors or rooms cannot be placed within
  the grid (due to no space).

  -1 is returned if point pool stack capacity is exceeded.
  -2 is returned if bad parameters are specified.

  startPoint, when non-nullptr, specifies the start location of the first
  passageway.  Otherwise, a random point within the grid is selected as the
  first passage.

  minShafts and maxShafts specify a range of connections for the
  generated grid to contain.
  ---------------------------------------------------------------------------*/
static int _genMines (Grid *grid, Point2d *startPoint,
		      int minShafts, int maxShafts)
{
  Point2d pt0, pt1;
  int numShafts, lastTurn, count, numRestarts;
  // lastTurn: 1 = horizontal, 0 = vertical
  
  if (!grid || !grid->buf)
    return -2;

  if (minShafts > maxShafts)
    return -2;

  if (maxShafts > MAX_NUM_SHAFTS)
    return -2;

  numShafts = (random_number (maxShafts - minShafts + 1)) + minShafts;

  if (startPoint)
    {
      pt0.x = startPoint->x;
      pt0.y = startPoint->y;
    }
  else
    {
      pt0.x = random_number (grid->width);
      pt0.y = random_number (grid->height);
    }

  count = numRestarts = 0;
  lastTurn = 0; // first turn is vertical
  do
    {
      if (numRestarts > ABORT_THRESHOLD)
        {
          _clearGrid (grid);
          return -3;
        }

      if (count > RESTART_THRESHOLD)
        {
	  numShafts = (random_number (maxShafts - minShafts + 1)) + minShafts;
          _clearGrid (grid);
          count = 0;

	  pt0.x = random_number (grid->width);
	  pt0.y = random_number (grid->height);

          numRestarts++;
        }


      pt1 = pt0; // pt1 will extend pt0
      if (lastTurn) // horizontal extension
	{
	  if (random_number (2)) // right
	    pt1.x += random_number (11) + 5; // 5 to 15 length
	  else // left
	    pt1.x -= random_number (11) + 5;
	}
      else // vertical extension
	{
	  if (random_number (2)) // down
	    pt1.y += random_number (11) + 5; // 5 to 15 length
	  else // up
	    pt1.y -= random_number (11) + 5;
	}

      if (pt1.x > 0 && pt1.x < grid->width && 
	  pt1.y > 0 && pt1.y < grid->height)
	{
	  _drawLine (grid, pt0, pt1, TUNNEL, 0);
	  numShafts--;
	  // Start next shaft where this one ends
	  pt0 = pt1;
	}

      lastTurn = !lastTurn;
      count++;
    }
  while (numShafts);

  return 1;
} // _genMines
