/*
 * =====================================================================================
 *
 *       Filename:  optimized_time.h
 *
 *    Description:  优化过的时间代码(参考: http://km.oa.com/articles/show/118505)
 *
 *        Version:  1.0
 *        Created:  05/07/2014 10:59:26 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  shaneyu
 *        Company:  Tencent
 *
 * =====================================================================================
 */
#ifndef __OPTIMIZED_TIME_H__
#define __OPTIMIZED_TIME_H__

#include <stdint.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

/*
 * Checked against the Intel manual and GCC --hpreg
 *
 * volatile because the tsc always changes without the compiler knowing it.
 */
#define RDTSC() ({ register uint64_t tim; __asm__ __volatile__( "rdtsc" : "=A" (tim) ); tim; })
#define RDTSCP(aux) ({ register uint64_t tim; __asm__ __volatile__( "rdtscp" : "=A" (tim),"=c" (aux) ); tim; })

// Static variables are initialized to 0
static struct timeval walltime;
static uint64_t walltick;
static int cpuspeed_mhz;

static inline int test_rdtscp()
{
	register uint32_t edx;
	__asm__ __volatile__(
		"movl $0x80000001, %%eax \n\t"
		"cpuid"
		: "=d" (edx) : : "eax","ebx","ecx"
	);
	return (edx & (1U<<27));
}

/*
 * Returns CPU clock in mhz
 * Notice that the function will cost the calling thread to sleep wait_us us
 */
static inline int getcpuspeed_mhz(unsigned int wait_us)
{
	uint64_t tsc1, tsc2;
	struct timespec t;

	t.tv_sec = 0;
	t.tv_nsec = wait_us * 1000;

	tsc1 = RDTSC();

	// If sleep failed, result is unexpected
	if(nanosleep(&t, NULL))
		return -1;

	tsc2 = RDTSC();

	return (tsc2 - tsc1)/(wait_us);
}

static int getcpuspeed()
{
	static int speed = -1;

	while(speed<100)
		speed = getcpuspeed_mhz(50*1000);

	return speed;
}

#define REGET_TIME_US	1

//#define getcpuspeed() 2000

# define TIME_ADD_US(a, usec)                      \
  do {                                             \
      (a)->tv_usec += usec;                        \
      while((a)->tv_usec >= 1000000)               \
      {                                            \
        (a)->tv_sec ++;                            \
        (a)->tv_usec -= 1000000;                   \
      }                                            \
  } while (0)


//
// Compile with -O2 to optimize mul/div instructions
// The performance is restricted by 2 factors:
//    1, the rdtsc instruction
//    2, division
//
// Another restriction for this function:
//    The caller thread should run on one CPU or on SMP with sinchronized TSCs,
// otherwise, rdtsc instruction will differ between multiple CPUs.
//    The good thing is that most multicore CPUs are shipped with sinchronized TSCs.
//
static int optimezed_gettimeofday(struct timeval *tv)
{
	uint64_t tick = 0;
	// max_time_us = max_ticks / cpuspeed_mhz > RELOAD_TIME_US us
	static unsigned int max_ticks = 2000*REGET_TIME_US;

	if(walltime.tv_sec==0 || cpuspeed_mhz==0 ||
		// If we are on a different cpu with unsynchronized tsc,
		// RDTSC() may be smaller than walltick
		// in this case tick will be a negative number,
		// whose unsigned value is much larger than max_ticks
	 	(tick=RDTSC()-walltick) > max_ticks)
	{
		if(tick==0 || cpuspeed_mhz==0)
		{
			cpuspeed_mhz = getcpuspeed();
			max_ticks = cpuspeed_mhz*REGET_TIME_US;
		}

		gettimeofday(tv, NULL);
		memcpy(&walltime, tv, sizeof(walltime));
		walltick = RDTSC();
		return 0;
	}

	memcpy(tv, &walltime, sizeof(walltime));

	// if REGET_TIME_US==1, we are currently in the same us, no need to adjust tv
#if REGET_TIME_US > 1
	{
		uint32_t t;
		t = ((uint32_t)tick) / cpuspeed_mhz;
		TIME_ADD_US(tv, t);
	}
#endif
	return 0;
}

static inline time_t optimized_time(time_t* pt)
{
	struct timeval tv;
	if ( 0 != optimezed_gettimeofday(&tv) )
	{
		return time(pt);
	}

	if ( NULL != pt )
	{
		*pt = tv.tv_sec;
	}

	return tv.tv_sec;
}

#endif
