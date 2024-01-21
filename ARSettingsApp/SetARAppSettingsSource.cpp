#include "Headers/SetARAppSettings.h"



bool writeSettingsToFile(std::string PCBFilePath, bool virtual_only_simulation, float inputVoltage, int selectedTestPoint, int selectedWaveForm, double simulationTime, double timeStep, double startTime, double endTime)
{
	std::string settingsPath = "C:\\Users\\jrhol\\OneDrive\\Documents\\University_Of_Nottingham\\EEE\\Year_4\\EEEE4008_Individual_Project\\Repos\\settings.txt";

	// Create an output file stream
	std::ofstream outputFile(settingsPath);

	// Check if the file is successfully created
	if (outputFile.is_open()) {
		std::cout << "File created successfully: " << settingsPath << std::endl;

		// You can add additional content to the file if needed
		outputFile << PCBFilePath << "\n";
		outputFile << "Virtual Only Simulation?:\t" << virtual_only_simulation << "\n";
		outputFile << "Input Voltage:\t\t\t" << inputVoltage << "\n";
		outputFile << "Selected Test Point:\t" << selectedTestPoint << "\n";
		outputFile << "Selected Wave Form:\t\t" << selectedWaveForm << "\n";
		outputFile << "Total Simulation Time:\t" << simulationTime << "\n";
		outputFile << "Simulation Time Step:\t" << timeStep << "\n";
		outputFile << "Simulation Start Time:\t" << startTime << "\n";
		outputFile << "Simulation End Time:\t" << endTime << "\n";


		// Close the file stream
		outputFile.close();
		return true;
	}

	else {
		std::cerr << "Error creating the AR settings file: " << settingsPath << std::endl;
		return false;
	}


}