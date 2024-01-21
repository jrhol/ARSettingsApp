#ifndef SetAR_def
#define SetAR_def

#include <string>
#include <fstream>
#include <iostream>

bool writeSettingsToFile(std::string PCBFilePath, bool virtual_only_simulation, float inputVoltage, int selectedTestPoint, int selectedWaveForm, double simulationTime, double timeStep, double startTime, double endTime); //Pass the location of the PCB in here and write to file - used by ar app

#endif // !SetAR_def