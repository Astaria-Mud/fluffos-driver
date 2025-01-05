// rectangle.c
#define __GG_RECTANGLE__

#include "packages/gridgen/gridgen.h"

// Geometry primitive routines
#ifndef __GG_ROW__
#include "packages/gridgen/row.cc"
#endif
#ifndef __GG_LINE__
#include "packages/gridgen/line.cc"
#endif


/*---------------------------------------------------------------------------
  _checkRectangle()

  Check if a rectangle can fit on the grid at the specified place without
  overlapping existing geometry.  Returns 0 if it overlaps, 1 if it can fit
  without overlap.
  ---------------------------------------------------------------------------*/
static int _checkRectangle (Grid *grid, Point2d start, int width, int height,
			    int filled)
{
  Point2d pt1, pt2;
  int i;
  if (!filled) // Rectangle's outline
    {
      pt1 = pt2 = start;
      pt1.x += 1;
      pt2.x += width;
      if (!_drawLine (grid, pt1, pt2, CHECK_OVERLAP, 0))
	return 0;

      pt1 = pt2 = start;
      pt2.y += height;
      if (!_drawLine (grid, pt1, pt2, CHECK_OVERLAP, 0))
	return 0;

      pt1 = pt2 = start;
      pt1.x += width;
      pt2 = pt1;
      pt2.y += height + 1;
      if (!_drawLine (grid, pt1, pt2, CHECK_OVERLAP, 0))
	return 0;

      pt1 = pt2 = start;
      pt1.y += height;
      pt2 = pt1;
      pt2.x += width;
      if (!_drawLine (grid, pt1, pt2, CHECK_OVERLAP, 0))
	return 0;

      return 1;
    }

  // Filled rectangle
  for (i = 0; i < height; i++)
      if (!_checkRow (grid, start.x, start.x + width, start.y + i))
	return 0;

  return 1;
} // _checkRectangle

/*---------------------------------------------------------------------------
  _drawRectangle()

  Draw a rectangle, only with regard for bounding.
  ---------------------------------------------------------------------------*/
static int _drawRectangle (Grid *grid, Point2d start, int width, int height,
			   int value, int filled)
{
  Point2d pt1, pt2;
  int i;

  if (!filled) // Rectangle's outline
    {
      pt1 = pt2 = start;
      pt1.x += 1;
      pt2.x += width;
      _drawLine (grid, pt1, pt2, value, 0);

      pt1 = pt2 = start;
      pt2.y += height;
      _drawLine (grid, pt1, pt2, value, 0);

      pt1 = pt2 = start;
      pt1.x += width;
      pt2 = pt1;
      pt2.y += height + 1;
      _drawLine (grid, pt1, pt2, value, 0);

      pt1 = pt2 = start;
      pt1.y += height;
      pt2 = pt1;
      pt2.x += width;
      _drawLine (grid, pt1, pt2, value, 0);

      return 1;
    }

  // Filled rectangle
  for (i = 0; i < height; i++)
    _drawRow (grid, start.x, start.x + width - 1, start.y + i, value);

  return 1;
} // _drawRectangle
