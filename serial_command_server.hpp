/*
 * Copyright 2022 Casey Sanchez
 */

#pragma once

#include <Arduino.h>

enum serial_command_status_e : uint8_t
{
    FAILURE = 0,
    SUCCESS = 1
};

struct serial_command_request_t
{
    uint8_t command;
    uint8_t size;
    uint8_t buffer[255];

    serial_command_request_t() :
        command { ~0U },
        size { 0U }
    {
    }
};

struct serial_command_response_t
{
    uint8_t status;
    uint8_t size;
    uint8_t buffer[255];

    serial_command_response_t() : 
        status { serial_command_status_e::FAILURE },
        size { 0U }
    {
    }
};

template <uint8_t MAX_COMMANDS>
class serial_command_server_t
{
    struct serial_command_t
    {
        uint8_t command;
        uint8_t (*callback)(uint8_t const &, uint8_t const *, uint8_t &, uint8_t *);
    };
  
    serial_command_t m_serial_commands[MAX_COMMANDS];
  
    size_t m_num_commands;
  
public:
    serial_command_server_t() :
        m_num_commands { 0 }
    {
    }

    void setup(uint32_t const &baud_rate)
    {
        Serial.begin(baud_rate);
        
        while (!Serial);
    }

    bool register_command(uint8_t const &command, uint8_t (*callback)(uint8_t const &, uint8_t const *, uint8_t &, uint8_t *))
    {
        if (m_num_commands >= MAX_COMMANDS || callback == nullptr) {
            return false;
        }
    
        size_t const index = m_num_commands++;
      
        m_serial_commands[index].command = command;
        m_serial_commands[index].callback = callback;
      
        return true;
    }
    
    void listen()
    {      
        if (Serial.available()) {
            serial_command_request_t request;
            serial_command_response_t response;
        
            Serial.readBytes(&request.command, sizeof(uint8_t));  
            Serial.readBytes(&request.size, sizeof(uint8_t));
        
            if (request.size > 0) {
                Serial.readBytes(&request.buffer[0], request.size);
            }

            for (size_t index = 0; index < m_num_commands; ++index) {
                if (m_serial_commands[index].command == request.command) {
                    response.status = m_serial_commands[index].callback(request.size, request.buffer, response.size, response.buffer);
            
                    break;
                }
            }

            if (response.status == serial_command_status_e::FAILURE) {
                response.size = 0;
            }

            Serial.write(&response.status, sizeof(uint8_t));
            Serial.write(&response.size, sizeof(uint8_t));
        
            if (response.size > 0) {
                Serial.write(&response.buffer[0], response.size);
            }
        }
    }
};
