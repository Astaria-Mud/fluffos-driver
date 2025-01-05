// line.c
#define __GG_LINE__
#include "packages/gridgen/gridgen.h"

// Geometry primitive routines
#ifndef __GG_POINT__
#include "packages/gridgen/point.cc"
#endif

int gg_abs(int num) {
  if (num > -1)
    return num;
  return num*-1;
}


/*---------------------------------------------------------------------------
  _drawLine()

  Uses simple DDA line drawing algorithm.

  If value is -1, this routine checks (without actually placing) if the line
  doesn't overlap existing coordinates or buffer edges and returns 0 if the
  line does overlap.

  Returns 1 on successful placement or if the line could be placed without
  overlap.
  ---------------------------------------------------------------------------*/
static int _drawLine (Grid *grid, Point2d start, Point2d end, int value,
		      int ignoreValue)
{
  int dx, dy, steps, k;
  float xinc, yinc, x, y;

  if (!grid || !grid->buf)
    return 0;

  dx = end.x - start.x;
  dy = end.y - start.y;
  if (gg_abs (dx) > gg_abs (dy))
    steps = gg_abs (dx);
  else
    steps = gg_abs (dy);

  if (!steps)
    return 1; // zero-length line, but that's ok!

  xinc = (float)dx / (float)steps;
  yinc = (float)dy / (float)steps;
  x = start.x;
  y = start.y;

  if (-1 == value) // check for overlap, don't draw anything.
    {
      if (!_checkPoint (grid, (int)x, (int)y))
	return 0;
      for (k = 1; k < steps; k++)
        {
          x += xinc;
          y += yinc;
	  if (!_checkPoint (grid, (int)x, (int)y))
	    return 0;
        }
      return 1;
    }
  else if (ignoreValue) // draw the line ignoring the specified value
    {
      _drawPointIgnore (grid, (int)x, (int)y, value, ignoreValue);
      for (k = 1; k < steps; k++)
        {
          x += xinc;
          y += yinc;
          _drawPointIgnore (grid, (int)x, (int)y, value, ignoreValue);
        }
      return 1;
    }
  else // Draw the line regardless of overlap with no ignores
    {
      _drawPoint (grid, (int)x, (int)y, value);
      for (k = 1; k < steps; k++)
	{
	  x += xinc;
	  y += yinc;
	  _drawPoint (grid, (int)x, (int)y, value);
	}
      return 1;
    }
} // _drawLine
