#include "Headers/SetESPSettings.h"

bool writeSettingsToESP32(settings& settingsToWrite) {

	HANDLE hSerial;
	DCB dcbSerialParams = { 0 };
	COMMTIMEOUTS timeout = { 0 };

	std::wstring comPort = settingsToWrite.getComPort();

	// Open the serial port
	hSerial = CreateFile(
		comPort.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	DWORD errMSG = GetLastError();

	if (errMSG == 2)
	{
		std::cout << "Plug in the device first...." << std::endl;
		return 1;
	}

	if (errMSG == 5)
	{
		std::cout << "Something else is using the com port..." << std::endl;
		return 1;
	}

	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	if (!GetCommState(hSerial, &dcbSerialParams)) {
		std::cerr << "Error getting serial port state" << std::endl;
		CloseHandle(hSerial);
		return 1;
	}

	dcbSerialParams.BaudRate = 9600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = TWOSTOPBITS;
	dcbSerialParams.Parity = EVENPARITY;
	dcbSerialParams.fRtsControl = RTS_CONTROL_DISABLE;

	if (!SetCommState(hSerial, &dcbSerialParams)) {
		std::cerr << "Error setting serial port state" << std::endl;
		CloseHandle(hSerial);
		return 1;
	}

	if (!PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR)) {
		std::cerr << "Error purging the serial port. Error code: " << GetLastError() << std::endl;
		CloseHandle(hSerial);
		return 1;
	}

	//Set serial port timeouts
	timeout.ReadIntervalTimeout = 60;
	timeout.ReadTotalTimeoutConstant = 60;
	timeout.ReadTotalTimeoutMultiplier = 15;
	//timeout.WriteTotalTimeoutConstant = 60;
	//timeout.WriteTotalTimeoutMultiplier = 8;

	if (!SetCommTimeouts(hSerial, &timeout)) {
		std::cerr << "Error setting serial port time-outs." << std::endl;
		CloseHandle(hSerial);
		return 1;
	}

	std::string output;

	output += "\n";
	output += "Input Voltage:";
	output += std::to_string(settingsToWrite.getInputVoltage());
	output += "\n";
	output += "Selected Test Point:";
	output += std::to_string(settingsToWrite.getSelectedTestPoint());
	output += "\n";
	output += "Selected Wave Form:";
	output += std::to_string(settingsToWrite.getSelectedWaveForm());
	output += "\n";
	output += "Total Simulation Time:";
	output += std::to_string(settingsToWrite.getSimulationTime());
	output += "\n";
	output += "Simulation Time Step:";
	output += std::to_string(settingsToWrite.getTimeStep());
	output += "\n";
	output += "Simulation Start Time:";
	output += std::to_string(settingsToWrite.getStartTime());
	output += "\n";
	output += "Simulation End Time:";
	output += std::to_string(settingsToWrite.getEndTime());
	output += "\n";

	DWORD bytesWritten;
	if (!WriteFile(hSerial, output.c_str(), output.length(), &bytesWritten, NULL)) { //TODO: Need To change Something here todo with size
		std::cerr << "Error writing to serial port." << std::endl;
		CloseHandle(hSerial);
		return 1;
	}

	//// Remove the trailing '\n' if it exists
	//if (!output.empty() && output.back() == '\n') {
	//	output.pop_back();
	//}
	std::cout << "Sent: " << output << ", Bytes written: " << bytesWritten << std::endl;

	// Receive data from the serial port
	char buffer[64]; // Adjust the buffer as needed
	DWORD bytesRead;

	// Clear the buffer before reading
	ZeroMemory(buffer, sizeof(buffer));

	if (ReadFile(hSerial, buffer, sizeof(buffer), &bytesRead, NULL)) {
		if (bytesRead > 0) {

			std::cout << "Bytes read: " << bytesRead << "	Received: " << buffer << std::endl;
		}
	}
	else {
		std::cerr << "Error reading from serial port. Error code: " << GetLastError() << std::endl;
		CloseHandle(hSerial);
		return 1;
	}

	FlushFileBuffers(hSerial);

	if (strstr(buffer, "True") != nullptr)
	{
		// Close the serial port
		CloseHandle(hSerial);

		//Need to Add Error Handling from the ESP
		return true;
	}
	else {
		// Close the serial port
		CloseHandle(hSerial);

		//Need to Add Error Handling from the ESP
		return false;
	}



}