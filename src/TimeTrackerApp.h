#ifndef TIMETRACKERAPP_H
#define TIMETRACKERAPP_H

#include <Application.h>
#include <StorageKit.h>		//We shoot a sparrow with a canon ;)
#include "TimeTrackerWindow.h"	

class TimeTrackerApp : public BApplication
{
public:
	TimeTrackerApp();

	void	LoadPrefs();
	void	SavePrefs();
	
	bool	QuitRequested();
private:
	TimeTrackerWindow*	m_Window;
};

#endif