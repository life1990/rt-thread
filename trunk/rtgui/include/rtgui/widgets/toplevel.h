/*
 * File      : toplevel.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-10-16     Bernard      first version
 */
#ifndef __RTGUI_TOPLEVEL_H__
#define __RTGUI_TOPLEVEL_H__

#include <rtgui/widgets/container.h>

/** Gets the type of a toplevel */
#define RTGUI_TOPLEVEL_TYPE       (rtgui_toplevel_type_get())
/** Casts the object to an rtgui_toplevel */
#define RTGUI_TOPLEVEL(obj)       (RTGUI_OBJECT_CAST((obj), RTGUI_TOPLEVEL_TYPE, rtgui_toplevel_t))
/** Checks if the object is an rtgui_toplevel */
#define RTGUI_IS_TOPLEVEL(obj)    (RTGUI_OBJECT_CHECK_TYPE((obj), RTGUI_TOPLEVEL_TYPE))

struct rtgui_toplevel
{
	/* inherit from container */
	rtgui_container_t parent;

	/* drawing count */
	rt_base_t drawing;

	/* external clip info */
	rtgui_rect_t*	external_clip_rect;
	rt_uint32_t		external_clip_size;

	/* server thread id */
	rt_thread_t server;

	/* current focus widget */
	rtgui_widget_t* focus;
};
typedef struct rtgui_toplevel rtgui_toplevel_t;

rtgui_type_t *rtgui_toplevel_type_get(void);

rt_bool_t rtgui_toplevel_event_handler(rtgui_widget_t* widget, rtgui_event_t* event);

void rtgui_toplevel_handle_clip(struct rtgui_toplevel* top,
								struct rtgui_event_clip_info* info);
void rtgui_toplevel_update_clip(rtgui_toplevel_t* top);

void rtgui_toplevel_set_focus(struct rtgui_toplevel* top, rtgui_widget_t* focus);
rtgui_widget_t* rtgui_toplevel_get_focus(struct rtgui_toplevel* top);

#endif
