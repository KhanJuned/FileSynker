# FileSynker
File Synker is an Application that lets you copy/append content of one file to all the other files that are provided by user

<h2>How to use as command line</h2>
Get the FileSynk.cpp <br>
Compile it. <br>
Add the .exe file to Environment variables where it's downloaded <br>
<br>
Example: <br>
<blockquote>
  FileSynk important.txt duplicate.txt foo.txt other.txt
</blockquote>
<br>
Copy the contens of important to duplicate, foo and other <br>
<i>Note:</i> You can only specify 10 arguments in the command line the first one is our app name the second one is the file from which data is to be read, which lefts us to specify 8 other files in which data will be written
<br>
<h2>How to Get the GUI Interface</h2>

<h2>Development Setup Requirements</h2> <br>
<ol>
  <li> <a href="https://visualstudio.microsoft.com/">Visual Studio</a> </li>
  <li> <a href="https://www.glfw.org/download.html">GLFW 64 bit binaries</a> </li>
  <li> <a href="https://github.com/ocornut/imgui/tree/tables">ImGui</a> </li>
  <li> <a href="https://github.com/eliasdaler/imgui-sfml">ImGui-SFML</a> (A binding for ImGui) </li>
  <li> <a href="https://www.sfml-dev.org/download.php">SFML</a> </li>
  <li> A <a href="https://github.com/BalazsJako/ImGuiColorTextEdit">TextEditor</a> for ImGui </li>
  <li> <a href="https://github.com/aiekick/ImGuiFileDialog#detail-view-mode">ImGui File Dialog</a></li>
  <li> <a href="https://github.com/tronkko/dirent">Dirent</a> </li>
</ol>

<i>Note: </i> The above requirements is for the GUI Interface only you can simply download the FileSync.cpp file as use it as a command line Interface <br>

<h2>Visual Studio Setup</h2>
**************************************************************************************
<h3>Visual Studio Project Properties Settings</h3>
Project Properties > C++ > General:
	$(SolutionDir)\Dependencies\include\imgui
	$(SolutionDir)\Dependencies\include\imgui-sfml
	$(SolutionDir)\Dependencies\include
	
C++ > Preprocessor: <br>
	> Preprocessor Definitions: <br>
		_CRT_SECURE_NO_WARNINGS   // Add this line <br>
<br><br>
Linker > General: <br>
	$(SolutionDir)\Dependencies\lib-vc2019;$(SolutionDir)\Dependencies\sfml-lib <br>
Linker > Input: <br>
	glfw3.lib;opengl32.lib;sfml-graphics.lib;sfml-window.lib;sfml-system.lib; <br>
************************************************************************************** <br>
<br>
<ol>
  <li>Install Visual Studio make sure you install C++ development environment, Remember to always built the project as 64 bit</li>
  <li>Make a Dependencies folder in your project and a subfolder include</li>
  <li>make a project and get GLFW connected, add the lib-vc2019 to Dependencies > include</li>
  <li>Get imgui and imgui-sfml and add it in Dependencies > include, replace the file that you get from imgui and ImGuiFileDialog from the files that are present in my repo</li>
  <li>Download SFML and add it to Dependencies > include, Add the sfml-lib to include</li>
  <li>Add the dll files to the project folder and where the .exe file will be built by visual studio</li>
  <li>Get dirent build it and add the dirent.h files in include, after building add environment variable of  Debug folder path</li>
  <li><strong>Remember to put all the .h files in include and the .cpp files as your source files</strong></li>
  <li>Get ImGuiFileDialog, Make sure you replace the file in ImGuiFileDialog from the file in my repo</li>
  <li>Get ImGui TextEditor Support refer 5</li>
  <li>Make sure to follow the guides that are in repo of imgui, SFML, TextEditor, ImGuiFileDialog</li>
</ol>
