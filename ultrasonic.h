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

#ifndef _ULTRASONIC_H_
#define _ULTRASONIC_H_

#define SENSOR_STATE_T0	0	// Device is idle
#define SENSOR_STATE_T1	1	// Device is in initiate pulse state
#define SENSOR_STATE_T2	2	// device is in WAIT state
#define SENSOR_STATE_T3	3	// device is in time state
#define SENSOR_STATE_T4	4	// device is in time state

class UltrasonicSensor : public ardio_task
{
	unsigned long 	m_now;
	unsigned long	m_start;
	unsigned long 	m_state_time;
	unsigned long 	m_prev_state_time;
    unsigned long   m_state;
    unsigned long   m_value;

	int	m_trigger;
    int m_sense;

	void changeState(int state);
	void timeout();
	public:
	UltrasonicSensor(int trigger);
	UltrasonicSensor(int trigger, int sense);
	~UltrasonicSensor();

	void setup();
	void loop();
    unsigned long getvalue(void);
};


#endif
