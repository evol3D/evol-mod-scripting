#include <evol/common/ev_macros.h>
#include <lautoc.h>

static void
__luaA_wrap_fn_0_void(
    void* out, 
    void* args, 
    void(*func)(), 
    size_t *sizes)
{
  EV_UNUSED_PARAM(out);
  EV_UNUSED_PARAM(args);
  EV_UNUSED_PARAM(sizes);
  func();
}

static void
__luaA_wrap_fn_1_void(
    void* out, 
    void* args, 
    void(*func)(), 
    size_t *sizes)
{
  EV_UNUSED_PARAM(out);
  EV_UNUSED_PARAM(sizes);
  void *a1 = args;
  func(a1);
}

static void
__luaA_wrap_fn_2_void(
    void* out, 
    void* args, 
    void(*func)(), 
    size_t *sizes)
{
  EV_UNUSED_PARAM(out);
  void *a1 = args;
  void *a2 = ((char*)a1) + sizes[1];
  func(a1, a2);
}

static void
__luaA_wrap_fn_3_void(
    void* out, 
    void* args, 
    void(*func)(), 
    size_t *sizes)
{
  EV_UNUSED_PARAM(out);
  void *a1 = args;
  void *a2 = ((char*)a1) + sizes[1];
  void *a3 = ((char*)a2) + sizes[2];
  func(a1, a2, a3);
}

static void
__luaA_wrap_fn_4_void(
    void* out, 
    void* args, 
    void(*func)(), 
    size_t *sizes)
{
  EV_UNUSED_PARAM(out);
  void *a1 = args;
  void *a2 = ((char*)a1) + sizes[1];
  void *a3 = ((char*)a2) + sizes[2];
  void *a4 = ((char*)a3) + sizes[3];
  func(a1, a2, a3, a4);
}

static void
__luaA_wrap_fn_5_void(
    void* out, 
    void* args, 
    void(*func)(), 
    size_t *sizes)
{
  EV_UNUSED_PARAM(out);
  void *a1 = args;
  void *a2 = ((char*)a1) + sizes[1];
  void *a3 = ((char*)a2) + sizes[2];
  void *a4 = ((char*)a3) + sizes[3];
  void *a5 = ((char*)a4) + sizes[4];
  func(a1, a2, a3, a4, a5);
}

static void
__luaA_wrap_fn_6_void(
    void* out, 
    void* args, 
    void(*func)(), 
    size_t *sizes)
{
  EV_UNUSED_PARAM(out);
  void *a0 = args;
  void *a1 = ((char*)a0) + sizes[0];
  void *a2 = ((char*)a1) + sizes[1];
  void *a3 = ((char*)a2) + sizes[2];
  void *a4 = ((char*)a3) + sizes[3];
  void *a5 = ((char*)a4) + sizes[4];
  func(a0, a1, a2, a3, a4, a5);
}

static void
__luaA_wrap_fn_7_void(
    void* out, 
    void* args, 
    void(*func)(), 
    size_t *sizes)
{
  EV_UNUSED_PARAM(out);
  void *a1 = args;
  void *a2 = ((char*)a1) + sizes[1];
  void *a3 = ((char*)a2) + sizes[2];
  void *a4 = ((char*)a3) + sizes[3];
  void *a5 = ((char*)a4) + sizes[4];
  void *a6 = ((char*)a5) + sizes[5];
  void *a7 = ((char*)a6) + sizes[6];
  func(a1, a2, a3, a4, a5, a6, a7);
}

static void
__luaA_wrap_fn_8_void(
    void* out, 
    void* args, 
    void(*func)(), 
    size_t *sizes)
{
  EV_UNUSED_PARAM(out);
  void *a1 = args;
  void *a2 = ((char*)a1) + sizes[1];
  void *a3 = ((char*)a2) + sizes[2];
  void *a4 = ((char*)a3) + sizes[3];
  void *a5 = ((char*)a4) + sizes[4];
  void *a6 = ((char*)a5) + sizes[5];
  void *a7 = ((char*)a6) + sizes[6];
  void *a8 = ((char*)a7) + sizes[7];
  func(a1, a2, a3, a4, a5, a6, a7, a8);
}

static void
__luaA_wrap_fn_9_void(
    void* out, 
    void* args, 
    void(*func)(), 
    size_t *sizes)
{
  EV_UNUSED_PARAM(out);
  void *a1 = args;
  void *a2 = ((char*)a1) + sizes[1];
  void *a3 = ((char*)a2) + sizes[2];
  void *a4 = ((char*)a3) + sizes[3];
  void *a5 = ((char*)a4) + sizes[4];
  void *a6 = ((char*)a5) + sizes[5];
  void *a7 = ((char*)a6) + sizes[6];
  void *a8 = ((char*)a7) + sizes[7];
  void *a9 = ((char*)a8) + sizes[8];
  func(a1, a2, a3, a4, a5, a6, a7, a8, a9);
}

static void
__luaA_wrap_fn_10_void(
    void* out, 
    void* args, 
    void(*func)(), 
    size_t *sizes)
{
  EV_UNUSED_PARAM(out);
  void *a1 = args;
  void *a2 = ((char*)a1)  + sizes[1];
  void *a3 = ((char*)a2)  + sizes[2];
  void *a4 = ((char*)a3)  + sizes[3];
  void *a5 = ((char*)a4)  + sizes[4];
  void *a6 = ((char*)a5)  + sizes[5];
  void *a7 = ((char*)a6)  + sizes[6];
  void *a8 = ((char*)a7)  + sizes[7];
  void *a9 = ((char*)a8)  + sizes[8];
  void *a10 = ((char*)a9) + sizes[9];
  func(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

static void
__luaA_wrap_fn_0(
    void* out, 
    void* args, 
    void(*func)(), 
    size_t *sizes)
{
  EV_UNUSED_PARAM(args);
  EV_UNUSED_PARAM(sizes);
  func(out);
}

static void
__luaA_wrap_fn_1(
    void* out, 
    void* args, 
    void(*func)(), 
    size_t *sizes)
{
  EV_UNUSED_PARAM(sizes);
  void *a1 = args;
  func(out, a1);
}

static void
__luaA_wrap_fn_2(
    void* out, 
    void* args, 
    void(*func)(), 
    size_t *sizes)
{
  void *a1 = args;
  void *a2 = ((char*)a1) + sizes[1];
  func(out, a1, a2);
}

static void
__luaA_wrap_fn_3(
    void* out, 
    void* args, 
    void(*func)(), 
    size_t *sizes)
{
  void *a1 = args;
  void *a2 = ((char*)a1) + sizes[1];
  void *a3 = ((char*)a2) + sizes[2];
  func(out, a1, a2, a3);
}

static void
__luaA_wrap_fn_4(
    void* out, 
    void* args, 
    void(*func)(), 
    size_t *sizes)
{
  void *a1 = args;
  void *a2 = ((char*)a1) + sizes[1];
  void *a3 = ((char*)a2) + sizes[2];
  void *a4 = ((char*)a3) + sizes[3];
  func(out, a1, a2, a3, a4);
}

static void
__luaA_wrap_fn_5(
    void* out, 
    void* args, 
    void(*func)(), 
    size_t *sizes)
{
  void *a1 = args;
  void *a2 = ((char*)a1) + sizes[1];
  void *a3 = ((char*)a2) + sizes[2];
  void *a4 = ((char*)a3) + sizes[3];
  void *a5 = ((char*)a4) + sizes[4];
  func(out, a1, a2, a3, a4, a5);
}

static void
__luaA_wrap_fn_6(
    void* out, 
    void* args, 
    void(*func)(), 
    size_t *sizes)
{
  void *a0 = args;
  void *a1 = ((char*)a0) + sizes[0];
  void *a2 = ((char*)a1) + sizes[1];
  void *a3 = ((char*)a2) + sizes[2];
  void *a4 = ((char*)a3) + sizes[3];
  void *a5 = ((char*)a4) + sizes[4];
  func(out, a0, a1, a2, a3, a4, a5);
}

static void
__luaA_wrap_fn_7(
    void* out, 
    void* args, 
    void(*func)(), 
    size_t *sizes)
{
  void *a1 = args;
  void *a2 = ((char*)a1) + sizes[1];
  void *a3 = ((char*)a2) + sizes[2];
  void *a4 = ((char*)a3) + sizes[3];
  void *a5 = ((char*)a4) + sizes[4];
  void *a6 = ((char*)a5) + sizes[5];
  void *a7 = ((char*)a6) + sizes[6];
  func(out, a1, a2, a3, a4, a5, a6, a7);
}

static void
__luaA_wrap_fn_8(
    void* out, 
    void* args, 
    void(*func)(), 
    size_t *sizes)
{
  void *a1 = args;
  void *a2 = ((char*)a1) + sizes[1];
  void *a3 = ((char*)a2) + sizes[2];
  void *a4 = ((char*)a3) + sizes[3];
  void *a5 = ((char*)a4) + sizes[4];
  void *a6 = ((char*)a5) + sizes[5];
  void *a7 = ((char*)a6) + sizes[6];
  void *a8 = ((char*)a7) + sizes[7];
  func(out, a1, a2, a3, a4, a5, a6, a7, a8);
}

static void
__luaA_wrap_fn_9(
    void* out, 
    void* args, 
    void(*func)(), 
    size_t *sizes)
{
  void *a1 = args;
  void *a2 = ((char*)a1) + sizes[1];
  void *a3 = ((char*)a2) + sizes[2];
  void *a4 = ((char*)a3) + sizes[3];
  void *a5 = ((char*)a4) + sizes[4];
  void *a6 = ((char*)a5) + sizes[5];
  void *a7 = ((char*)a6) + sizes[6];
  void *a8 = ((char*)a7) + sizes[7];
  void *a9 = ((char*)a8) + sizes[8];
  func(out, a1, a2, a3, a4, a5, a6, a7, a8, a9);
}

static void
__luaA_wrap_fn_10(
    void* out, 
    void* args, 
    void(*func)(), 
    size_t *sizes)
{
  void *a1 = args;
  void *a2 = ((char*)a1)  + sizes[1];
  void *a3 = ((char*)a2)  + sizes[2];
  void *a4 = ((char*)a3)  + sizes[3];
  void *a5 = ((char*)a4)  + sizes[4];
  void *a6 = ((char*)a5)  + sizes[5];
  void *a7 = ((char*)a6)  + sizes[6];
  void *a8 = ((char*)a7)  + sizes[7];
  void *a9 = ((char*)a8)  + sizes[8];
  void *a10 = ((char*)a9) + sizes[9];
  func(out, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

EV_UNUSED static const luaA_Func luaA_wrap_functions[] = {
  __luaA_wrap_fn_0,
  __luaA_wrap_fn_1,
  __luaA_wrap_fn_2,
  __luaA_wrap_fn_3,
  __luaA_wrap_fn_4,
  __luaA_wrap_fn_5,
  __luaA_wrap_fn_6,
  __luaA_wrap_fn_7,
  __luaA_wrap_fn_8,
  __luaA_wrap_fn_9,
  __luaA_wrap_fn_10,
};

EV_UNUSED static const luaA_Func luaA_wrap_functions_void[] = {
  __luaA_wrap_fn_0_void,
  __luaA_wrap_fn_1_void,
  __luaA_wrap_fn_2_void,
  __luaA_wrap_fn_3_void,
  __luaA_wrap_fn_4_void,
  __luaA_wrap_fn_5_void,
  __luaA_wrap_fn_6_void,
  __luaA_wrap_fn_7_void,
  __luaA_wrap_fn_8_void,
  __luaA_wrap_fn_9_void,
  __luaA_wrap_fn_10_void,
};
