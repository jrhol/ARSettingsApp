#ifndef SetAR_def
#define SetAR_def

#include <string>
#include <fstream>
#include <iostream>
#include "Settings.h"

bool writeSettingsToFile(std::string settingsFilePath, settings& settingsToWrite); //Pass the location of the PCB in here and write to file - used by ar app

#endif // !SetAR_def