// caverns.c
#define __GG_CAVERNS__

#include "base/package_api.h"

#include "packages/gridgen/gridgen.h"
// Geometry primitive routines

#ifndef __GG_POINT_POOL__
#include "packages/gridgen/pointpool.cc"
#endif
#ifndef __GG_CIRCLE__
#include "packages/gridgen/circle.cc"
#endif
#ifndef __GG_LINE__
#include "packages/gridgen/line.cc"
#endif
#ifndef __GG_GRID__
#include "packages/gridgen/grid.cc"
#endif

#define MAX_NUM_CAVERNS    500  // don't generate more than this many caverns

/*---------------------------------------------------------------------------
  _genCaverns()

  Generates a caverns terrain within a grid.  Returns 1 on success or -3 on
  failure.  Failure can occur if caverns cannot be placed within the grid
  (if there is no room).

  -1 is returned if point pool stack capacity is exceeded.
  -2 is returned if bad parameters are specified.

  startPoint, when non-nullptr, specifies the start location of the first
  cavern.  Otherwise, a random point within the grid is selected as the
  first cavern.

  minCaverns and maxCaverns specify a range of caverns for the generated
  grid to contain.

  minCavernSize and maxCavernSize specify a range of sizes caverns can be,
  in radiuses.
  ---------------------------------------------------------------------------*/
static int _genCaverns (Grid *grid, Point2d *startPoint,
			int minCaverns, int maxCaverns)
{
  Point2d pt, pt0, pt1;
  int numCircles, radius, i, count, numRestarts;

  if (!grid || !grid->buf)
    return -2;

  if (minCaverns > maxCaverns)
    return -2;

  if (maxCaverns > MAX_NUM_CAVERNS)
    return -2;

  _clearPoint2dPool ();
  numCircles = (random_number (maxCaverns - minCaverns + 1)) + minCaverns;
  numRestarts = count = 0;

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

  do
  {
    if (numRestarts > ABORT_THRESHOLD) // the algorithm is in danger of
    {                                // infinitely looping, so we
      _clearGrid (grid);            // abort this grid generation.
      _clearPoint2dPool ();          // If sane parameters are specified,
      return -3;                      // this should never happen.
    }

    if (count > RESTART_THRESHOLD) // restart algorithm; we're stuck
    {
      numCircles = (random_number (maxCaverns - minCaverns + 1)) + minCaverns;
      _clearGrid (grid);
      count = 0;
      _clearPoint2dPool ();
      numRestarts++;
    }

    radius = random_number (3) + 1; // 1 to 3
    if (_drawCircle (grid, pt, radius + 3, 1, CHECK_OVERLAP))
    {
      if (!_pushPoint2d (pt))
        return -1;
      _drawCircle (grid, pt, radius, 1, OPEN);
      numCircles--;
    }

    // Get next cavern point
    pt.x = random_number (grid->width);
    pt.y = random_number (grid->height);
    count++;
  }
  while (numCircles);
  
  // Draw tunnels guaranteeing interconnectedness
  for (i = 0; i < _getPoolNdx (); i++)
  {
    pt0 = *(_getPoint2d (i));
    if (i + 1 == _getPoolNdx ())
      pt1 = *(_getPoint2d (0));
    else
      pt1 = *(_getPoint2d (i + 1));
    _drawLine (grid, pt0, pt1, TUNNEL, OPEN); // ignore OPEN
  }

  return 1;
} // _genCaverns
