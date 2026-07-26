#include <string>
#include <iostream>

#include "serial_port.hpp"

// void parse(SerialPort& sp, glm::vec3& rot) {
void parse(SerialPort& sp, glm::quat& q) {
    // static std::string rx_buffer = "7.1,5.4,9.88888\n77.0,3.2,k\n";
    static std::string rx_buffer;
    std::string incoming_data = sp.pull_buffer();
    rx_buffer += incoming_data;

    // std::cout << "Buffer: " << rx_buffer << std::endl;
    if (rx_buffer.size() == 0) return;

    for (int idx = 0; idx < rx_buffer.size(); idx++) {
        char c = rx_buffer.at(idx);

        float val[4];
        if (c == '\n') {
            // found newline
            std::string line = rx_buffer.substr(0, idx);
            // std::cout << "Line: " << line << std::endl;
            int n = sscanf(line.data(), "%f,%f,%f,%f\n", &val[0], &val[1],
                           &val[2], &val[3]);
            if (n == 4) {
                // success
                // printf("Read: %f, %f, %f, %f\n", val[0], val[1], val[2],
                //        val[3]);

                // update rot
                q = glm::quat(val[0], val[1], val[2], val[3]);
            }

            if (rx_buffer.size() > idx + 1) {
                rx_buffer = rx_buffer.substr(idx + 1, rx_buffer.size() - idx);
                // std::cout << "Remainder: " << rx_buffer << std::endl;
            }

            return;
        }
    }
}
