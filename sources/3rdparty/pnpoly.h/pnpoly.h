/*******************************************************************************
 *
 * PNPOLY_INCLUDE_H
 *
 ******************************************************************************/

#ifndef PNPOLY_INCLUDE_H
#define PNPOLY_INCLUDE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PNPOLY_DEF
#  ifdef PNPOLY_STATIC
#    define PNPOLY_DEF static
#  else
#    define PNPOLY_DEF extern
#  endif
#endif

PNPOLY_DEF int pnpoly(int nvert, const float *vertx, const float *verty, float testx, float testy);

#ifdef __cplusplus
}
#endif

#endif // PNPOLY_INCLUDE_H

/*******************************************************************************
 *
 * PNPOLY_IMPLEMENTATION
 *
 ******************************************************************************/

#ifdef PNPOLY_IMPLEMENTATION

PNPOLY_DEF int pnpoly(int nvert, const float *vertx, const float *verty, float testx, float testy)
{
  int i, j, c = 0;
  for (i = 0, j = nvert-1; i < nvert; j = i++) {
    if ( ((verty[i]>testy) != (verty[j]>testy)) &&
         (testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
       c = !c;
  }
  return c;
}

#endif // PNPOLY_IMPLEMENTATION

/*
Copyright (c) 1970-2003, Wm. Randolph Franklin
Copyright (c) 2020, by Matvey Cherevko (blackmatov@gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimers.
2. Redistributions in binary form must reproduce the above copyright notice in
   the documentation and/or other materials provided with the distribution.
3. The name of W. Randolph Franklin may not be used to endorse or promote
   products derived from this Software without specific prior written permission.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

