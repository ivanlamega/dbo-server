#include "MainForm.h"

#include <Windows.h>

using namespace DataEditor;

[STAThreadAttribute]
int main()
{
	try
	{
		Application::EnableVisualStyles();
		Application::SetCompatibleTextRenderingDefault(false);
		Application::Run(gcnew MainForm());
	}
	catch (Exception^ ex)
	{
		throw gcnew System::Exception(gcnew System::String(ex->Message));
	}
	return 0;
}