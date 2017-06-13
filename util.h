/**
  ******************************************************************************
  * @file    util.h
  * @author  ochoaha
  * @date    May 30, 2017
  * @brief   Description of module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 MSD Performance </center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of MSD Performance nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************  
  */



/*
**====================================================================================
** Double inclusion protection
**====================================================================================
*/
#ifndef SRC_UTIL_H_
#define SRC_UTIL_H_


/*
**====================================================================================
** Imported definitions
**====================================================================================
*/


/*
**====================================================================================
** Public type definitions
**====================================================================================
*/


/*
**====================================================================================
** Public constant definitions for external access
**====================================================================================
*/
/*
**====================================================================================
** Public macro definitions for external access
**====================================================================================
*/

/**< Stops the CPU as a break point */
#define __debug()         __ASM volatile("BKPT #01");

/**< Get the count of items in an array */
#define COUNT_OF(array)           ((sizeof(array)) / (sizeof((array)[0])))

/**< Get the absolute value of the number  */
#define ABS(x)                    (((x) > (0) ? (x) : -(x))

/**< Get the minimum value of two values */
#define MIN(a, b)                 (((a) < (b)) ? (a) : (b))

/**< Get the maximum value of two values */
#ifndef MAX
#define MAX(a, b)                 (((a) > (b)) ? (a) : (b))
#endif

/**< Get the value between limits */
#define LIMIT(x, l, h)            (MIN(MAX((x), (l)), (h)))

/**< Get the absolute (positive number) difference of two values */
#define DIFF(a, b)                (((a) > (b)) ? ((a) - (b)) : ((b) - (a)))

/**< Convert a C language token to string */
/*lint -save -e961 '#/##' operator used in macro*/
#define TOK_TO_STR(t)             (#t)
/*lint -restore */

/**< Invert Boolean in a way that Lint likes */
#define INVERT_BOOLEAN(b)         (((b) == false) ? true : false)

/**
 ** Macro to deal with unused function parameters or dummy variables.
 ** This won't cause Lint warnings.
 ** Don't use it to "void" function return values!
 */
#define NOT_USED(x)               ((void)(x))

/* To suppress Lint and MISRA warnings regarding member offset and size,
 * we trick the Lint by providing constant random (non-zero) return value */
#ifdef LINT

#define member_offset(type, member) 1U
#define member_size(type, member)   1U

#else

/**< Get structure member offset address */
#define member_offset(type, member) ((U32)((&((type *)NULL)->member)))

/**< Get structure member size in bytes */
#define member_size(type, member)   (sizeof(((type *)NULL)->member))

#endif


/*
**====================================================================================
** Function prototype declarations for external access
**====================================================================================
*/

#endif /* SRC_UTIL_H_ */
