#include "TimeTrackerApp.h"

BMessage	ApplicationPreferences;


TimeTrackerApp::TimeTrackerApp() : BApplication("application/x-vnd.MWN-TimeTracker")
{
	LoadPrefs();
	BRect	Temp;
	Temp=ApplicationPreferences.FindRect("Window");
	
	m_Window=new TimeTrackerWindow(Temp);
	m_Window->Show();
}

bool	TimeTrackerApp::QuitRequested()
{
	m_Window->SaveTasks();
	SavePrefs();
	return(true);
}

void	TimeTrackerApp::LoadPrefs()
{
	BPath	MyPath;
	
	if(find_directory(B_USER_SETTINGS_DIRECTORY,&MyPath,false)==B_OK)
	{
			
		BString	filename(MyPath.Path());
		filename.Append("/TimeTracker");

		BFile	prefsfile(filename.String(),B_READ_ONLY);
		if(prefsfile.InitCheck()==B_OK)
		{
			ApplicationPreferences.Unflatten(&prefsfile);
		}
		else
		{
			ApplicationPreferences.MakeEmpty();
			BRect	Temp(100,100,300,250);
			ApplicationPreferences.AddRect("Window",Temp);
			SavePrefs();
		}
	}
}

void	TimeTrackerApp::SavePrefs()
{
	BPath	MyPath;
	
	if(find_directory(B_USER_SETTINGS_DIRECTORY,&MyPath,false)==B_OK)
	{
		BString	filename(MyPath.Path());
		filename.Append("/TimeTracker");
		BFile	prefsfile(filename.String(),B_WRITE_ONLY|B_CREATE_FILE|B_ERASE_FILE);	
		if(prefsfile.InitCheck()==B_OK)
		{
			ApplicationPreferences.Flatten(&prefsfile);
		}
	}

}

int main(void)
{
	TimeTrackerApp	MyApp;
	MyApp.Run();
	return(0);
}