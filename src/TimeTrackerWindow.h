#ifndef TIMETRACKERWINDOW_H
#define TIMETRACKERWINDOW_H

#include <Window.h>
#include <View.h>
#include <ListView.h>
#include <ScrollView.h>
#include <MenuBar.h>
#include <Menu.h>
#include <MenuItem.h>
#include <Application.h>
#include <Alert.h>
#include <stdio.h>
#include <StorageKit.h>	//Once again the sparrow gets shot!!!!
#include <stdio.h>

#include "Defines.h"
#include "TaskListItem.h"
#include "NewTaskView.h"

class TimeTrackerWindow : public BWindow
{
public:
	TimeTrackerWindow(BRect Frame);

	void	MessageReceived(BMessage *message);
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

	thread_id	m_Thread;

	bool	m_TasksSaved;
};

#endif