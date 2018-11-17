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
*/

#include "Arduino.h"
#include "ardio.h"
#include "DS18B20.h"

#include <OneWire.h> 
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

void DS18B20::setup()
{
	m_task_state = TASK_IDLE; // Don't run 
	sensors.begin(); 
}

unsigned long DS18B20::getvalue(int device)
{
	DeviceAddress deviceAddress;
	sensors.requestTemperatures();
	if (!sensors.getAddress(deviceAddress, device)) 
	{
		return 0;
	}

	float temp = sensors.getTemp((uint8_t*) deviceAddress);
	return (unsigned long) temp;
}
