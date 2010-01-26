/*
 * File      : list_view.h
 * This file is part of RTGUI in RT-Thread RTOS
 * COPYRIGHT (C) 2010, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2010-01-06     Bernard      first version
 */

#ifndef __RTGUI_LIST_VIEW_H__
#define __RTGUI_LIST_VIEW_H__

#include <rtgui/rtgui.h>
#include <rtgui/image.h>
#include <rtgui/rtgui_system.h>

#include <rtgui/widgets/view.h>

typedef void (*item_action)(void* parameter);
struct rtgui_list_item
{
    char* name;
	rtgui_image_t *image;

    item_action action;
    void *parameter;
};

/** Gets the type of a list view */
#define RTGUI_LIST_VIEW_TYPE       (rtgui_list_view_type_get())
/** Casts the object to a filelist */
#define RTGUI_LIST_VIEW(obj)       (RTGUI_OBJECT_CAST((obj), RTGUI_LIST_VIEW_TYPE, rtgui_list_view_t))
/** Checks if the object is a filelist view */
#define RTGUI_IS_LIST_VIEW(obj)    (RTGUI_OBJECT_CHECK_TYPE((obj), RTGUI_LIST_VIEW_TYPE))

struct rtgui_list_view
{
	struct rtgui_view parent;

	/* widget private data */
	/* list item */
    struct rtgui_list_item* items;

	/* total number of items */
	rt_uint16_t items_count;
    /* the number of item in a page */
    rt_uint16_t page_items;
    /* current item */
    rt_uint16_t current_item;
};
typedef struct rtgui_list_view rtgui_list_view_t;

rtgui_type_t *rtgui_list_view_type_get(void);

rtgui_list_view_t* rtgui_list_view_create(struct rtgui_list_item* items, rt_uint16_t count,
    rtgui_rect_t *rect);
void rtgui_list_view_clear(rtgui_list_view_t* view);

rt_bool_t rtgui_list_view_event_handler(struct rtgui_widget* widget, struct rtgui_event* event);

#endif
