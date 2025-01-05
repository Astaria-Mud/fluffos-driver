/*---------------------------------------------------------------------------
  gridgen.c

  Grid generation algorithms using some traditional and not-so-traditional
  techniques inspired by Roguelikes.

  Caverns can double as canals.  Most can double as various types of grids
  just so long as the rooms are desc'd properly.

  Grid Types: (+ means currently available, . means planned for future)
  + dungeon - A series of rectangular rooms connected via winding tunnels
  + caverns - A series of rounded rooms connected via straight tunnels
  + maze    - A series of interconnected tunnels, branching at right angles
  + mines   - A series of straight corridors connecting at right angles
 
  Mark Schlosser (Raganim@Astaria)
  16 Aug 2008
  ---------------------------------------------------------------------------*/
#include "base/package_api.h"

#include "packages/gridgen/gridgen.h"

// Public static Point2d pool
#ifndef __GG_POINT_POOL__
#include "packages/gridgen/pointpool.cc"
#endif

// Grid description
#ifndef __GG_GRID__
#include "packages/gridgen/grid.cc"
#endif

// Geometry primitive routines
#ifndef __GG_POINT__
#include "packages/gridgen/point.cc"
#endif
#ifndef __GG_ROW__
#include "packages/gridgen/row.cc"
#endif
#ifndef __GG_TWIST__
#include "packages/gridgen/twist.cc"
#endif
#ifndef __GG_LINE__
#include "packages/gridgen/line.cc"
#endif
#ifndef __GG_CIRCLE__
#include "packages/gridgen/circle.cc"
#endif
#ifndef __GG_RECTANGLE__
#include "packages/gridgen/rectangle.cc"
#endif

// Routines for generating each terrain type
#ifndef __GG_CAVERNS__
#include "packages/gridgen/caverns.cc"
#endif
#ifndef __GG_MAZE__
#include "packages/gridgen/maze.cc"
#endif
#ifndef __GG_DUNGEON__
#include "packages/gridgen/dungeon.cc"
#endif
#ifndef __GG_MINES__
#include "packages/gridgen/mines.cc"
#endif

/*-----------------*/
/* privates follow */
/*-----------------*/

static int NumSet = 0;
static array_t *SetCoordinates = nullptr;

// converts the buffers in a grid to a 2d mixed array; fills the
// SetCoordinates array.
static struct array_t *convert_grid_to_mixed(Grid grid)
{
  int i, j, s;
  array_t *v, *p, *pt;

  s = 0;
  if (SetCoordinates && NumSet)
  {
    SetCoordinates = nullptr;
    NumSet = 0;
  }

  SetCoordinates = allocate_array(grid.numSet);
  NumSet = grid.numSet;

  v = allocate_array(grid.height);
  for (i = 0; i < grid.height; i++)
  {
    v->item[i].type = T_ARRAY;
    v->item[i].u.arr = p = allocate_array(grid.width);
    for (j = 0; j < grid.width; j++)
    {
      p->item[j].type = T_NUMBER;
      p->item[j].u.number = grid.buf[i][j];

      if (grid.buf[i][j]) // if value set, we add it to the set array
      {
        SetCoordinates->item[s].type = T_ARRAY;
        SetCoordinates->item[s].u.arr = pt = allocate_array(2);
        pt->item[0].type = T_NUMBER;
        pt->item[0].u.number = j; // x
        pt->item[1].type = T_NUMBER;
        pt->item[1].u.number = i; // y
        s++;
      }
    }
  }

  return v;
} // convert_grid_to_mixed


/*---------------------------------*/
/* public module interface follows */
/*---------------------------------*/

/*---------------------------------------------------------------------------
  mixed gg_get_set_list();

  Returns an array of ({x,y}) pairs for each space in the previously-generated
  grid that is "set" (walkable).
 ---------------------------------------------------------------------------*/
#ifdef F_GG_GET_SET_LIST
void f_gg_get_set_list()
{
  if (!SetCoordinates)
    push_number(0);
  else
    push_refed_array(SetCoordinates);
}
#endif

/*---------------------------------------------------------------------------
  int gg_get_tunnel_symbol();

  Returns the integer value considered a "tunnel" in generated grids.
 ---------------------------------------------------------------------------*/
#ifdef F_GG_GET_TUNNEL_SYMBOL
void f_gg_get_tunnel_symbol()
{
  push_number(TUNNEL);
}
#endif

/*---------------------------------------------------------------------------
  int gg_get_open_symbol();

  Returns the integer value considered an "open room" in generated grids.
 ---------------------------------------------------------------------------*/
#ifdef F_GG_GET_OPEN_SYMBOL
void f_gg_get_open_symbol()
{
  push_number(OPEN);
} // f_gg_get_open_symbol
#endif

/*---------------------------------------------------------------------------
  mixed gg_generate_grid(string grid_type, int grid_width, int grid_height,
                          int min_config, int max_config);

  Returns a 2d array of grid values for the specified grid type.  Throws an
  error and returns 0 upon failure due to incorrect parameters or invalid
  grid complexity.

  Example call:
  mixed result = gg_generate_grid("DUNGEON", // grid type
                                  50, 50,    // a 50x50 grid
                                  10, 20);   // 10 to 20 chambers

  Each parameter is optional.  For a MAZE type grid, min and max_config are
  ignored even if specified.

  Grid Types: (+ means currently available, . means planned for future)
  + DUNGEON - A series of rectangular chambers connected via winding tunnels
  + CAVERNS - A series of rounded rooms connected via straight tunnels
  + MAZE    - A series of interconnected tunnels, branching at right angles
  + MINES   - A series of straight corridors connecting at right angles

  min_config and max_config specify a range of values for the output of the
  grid depending on type.

  GRID TYPE   MEANING OF min_config - max_config
  ---------   ----------------------------------
  - DUNGEON : number of chambers connected by windy tunnels
  - MAZE    : unused
  - CAVERNS : number of caverns connected by straight tunnels
  - MINES   : number of straight shafts connected by right angles

  If not specified, each parameter will default to the following:
  - grid_type   : "DUNGEON"
  - grid_height : 50
  - grid_width  : 50
  - min_config  : 15
  - max_config  : 30
 ---------------------------------------------------------------------------*/
#ifdef F_GG_GENERATE_GRID
void f_gg_generate_grid()
{
  //char debug_str[100];
  const char *grid_type;
  int grid_width, grid_height, min_config, max_config, result, args;
  array_t *ret;
  Grid grid;

  // Default parameters
  grid_type = "DUNGEON";
  grid_height = 50;
  grid_width = 50;
  min_config = 15;
  max_config = 30;

  // User-specified paramaters
  // Fallthrough on the switch cases is intentional
  args = 0;
  switch (st_num_arg)
  {
    case 5: max_config  = (sp - (args++))->u.number;
    case 4: min_config  = (sp - (args++))->u.number;
    case 3: grid_height = (sp - (args++))->u.number;
    case 2: grid_width  = (sp - (args++))->u.number;
    case 1: grid_type   = (sp - (args++))->u.string;
  }

  if (st_num_arg)
    pop_n_elems(st_num_arg);

  // if ( grid_width < 1 || grid_height < 1 )
  // {
    // error ("gg: Specified dimensions below minimum allowed.\n");
    // push_number(0);
    // return;
  // }
  if (grid_width > MAX_BUF_WIDTH || grid_height > MAX_BUF_HEIGHT)
  {
    error ("gg: Specified dimensions exceed maximum allowed.\n");
    push_number(0);
    return;
  }

  grid.width = grid_width;
  grid.height = grid_height;

  if (!_initGrid(&grid))
  { // grid cannot be initialized
    _deinitGrid(&grid);
    error ("gg: Failure initializing grid buffers\n");
    push_number(0);
    return;
  }

  /* 
     Below, call the proper generation function:
   */
  if (0 == strcmp(grid_type, "DUNGEON"))
    result = _genDungeon(&grid, nullptr, min_config, max_config);

  else if (0 == strcmp(grid_type, "MAZE"))
    result = _genMaze (&grid, nullptr);

  else if (0 == strcmp(grid_type, "CAVERNS"))
    result = _genCaverns(&grid, nullptr, min_config, max_config);

  else if (0 == strcmp(grid_type, "MINES"))
    result = _genMines(&grid, nullptr, min_config, max_config);

  else
  { // Error, unknown grid type
    _deinitGrid(&grid);
    error ("gg: Unknown grid type specified\n");
    push_number(0);
    return;
  }

  /*
    Grid generation function called, check result:
  */
  
  switch ( result )
  {
    case -3 : // cannot place geometry with this grid
    {
      _deinitGrid(&grid);
      error("gg: Cannot place all geometry within specified dimensions\n");
      push_number(0);
      break;
    }
    case -2 : // bad parameters specified to generation routine
    {
      _deinitGrid(&grid);
      error("gg: Invalid parameters specified\n");
      push_number(0);
      break;
    }
    case -1 : // grid complexity exceeds maximum allowed
    {
      _deinitGrid(&grid);
      error("gg: Grid complexity exceeds maximum allowed\n");
      push_number(0);
      break;
    }
    case 1 : // success state!
    {
      /*
        Grid generation done and successful, so we convert the grid buffer to a
        mixed* array and return.
      */
      ret = convert_grid_to_mixed(grid);
      _deinitGrid(&grid);
      push_refed_array(ret);
      ret->ref--; // Why is this needed?  Resolves a memory leak but I want to
                  // know why it has to be done manually before the return 
                  // - shouldn't this be being done in push_vector?    
      break;
    }
    default : // unknown error state
    {
      _deinitGrid(&grid);
      error("gg: Unknown error state\n");
      push_number(0);
    }
  }
  
} // f_gg_generate_grid
#endif