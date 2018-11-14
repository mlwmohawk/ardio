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
#include <limits.h>

void setPwmFrequency (int pin, int divisor)
{
	byte mode;
	if (pin == 5 || pin == 6 || pin == 9 || pin == 10)
	{
		switch (divisor)
		{
		case 1:
			mode = 0x01;
			break;
		case 8:
			mode = 0x02;
			break;
		case 64:
			mode = 0x03;
			break;
		case 256:
			mode = 0x04;
			break;
		case 1024:
			mode = 0x05;
			break;
		default:
			return;
		}
		if (pin == 5 || pin == 6)
		{
			TCCR0B = (TCCR0B & 0b11111000) | mode;
		}
		else
		{
			TCCR1B = (TCCR1B & 0b11111000) | mode;
		}
	}
	else if (pin == 3 || pin == 11)
	{
		switch (divisor)
		{
		case 1:
			mode = 0x01;
			break;
		case 8:
			mode = 0x02;
			break;
		case 32:
			mode = 0x03;
			break;
		case 64:
			mode = 0x04;
			break;
		case 128:
			mode = 0x05;
			break;
		case 256:
			mode = 0x06;
			break;
		case 1024:
			mode = 0x7;
			break;
		default:
			return;
		}
		TCCR2B = (TCCR2B & 0b11111000) | mode;
	}
}

unsigned long longDiff(unsigned long start, unsigned long finish)
{
	if(finish < start)
		return start + (0-finish);
	else
		return finish - start;
}

unsigned long microDiff(unsigned long *lastTime)
{
	unsigned long t;
	unsigned long before = *lastTime;
	unsigned long now = *lastTime = micros();
	if (before > now) // Clock wrapped
		t = now + (0 - before);
	else
		t = (before == 0) ? 0 : now - before;
	return t;
}


unsigned long milliDiff( unsigned long *lastTime)
{
	unsigned long t;
	unsigned long before = *lastTime;
	unsigned long now = *lastTime = millis();
	if (before > now) // Clock wrapped
		t = now + (0 - before);
	else
		t = (before == 0) ? 0 : now - before;
	return t;
}

unsigned long difftime(unsigned long tmprev, unsigned long tmnow)
{
	return (tmprev < tmnow) ?  tmnow - tmprev : (ULONG_MAX - tmprev) + tmnow;
}
