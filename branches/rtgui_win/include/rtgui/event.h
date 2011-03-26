/*
 * File      : event.h
 * This file is part of RTGUI in RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-10-04     Bernard      first version
 */
#ifndef __RTGUI_EVENT_H__
#define __RTGUI_EVENT_H__

#include <rtgui/rtgui.h>
#include <rtgui/rtgui_system.h>

/* Use new name & namespace */
typedef struct rtgui_event 					rtgui_event_t;
typedef struct rtgui_event_win				rtgui_event_win_t;
typedef struct rtgui_event_win_move			rtgui_event_win_move_t;
typedef struct rtgui_event_win_resize		rtgui_event_win_resize_t;

typedef struct rtgui_event_set_wm			rtgui_event_set_wm_t;
typedef struct rtgui_event_update			rtgui_event_update_t;
typedef struct rtgui_event_monitor			rtgui_event_monitor_t;
typedef struct rtgui_event_paint			rtgui_event_paint_t;
typedef struct rtgui_event_timer			rtgui_event_timer_t;
typedef struct rtgui_event_clip				rtgui_event_clip_t;

typedef struct rtgui_event_mouse			rtgui_event_mouse_t;
typedef struct rtgui_event_kbd				rtgui_event_kbd_t;
typedef struct rtgui_event_command			rtgui_event_command_t;
typedef struct rtgui_event_scrollbar		rtgui_event_scrollbar_t;

typedef struct rtgui_event_focused			rtgui_event_focused_t;
typedef struct rtgui_event_resize			rtgui_event_resize_t;

enum _rtgui_event_type
{
	/* panel event NULL */
	/* window event */
	RTGUI_EVENT_WIN_CREATE=8,       /* create a window 		*/
	RTGUI_EVENT_WIN_DESTROY,        /* destroy a window 	*/
	RTGUI_EVENT_WIN_SHOW,           /* show a window 		*/
	RTGUI_EVENT_WIN_HIDE,           /* hide a window 		*/
	RTGUI_EVENT_WIN_ACTIVATE,       /* activate a window 	*/
	RTGUI_EVENT_WIN_DEACTIVATE,     /* deactivate a window 	*/
	RTGUI_EVENT_WIN_CLOSE,          /* close a window 		*/
	RTGUI_EVENT_WIN_MAX,            /* max a window 		*/
	RTGUI_EVENT_WIN_MIN,            /* min a window 		*/
	RTGUI_EVENT_WIN_MOVE,           /* move a window 		*/
	RTGUI_EVENT_WIN_RESIZE,         /* resize a window 		*/

	RTGUI_EVENT_UPDATE,             /* update a rect 		*/
	RTGUI_EVENT_PAINT,              /* paint on screen 		*/
	RTGUI_EVENT_TIMER,              /* timer 0x17			*/
	/* clip rect information */
	RTGUI_EVENT_CLIP,               /* clip rect info		*/
	/* mouse and keyboard event */
	RTGUI_EVENT_MOUSE_MOTION,       /* mouse motion         */
	RTGUI_EVENT_MOUSE_BUTTON,       /* mouse button info 	*/
	RTGUI_EVENT_KBD,                /* keyboard info	    */
	/* user command event */
	RTGUI_EVENT_COMMAND,            /* user command 		*/
	/* widget event */
	RTGUI_EVENT_FOCUSED,            /* widget focused       */
	RTGUI_EVENT_SCROLLED,           /* scroll bar scrolled  */
	RTGUI_EVENT_RESIZE,             /* widget resize 		*/
}; 
typedef enum _rtgui_event_type rtgui_event_type;

enum {
	RTGUI_STATUS_OK = 0,		/* status ok 		*/
	RTGUI_STATUS_ERROR,			/* generic error 	*/
	RTGUI_STATUS_NRC,			/* no resource 		*/
};

struct rtgui_event
{
	/* the event type */
	rt_uint16_t type;
	/* user field of event */
	rt_uint16_t user;

	/* the event sender */
	struct rt_thread* sender;

	/* mailbox to acknowledge request */
	struct rt_mailbox* ack;
};

#define RTGUI_EVENT(e)	((rtgui_event_t*)(e))

#define RTGUI_EVENT_INIT(e, t)	do		\
{										\
	(e)->type = (t);					\
	(e)->user = 0;						\
	(e)->sender = rt_thread_self();		\
	(e)->ack = RT_NULL;					\
} while(0)

/*
 * RTGUI Window Event
 */
struct rtgui_event_win
{
	rtgui_event_t parent;

	/* the window id */
	rtgui_win_t* wid;
};

struct rtgui_event_win_move
{
	rtgui_event_t parent;

	/* the window id */
	rtgui_win_t* wid;

	rt_int16_t x, y;
};

struct rtgui_event_win_resize
{
	rtgui_event_t parent;

	/* the window id */
	rtgui_win_t* wid;

	rtgui_rect_t rect;
};

/* window event init */
#define RTGUI_EVENT_WIN_CREATE_INIT(e)		RTGUI_EVENT_INIT(&((e)->parent), RTGUI_EVENT_WIN_CREATE)
#define RTGUI_EVENT_WIN_DESTROY_INIT(e)		RTGUI_EVENT_INIT(&((e)->parent), RTGUI_EVENT_WIN_DESTROY)
#define RTGUI_EVENT_WIN_SHOW_INIT(e)		RTGUI_EVENT_INIT(&((e)->parent), RTGUI_EVENT_WIN_SHOW)
#define RTGUI_EVENT_WIN_HIDE_INIT(e)		RTGUI_EVENT_INIT(&((e)->parent), RTGUI_EVENT_WIN_HIDE)
#define RTGUI_EVENT_WIN_ACTIVATE_INIT(e)	RTGUI_EVENT_INIT(&((e)->parent), RTGUI_EVENT_WIN_ACTIVATE)
#define RTGUI_EVENT_WIN_DEACTIVATE_INIT(e)	RTGUI_EVENT_INIT(&((e)->parent), RTGUI_EVENT_WIN_DEACTIVATE)
#define RTGUI_EVENT_WIN_CLOSE_INIT(e)		RTGUI_EVENT_INIT(&((e)->parent), RTGUI_EVENT_WIN_CLOSE)
#define RTGUI_EVENT_WIN_MAX_INIT(e)			RTGUI_EVENT_INIT(&((e)->parent), RTGUI_EVENT_WIN_MAX)
#define RTGUI_EVENT_WIN_MIN_INIT(e)			RTGUI_EVENT_INIT(&((e)->parent), RTGUI_EVENT_WIN_MIN)
#define RTGUI_EVENT_WIN_MOVE_INIT(e)		RTGUI_EVENT_INIT(&((e)->parent), RTGUI_EVENT_WIN_MOVE)
#define RTGUI_EVENT_WIN_RESIZE_INIT(e)		RTGUI_EVENT_INIT(&((e)->parent), RTGUI_EVENT_WIN_RESIZE)

/*
 * RTGUI Other Event
 */
struct rtgui_event_update
{
	rtgui_event_t parent;

	/* the update rect */
	rtgui_rect_t rect;
};

struct rtgui_event_monitor
{
	rtgui_event_t parent;

	/* the monitor rect */
	rtgui_rect_t rect;

	/* under panel */
	rtgui_panel_t* panel;

	/* or under window */
	rtgui_win_t* wid;
};

struct rtgui_event_paint
{
	rtgui_event_t parent;
	rtgui_win_t* wid;		/* destination window */
	rt_bool_t bfull;//更新全部,还是更新子控件
};

struct rtgui_event_timer
{
	rtgui_event_t parent;

	rtgui_timer_t *timer;
};

struct rtgui_event_clip
{
	rtgui_event_t parent;
	/* destination window */
	rtgui_win_t* wid;
	/* the number of rects */
	rt_uint32_t num_rect;
	/* rtgui_rect_t *rects */
};
#define RTGUI_EVENT_GET_RECT(e, i)			&(((rtgui_rect_t*)(e + 1))[i])

#define RTGUI_EVENT_UPDATE_INIT(e)			RTGUI_EVENT_INIT(&((e)->parent), RTGUI_EVENT_UPDATE)
#define RTGUI_EVENT_CLIP_INIT(e)				RTGUI_EVENT_INIT(&((e)->parent), RTGUI_EVENT_CLIP)
#define RTGUI_EVENT_PAINT_INIT(e)				RTGUI_EVENT_INIT(&((e)->parent), RTGUI_EVENT_PAINT)
#define RTGUI_EVENT_TIMER_INIT(e)				RTGUI_EVENT_INIT(&((e)->parent), RTGUI_EVENT_TIMER)

/*
 * RTGUI Mouse and Keyboard Event
 */
struct rtgui_event_mouse
{
	rtgui_event_t parent;

	rtgui_win_t* wid;		/* destination window */

	rt_uint16_t x, y;
	rt_uint16_t button;
};

#define RTGUI_MOUSE_BUTTON_RIGHT		0x01
#define RTGUI_MOUSE_BUTTON_LEFT			0x02
#define RTGUI_MOUSE_BUTTON_MIDDLE		0x03
#define RTGUI_MOUSE_BUTTON_WHEELUP		0x04
#define RTGUI_MOUSE_BUTTON_WHEELDOWN	0x08

#define RTGUI_MOUSE_BUTTON_DOWN			0x10
#define RTGUI_MOUSE_BUTTON_UP			0x20

struct rtgui_event_kbd
{
	rtgui_event_t parent;

	rtgui_win_t* wid;		/* destination window */

	rt_uint16_t type;		/* key down or up */
	rt_uint16_t key;		/* current key */
	rt_uint16_t mod;		/* current key modifiers */
	rt_uint16_t unicode;	/* translated character */
};

struct rtgui_event_current_pos
{
	rtgui_event_t parent;
};

//液晶屏背光
struct rtgui_event_backlight
{
	rtgui_event_t parent;
	rt_uint16_t light;
};

#define RTGUI_KBD_IS_SET_CTRL(e)	((e)->mod & (RTGUI_KMOD_LCTRL | RTGUI_KMOD_RCTRL))
#define RTGUI_KBD_IS_SET_ALT(e)		((e)->mod & (RTGUI_KMOD_LALT  | RTGUI_KMOD_RALT))
#define RTGUI_KBD_IS_SET_SHIFT(e)	((e)->mod & (RTGUI_KMOD_LSHIFT| RTGUI_KMOD_RSHIFT))
#define RTGUI_KBD_IS_UP(e)			((e)->type == RTGUI_KEYUP)
#define RTGUI_KBD_IS_DOWN(e)		((e)->type == RTGUI_KEYDOWN)

#define RTGUI_EVENT_MOUSE_MOTION_INIT(e)	RTGUI_EVENT_INIT(&((e)->parent), RTGUI_EVENT_MOUSE_MOTION)
#define RTGUI_EVENT_MOUSE_BUTTON_INIT(e)	RTGUI_EVENT_INIT(&((e)->parent), RTGUI_EVENT_MOUSE_BUTTON)
#define RTGUI_EVENT_KBD_INIT(e)				RTGUI_EVENT_INIT(&((e)->parent), RTGUI_EVENT_KBD)

struct rtgui_event_command
{
	rtgui_event_t parent;

	/* command type */
	rt_int32_t type;

	/* command id */
	rt_int32_t command;

	/* command string */
	char command_string[RTGUI_NAME_MAX];
};
#define RTGUI_EVENT_COMMAND_INIT(e)	RTGUI_EVENT_INIT(&((e)->parent), RTGUI_EVENT_COMMAND)

#define RTGUI_CMD_UNKNOWN		0x00
#define RTGUI_CMD_WM_CLOSE		0x10

#define RTGUI_CMD_USER_INT		0x20
#define RTGUI_CMD_USER_STRING	0x21

/************************************************************************/
/* Widget Event                                                         */
/************************************************************************/
#define RTGUI_WIDGET_EVENT_INIT(e, t)	do		\
{										\
	(e)->type = (t);					\
	(e)->sender = RT_NULL;				\
	(e)->ack = RT_NULL;					\
} while(0)

/*
 * RTGUI Scrollbar Event
 */
struct rtgui_event_scrollbar
{
    rtgui_event_t parent;

    rt_uint8_t event;
};
#define RTGUI_SCROLL_LINEUP             0x01
#define RTGUI_SCROLL_LINEDOWN           0x02
#define RTGUI_SCROLL_PAGEUP             0x03
#define RTGUI_SCROLL_PAGEDOWN           0x04
#define RTGUI_EVENT_SCROLLED_INIT(e)	RTGUI_EVENT_INIT(&((e)->parent), RTGUI_EVENT_SCROLLED)

/*
 * RTGUI Widget Focused Event
 */
struct rtgui_event_focused
{
    rtgui_event_t parent;

	rtgui_widget_t* widget;
};
#define RTGUI_EVENT_FOCUSED_INIT(e)		RTGUI_EVENT_INIT(&((e)->parent), RTGUI_EVENT_FOCUSED)

/*
 * RTGUI Widget Resize Event
 */
struct rtgui_event_resize
{
	rtgui_event_t parent;
	rt_int16_t x, y;
	rt_int16_t w, h;
};
#define RTGUI_EVENT_RESIZE_INIT(e) RTGUI_EVENT_INIT(&((e)->parent), RTGUI_EVENT_RESIZE)

#endif

