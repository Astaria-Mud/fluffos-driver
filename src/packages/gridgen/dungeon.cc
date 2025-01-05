// dungeon.c
#define __GG_DUNGEON__
#include "base/package_api.h"

#include "packages/gridgen/gridgen.h"

// Geometry primitive routines
#ifndef __GG_POINT_POOL__
#include "packages/gridgen/pointpool.cc"
#endif
#ifndef __GG_GRID__
#include "packages/gridgen/grid.cc"
#endif
#ifndef __GG_RECTANGLE__
#include "packages/gridgen/rectangle.cc"
#endif
#ifndef __GG_TWIST__
#include "packages/gridgen/twist.cc"
#endif

#define MAX_NUM_CONNECTIONS 500

/*---------------------------------------------------------------------------
  _genDungeon()

  Generates a dungeon terrain within a grid.  Returns 1 on success or -3 on
  failure.  Failure can occur if corridors or rooms cannot be placed within
  the grid (due to no space).

  -1 is returned if point pool stack capacity is exceeded.
  -2 is returned if bad parameters are specified.

  startPoint, when non-nullptr, specifies the start location of the first
  passageway.  Otherwise, a random point within the grid is selected as the
  first passage.

  minConnections and maxConnections specify a range of connections for the
  generated grid to contain.
  ---------------------------------------------------------------------------*/
static int _genDungeon (Grid *grid, Point2d *startPoint,
			int minConnections, int maxConnections)
{
  Point2d pt;
  int numConnections, lastNdx, result, numRestarts, count, width, height;
  
  if (!grid || !grid->buf)
    return -2;

  if (minConnections > maxConnections)
    return -2;

  if (maxConnections > MAX_NUM_CONNECTIONS)
    return -2;

  numConnections = (random_number (maxConnections - minConnections + 1))
    + minConnections;

  if (startPoint)
  {
    pt.x = startPoint->x;
    pt.y = startPoint->y;
  }
  else
  {
    pt.x = random_number (grid->width);
    pt.y = random_number (grid->height);
  }

  numRestarts = count = 0;
  _clearPoint2dPool ();
  _pushPoint2d (pt);
  do
  {
    if (numRestarts > ABORT_THRESHOLD)
    {
      _clearGrid (grid);
      _clearPoint2dPool ();
      return -3;
    }

    if (count > RESTART_THRESHOLD)
    {
      numConnections = (random_number (maxConnections - minConnections + 1))
        + minConnections;
      _clearGrid (grid);
      count = 0;

      pt = *(_getPoint2d (random_number (_getPoolNdx ())));
      _clearPoint2dPool ();
      _pushPoint2d (pt);

      numRestarts++;
    }

      lastNdx = _getPoolNdx (); // save stack before twist generation

      result = _generateTwist (grid, pt, 8, 15); // 8-15 length
    if (result > 0) // Ok, so the twist will fit, but the room might not,
    {             // so we do another check before committing to anything.
      width = random_number (3) + 2;  // 1 to 4
      height = random_number (3) + 2; // 1 to 4
      pt = *(_getPoint2d (_getPoolNdx () - 1)); // get last point of twist

      pt.x--; // move back one x - for check only
      pt.y--; // move back one y - for check only
      result = _checkRectangle (grid, pt, width+2, height+2, 1);
      if (result > 0)
      {
        pt.x++;
        pt.y++;
        _drawPreviousTwist (grid, TUNNEL);
        _drawRectangle (grid, pt, width, height, OPEN, 1);
        numConnections--; // one [twist <--> room] placed successfully
      }
    }

    if (0 == result)
    {
      _setPoolNdx (lastNdx); // restore stack after twist if twist and room
                               // doesn't fit.
    }
    else if (-1 == result)
    {
      return -1; // out of memory
    }
      // Pick a random point in the pool for next twist.
      pt = *(_getPoint2d (random_number (_getPoolNdx ())));
      count++;
  }
  while (numConnections);

  return 1;
} // _genDungeon
