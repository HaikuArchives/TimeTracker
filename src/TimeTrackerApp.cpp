/*
 * Copyright 200?, Michael Wulff Nielsen <Naish@worldonline.dk>
 * Copyright 2016, Humdinger <humdingerb@gmail.com>
 * All rights reserved. Distributed under the terms of the GPL license.
 */


#include <Alert.h>
#include "TimeTrackerApp.h"

BMessage ApplicationPreferences;


TimeTrackerApp::TimeTrackerApp()
	:
	BApplication("application/x-vnd.MWN-TimeTracker")
{
	LoadPrefs();
	BRect Temp;
	Temp = ApplicationPreferences.FindRect("Window");

	m_Window = new TimeTrackerWindow(Temp);
	m_Window->Show();
}


bool
TimeTrackerApp::QuitRequested()
{
	m_Window->SaveTasks();
	SavePrefs();
	return(true);
}


void
TimeTrackerApp::AboutRequested()
{
	BAlert *alert = new BAlert("about",
		"TimeTracker v0.2\n"
		"\toriginally by Michael Wulff Nielsen,\n"
		"\tCopyright 200?\n\n"
		"\tadapted and improved for Haiku\n"
		"\tby Humdinger, Copyright 2016\n\n"
		"Keep track of the time you spend on your various projects.",
		"Thank you");

	BTextView *view = alert->TextView();
	BFont font;
	view->SetStylable(true);
	view->GetFont(&font);
	font.SetSize(font.Size() + 4);
	font.SetFace(B_BOLD_FACE);
	view->SetFontAndColor(0, 11, &font);
	alert->Go();
}


void
TimeTrackerApp::LoadPrefs()
{
	BPath MyPath;

	if (find_directory(B_USER_SETTINGS_DIRECTORY,&MyPath,false) == B_OK) {
		BString	filename(MyPath.Path());
		filename.Append("/TimeTracker");

		BFile prefsfile(filename.String(),B_READ_ONLY);
		if (prefsfile.InitCheck() == B_OK)
			ApplicationPreferences.Unflatten(&prefsfile);
		else {
			ApplicationPreferences.MakeEmpty();
			BRect Temp(100, 100, 300, 250);
			ApplicationPreferences.AddRect("Window", Temp);
			SavePrefs();
		}
	}
}


void
TimeTrackerApp::SavePrefs()
{
	BPath MyPath;

	if (find_directory(B_USER_SETTINGS_DIRECTORY,&MyPath,false) == B_OK) {
		BString	filename(MyPath.Path());
		filename.Append("/TimeTracker");
		BFile prefsfile(filename.String(), B_WRITE_ONLY | B_CREATE_FILE |
			B_ERASE_FILE);
		if (prefsfile.InitCheck() == B_OK)
			ApplicationPreferences.Flatten(&prefsfile);
	}
}


int
main()
{
	TimeTrackerApp MyApp;
	MyApp.Run();
	return(0);
}
