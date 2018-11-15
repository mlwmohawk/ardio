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
/*
 * This code module handles the ultrasonic sensors
 *
 * The ultrasonic module has a one or two pin interface. A high is 
 * written to the trigger pin for > 5us. Then the trigger pin is 
 * pulled low. About 200us later, the sense pin will be pulled 
 * high by the module. It will reman high until the echo is recieved.
 * The sense pin will go low after about 20ms if no echo is received.
 *
 * On a single pin module, the same pin is used for trigger and signal,
 * The trigger pin after being pulled low, will be switched to "input"
 * and be used as the sense pin.
 */
#include "Arduino.h"
#include "ardio.h"
#include "utils.h"
#include "ultrasonic.h"

UltrasonicSensor::UltrasonicSensor(int trigger)
{
    m_start=0;
    m_trigger = trigger;
    m_sense = trigger;
    m_state = SENSOR_STATE_T0;  // Start of transmit pulse
}

UltrasonicSensor::UltrasonicSensor(int trigger, int sense)
{
    m_start=0;
    m_trigger = trigger;
    m_sense = sense;
    m_state = SENSOR_STATE_T0;  // Start of transmit pulse
}

UltrasonicSensor::~UltrasonicSensor()
{
}

void UltrasonicSensor::setup()
{
    m_state = 0;
    m_value = 0;
}

/*
 * Convert microseconds to millimetres
 * Speed of sound 343.2 metres per second
 * That's 0.3432 millimetres per microsecond
 * divide by 2 for round trip
 * 0.1716 mm per us round trip 
 */
unsigned long usec2mm(unsigned long usecStart, unsigned long usecEnd)
{
    // A long is be 32 bits, this calculation should not overflow
    unsigned long mm = longDiff(usecStart, usecEnd);
    mm = (mm*1716)/10000;
    return mm;
}

void UltrasonicSensor::changeState(int state)
{
    m_prev_state_time = m_state_time;
    m_state_time = m_now;
    m_state = state;
}

void UltrasonicSensor::timeout(unsigned long tmout)
{
    m_value = tmout;
    changeState(SENSOR_STATE_T0);
}

unsigned long UltrasonicSensor::getvalue(void)
{
    return m_value;
}

/*
 * This comprises the "state machine" for the sensor.
 * After a trigger is sent, this will step through the
 * states until complete.
 * T0   T1      T2          T3        T4   complete (back to T0)
 * v    v       v           v         v    v
 *       -------             ---------
 * _____|       |_____|||||||         |_____
 *
 * idle |trigger|settle/ping|wait echo| idle
 * 
 * T1 to T2 should be greater than 5us
 * T2 to T3 varies depending on sensor used
 * T3 to T4 is the round trip time of the sound wave
 * 
 * 
 */ 
void UltrasonicSensor::loop()
{
    m_now = micros();

    switch(m_state)
    {
        case SENSOR_STATE_T0:       // Start, initiate a ping
            m_state = SENSOR_STATE_T1;
            if(m_sense != m_trigger)
                pinMode(m_sense, INPUT);
            pinMode(m_trigger, OUTPUT);
            digitalWrite(m_trigger, 1);
            m_start = micros();
            m_state_time = 0;
            break;
        case SENSOR_STATE_T1:       // We've started the ping, now time it
            if(longDiff(m_start, m_now) > 5)
            {   // Stop ping, now wait to settle
                digitalWrite(m_trigger, 0);
                if(m_sense == m_trigger)
                    pinMode(m_sense, INPUT);
                changeState(SENSOR_STATE_T2);
            }
            break;
        case SENSOR_STATE_T2:       // Wait for sensor to settle
            if(longDiff(m_state_time, m_now) > 100)
                changeState(SENSOR_STATE_T3);

        case SENSOR_STATE_T3:       // Waiting for rise on leading edge
            if(digitalRead(m_sense))
                changeState(SENSOR_STATE_T4);
            else if(longDiff(m_start, m_now) > 1000)
                timeout(20001);
            break;
        case SENSOR_STATE_T4:       // Waiting for dropping edge of signal
            if(!digitalRead(m_sense))
            {
                m_value = usec2mm(m_state_time, m_now);
                changeState(SENSOR_STATE_T0);
            }
            else if(longDiff(m_start, m_now) > 21000)
                timeout(21000);
            break;
        default:
            timeout(21002);
            break;
    }
}


