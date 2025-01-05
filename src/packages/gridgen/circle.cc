// circle.c
#define __GG_CIRCLE__

#include "packages/gridgen/gridgen.h"
// Geometry primitive routines
#ifndef __GG_POINT__
#include "packages/gridgen/point.cc"
#endif
#ifndef __GG_ROW__
#include "packages/gridgen/row.cc"
#endif
/*---------------------------------------------------------------------------
  _checkCircle()

  Checks each quadrant in a circle and returns 0 if one overlaps existing
  geometry.  Returns 1 otherwise.
  ---------------------------------------------------------------------------*/
static int _checkCircle (Grid *grid, Point2d xyc, Point2d xy, int val,
			 int filled)
{
  // None of the check functions take value as an argument.
  if (!filled)
    {
      if (!_checkPoint (grid, (xyc.x + xy.x), (xyc.y + xy.y)))
        return 0;
      if (!_checkPoint (grid, (xyc.x + xy.x), (xyc.y - xy.y)))
        return 0;
      if (!_checkPoint (grid, (xyc.x - xy.x), (xyc.y + xy.y)))
        return 0;
      if (!_checkPoint (grid, (xyc.x - xy.x), (xyc.y - xy.y)))
        return 0;
      if (!_checkPoint (grid, (xyc.x + xy.y), (xyc.y + xy.x)))
        return 0;
      if (!_checkPoint (grid, (xyc.x + xy.y), (xyc.y - xy.x)))
        return 0;
      if (!_checkPoint (grid, (xyc.x - xy.y), (xyc.y + xy.x)))
        return 0;
      if (!_checkPoint (grid, (xyc.x - xy.y), (xyc.y - xy.x)))
        return 0;
      
      return 1; // Quadrants can be placed without overlap
    }
      
  if (!_checkRow (grid, (xyc.x - xy.x), (xyc.x + xy.x), (xyc.y + xy.y)))
    return 0;
  if (!_checkRow (grid, (xyc.x - xy.x), (xyc.x + xy.x), (xyc.y - xy.y)))
    return 0;
  if (!_checkRow (grid, (xyc.x - xy.y), (xyc.x + xy.y), (xyc.y + xy.x)))
    return 0;
  if (!_checkRow (grid, (xyc.x - xy.y), (xyc.x + xy.y), (xyc.y - xy.x)))
    return 0;
  
  return 1; // Filled quadrants can be placed
} // _checkCircle

/*---------------------------------------------------------------------------
  _plotCircle()

  Plots each quadrant of a circle.
  ---------------------------------------------------------------------------*/
static int _plotCircle (Grid *grid, Point2d xyc, Point2d xy, int val,
			int filled)
{
  // Here we don't check if quadrant can be placed; just place it
  if (!filled)
  {
    _drawPoint (grid, (xyc.x + xy.x), (xyc.y + xy.y), val);
    _drawPoint (grid, (xyc.x + xy.x), (xyc.y - xy.y), val);
    _drawPoint (grid, (xyc.x - xy.x), (xyc.y + xy.y), val);
    _drawPoint (grid, (xyc.x - xy.x), (xyc.y - xy.y), val);
    _drawPoint (grid, (xyc.x + xy.y), (xyc.y + xy.x), val);
    _drawPoint (grid, (xyc.x + xy.y), (xyc.y - xy.x), val);
    _drawPoint (grid, (xyc.x - xy.y), (xyc.y + xy.x), val);
    _drawPoint (grid, (xyc.x - xy.y), (xyc.y - xy.x), val);
    return 1; // Quadrants drawn
  }
  
  _drawRow (grid, (xyc.x - xy.x), (xyc.x + xy.x), (xyc.y + xy.y), val);
  _drawRow (grid, (xyc.x - xy.x), (xyc.x + xy.x), (xyc.y - xy.y), val);
  _drawRow (grid, (xyc.x - xy.y), (xyc.x + xy.y), (xyc.y + xy.x), val);
  _drawRow (grid, (xyc.x - xy.y), (xyc.x + xy.y), (xyc.y - xy.x), val);
  return 1; // Filled quadrants can be placed
} // _plotCircle

/*---------------------------------------------------------------------------
  _drawCircle()

  Uses Bresenham's circle algorithm.

  If value is -1, this routine checks (without actually placing) if a circle
  doesn't overlap existing coordinates or buffer edges and returns 0 if the
  circle does overlap.

  Returns 1 on successful placement or if the circle could be placed without
  overlap.
  ---------------------------------------------------------------------------*/
static int _drawCircle (Grid *grid, Point2d center, int radius, int filled, 
			int value)
{
  int p;
  Point2d pt;

  pt.x = 0;
  pt.y = radius;
  p = 3 - 2 * radius;

  if (!grid || !grid->buf)
    return 0;

  if (-1 == value) // Check if circle can be placed, tracked inside loop
  {              // More expensive than normal plot, so separate case.
    while (pt.x < pt.y)
    {
      if (!_checkCircle (grid, center, pt, value, filled))
        return 0;
      
      if (p < 0)
        p += (4 * pt.x + 6);
      else
      {
        p += (4 * (pt.x - pt.y) + 10);
        pt.y--;
      }
      
      pt.x++;
    }
      
      if (pt.x == pt.y)
        if (! _checkCircle (grid, center, pt, value, filled))
          return 0;

      return 1;
  }
  else // Draw regardless of overlap
  {
    while (pt.x < pt.y)
    {
      _plotCircle (grid, center, pt, value, filled);

      if (p < 0)
        p += (4 * pt.x + 6);
      else
      {
        p += (4 * (pt.x - pt.y) + 10);
        pt.y--;
      }
      
      pt.x++;
    }

    if (pt.x == pt.y)
      _plotCircle (grid, center, pt, value, filled);

    return 1;
  }
} // _drawCircle
