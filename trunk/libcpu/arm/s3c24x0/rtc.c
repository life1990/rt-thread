/*
 * File      : rtc.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://openlab.rt-thread.com/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-04-26     yi.qiu       first version
 * 2010-03-18     Gary Lee     add functions such as GregorianDay
 *                             and rt_rtc_time_to_tm
 */

#include <rtthread.h>
#include <time.h>
#include <s3c24x0.h>
#include "rtc.h"


/**
 * This function get rtc time
 */
void rt_hw_rtc_get(struct rtc_time *ti)
{
	rt_uint8_t sec, min, hour, mday, wday, mon, year;

	/* enable access to RTC registers */
	RTC_ENABLE();

	/* read RTC registers */
	do
	{
		sec 	= BCDSEC;
		min 	= BCDMIN;
		hour 	= BCDHOUR;
		mday	= BCDDATE;
		wday 	= BCDDAY;
		mon 	= BCDMON;
		year 	= BCDYEAR;
    } while (sec != BCDSEC);

	/* disable access to RTC registers */
	RTC_DISABLE();

	ti->tm_sec  	= BCD2BIN(sec  & 0x7F);
	ti->tm_min  	= BCD2BIN(min  & 0x7F);
	ti->tm_hour 	= BCD2BIN(hour & 0x3F);
	ti->tm_mday 	= BCD2BIN(mday & 0x3F);
	ti->tm_mon  	= BCD2BIN(mon & 0x1F);
	ti->tm_year 	= BCD2BIN(year);
	ti->tm_wday 	= BCD2BIN(wday & 0x07);
	ti->tm_yday 	= 0;
	ti->tm_isdst 	= 0;
}

/**
 * This function set rtc time
 */
void rt_hw_rtc_set(struct rtc_time *ti)
{
	rt_uint8_t sec, min, hour, mday, wday, mon, year;

	year 	= BIN2BCD(ti->tm_year);
	mon 	= BIN2BCD(ti->tm_mon);
	wday 	= BIN2BCD(ti->tm_wday);
	mday 	= BIN2BCD(ti->tm_mday);
	hour 	= BIN2BCD(ti->tm_hour);
	min 	= BIN2BCD(ti->tm_min);
	sec 	= BIN2BCD(ti->tm_sec);

	/* enable access to RTC registers */
	RTC_ENABLE();

	/* write RTC registers */
	BCDSEC 		= sec;
	BCDMIN 		= min;
	BCDHOUR 	= hour;
	BCDDATE 	= mday;
	BCDDAY 		= wday;
	BCDMON 		= mon;
	BCDYEAR 	= year;

	/* disable access to RTC registers */
	RTC_DISABLE();
}

/**
 * This function reset rtc
 */
void rt_hw_rtc_reset (void)
{
	RTCCON = (RTCCON & ~0x06) | 0x08;
	RTCCON &= ~(0x08|0x01);
}

/*
 * This only works for the Gregorian calendar - i.e. after 1752 (in the UK)
 */
void GregorianDay(struct rtc_time * tm)
{
	int leapsToDate;
	int lastYear;
	int day;
	int MonthOffset[] = { 0,31,59,90,120,151,181,212,243,273,304,334 };

	lastYear=tm->tm_year-1;

	/*
	 * Number of leap corrections to apply up to end of last year
	 */
	leapsToDate = lastYear/4 - lastYear/100 + lastYear/400;

	/*
	 * This year is a leap year if it is divisible by 4 except when it is
	 * divisible by 100 unless it is divisible by 400
	 *
	 * e.g. 1904 was a leap year, 1900 was not, 1996 is, and 2000 will be
	 */
	if((tm->tm_year%4==0) &&
	   ((tm->tm_year%100!=0) || (tm->tm_year%400==0)) &&
	   (tm->tm_mon>2)) {
		/*
		 * We are past Feb. 29 in a leap year
		 */
		day=1;
	} else {
		day=0;
	}

	day += lastYear*365 + leapsToDate + MonthOffset[tm->tm_mon-1] + tm->tm_mday;

	tm->tm_wday=day%7;
}

void rt_rtc_time_to_tm(rt_uint32_t tim, struct rtc_time *tm)
{
	register int    i;
	register long   hms, day;

	day = tim / SECDAY;
	hms = tim % SECDAY;

	/* Hours, minutes, seconds are easy */
	tm->tm_hour = hms / 3600;
	tm->tm_min = (hms % 3600) / 60;
	tm->tm_sec = (hms % 3600) % 60;

	/* Number of years in days */
	for (i = STARTOFTIME; day >= days_in_year(i); i++) {
		day -= days_in_year(i);
	}
	tm->tm_year = i;

	/* Number of months in days left */
	if (LEAP_YEAR(tm->tm_year)) {
		days_in_month(FEBRUARY) = 29;
	}
	for (i = 1; day >= days_in_month(i); i++) {
		day -= days_in_month(i);
	}
	days_in_month(FEBRUARY) = 28;
	tm->tm_mon = i;

	/* Days are what is left over (+1) from all that. */
	tm->tm_mday = day + 1;

	/*
	 * Determine the day of week
	 */
	GregorianDay(tm);
}



static struct rt_device rtc;
static rt_err_t rt_rtc_open(rt_device_t dev, rt_uint16_t oflag)
{
	RTC_ENABLE();
	return RT_EOK;
}

static rt_err_t rt_rtc_close(rt_device_t dev)
{
	RTC_DISABLE();
	return RT_EOK;
}

static rt_size_t rt_rtc_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size)
{
	return RT_EOK;
}

static rt_err_t rt_rtc_control(rt_device_t dev, rt_uint8_t cmd, void *args)
{
	struct rtc_time* time;
	RT_ASSERT(dev != RT_NULL);

	time = (struct rtc_time*)args;
	switch (cmd)
	{
	case RT_DEVICE_CTRL_RTC_GET_TIME:
		/* read device */
		rt_hw_rtc_get(time);
		break;

	case RT_DEVICE_CTRL_RTC_SET_TIME:
		/* write device */
		rt_hw_rtc_set(time);
		break;
	}

	return RT_EOK;
}

void rt_hw_rtc_init(void)
{
	rtc.type	= RT_Device_Class_RTC;

	/* register rtc device */
	rtc.init 	= RT_NULL;
	rtc.open 	= rt_rtc_open;
	rtc.close	= rt_rtc_close;
	rtc.read 	= rt_rtc_read;
	rtc.write	= RT_NULL;
	rtc.control = rt_rtc_control;
	
	/* no private */
	rtc.private = RT_NULL;
	
	rt_device_register(&rtc, "rtc", RT_DEVICE_FLAG_RDWR);

}

time_t time(time_t* t)
{
	rt_device_t device;
	struct tm ti;
	time_t time;
	
	device = rt_device_find("rtc");
	if (device != RT_NULL)
	{
		rt_device_control(device, RT_DEVICE_CTRL_RTC_GET_TIME, &ti);
		if (t != RT_NULL) 
		{
			time = mktime(&ti);
			*t = time;
		}	
	}
	
	return time;
}

#ifdef RT_USING_FINSH
#include <finsh.h>
void set_date(rt_uint32_t year, rt_uint32_t month, rt_uint32_t day)
{
	struct rtc_time ti;
	rt_device_t device;
		
	device = rt_device_find("rtc");
	if (device != RT_NULL)
	{
		rt_rtc_control(device, RT_DEVICE_CTRL_RTC_GET_TIME, &ti);
		ti.tm_year 	= year - 1900;
		ti.tm_mon 	= month - 1;
		ti.tm_mday 	= day;
		rt_rtc_control(device, RT_DEVICE_CTRL_RTC_SET_TIME, &ti);
	}
}
FINSH_FUNCTION_EXPORT(set_date, set date(year, month, day))

void set_time(rt_uint32_t hour, rt_uint32_t minute, rt_uint32_t second)
{
	struct rtc_time ti;
	rt_device_t device;
		
	device = rt_device_find("rtc");
	if (device != RT_NULL)
	{
		rt_rtc_control(device, RT_DEVICE_CTRL_RTC_GET_TIME, &ti);
		ti.tm_hour	= hour;
		ti.tm_min	= minute;
		ti.tm_sec 	= second;
		rt_rtc_control(device, RT_DEVICE_CTRL_RTC_SET_TIME, &ti);
	}
}
FINSH_FUNCTION_EXPORT(set_time, set time(hour, minute, second))

void list_date(void)
{
	time_t now;
	
	time(&now);
	rt_kprintf("%s\n", ctime(&now));
}
FINSH_FUNCTION_EXPORT(list_date, list date)
#endif

