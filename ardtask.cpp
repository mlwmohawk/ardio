/*
    Copyright (C) 2018 Mark L. Woodward
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.
 
    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
    MA 02111-1307, USA
 
    If you want support or to commercially license this library, the author
    can be reached at markw@mohawksoft.com
*/ 

#include <Arduino.h>
#include "libardio.h"
#include "ardio.h"

ardio_task::ardio_task()
{
    m_task_state = TASK_RUNABLE;
}
ardio_task::~ardio_task()
{
}

void yield()
{
    for(int i=0; tasks[i] != NULL; i++)
    {
        int oldstate = tasks[i]->getstate();
        if((oldstate & TASK_RUNABLE) == TASK_RUNABLE)
        {
            tasks[i]->setstate(TASK_BUSY);
            tasks[i]->loop();
            tasks[i]->setstate(oldstate);
        }
    }
}

void setup()
{
    for(int i=0; tasks[i] != NULL; i++)
        tasks[i]->setup();
}

void loop ()
{
    yield();
}
