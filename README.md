# FileSynker
File Synker is an Application that lets you copy content of one file to all the other files that are provided by user

<h2>Requirements for GUI Interface:</h2> <br>
<ol>
<li> Visual Studio </li>
<li> GLFW connected to your Visual Studio project </li>
<li> ImGui </li>
<li> ImGui-SFML (A binding for ImGui) </li>
<li> SFML </li>
<li> A TextEditor.h File </li>
</ol>

<i>Note: </i> The above requirements is for the GUI Interface only you can simply download the FileSync.cpp file as use it as a command line Interface <br>

<h2>How to use as a command</h2>
Get the FileSync.cpp <br>
Add it to Environment variables where it's downloaded <br>
<br>
<blockquote>
FileSync important.txt duplicate.txt foo.txt other.txt
</blockquote>
<br>
Copy the contens of important to duplicate, foo and other <br>
<i>Note:</i> You can only specify 10 arguments in the command line the first one is our app name the second one is the file from which data is to be read, which lefts us to specify 8 other files in which data will be written
