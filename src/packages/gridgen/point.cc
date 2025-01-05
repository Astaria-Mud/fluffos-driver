#define __GG_POINT__
#include "packages/gridgen/gridgen.h"

/*---------------------------------------------------------------------------
  Returns nonzero if the point can be drawn
  zero if the point cannot be drawn (out of bounds, hits another point, or
  buffer is not allocated)
  
  Will return 1 if nothing is there.
  ---------------------------------------------------------------------------*/
static int _checkPoint (Grid *grid, int x, int y)
{
  if (!grid || !grid->buf)
    return 0;
  if (x >= grid->width || y >= grid->height || x < 0 || y < 0)
    return 0;

  return !grid->buf[y][x]; // will return !NOTHING if nothing is there
} // _checkPoint

/*---------------------------------------------------------------------------
  Draws a value to the location only with regard for buffer bounds.
  ---------------------------------------------------------------------------*/
static int _drawPoint (Grid *grid, int x, int y, int value)
{
  if (!grid->buf)
    return 0;
  if (x >= grid->width || y >= grid->height || x < 0 || y < 0)
    return 0;

#if DEBUG_OVERLAP
  if (grid->buf[y][x])
    {
      grid->buf[y][x] = DEBUG_OVERLAP_SYM;
      return 1;
    }
#endif

  if (!grid->buf[y][x]) // first time this coordinate was set
    grid->numSet++;

  grid->buf[y][x] = value;
  return 1;
} // _drawPoint

/*---------------------------------------------------------------------------
  Draws a value to the location only with regard for buffer bounds, ignoring
  any instances of the specified value (not overwriting those instances)
  ---------------------------------------------------------------------------*/
static int _drawPointIgnore (Grid *grid, int x, int y, int value, int ignore)
{
  if (!grid->buf)
    return 0;
  if (x >= grid->width || y >= grid->height || x < 0 || y < 0)
    return 0;
  if (ignore == grid->buf[y][x])
    return 0;

  if (!grid->buf[y][x]) // first time this coordinate was set
    grid->numSet++;

  grid->buf[y][x] = value;
  return 1;
} // _drawPointIgnore
