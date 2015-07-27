#define DDEBUG 1

#include <lauxlib.h>
#include <stdlib.h>
#include <time.h>


//static const long mask = (1L << 48) - 1;
//static const long multiplier = 0x5DEECE66DL;
//static const long addend = 0xBL;

static const long long multiplier  = 25214903917/*0x5DEECE66DL*/;
static const long long addend  = 11/*0xBL*/;
static const long long mask  = 281474976710655/*(1L << 48) - 1*/;
//static const long long _seedUniquifier  = 8682522807148012L/*8682522807148012L*/;


static long long seed;

static int next(int bits);
static int nextInt(lua_State *L);
static int nextLong(lua_State *L);
static int setSeed(lua_State *L);

//static long nanoTime();



static const struct luaL_Reg random_lib[] = {
    {"nextInt", nextInt},
    {"nextLong", nextLong},
    {"setSeed",setSeed},
    {NULL, NULL}
};


int
luaopen_random(lua_State *L)
{
    luaL_register(L, "random", random_lib);
    return 1;
}


//static long nanoTime()
//{
//    struct timespec ts;
//    clock_gettime(CLOCK_REALTIME, &ts);
//
//    dd("time is: %ld(nanoseconds), %ld(seconds).", ts.tv_nsec, ts.tv_sec);
//
//    return ts.tv_nsec;
//}


static long long initialScramble(long long seed) {
    return (seed ^ multiplier) & mask;
}
static int setSeed(lua_State *L)
{
    long long _seed =  lua_tointeger(L, 1);// lua_gettop(L);
    printf("_seed = %lld",_seed);
    
    seed = initialScramble(_seed);
    return 1;
}

//static long genSeed()
//{
//    long current = 8682522807148012L;
//
//    for (;;) {
//        long next = current * 181783497276652981L;
//        if (current != next) {
//            seed = next;
//            break;
//        }
//    }
//
//    return initialScramble(seed ^ nanoTime());
//}


static int next(int bits)
{
//    long oldseed, nextseed;
////    seed = genSeed();
//    do {
//        oldseed = seed;
//        nextseed = (oldseed * multiplier + addend) & mask;
//        seed = nextseed;
//    } while (oldseed != nextseed);
//    seed = nextseed;
//    return (int)(nextseed >> (48 - bits));
    
    long long oldseed, nextseed;
    
    oldseed = seed;
    nextseed = (oldseed * multiplier + addend) & mask;
    
    seed = nextseed;
    
    //    printf("(int)(nextseed >> (48 - bits)) ---%d \n",(int)(nextseed >> (48 - bits)));
    return (int)(nextseed >> (48 - bits));
}


static int nextLong(lua_State *L)
{
    long res = ((long)next(32) << 32) + next(32);
//    dd("the number is: %ld", res);

    lua_pushinteger(L, res);

    return 1;
}


static int nextInt(lua_State *L)
{
    int         n;
    int         num;
    int         bits, val;
//     long long _seed =
    n = lua_tointeger(L, 1);//lua_gettop(L);
    if (n <= 0) {
        return luaL_error(L, "n must be positive");
    }

    if ((n & -n) == n) { // i.e., n is a power of 2
        num = (int)((n * (long)next(31)) >> 31);
    }

    do {
        bits = next(31);
//        dd("bits is: %d\n", bits);
        val = bits % n;
//        dd("val is: %d\n", val);
    } while (bits - val + (n-1) < 0);

    num = val;
    lua_pushinteger(L, num);

    return 1;
}
