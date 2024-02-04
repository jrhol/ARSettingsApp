#include "Headers/GUI.h"

//********************************************************//
//						VARIABLES
//********************************************************//

//IMGUI Variables which store current state alongside other GUI variables
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

static bool virtual_only_simulation = false;

static bool ESP_connected = false;

static bool PCB_FileLoaded = false; //TODO: Implement Checking

float inputVoltage = 0.0f;
int selectedTestPoint = 0; // Index of the selected TestPoint
int selectedWaveform = 0; // Index of the selected Waveform

// Min and max values for the Voltage input
double minVoltage = 0.0f;
double maxVoltage = 5.0f;

const char* waveforms[] = { "Sawtooth", "Square", "Triangle", "DC" };

//Simulation Parameters
double simulationTime = 0.0f;
double timeStep = 0.01f;  // Default time step
double startTime = 0.0f;
double endTime = 10.0f;

//COM port parameters
int selectedComPortIndex = 0; // Initialize with the first COM port
std::vector<std::string> comPortList; //List of available Com ports

//PCB loadedPCB;
const char* testPoints[] = { "TP1", "TP2", "TP3" };

PCBDisplaySetting pcbSettings; //List of PCB layers and if they should be displayed

settings setSettings;


//********************************************************//
//					TEMP FAKE LOADING OF PCB 
//					REPLACE WITH REAL LOADING
//********************************************************//
void populateLayerOptions() {

	//for (int i = 0; i < loadedPCB.getNumberOfLayers(); i++) {
	//	pcbSettings.LayerControl.push_back({ loadedPCB.getPCBLayerNames()[i], true });
	//}

	for (int i = 0; i < 10; i++) {

		std::string demolayer = "Layer " + std::to_string(i);
		pcbSettings.LayerControl.push_back({ demolayer , true });
	}

	//TODO:FILE READING GOES HERE

}


//********************************************************//
//					Clamp Definition (for ImGui)
//********************************************************//

// Function to clamp a value between a minimum and maximum
float clamp(float value, float min, float max) {
	return (value < min) ? min : (value > max) ? max : value;
}


//********************************************************//
//				ImGUI Create Settings Window
//********************************************************//
void createSettingsWindow() {

	populateLayerOptions();

	glutInitWindowSize(1280, 720);
	glutCreateWindow("Settings Menu");

	// Setup GLUT display function
	// We will also call ImGui_ImplGLUT_InstallFuncs() to get all the other functions installed for us,
	// otherwise it is possible to install our own functions and call the imgui_impl_glut.h functions ourselves.
	glutDisplayFunc(runSettingsWindow);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGLUT_Init();
	ImGui_ImplOpenGL2_Init();

	// Install GLUT handlers (glutReshapeFunc(), glutMotionFunc(), glutPassiveMotionFunc(), glutMouseFunc(), glutKeyboardFunc() etc.)
	// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
	// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
	// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
	// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
	ImGui_ImplGLUT_InstallFuncs();

	// Main loop
	glutMainLoop();

	// Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();
}

//********************************************************//
//				GLUT Timer Call Back
//********************************************************//

void timerCB(int millisec)
{
	glutTimerFunc(millisec, timerCB, millisec);
	glutPostRedisplay();
}

//********************************************************//
//				ImGUI Run Settings Window
//********************************************************//

void runSettingsWindow()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGLUT_NewFrame();
	ImGui::NewFrame();
	ImGuiIO& io = ImGui::GetIO();

	//Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		ImGui::Begin("System Settings");                          // Create a window called "Hello, world!" and append into it.
		ImGui::Text("Please Select What Type of Simulation you want to run");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Enable Virtual Only Simulation?", &virtual_only_simulation); // Edit bools storing our window open/close state

		ImGui::Separator(); // Add A section break
		ImGui::Spacing();

		if (virtual_only_simulation == false)
		{
			ImGui::Text("Probe Connection Setup");

			//Get Parameters From the Probe
			minVoltage = 0.0;
			maxVoltage = 5.0;
			//Also Get the possible waveforms that can be generated
			//Here

			// Create a button to refresh the list of COM ports
			if (ImGui::Button("Refresh COM Ports")) {
				// Call the function to get the COM ports
				getComPorts(comPortList);
				if (comPortList.empty())
				{
					ESP_connected = false;
				}
				else
				{
					ESP_connected = true;
				}
			}

			// Create a combo box to select a COM port
			if (ImGui::BeginCombo("Select COM Port", comPortList.empty() ? "No COM Ports" : comPortList[selectedComPortIndex].c_str())) {
				for (unsigned int i = 0; i < comPortList.size(); ++i) {
					bool isSelected = (i == selectedComPortIndex);
					if (ImGui::Selectable(comPortList[i].c_str(), isSelected)) {
						selectedComPortIndex = i; // Update the selected index when an item is selected
					}

					if (isSelected) {
						ImGui::SetItemDefaultFocus(); // Set focus to the default selected item
					}
				}
				ImGui::EndCombo();
			}
		}
		else
		{
			//Set Values reasonable for computation only
			minVoltage = -50000;
			maxVoltage = +50000;
		}

		ImGui::Separator(); // Add A section break
		ImGui::Spacing();

		{
			// Add a title for the second section
			ImGui::Text("Simulation Parameters");

			// Input box for Voltage
			ImGui::InputFloat("Voltage", &inputVoltage);
			// Display text indicating the min and max values for Voltage
			ImGui::Text("Min: %.2f  Max: %.2f", minVoltage, maxVoltage);

			// Clamp the voltage value between min and max
			inputVoltage = clamp(inputVoltage, minVoltage, maxVoltage);

			// Combo box for TestPoint
			if (ImGui::Combo("TestPoint", &selectedTestPoint, testPoints, IM_ARRAYSIZE(testPoints))) {
				// Handle TestPoint selection change if needed
			}

			// Combo box for Waveform
			if (ImGui::Combo("Waveform", &selectedWaveform, waveforms, IM_ARRAYSIZE(waveforms))) {
				// Handle Waveform selection change if needed
			}


			// Input box for Simulation Time
			ImGui::InputDouble("Simulation Time", &simulationTime);

			// Input box for Time Step
			ImGui::InputDouble("Time Step", &timeStep, 0.001f, 1.0f, "%.3f");  // Adjust the increment and precision as needed

			// Input box for Start Time
			ImGui::InputDouble("Start Time", &startTime);

			// Input box for End Time
			ImGui::InputDouble("End Time", &endTime);

			// Ensure sensible values for the parameters
			simulationTime = std::max(startTime, std::min(simulationTime, endTime));
			timeStep = std::max(0.001, timeStep);  // Minimum time step value
			endTime = std::max(startTime, endTime);  // Ensure End Time is not less than Start Time
		}



		//Debugging
		//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

		// End the System Settings window
		ImGui::End();
	}

	{
		ImGui::Begin("PCB Settings");

		// File selector for PCB file
		ImGui::Text("PCB File:");

		ImGui::SameLine();

		if (ImGui::Button("Browse...")) {
			// Implement your file selection logic here
			// You can use external libraries like ImGuiFileDialog for file selection
			// For simplicity, we are using a text input in this example

			// Code for Option 1
			OPENFILENAME ofn;
			TCHAR szFilePCBPath[MAX_PATH] = { 0 };
			// Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFile = szFilePCBPath;
			ofn.nMaxFile = sizeof(szFilePCBPath);
			ofn.lpstrFilter = L"KiCad PCB Files (*.kicad_pcb)\0*.kicad_pcb\0"; //Filter to Kicad PCB files only
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if (GetOpenFileName(&ofn) == TRUE)
			{
				// The user selected a file, and the file path is in szFileName
				// You can use szFileName for further processing
				MessageBox(NULL, szFilePCBPath, L"File Selected", MB_OK);


				//Converting Tchar to Char 
				// Calculate the size of the buffer needed
				int charCount = WideCharToMultiByte(CP_UTF8, 0, szFilePCBPath, -1, NULL, 0, NULL, NULL);

				// Allocate a buffer for the converted string
				char* charFilePath = new char[charCount];

				// Perform the conversion
				WideCharToMultiByte(CP_UTF8, 0, szFilePCBPath, -1, charFilePath, charCount, NULL, NULL);

				setSettings.setPCBFilePath(charFilePath); //Store file path in settings object
				PCB_FileLoaded = true;

				// Now you can use charFilePath for further processing or saving data to the selected file
				// Don't forget to delete[] charFilePath when done using it to avoid memory leaks
				delete[] charFilePath;
			}
		}

		// Layer selector with checkboxes
		ImGui::Text("PCB Layers:");
		for (unsigned int i = 0; i < pcbSettings.LayerControl.size(); ++i) {
			ImGui::Checkbox(pcbSettings.LayerControl[i].name.c_str(), &pcbSettings.LayerControl[i].display);
		}

		// End the PCB Settings window
		ImGui::End();
	}

	{

		ImGui::Begin("Write Settings");
		if (virtual_only_simulation)
		{
			populateSettings();

			if (ImGui::Button("Write Settings for AR Application")) {
				if (PCB_FileLoaded == false) {
					MessageBox(NULL, L"Load a PCB File before attempting to Write Settings", L"Warning", MB_OK | MB_ICONWARNING); //Alert trying to write to ESP when not connected
				}
				else {
					// Code for Saving File
					OPENFILENAME ofn;
					TCHAR szFileSettingsFilePath[MAX_PATH] = { 0 };

					// Initialize OPENFILENAME
					ZeroMemory(&ofn, sizeof(ofn));
					ofn.lStructSize = sizeof(ofn);
					ofn.hwndOwner = NULL;
					ofn.lpstrFile = szFileSettingsFilePath;
					ofn.nMaxFile = sizeof(szFileSettingsFilePath);
					ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0"; // Filter to .txt files only
					ofn.nFilterIndex = 1;
					ofn.lpstrFileTitle = NULL;
					ofn.nMaxFileTitle = 0;
					ofn.lpstrInitialDir = NULL;
					ofn.Flags = OFN_OVERWRITEPROMPT; // Use OFN_OVERWRITEPROMPT for save dialog

					if (GetSaveFileName(&ofn) == TRUE)
					{
						// Ensure the file has a .txt extension
						size_t fileNameLength = wcslen(szFileSettingsFilePath);

						// Check if the filename already has an extension
						if (fileNameLength == 0 || (szFileSettingsFilePath[fileNameLength - 1] != L'.' && _wcsicmp(szFileSettingsFilePath + fileNameLength - 4, L".txt") != 0))
						{
							// If the filename doesn't have an extension or has an extension other than .txt, add .txt
							wcscat_s(szFileSettingsFilePath, _countof(szFileSettingsFilePath), L".txt");
						}

						// The user selected a file or entered a new filename,
						// and the file path is in szFileName
						// You can use szFileName for further processing
						MessageBox(NULL, szFileSettingsFilePath, L"File Selected", MB_OK);


						// Converting Tchar to Char
						// Calculate the size of the buffer needed
						int charCount = WideCharToMultiByte(CP_UTF8, 0, szFileSettingsFilePath, -1, NULL, 0, NULL, NULL);

						// Allocate a buffer for the converted string
						char* charFilePath = new char[charCount];

						// Perform the conversion
						WideCharToMultiByte(CP_UTF8, 0, szFileSettingsFilePath, -1, charFilePath, charCount, NULL, NULL);


						if (!writeSettingsToFile(charFilePath, setSettings)) //Writing AR Settings
						{
							throw std::invalid_argument("Failed to write settings to file");
						}

						// Now you can use charFilePath for further processing or saving data to the selected file
						// Don't forget to delete[] charFilePath when done using it to avoid memory leaks
						delete[] charFilePath;
					}
				}
			}
		}
		else {
			if (ImGui::Button("Write Settings to AR Application and ESP32")) {

				if (PCB_FileLoaded == false) {
					MessageBox(NULL, L"Load a PCB File before attempting to Write Settings", L"Warning", MB_OK | MB_ICONWARNING); //Alert trying to write to ESP when not connected
				}
				else {
					if (ESP_connected == false)
					{
						MessageBox(NULL, L"Connect ESP32 or Enable Virtual Only Simulation!", L"Warning", MB_OK | MB_ICONWARNING); //Alert trying to write to ESP when not connected.
					}
					else
					{
						populateSettings(); //Refresh the Settigns

						// Code for Saving File
						OPENFILENAME ofn;
						TCHAR szFileSettingsFilePath[MAX_PATH] = { 0 };

						// Initialize OPENFILENAME
						ZeroMemory(&ofn, sizeof(ofn));
						ofn.lStructSize = sizeof(ofn);
						ofn.hwndOwner = NULL;
						ofn.lpstrFile = szFileSettingsFilePath;
						ofn.nMaxFile = sizeof(szFileSettingsFilePath);
						ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0"; // Filter to .txt files only
						ofn.nFilterIndex = 1;
						ofn.lpstrFileTitle = NULL;
						ofn.nMaxFileTitle = 0;
						ofn.lpstrInitialDir = NULL;
						ofn.Flags = OFN_OVERWRITEPROMPT; // Use OFN_OVERWRITEPROMPT for save dialog

						if (GetSaveFileName(&ofn) == TRUE)
						{
							// Ensure the file has a .txt extension
							size_t fileNameLength = wcslen(szFileSettingsFilePath);

							// Check if the filename already has an extension
							if (fileNameLength == 0 || (szFileSettingsFilePath[fileNameLength - 1] != L'.' && _wcsicmp(szFileSettingsFilePath + fileNameLength - 4, L".txt") != 0))
							{
								// If the filename doesn't have an extension or has an extension other than .txt, add .txt
								wcscat_s(szFileSettingsFilePath, _countof(szFileSettingsFilePath), L".txt");
							}

							// The user selected a file or entered a new filename,
							// and the file path is in szFileName
							// You can use szFileName for further processing
							MessageBox(NULL, szFileSettingsFilePath, L"File Selected", MB_OK);


							// Converting Tchar to Char
							// Calculate the size of the buffer needed
							int charCount = WideCharToMultiByte(CP_UTF8, 0, szFileSettingsFilePath, -1, NULL, 0, NULL, NULL);

							// Allocate a buffer for the converted string
							char* charFilePath = new char[charCount];

							// Perform the conversion
							WideCharToMultiByte(CP_UTF8, 0, szFileSettingsFilePath, -1, charFilePath, charCount, NULL, NULL);


							if (!writeSettingsToESP32(setSettings)) //Write settings to ESP32
							{
								MessageBox(NULL, L"Could not write data to ESP32, please try again", L"Warning", MB_OK | MB_ICONWARNING); //Alert when writing to ESP failed
							}

							if (!writeSettingsToFile(charFilePath, setSettings)) //Writing AR Settings
							{
								throw std::invalid_argument("Failed to write settings to file"); //Throwing exception here as likely to be more severe bug if cannot write to file
							}

							// Now you can use charFilePath for further processing or saving data to the selected file
							// Don't forget to delete[] charFilePath when done using it to avoid memory leaks
							delete[] charFilePath;
						}
					}
				}
			}
		}

		// End the PCB Settings window
		ImGui::End();
	}

	// Rendering
	ImGui::Render();
	glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
	//glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound, but prefer using the GL3+ code.
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	glutSwapBuffers();
	glutTimerFunc(16.6, timerCB, 16.6); // draw every 16.6 ms = 60FPS 
}


void populateSettings()
{
	setSettings.setVirtualOnlySimulation(virtual_only_simulation);
	setSettings.setInputVoltage(inputVoltage);
	setSettings.setSelectedTestPoint(selectedTestPoint);
	setSettings.setSelectedWaveForm(selectedWaveform);
	setSettings.setSimulationTime(simulationTime);
	setSettings.setTimeStep(timeStep);
	setSettings.setStartTime(startTime);
	setSettings.setEndTime(endTime);

	if (virtual_only_simulation == false) {
		// Convert the string to wstring
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		std::wstring wComport = converter.from_bytes(comPortList[selectedComPortIndex]);
		setSettings.setComPort(wComport);
	}
}