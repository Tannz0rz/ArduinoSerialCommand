#include "serial_command_server.hpp"

enum : uint8_t
{
    ADD_COMMAND = 0,
    SUB_COMMAND = 1,
    MUL_COMMAND = 2,
    DIV_COMMAND = 3
};

serial_command_server_t<4> serial_command_server;

// Command identifier: ADD_COMMAND (0)
// Request: a -> uint32_t, b -> uint32_t
// Response: sum -> uint32_t
uint8_t add_command(uint8_t const &request_size, uint8_t const *request_buffer, uint8_t &response_size, uint8_t *response_buffer) 
{
    if (request_size == sizeof(uint32_t) + sizeof(uint32_t)) {
        uint32_t a;
        uint32_t b;
        
        memcpy(&a, &request_buffer[0], sizeof(uint32_t));
        memcpy(&b, &request_buffer[sizeof(uint32_t)], sizeof(uint32_t));
      
        response_size = sizeof(uint32_t);
        
        uint32_t sum = a + b;
      
        memcpy(&response_buffer[0], &sum, sizeof(uint32_t));      
      
        return serial_command_status_e::SUCCESS;
    }
  
    return serial_command_status_e::FAILURE;
}

// Command identifier: SUB_COMMAND (1)
// Request: a -> uint32_t, b -> uint32_t
// Response: difference -> uint32_t
uint8_t sub_command(uint8_t const &request_size, uint8_t const *request_buffer, uint8_t &response_size, uint8_t *response_buffer) 
{
    if (request_size == sizeof(uint32_t) + sizeof(uint32_t)) {
        uint32_t a;
        uint32_t b;
        
        memcpy(&a, &request_buffer[0], sizeof(uint32_t));
        memcpy(&b, &request_buffer[sizeof(uint32_t)], sizeof(uint32_t));
      
        response_size = sizeof(uint32_t);
        
        uint32_t difference = a - b;
      
        memcpy(&response_buffer[0], &difference, sizeof(uint32_t));      
      
        return serial_command_status_e::SUCCESS;
    }
  
    return serial_command_status_e::FAILURE;
}

// Command identifier: MUL_COMMAND (2)
// Request: a -> uint32_t, b -> uint32_t
// Response: product -> uint32_t
uint8_t mul_command(uint8_t const &request_size, uint8_t const *request_buffer, uint8_t &response_size, uint8_t *response_buffer) 
{
    if (request_size == sizeof(uint32_t) + sizeof(uint32_t)) {
        uint32_t a;
        uint32_t b;
        
        memcpy(&a, &request_buffer[0], sizeof(uint32_t));
        memcpy(&b, &request_buffer[sizeof(uint32_t)], sizeof(uint32_t));
      
        response_size = sizeof(uint32_t);
        
        uint32_t product = a * b;
      
        memcpy(&response_buffer[0], &product, sizeof(uint32_t));      
      
        return serial_command_status_e::SUCCESS;
    }
  
    return serial_command_status_e::FAILURE;
}

// Command identifier: DIV_COMMAND (3)
// Request: a -> uint32_t, b -> uint32_t
// Response: quotient -> uint32_t
uint8_t div_command(uint8_t const &request_size, uint8_t const *request_buffer, uint8_t &response_size, uint8_t *response_buffer) 
{
    if (request_size == sizeof(uint32_t) + sizeof(uint32_t)) {
        uint32_t a;
        uint32_t b;
        
        memcpy(&a, &request_buffer[0], sizeof(uint32_t));
        memcpy(&b, &request_buffer[sizeof(uint32_t)], sizeof(uint32_t));
      
        response_size = sizeof(uint32_t);
        
        uint32_t quotient = a / b;
      
        memcpy(&response_buffer[0], &quotient, sizeof(uint32_t));      
      
        return serial_command_status_e::SUCCESS;
    }
  
    return serial_command_status_e::FAILURE;
}

void setup()
{
    serial_command_server.setup(115200);
    
    serial_command_server.register_command(ADD_COMMAND, &add_command);
    serial_command_server.register_command(SUB_COMMAND, &sub_command);
    serial_command_server.register_command(MUL_COMMAND, &mul_command); 
    serial_command_server.register_command(DIV_COMMAND, &div_command); 
}

void loop()
{
    serial_command_server.listen();  
}
