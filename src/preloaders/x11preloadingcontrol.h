// -*- mode: c++; c-basic-offset: 4; c-basic-style: bsd; -*-
/*
 *   This program is free software; you can redistribute it and/or
 *   modify
 *   it under the terms of the GNU Lesser General Public License as
 *   published by the Free Software Foundation; either version 3.0 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *   02111-1307 USA
 *
 *   This file is part of the Open-HMI Tester,
 *   http://openhmitester.sourceforge.net
 *
 */
#ifndef X11PreloadingControl_H
#define X11PreloadingControl_H

#include <preloadingcontrol.h>
#include <qteventconsumer.h>
#include <qteventexecutor.h>

class X11PreloadingControl : public PreloadingControl
{

public:

    X11PreloadingControl(EventConsumer* ec, EventExecutor* ex);
    virtual ~X11PreloadingControl();

private:

    EventConsumer* _event_consumer;
    EventExecutor* _event_executor;


public:

    static bool do_preload();

private:

    static PreloadingControl *pc;
};


#endif // X11PreloadingControl_H
