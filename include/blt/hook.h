#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * BLT4L Linux Modding Platform for PayDay2
 * (C) 2016- Roman Hargrave 
 * 
 * Unit: Hook interface declarations
 *
 * NOTE: I usually program in C, not C++
 * NOTE: Please see multiplat_detours_ex.cc for detour documention
 */

struct lua_State_s {
	int placeholder;
};
typedef struct lua_state_s lua_State;
typedef const char* (*lua_reader) (lua_State*, void*, size_t*);
typedef int (*lua_cfunction) (lua_State*);
typedef void* (*lua_alloc) (void*, void*, size_t, size_t);
typedef struct {
    const char* name;
    lua_cfunction func;
} luaL_reg;

/*
 * Foreign function pointers
 */

//extern void        (*lua_call)         (lua_State*, int, int);
//extern int         (*lua_pcall)        (lua_State*, int, int, int);
//extern int         (*lua_gettop)       (lua_State*);
//extern void        (*lua_settop)       (lua_State*, int);
//extern const char* (*lua_tolstring)    (lua_State*, int, size_t*);
//extern int         (*luaL_loadfile)    (lua_State*, const char*);
//extern int         (*lua_load)         (lua_State*, lua_reader*, void*, const char*);
//extern void        (*lua_setfield)     (lua_State*, int, const char*);
//extern void        (*lua_createtable)  (lua_State*, int, int);
//extern void        (*lua_insert)       (lua_State*, int);
//extern lua_State*  (*lua_newstate)     (lua_alloc, void*);
//extern void        (*lua_close)        (lua_State*);
//extern void        (*lua_rawset)       (lua_State*, int);
//extern void        (*lua_settable)     (lua_State*, int);
//extern void        (*lua_pushnumber)   (lua_State*, double);
//extern void        (*lua_pushinteger)  (lua_State*, ptrdiff_t);
//extern void        (*lua_pushboolean)  (lua_State*, bool);
//extern void        (*lua_pushcclosure) (lua_State*, lua_cfunction, int);
//extern void        (*lua_pushlstring)  (lua_State*, const char*, size_t);
//extern void        (*luaI_openlib)     (lua_State*, const char*, const luaL_reg*, int);
//extern void        (*luaL_ref)         (lua_State*, int);
//extern void        (*lua_rawgeti)      (lua_State*, int, int);
//extern void        (*luaL_unref)       (lua_State*, int, int);
//extern void        (*do_game_update)   ();
//extern int         (*luaL_newstate)    (char, char, int);

/*
 * Internal
 */

void InitLUAHooks(void*);
