/*
 * GTK VNC Widget
 *
 * Copyright (C) 2006  Anthony Liguori <anthony@codemonkey.ws>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 */

#ifndef _COROUTINE_H_
#define _COROUTINE_H_

#include "config.hpp"

#if WITH_UCONTEXT
#include "continuation.hpp"
#else
#include <glib.h>
#endif

struct coroutine
{
    size_t stack_size;
    void *(*entry)(void *);
    int (*release)(struct coroutine *);

    /* read-only */
    int exited;

    /* private */
    struct coroutine *caller;
    void *data;

#if WITH_UCONTEXT
    struct continuation cc;
#else
    GThread *thread;
    gboolean runnable;
#endif
};

int coroutine_init(struct coroutine *co);

int coroutine_release(struct coroutine *co);

void *coroutine_swap(struct coroutine *from, struct coroutine *to, void *arg);

struct coroutine *coroutine_self(void);

void *coroutine_yieldto(struct coroutine *to, void *arg);

void *coroutine_yield(void *arg);

#endif
/*
 * Local variables:
 *  c-indent-level: 4
 *  c-basic-offset: 4
 *  indent-tabs-mode: nil
 * End:
 */
