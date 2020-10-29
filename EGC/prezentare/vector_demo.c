/*
* vec3lib.c
* add 3d vectors to Lua
* this code is in the public domain
*/

#include "lua.h"
#include "lauxlib.h"

LUALIB_API int lua_vec3libopen (lua_State *L);

typedef struct { double a[3]; } Vector3;

static int TAG; /* lazy! */

static int vec3new(lua_State *L) {
  Vector3* v;
  lua_newuserdata(L,sizeof(Vector3));
  lua_settag(L,TAG);
  v=lua_touserdata(L,-1);
  if (lua_gettop(L)>1) {
    v->a[0]=luaL_check_number(L,1);
    v->a[1]=luaL_check_number(L,2);
    v->a[2]=luaL_check_number(L,3);
  }
  else
    v->a[0] = v->a[1] = v->a[2] = 0;
  return 1;
}

static int vec3get(lua_State *L) {
  Vector3* v=lua_touserdata(L,1);
  const char* i=luaL_check_string(L,2);
  switch (*i) { /* lazy! */
    case 'x': case 'r': lua_pushnumber(L,v->a[0]); break;
    case 'y': case 'g': lua_pushnumber(L,v->a[1]); break;
    case 'z': case 'b': lua_pushnumber(L,v->a[2]); break;
    default: lua_pushnumber(L,0.0); break;
  }
  return 1;
}

static int vec3set(lua_State *L) {
  Vector3* v=lua_touserdata(L,1);
  const char* i=luaL_check_string(L,2);
  double t=luaL_check_number(L,3);
  switch (*i) { /* lazy! */
    case 'x': case 'r': v->a[0]=t; break;
    case 'y': case 'g': v->a[1]=t; break;
    case 'z': case 'b': v->a[2]=t; break;
    default: break;
  }
  return 1;
}

static const struct luaL_reg vec3lib[] = {
  {"vector3", vec3new},
};

int lua_vec3libopen (lua_State *L) {
  luaL_openl(L,vec3lib);
  TAG=lua_newtag(L);
  lua_pushcfunction(L,vec3get); lua_settagmethod(L,TAG,"gettable");
  lua_pushcfunction(L,vec3set); lua_settagmethod(L,TAG,"settable");
  return 0;
}

int main (void *) 
{
	//a = vector3()
	a = vector3(10,20,30)
	print(a,tag(a))

	print(a.x,a.y,a.z)
	print(a.t,a[1])

	a.x,a.y,a.z=100,200,300
	print(a.x,a.y,a.z)
	return 0;
}
