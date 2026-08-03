#include <string>

#include "imu.hpp"

std::string string_buf;
char buf[1024];

void process_input() {
    if (fgets(buf, sizeof(buf), stdin) != nullptr) {
        string_buf.append(buf);

        int idx = string_buf.find('\n');
        if (idx != std::string::npos) {
            auto line = string_buf.substr(0, idx);
            string_buf = string_buf.substr(idx + 1);
            // printf("%s\n", line.data());

            float val;
            if (sscanf(line.data(), "p:%f\n", &val) == 1) {
                MAHONY_P = val;
                printf("Set Mahony P val: %f\n", MAHONY_P);
            }

            if (sscanf(line.data(), "m:%f\n", &val) == 1) {
                MAHONY_MAG_WEIGHT = val;
                printf("Set Mahony Mag Weight val: %f\n", MAHONY_MAG_WEIGHT);
            }
        }
    }
}