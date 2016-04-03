/**
 * (C) 2016- Roman Hargrave
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE     // for RTLD_NEXT
#endif

#include <unistd.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdbool.h>

#include <blt/hook.h>

/*
 * Test for a GCC-compatible compiler, because we need
 * the compiler to support the constructor attribute
 */
#if (!defined(__GNUC__))
#   warn GCC is required for this program to work. ignore this if your compiler supports the constructor attribute
#endif



/* the pointer to payday 2's main() function. */
int (*origmain)(int, char**);

/* our new main function delivered to the original
 * __libc_start_main(). We will call origmain() from here
 */
int newmain(int argc, char *argv[])
{
	void* dlHandle = dlopen(NULL, RTLD_LAZY);
	fprintf(stderr, "dlHandle = %p\n", dlHandle);

	/*
	 * Hack: test for presence of a known unique function amongst the libraries loaded by payday
	 * so that we don't try to init on some other image or some shit.
	 *
	 */
	void *tmp = dlsym(dlHandle, "_ZN3dsl12EventManager6updateEv");
	if (dlHandle) {
		printf("_ZN3dsl12EventManager6updateEv = %p\n", tmp);
		InitLUAHooks(dlHandle);
	} else if(dlHandle) {
		printf("_ZN3dsl12EventManager6updateEv wasn't found in dlHandle, won't load!\n");
		dlclose(dlHandle);
	}

	return origmain(argc, argv);
}


/* original __libc_start_main(); decl. taken from here:
 * https://sourceware.org/git/?p=glibc.git;a=blob;f=csu/libc-start.c;h=4f2a3366a6c2e7936ba0870ceddd0b6f015dc84a;hb=70d9946a44ba381f81eb08c71cc150315cc112ad
 */
int (*origfcn)(int (*main) (int, char **, char **
                            MAIN_AUXVEC_DECL),
               int argc,
               char **argv,
#ifdef LIBC_START_MAIN_AUXVEC_ARG
               ElfW(auxv_t) *auxvec,
#endif
               __typeof (main) init,
               void (*fini) (void),
               void (*rtld_fini) (void),
               void *stack_end);
//__attribute__ ((noreturn)))


/*
 * Our preloaded function, calling the original in the end
 */
int __libc_start_main(int (*main) (int, char **, char **
                                   MAIN_AUXVEC_DECL),
                      int argc,
                      char **argv,
#ifdef LIBC_START_MAIN_AUXVEC_ARG
                      ElfW(auxv_t) *auxvec,
#endif
                      __typeof (main) init,
                      void (*fini) (void),
                      void (*rtld_fini) (void),
                      void *stack_end)
//__attribute__ ((noreturn))
{
	origmain = (__typeof (origmain))main;
//    printf("main: %p\n", main);


	/* see man 3 dlopen on why this is required to be so weird */
	*(void **) (&origfcn) = dlsym(RTLD_NEXT, "__libc_start_main");
//    printf("__libc_start_main() = %p\n", origfcn);


    /* call the original __libc_start_main() with our hooked main() */
	return origfcn((__typeof (main))newmain, argc, argv,
#ifdef LIBC_START_MAIN_AUXVEC_ARG
	               auxvec,
#endif
	               init, fini, rtld_fini, stack_end);
}


/* vim: set ts=4 softtabstop=0 sw=4 expandtab: */
