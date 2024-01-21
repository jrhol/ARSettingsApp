#ifndef SetESP_def
#define SetESP_def

#include <string>
#include <sstream>
#include <iostream>
#include <windows.h>

bool writeSettingsToESP32(std::string comPort, bool virtual_only_simulation, float inputVoltage, int selectedTestPoint, int selectedWaveForm, double simulationTime, double timeStep, double startTime, double endTime);

#endif // !SetESP_def