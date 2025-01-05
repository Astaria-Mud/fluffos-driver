// grid.c
#define __GG_GRID__

#include "packages/gridgen/gridgen.h"
#include <cstdlib>


// Clear grid spaces to empty
static int _clearGrid (Grid *grid)
{
  int i, j;
  
  if (!grid || !grid->buf)
    return 0;

  for (i = 0; i < grid->height; i++)
    for (j = 0; j < grid->width; j++)
      grid->buf[i][j] = NOTHING;

  grid->numSet = 0;
  return 1;
} // _clearGrid

/*---------------------------------------------------------------------------
  _initGrid()

  Initialize a grid data structure, allocating its buffer to the desired
  dimensions and filling it with "nothing".

  Returns 0 on failure, 1 on success.
  ---------------------------------------------------------------------------*/
static int _initGrid (Grid *grid)
{
  int i, j;

  if (!grid)
    return 0;

  grid->numSet = 0;

  grid->buf = (int**) malloc (sizeof (int*) * grid->height);
  if (!grid->buf)
    return 0;
  for (i = 0; i < grid->height; i++)
    {
      grid->buf[i] = (int*) malloc (sizeof (int*) * grid->width);
      if (!grid->buf[i])
	return 0;
    }

  for (i = 0; i < grid->height; i++)
    for (j = 0; j < grid->width; j++)
      grid->buf[i][j] = NOTHING;

  return 1;
} // _initGrid

/*---------------------------------------------------------------------------
  _deinitGrid()

  Uninitialize a grid data structure.  Deletes the buffer and returns 1 on
  success, 0 on failure.
  ---------------------------------------------------------------------------*/
static int _deinitGrid (Grid *grid)
{
  int i;

  if (!grid || !grid->buf)
    return 0;

  grid->numSet = 0;

  for (i = 0; i < grid->height; i++)
    free (grid->buf[i]);

  free (grid->buf);
  return 1;
} // _deinitGrid
