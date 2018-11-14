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

#define ARDIO_MAJOR 0
#define ARDIO_MINOR 1

// Task states
enum task_states
{
    TASK_IDLE,
    TASK_RUNABLE,
    TASK_BUSY
};

class ardio_task
{
    protected:
    int m_task_state;

    public:
    ardio_task();
    ~ardio_task();
    int getstate() { return m_task_state; }
    void setstate(int state) { m_task_state = state;}

    virtual void setup()=0;
    virtual void loop()=0;
};


typedef unsigned long (*cmdfunc)(char *buffer);
struct command
{
    char chop;
    const char *name;
    cmdfunc func;
};


// Standard classes
class ardio_serial : public ardio_task
{
    protected:
    static int readchar(void); 
    public:
    void setup();
    void loop();
};

extern ardio_task * tasks[];
extern command extra_commands[];
extern char do_echo;
extern unsigned long handle_cmd(char *buffer);
