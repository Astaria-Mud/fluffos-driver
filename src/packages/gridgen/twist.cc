// twist.c, draw a twisty corridor
#define __GG_TWIST__

#include "base/package_api.h"

#ifndef __GG_POINT__
#include "packages/gridgen/point.cc"
#endif
#ifndef __GG_POINT_POOL__
#include "packages/gridgen/pointpool.cc"
#endif

#define NUM_DIRECTIONS   8

#define TWIST_NORTH      0
#define TWIST_SOUTH      1
#define TWIST_EAST       2
#define TWIST_WEST       3
#define TWIST_NORTHWEST  4
#define TWIST_NORTHEAST  5
#define TWIST_SOUTHWEST  6
#define TWIST_SOUTHEAST  7

static Point2d TwistTranslations[] =
  {
    { 0,-1}, // north
    { 0, 1}, // south
    { 1, 0}, // east
    {-1, 0}, // west
    {-1,-1}, // northwest
    { 1,-1}, // northeast
    {-1, 1}, // southwest
    { 1, 1}, // southeast
  };

#define NUM_PRIMARY_DIRECTIONS 4
static int PrimaryDirections[] =
  {TWIST_NORTH, TWIST_SOUTH, TWIST_EAST, TWIST_WEST};

#define NUM_GENERAL_DIRECTIONS 4
#define NUM_DIRS_IN_GENERALS   3
static int GeneralNorth[] = {TWIST_NORTH, TWIST_NORTHWEST, TWIST_NORTHEAST};
static int GeneralSouth[] = {TWIST_SOUTH, TWIST_SOUTHWEST, TWIST_SOUTHEAST};
static int GeneralEast [] = {TWIST_EAST,  TWIST_NORTHEAST, TWIST_SOUTHEAST};
static int GeneralWest [] = {TWIST_WEST,  TWIST_NORTHWEST, TWIST_SOUTHWEST};
static int *GeneralDirections[] = {GeneralNorth, GeneralSouth, GeneralEast,
				   GeneralWest};

static int TwistStart;

/*---------------------------------------------------------------------------
  _drawPreviousTwist()

  Using TwistStart as an offset into the stack, this function draws each
  point from TwistStart to the end of the stack.
  ---------------------------------------------------------------------------*/
static int _drawPreviousTwist (Grid *grid, int value)
{
  int i;
  Point2d *pt;
  for (i = TwistStart; i < _getPoolNdx (); i++)
    {
      pt = _getPoint2d (i);
      if (!pt)
	return 0;
      _drawPoint (grid, pt->x, pt->y, value);
    }
  return 1;
} // _drawPreviousTwist

/*---------------------------------------------------------------------------
  _generateTwist()

  Generate a twisty corridor.  Returns 1 if no overlap, 0 if overlaps.
  Returns -1 if the stack runs out of memory.
  ---------------------------------------------------------------------------*/
static int _generateTwist (Grid *grid, Point2d start, 
			   int minLength, int maxLength)
{
  int length, direction, *general_dirs, actual_dir;
  Point2d pt = start, translate;

  TwistStart = _getPoolNdx ();
  direction = PrimaryDirections[random_number (NUM_PRIMARY_DIRECTIONS)];
  general_dirs = GeneralDirections[direction];

  length = (random_number (maxLength - minLength + 1)) + minLength;
  while (length--)
    {
      actual_dir = general_dirs[random_number (NUM_DIRS_IN_GENERALS)];
      translate = TwistTranslations[actual_dir];
      pt.x += translate.x;
      pt.y += translate.y;
      if (!_checkPoint (grid, pt.x, pt.y))
	return 0;
      if (!_pushPoint2d (pt))
	return -1;
    }

  return 1;
} // _generateTwist
