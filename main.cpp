//C++ Header Files
#include <iostream>
#include <vector>
#include <map>
#include <iterator>
#include <string>
//ImGui Header Files
#include <imgui.h>
#include <imgui-SFML.h>
#include <imgui_internal.h>
#include <stb_textedit.h>
//Config Header Files for a File Dialog
#include <CustomImGuiFileDialogConfig.h>
#include <ImGuiFileDialogConfig.h>
#include <ImGuiFileDialog.h>
//SFML Header Files
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
//TextEditor Header File
#include <TextEditor.h>

std::string filepathname, filepath, filename, filter, userDatas, fullpath, tempfilename;
std::string BufferData;
char* FileData;
int e = 0;
bool flag = true, RenderFlag = true, WinFlag = false, SaveFlag = false, SyncFlag = false, RenderFileDialogFlag = false;
bool EmptyFlag = false, ErrorFlag = false, ErrorWriteFlag = false, SuccessFlag = false;

const char* current_item = NULL;

std::multimap<std::string, std::string> Map;
std::multimap<std::string, std::string>::iterator itr;
const char* charptr;

std::pair<std::string, std::string> OpenFileDialog(std::string key) {
	if (igfd::ImGuiFileDialog::Instance()->FileDialog(key))
	{
		//if (igfd::ImGuiFileDialog::Instance()->IsOk == true) {}
		filepathname = igfd::ImGuiFileDialog::Instance()->GetFilePathName();
		filepath = igfd::ImGuiFileDialog::Instance()->GetCurrentPath();
		filename = igfd::ImGuiFileDialog::Instance()->GetCurrentFileName();
		filter = igfd::ImGuiFileDialog::Instance()->GetCurrentFilter();
		// here convert from string because a string was passed as a userDatas, but it can be what you want

		if (igfd::ImGuiFileDialog::Instance()->GetUserDatas())
			userDatas = std::string((const char*)igfd::ImGuiFileDialog::Instance()->GetUserDatas());
		auto selection = igfd::ImGuiFileDialog::Instance()->GetSelection(); // multiselection

		fullpath = filepathname;
		tempfilename = filename;
		for (itr = Map.begin(); itr != Map.end(); ++itr) {
			if (itr->first == tempfilename)
				flag = false;
		}
		if (flag == true && tempfilename != "")
			Map.insert(std::pair<std::string, std::string>(tempfilename, fullpath));
		else
			flag = true;

		igfd::ImGuiFileDialog::Instance()->CloseDialog(key);
	}
	return std::make_pair(filename, filepathname);
}
char* ReadFile(const char* filename) {
	static char buffer[256];
	FILE* fptr = fopen(filename, "r");

	if (!fptr) { ErrorFlag = true; }
	else {
		int fptrChars = 0;
		if (fptr != NULL) {
			while (fgetc(fptr) != EOF)
				++fptrChars;

			fseek(fptr, 0, SEEK_SET);
			for (int i = 0; i != fptrChars; ++i)
				buffer[i] = fgetc(fptr);
			fclose(fptr);
		}
	}
	return buffer;
}
void WriteFile(const char* filename, const char* data) {
	FILE* fptr = NULL;
	if (e == 101)
		fptr = fopen(filename, "a");
	else
		fptr = fopen(filename, "w");
	if (!fptr) {
		ErrorFlag = true;
		ErrorWriteFlag = true;
	}
	else {
		while ((*data)) {
			fputc(*data, fptr);
			++data;
		}
		fclose(fptr);
	}
}
std::string convertToString(char* a, int size)
{
	std::string s = "";
	for (int i = 0; i < size; i++) {
		if (a[i] != NULL)
			s = s + a[i];
	}
	return s;
}
int main()
{
	sf::RenderWindow window(sf::VideoMode(1360, 760), "");
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);

	sf::Color bgColor;

	float color[3] = { 255.f, 255.f, 255.f };

	char windowTitle[255] = "File Sync Application";
	window.setTitle(windowTitle);
	window.resetGLStates(); // call it if you only draw ImGui. Otherwise not needed.

	sf::Clock deltaClock;
	bgColor.r = static_cast<sf::Uint8>(color[0]);
	bgColor.g = static_cast<sf::Uint8>(color[1]);
	bgColor.b = static_cast<sf::Uint8>(color[2]);

	const char* filters = "Files{.txt,.c,.cpp,.h,.hpp,.java,.class,.py,.js,.htm,.html,.cs,.css,.ini,.obj,.doc,.rtf,.tex,.wpd,odt,.sh,.vb,.swift,.cgi,.pl,.bak,.cab,.cfg,.cpl,.cur,.dll,.dmp,.drv,.icns,.ico,.lnk,.msi,.sys,.tmp,.docx,.ilk,.pdb},C/C++ files(.c .cpp .h .hpp){.c,.cpp,.h,.hpp},Java (.java .class){.java,.class}";

	TextEditor texteditor;
	texteditor.SetPalette(TextEditor::GetDarkPalette());
	//texteditor.SetLanguageDefinition(TextEditor::LanguageDefinition::Plain);
	texteditor.SetTabSize(2);
	texteditor.SetReadOnly(false);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed) {
				BufferData = texteditor.GetText();
				if (BufferData.length() > 1) {
					WinFlag = true;
				}
				else
					window.close();
			}
		}
		ImGui::SFML::Update(window, deltaClock.restart());

		if (WinFlag) {
			ImGui::OpenPopup("Are you sure?");
			if (ImGui::BeginPopupModal("Are you sure?", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
			{
				ImGui::Text("Buffer is not empty\n\n");
				ImGui::Separator();

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
				ImGui::PopStyleVar();

				if (ImGui::Button("Exit", ImVec2(120, 0))) { window.close(); }
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(120, 0))) {
					ImGui::CloseCurrentPopup();
					WinFlag = false;
				}
				ImGui::EndPopup();
			}
		}

		ImGui::Begin("File Sync window");
		ImGui::BeginGroup();
		if (ImGui::Button("Open/Close text editor")) {
			if (RenderFlag) {
				RenderFlag = false;
				SyncFlag = false;
			}
			else
				RenderFlag = true;
		}
		if (RenderFlag == false) {
			BufferData = texteditor.GetText();
			if (BufferData.length() > 1) {
				ImGui::SameLine();
				if (ImGui::Button("Save")) {
					igfd::ImGuiFileDialog::Instance()->OpenDialog("SaveKey", "Choose File", filters, ".");
					if (SaveFlag)
						SaveFlag = false;
					else
						SaveFlag = true;
				}
			}
		}
		if (SaveFlag) {
			std::pair<std::string, std::string> filepair = OpenFileDialog("SaveKey");
			if (!filepair.first.empty()) {
				WriteFile(filepair.second.c_str(), texteditor.GetText().c_str());
			}
		}
		if (ImGui::Button("Open File Dialog"))
			igfd::ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", filters, ".");
		std::pair<std::string, std::string> idontcareMap = OpenFileDialog("ChooseFileDlgKey");
		ImGui::Separator();

		if (!Map.empty()) {
			itr = Map.begin();
			if (itr->first.empty())
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "No Files Synced");
			else
			{
				ImGui::Text("List of opened files:");
				for (itr = Map.begin(); itr != Map.end(); ++itr) {
					if (!itr->first.empty()) {
						charptr = itr->first.c_str();
						FILE* OpenedFiles = fopen(itr->second.c_str(), "r");
						if (OpenedFiles)
							ImGui::Text(charptr);
						fclose(OpenedFiles);
					}
				}
			}
			if (ImGui::Button("Empty List"))
				EmptyFlag = true;
		}
		else {
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "No Files Synced");
		}
		if (EmptyFlag) {
			ImGui::OpenPopup("Empty List?");
			if (ImGui::BeginPopupModal("Empty List?", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
			{
				ImGui::Text("files will not be deleted.\n\n");
				ImGui::Separator();

				if (ImGui::Button("Empty", ImVec2(120, 0))) {
					Map.clear();
					SyncFlag = false;
					EmptyFlag = false;
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(120, 0))) {
					ImGui::CloseCurrentPopup();
					EmptyFlag = false;
				}
				ImGui::EndPopup();
			}
		}
		if (Map.size() > 1) {
			ImGui::Separator();
			ImGui::Dummy(ImVec2(0.0f, 20.0f));
			if (ImGui::Button("Sync Files")) {
				if (SyncFlag)
					SyncFlag = false;
				else
					SyncFlag = true;
			}
		}
		ImGui::SameLine();
		if (Map.size() > 1) {
			if (ImGui::Button("Advanced Sync")) {}
		}
		ImGui::EndGroup();
		if (SyncFlag) {
			RenderFlag = true;

			int size = Map.size();
			std::string* items = nullptr;
			delete[] items;
			items = new std::string[size];
			for (itr = Map.begin(); itr != Map.end(); ++itr) {
				if (!itr->first.empty()) {
					for (int i = 0; i < size; ++i) {
						if (items[i] == "") {
							items[i] = itr->first.c_str();
							break;
						}
					}
				}
			}
			ImGui::Text("Read Data from: "); ImGui::SameLine();
			if (ImGui::BeginCombo("##combo", current_item)) {
				for (int n = 0; n < size; n++) {
					bool is_selected = (current_item == items[n].c_str());
					if (ImGui::Selectable(items[n].c_str(), is_selected))
						current_item = items[n].c_str();
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo(); ImGui::SameLine();
			}
			ImGui::SameLine();
			ImGui::RadioButton("Append", &e, 101); ImGui::SameLine();
			ImGui::RadioButton("OverWrite", &e, 102);

			ImGui::Dummy(ImVec2(0.0f, 20.0f));
			ImGui::Text("Read Data from the provided file and sync it to all the other files in the list");
			if (current_item) {
				ImGui::Dummy(ImVec2(0.0f, 20.0f));
				if (ImGui::Button("CONFIRM SYNC")) {
					FILE* file = fopen("DoNotDelete.txt", "w");
					for (itr = Map.begin(); itr != Map.end(); ++itr) {
						if (itr->first == current_item)
							FileData = ReadFile(itr->second.c_str());
						fputs(itr->second.c_str(), file);
						fputc('\n', file);
					}
					fclose(file);
					if (ErrorFlag == false) {
						for (itr = Map.begin(); itr != Map.end(); ++itr) {
							if (itr->first != current_item) {
								WriteFile(itr->second.c_str(), FileData);
							}
						}
					}
					if (ErrorFlag == false && ErrorWriteFlag == false)
						SuccessFlag = true;
				}
			}
		}
		if (SuccessFlag) {
			ImGui::OpenPopup("Message Logs");
			if (ImGui::BeginPopupModal("Message Logs", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
			{

				ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Success!");
				ImGui::Separator();

				if (ImGui::Button("OK", ImVec2(120, 0))) {
					SuccessFlag = false;
				}
				ImGui::EndPopup();
			}
		}
		if (ErrorFlag) {
			ImGui::OpenPopup("Error Logs");
			if (ImGui::BeginPopupModal("Error Logs", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
			{
				if (ErrorWriteFlag) {
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Error writing file");
				}
				else {
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Error reading file");
				}
				ImGui::Separator();

				if (ImGui::Button("OK", ImVec2(120, 0))) {
					ErrorFlag = false;
				}
				ImGui::EndPopup();
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			BufferData = texteditor.GetText();
			if (BufferData.length() > 1) {
				igfd::ImGuiFileDialog::Instance()->OpenDialog("NewSaveKey", "Choose File", filters, ".");
				RenderFileDialogFlag = true;
			}
		}
		if (RenderFileDialogFlag) {
			std::pair<std::string, std::string> filepair = OpenFileDialog("NewSaveKey");
			if (!filepair.first.empty()) {
				WriteFile(filepair.second.c_str(), texteditor.GetText().c_str());
			}
		}
		//ImGuiKey_
		if (!RenderFlag) {
			ImGui::SameLine();  texteditor.Render("Text Editor");
		}
		ImGui::End();
		window.clear(bgColor);
		ImGui::SFML::Render(window);
		window.display();
	}
	ImGui::SFML::Shutdown();
}
