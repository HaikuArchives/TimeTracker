/*
 * Copyright 200?, Michael Wulff Nielsen <Naish@worldonline.dk>
 * Copyright 2016, Humdinger <humdingerb@gmail.com>
 * All rights reserved. Distributed under the terms of the GPL license.
 */


#ifndef TIMETRACKERWINDOW_H
#define TIMETRACKERWINDOW_H

#include <Application.h>
#include <Alert.h>
#include <CardLayout.h>
#include <ListView.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <ScrollView.h>
#include <StorageKit.h>	//Once again the sparrow gets shot!!!!
#include <View.h>
#include <Window.h>

#include <stdio.h>

#include "Defines.h"
#include "NewTaskView.h"
#include "TaskListItem.h"


class TimeTrackerWindow : public BWindow
{
public:
	TimeTrackerWindow(BRect Frame);

	void	MessageReceived(BMessage* message);
	bool	QuitRequested();

	void	FrameResized(float width, float height);
	void	FrameMoved(BPoint origin);

	void	LoadTasks();
	void	SaveTasks();
protected:
	void	ToggleTask();
private:
	BListView*		m_ListView;
	NewTaskView*	m_TaskView;
	BScrollView*	m_ScrollView;

	BCardLayout*	m_CardLayout;

	BRect			m_Frame;
	thread_id		m_Thread;

	bool			m_TasksSaved;
};

#endif
