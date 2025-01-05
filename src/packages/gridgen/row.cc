// row.c
#define __GG_ROW__

#include "packages/gridgen/gridgen.h"

#ifndef __GG_POINT__
#include "packages/gridgen/point.cc"
#endif

/*---------------------------------------------------------------------------
  _checkRow()
  
  Returns 0 if the row overlaps, nonzero if it doesn't.
  ---------------------------------------------------------------------------*/
static int _checkRow (Grid *grid, int startx, int endx, int row)
{
  int x;
  for (x = startx; x <= endx; x++)
    if (!_checkPoint (grid, x, row))
      return 0;
  return 1;
} // _checkRow

/*---------------------------------------------------------------------------
  _drawRow()
  
  Fills a specified row from startx to endx.
  ---------------------------------------------------------------------------*/
static void _drawRow (Grid *grid, int startx, int endx, int row, int val)
{
  int x;
  for (x = startx; x <= endx; x++)
    _drawPoint (grid, x, row, val);
} // // _drawRow
