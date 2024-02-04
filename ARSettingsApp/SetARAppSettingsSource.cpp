#include "Headers/SetARAppSettings.h"



bool writeSettingsToFile(std::string settingsFilePath, settings& settingsToWrite)
{
	// Create an output file stream
	std::ofstream outputFile(settingsFilePath);

	// Check if the file is successfully created
	if (outputFile.is_open()) {
		std::cout << "File created successfully: " << settingsFilePath << std::endl;

		// You can add additional content to the file if needed
		outputFile << "File Path:" << settingsToWrite.getPCBFilePath() << "\n";
		outputFile << "Com Port:" << settingsToWrite.getComPort().c_str() << "\n";
		outputFile << "Virtual Only Simulation?:" << settingsToWrite.getVirtualOnlySimulation() << "\n";
		outputFile << "Input Voltage:" << settingsToWrite.getInputVoltage() << "\n";
		outputFile << "Selected Test Point:" << settingsToWrite.getSelectedTestPoint() << "\n";
		outputFile << "Selected Wave Form:" << settingsToWrite.getSelectedWaveForm() << "\n";
		outputFile << "Total Simulation Time:" << settingsToWrite.getSimulationTime() << "\n";
		outputFile << "Simulation Time Step:" << settingsToWrite.getTimeStep() << "\n";
		outputFile << "Simulation Start Time:" << settingsToWrite.getStartTime() << "\n";
		outputFile << "Simulation End Time:" << settingsToWrite.getEndTime() << "\n";


		// Close the file stream
		outputFile.close();
		return true;
	}

	else {
		std::cerr << "Error creating the AR settings file: " << settingsFilePath << std::endl;
		return false;
	}


}