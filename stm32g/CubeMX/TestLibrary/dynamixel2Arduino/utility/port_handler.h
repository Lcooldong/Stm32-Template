/*******************************************************************************
* Copyright 2016 ROBOTIS CO., LTD.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#ifndef DYNAMIXEL_PORT_HANDLER_HPP_
#define DYNAMIXEL_PORT_HANDLER_HPP_


// #include <Arduino.h>
#include "hw_def.h"


bool     DXLPortHandler(void);  //init

void     begin(uint64_t baud);
uint64_t getBaud();

void     end(void);
void     available(void);
int      read();
uint32_t write(uint8_t);
uint32_t writebuffer(uint8_t *buf, size_t len);

bool     getOpenState();
void     setOpenState(bool state);



bool open_state_;
// HardwareSerial& port_;
int dir_pin_;
unsigned long baud_;
unsigned int mbedTXdelayus;




#endif /* DYNAMIXEL_PORT_HANDLER_HPP_ */