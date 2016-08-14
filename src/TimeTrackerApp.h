/*
 * Copyright 200?, Michael Wulff Nielsen <Naish@worldonline.dk>
 * Copyright 2016, Humdinger <humdingerb@gmail.com>
 * All rights reserved. Distributed under the terms of the GPL license.
 */


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

	void	AboutRequested();
	bool	QuitRequested();
private:
	TimeTrackerWindow*	m_Window;
};

#endif
