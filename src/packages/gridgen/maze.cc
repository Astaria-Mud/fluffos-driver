// maze.c
#define __GG_MAZE__

#include "base/package_api.h"

#include "packages/gridgen/gridgen.h"
#ifndef __GG_POINT__
#include "packages/gridgen/point.cc"
#endif
#ifndef __GG_POINT_POOL__
#include "packages/gridgen/pointpool.cc"
#endif

static int hasUnmadeCells (Grid *grid, Point2d cell)
{
  if (!grid || !grid->buf)
    return 0; // no grid avail

  if (cell.x < 0 || cell.y < 0 || cell.x >= grid->width
      || cell.y >= grid->height)
    return 0; // not on grid

  if (cell.y - 2 >= 0 && !grid->buf[cell.y - 2][cell.x])
    return 1; // north is unmade
  if (cell.y + 2 < grid->height && !grid->buf[cell.y + 2][cell.x])
    return 1; // south is unmade
  if (cell.x - 2 >= 0 && !grid->buf[cell.y][cell.x - 2])
    return 1; // west is unmade
  if (cell.x + 2 < grid->width && !grid->buf[cell.y][cell.x + 2])
    return 1; // east is unmade

  return 0; // none adjacent are unmade
} // hasUnmadeCells

#define AVAIL_NONE  0
#define AVAIL_NORTH 1
#define AVAIL_SOUTH 2
#define AVAIL_WEST  3
#define AVAIL_EAST  4
static Point2d chiselCell (Grid *grid, Point2d cell)
{
  Point2d new_cell;
  int avail_dirs[4] = {AVAIL_NONE}, ndx = 0;
  new_cell.x = new_cell.y = -1;

  if (cell.y - 2 >= 0 && !grid->buf[cell.y - 2][cell.x])
    { // north is unmade and fits; chisel there
      avail_dirs[ndx++] = AVAIL_NORTH;
    }
  else if (cell.y + 2 < grid->height && !grid->buf[cell.y + 2][cell.x])
    { // south is unmade and fits; chisel there
      avail_dirs[ndx++] = AVAIL_SOUTH;
    }
  else if (cell.x - 2 >= 0 && !grid->buf[cell.y][cell.x - 2])
    { // west is unmade and fits; chisel there
      avail_dirs[ndx++] = AVAIL_WEST;
    }
  else if (cell.x + 2 < grid->width && !grid->buf[cell.y][cell.x + 2])
    { // east is unmade and fits; chisel there
      avail_dirs[ndx++] = AVAIL_EAST;
    }

  if (!ndx)
    return new_cell;

  // Pick a random direction from the avail list until we have a good one
  do {ndx = random_number (4);} while (AVAIL_NONE == avail_dirs[ndx]);
  switch (avail_dirs[ndx])
    {
    case AVAIL_NORTH:
      new_cell = cell;
      new_cell.y -= 1;
      _drawPoint (grid, new_cell.x, new_cell.y, TUNNEL);
      new_cell.y -= 1;
      _drawPoint (grid, new_cell.x, new_cell.y, TUNNEL);
      return new_cell;

    case AVAIL_SOUTH:
      new_cell = cell;
      new_cell.y += 1;
      _drawPoint (grid, new_cell.x, new_cell.y, TUNNEL);
      new_cell.y += 1;
      _drawPoint (grid, new_cell.x, new_cell.y, TUNNEL);
      return new_cell;

    case AVAIL_WEST:
      new_cell = cell;
      new_cell.x -= 1;
      _drawPoint (grid, new_cell.x, new_cell.y, TUNNEL);
      new_cell.x -= 1;
      _drawPoint (grid, new_cell.x, new_cell.y, TUNNEL);
      return new_cell;

    case AVAIL_EAST:
      new_cell = cell;
      new_cell.x += 1;
      _drawPoint (grid, new_cell.x, new_cell.y, TUNNEL);
      new_cell.x += 1;
      _drawPoint (grid, new_cell.x, new_cell.y, TUNNEL);
      return new_cell;
    }

  return new_cell; // can't get here
} // chiselCell

/*---------------------------------------------------------------------------
  _genMaze ()

  Uses a growing tree algorithm.
  
  Generates a maze terrain within a grid.  Returns 1 on success or 0 on
  failure.  Failure can only occur on bad parameters (bad grid, 
  bad dimensions).

  -1 is returned if point pool stack capacity is exceeded.
  -2 is returned if bad parameters are specified.

  startPoint, when non-nullptr, specifies the start location of the first
  passageway.  Otherwise, a random point within the grid is selected as the
  first passage.
  ---------------------------------------------------------------------------*/
static int _genMaze (Grid *grid, Point2d *startPoint)
{
  Point2d pt, chisel_pt;

  if (!grid || !grid->buf)
    return -2;

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

  _pushPoint2d (pt);
  while (_getPoolNdx ())
    {
      _shufflePoint2dPool ();
      pt = *(_popPoint2d ());
      if (hasUnmadeCells (grid, pt))
	{
	  chisel_pt = chiselCell (grid, pt);
	  if (hasUnmadeCells (grid, chisel_pt))
	    if (!_pushPoint2d (chisel_pt))
	      return -1;
	}
      if (hasUnmadeCells (grid, pt))
	if (!_pushPoint2d (pt))
	  return -1;
    }

  _clearPoint2dPool ();
  return 1;
} // _genMaze
