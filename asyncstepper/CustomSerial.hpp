/***************************************************
Copyright (c) 2025 Kyle Rush
(quackings.com)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses
****************************************************/

#include <Arduino.h>
#include <Array.h>

class CustomSerial
{
public:
    
    uint16_t m_baudrate;
    uint16_t m_timeout;

    String keys[10];



    CustomSerial(uint16_t baudrate, uint16_t timeout)
    {
        m_baudrate = baudrate;
        m_timeout = timeout;
    }

    void create()
    {
        Serial.begin(m_baudrate);
        Serial.setTimeout(m_timeout);
        Serial.print(OPENING_MSG);
        HAS_INIT = true;
    }

    void addDataKey(String key)
    {
    }

    void update()
    {

    }

private:
    bool HAS_INIT = false;
    
    String OPENING_MSG = "Open";

    void p_submitHandshake() {

    }
}