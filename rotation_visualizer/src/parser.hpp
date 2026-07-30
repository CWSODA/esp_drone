#include <string>
#include <iostream>

#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>

#include "serial_port.hpp"

void parse(SerialPort& sp, glm::quat& quat, glm::vec3& accel, glm::vec3& mag) {
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

            // Check for label
            switch (line.at(0)) {
                case 'q': {
                    int n = sscanf(line.data(), "q:%f,%f,%f,%f\n", &val[0],
                                   &val[1], &val[2], &val[3]);
                    if (n == 4) {
                        // update rot
                        quat = glm::quat(val[0], val[1], val[2], val[3]);
                    }
                    break;
                }
                case 'a': {
                    int n = sscanf(line.data(), "a:%f,%f,%f\n", &val[0],
                                   &val[1], &val[2]);
                    if (n == 3) {
                        accel = glm::vec3(val[0], val[1], val[2]);
                    }
                    break;
                }
                case 'm': {
                    int n = sscanf(line.data(), "m:%f,%f,%f\n", &val[0],
                                   &val[1], &val[2]);
                    if (n == 3) {
                        mag = glm::vec3(val[0], val[1], val[2]);
                    }
                    break;
                }
                default:
                    continue;
            }

            if (rx_buffer.size() > idx + 1) {
                rx_buffer = rx_buffer.substr(idx + 1, rx_buffer.size() - idx);
                // std::cout << "Remainder: " << rx_buffer << std::endl;
            }

            return;
        }
    }
}
