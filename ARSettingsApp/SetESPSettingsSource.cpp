#include "Headers/SetESPSettings.h"

bool writeSettingsToESP32(std::string comPort, bool virtual_only_simulation, float inputVoltage, int selectedTestPoint, int selectedWaveForm, double simulationTime, double timeStep, double startTime, double endTime) {

	//Struct for Data
	struct DataPacket {
		char charArray[10];
		float inputVoltage;
		int selectedWaveForm;
		double simulationTime;
		double timeStep;
		double startTime;
		double endTime;
	};


	HANDLE hSerial;
	DCB dcbSerialParams = { 0 };
	COMMTIMEOUTS timeouts = { 0 };

	//Convert string
	LPCWSTR comPortWide = std::wstring(comPort.begin(), comPort.end()).c_str();

	// Open COM port
	hSerial = CreateFile(comPortWide, GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD errMSG = GetLastError();

	if (errMSG == 2)
	{
		std::cout << "Plug in the device first...." << std::endl;
		return false;
	}

	if (errMSG == 5)
	{
		std::cout << "Something else is using the com port..." << std::endl;
		return false;
	}


	if (hSerial == INVALID_HANDLE_VALUE) {
		std::cerr << "Error opening COM port." << std::endl;
		return false;
	}

	// Set parameters for the COM port
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	if (!GetCommState(hSerial, &dcbSerialParams)) {
		std::cerr << "Error getting state." << std::endl;
		CloseHandle(hSerial);
		return false;
	}

	dcbSerialParams.BaudRate = CBR_115200; // Set your baud rate
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	dcbSerialParams.fRtsControl = RTS_CONTROL_DISABLE;

	if (!SetCommState(hSerial, &dcbSerialParams)) {
		std::cerr << "Error setting serial port state." << std::endl;
		CloseHandle(hSerial);
		return false;
	}

	// Set timeouts
	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	if (!SetCommTimeouts(hSerial, &timeouts)) {
		std::cerr << "Error setting timeouts." << std::endl;
		CloseHandle(hSerial);
		return false;
	}

	// Send doubles and ints using binary protocol
	DataPacket packet;
	// Populate char array
	strcpy_s(packet.charArray, "dataWrite");  // Make sure the string length is within the array size
	packet.inputVoltage = inputVoltage;
	packet.selectedWaveForm = selectedWaveForm;
	packet.simulationTime = simulationTime;
	packet.timeStep = timeStep;
	packet.startTime = startTime;
	packet.endTime = endTime;

	// Send binary data
	DWORD bytesWritten;
	if (!WriteFile(hSerial, &packet, sizeof(DataPacket), &bytesWritten, NULL)) {
		std::cerr << "Error writing to serial port." << std::endl;
	}
	else {
		std::cout << "Data sent successfully." << std::endl;
	}

	//Wait until we get response from ESP
	while (1)
	{

	}

	// Close COM port
	CloseHandle(hSerial);


	//Need to Add Error Handling from the ESP
	return true;

}