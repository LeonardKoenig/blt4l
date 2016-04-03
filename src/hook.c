#include <subhook.h>
#include <blt/hook.h>
#include <stdio.h>
#include <dlfcn.h>



void	    (*lua_call)		(lua_State*, int, int);
int	    (*lua_pcall)	(lua_State*, int, int, int);
int	    (*lua_gettop)	(lua_State*);
void	    (*lua_settop)	(lua_State*, int);
const char* (*lua_tolstring)	(lua_State*, int, size_t*);
int	    (*luaL_loadfile)	(lua_State*, const char*);
int	    (*lua_load)		(lua_State*, lua_reader*, void*, const char*);
void	    (*lua_setfield)	(lua_State*, int, const char*);
void	    (*lua_createtable)	(lua_State*, int, int);
void	    (*lua_insert)	(lua_State*, int);
lua_State*  (*lua_newstate)	(lua_alloc, void*);
void	    (*lua_close)	(lua_State*);
void	    (*lua_rawset)	(lua_State*, int);
void	    (*lua_settable)	(lua_State*, int);
void	    (*lua_pushnumber)	(lua_State*, double);
void	    (*lua_pushinteger)	(lua_State*, ptrdiff_t);
void	    (*lua_pushboolean)	(lua_State*, bool);
void	    (*lua_pushcclosure) (lua_State*, lua_cfunction, int);
void	    (*lua_pushlstring)	(lua_State*, const char*, size_t);
void	    (*luaL_openlib)	(lua_State*, const char*, const luaL_reg*, int);
void	    (*luaL_ref)		(lua_State*, int);
void	    (*lua_rawgeti)	(lua_State*, int, int);
void	    (*luaL_unref)	(lua_State*, int, int);
void	    (*do_game_update)	();
int	    (*luaL_newstate)	(char, char, int);

/*
 * Internal
 */

subhook_t     gameUpdateDetour;
subhook_t     newStateDetour;
subhook_t     luaCallDetour;
subhook_t     luaCloseDetour;

void
dslUpdateDetour()
{
    subhook_remove(gameUpdateDetour);
    fprintf(stderr, "dsl::EventManager::update() detour called\n");

    do_game_update();

    subhook_install(gameUpdateDetour);
    return;
}


/* we find the addresses of the luafunctions here and hook game_update */
void
InitLUAHooks(void* dlHandle)
{
#define setcall(name) \
    *(void **) (&name) = dlsym(dlHandle, #name); \
    printf(#name " = %p \n", *(void **)(&name)); \

    fprintf(stderr, "setting up lua function access\n");

    setcall(lua_call);
    setcall(lua_pcall);
    setcall(lua_gettop);
    setcall(lua_settop);
    setcall(lua_tolstring);
    setcall(luaL_loadfile);
    setcall(lua_load);
    setcall(lua_setfield);
    setcall(lua_createtable);
    setcall(lua_insert);
    setcall(lua_newstate);
    setcall(lua_close);
    setcall(lua_rawset);
    setcall(lua_settable);
    setcall(lua_pushnumber);
    setcall(lua_pushinteger);
    setcall(lua_pushboolean);
    setcall(lua_pushcclosure);
    setcall(lua_pushlstring);
    setcall(luaL_openlib);
    setcall(luaL_ref);
    setcall(lua_rawgeti);
    setcall(luaL_unref);
    setcall(luaL_newstate);

    /* do this one manually, we have C++ code name-mangling here */
    *(void **) (&do_game_update) = dlsym(dlHandle, "_ZN3dsl12EventManager6updateEv");	 // dsl::EventManager::update
    fprintf(stderr, "_ZN3dsl12EventManager6updateEv = %p\n", *(void **)&do_game_update);


    if (do_game_update == NULL) {
	return;
    }

    fprintf(stderr, "setting up intercepts\n");

    gameUpdateDetour = subhook_new(*(void **)&do_game_update, (void *)dslUpdateDetour);
    subhook_install(gameUpdateDetour);

#undef	setcall
}
