/*
 * Copyright 200?, Michael Wulff Nielsen <Naish@worldonline.dk>
 * Copyright 2016-2020, Humdinger <humdingerb@gmail.com>
 * All rights reserved. Distributed under the terms of the GPL license.
 */


#include <AboutWindow.h>
#include <Alert.h>
#include <Catalog.h>

#include "TimeTrackerApp.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "TimeTrackerApp"

BMessage ApplicationPreferences;


TimeTrackerApp::TimeTrackerApp()
	:
	BApplication(AppSignature)
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
	BAboutWindow* aboutWin = new BAboutWindow(B_TRANSLATE_SYSTEM_NAME(
		"TimeTracker"), AppSignature);
	aboutWin->AddDescription(B_TRANSLATE(
		"Keep track of the time you spend on your various projects."));
	const char* extraCopyrights[] = {
		"200? Michael Wulff Nielsen",
		"2016-2020 Humdinger",
		NULL
	};
	const char* authors[] = {
		B_TRANSLATE("Michael Wulff Nielsen (original author)"),
		"Begasus",
		"Humdinger",
		NULL
	};
	aboutWin->AddCopyright(2000, "Michael Wulff Nielsen", extraCopyrights);
	aboutWin->AddAuthors(authors);
	aboutWin->Show();
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
