/*
** $Id: lpcode.h,v 1.7 2015/06/12 18:24:45 roberto Exp $
*/

#if !defined(lpcode_h)
#define lpcode_h
#if __cplusplus
extern "C" {
#endif
#include "lua.h"
#if __cplusplus
}
#endif

#include "lptypes.h"
#include "lptree.h"
#include "lpvm.h"

int tocharset (TTree *tree, Charset *cs);
int checkaux (TTree *tree, int pred);
int fixedlenx (TTree *tree, int count, int len);
int hascaptures (TTree *tree);
int lp_gc (lua_State *L);
Instruction *compile (lua_State *L, Pattern *p);
void realloccode (lua_State *L, Pattern *p, int nsize);
int sizei (const Instruction *i);


#define PEnullable      0
#define PEnofail        1

/*
** nofail(t) implies that 't' cannot fail with any input
*/
#define nofail(t)	checkaux(t, PEnofail)

/*
** (not nullable(t)) implies 't' cannot match without consuming
** something
*/
#define nullable(t)	checkaux(t, PEnullable)

#define fixedlen(t)     fixedlenx(t, 0, 0)



#endif
