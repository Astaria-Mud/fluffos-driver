// pointpool.c
#define __GG_POINT_POOL__
#include "base/package_api.h"

#include "packages/gridgen/gridgen.h"

/*---------------------------------------------------------------------------
  This is a common pool of points which algorithms share. - they can
  place points inside here, keeping track of them with indices.  When
  a point is placed inside, the PoolNdx counter needs to be incremented.
  For instances, do: PointPool[PoolNdx++] = point;
  ---------------------------------------------------------------------------*/

#define POOL_BLOCKSIZE 1024
static int PoolSize = POOL_BLOCKSIZE;
static int PoolNdx = 0;
static Point2d PointPool[POOL_BLOCKSIZE];

static int _setPoolNdx (int ndx)
{ 
  if (ndx < 0 || ndx >= PoolSize)
    return 0;
  PoolNdx = ndx;
  return 1;
} // _setPoolSize

static int _getPoolNdx ()
{
  return PoolNdx;
} // _getPoolSize

// Returns 0 when pool size limit reached
static int _pushPoint2d (Point2d pt)
{
  if (PoolNdx + 1 >= PoolSize)
    {
      return 0; // could expand if we need huge maps
    }
  PointPool[PoolNdx++] = pt;
  return 1;
} // _pushPoint2d

static Point2d *_popPoint2d ()
{
  if (PoolNdx - 1 < 0)
    return nullptr;
  return &(PointPool[--PoolNdx]);
} // _popPoint2d

static Point2d *_getPoint2d (int ndx)
{
  if (ndx < 0 || ndx >= PoolNdx)
    return nullptr;
  return &(PointPool[ndx]);
} // _getPoint2d

static void _clearPoint2dPool ()
{
  PoolNdx = 0;
} // _clearPointPool

// Randomly swap entries in the pool.
static void _shufflePoint2dPool ()
{
  Point2d tmp;
  int src, dst, i;

  if (PoolNdx <= 1)
    return;

  for (i = 0; i < PoolNdx; i++)
    {
      src = i;
      do {dst = random_number (PoolNdx);} while (dst == src);
      tmp = PointPool[src];
      PointPool[src] = PointPool[dst];
      PointPool[dst] = tmp;
    }
} // _shufflePoint2dPool
