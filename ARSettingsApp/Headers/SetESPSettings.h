#ifndef SetESP_def
#define SetESP_def

#include "string"



bool writeSettingsToESP32(std::string comPort, double inputVoltage, int waveform);

#endif // !SetESP_def