idf_component_register(SRCS "main.cpp" "pwm.cpp" "i2c.cpp" "motor.cpp" "imu.cpp"
                        REQUIRES esp_driver_mcpwm esp_driver_i2c esp_timer
                        INCLUDE_DIRS "include/." ".")
