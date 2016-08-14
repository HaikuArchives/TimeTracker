/*
 * Copyright 200?, Michael Wulff Nielsen <Naish@worldonline.dk>
 * Copyright 2016, Humdinger <humdingerb@gmail.com>
 * All rights reserved. Distributed under the terms of the GPL license.
 */


#include <LayoutBuilder.h>

//#include "TimeTrackerApp.h"
#include "TimeTrackerWindow.h"

extern	int32 Function(void* Data);
extern	BMessage ApplicationPreferences;


TimeTrackerWindow::TimeTrackerWindow(BRect Frame)
	:
	BWindow(Frame, "TimeTracker", B_TITLED_WINDOW, B_FRAME_EVENTS |
		B_AUTO_UPDATE_SIZE_LIMITS)
{
	m_TasksSaved = false;

	BMenuBar* bar = new BMenuBar("root menu");
	BMenu* MyMenu = new BMenu("App");

	BMenuItem* item = new BMenuItem("About", new BMessage(B_ABOUT_REQUESTED));
	item->SetTarget(be_app);
	MyMenu->AddItem(item);
	MyMenu->AddSeparatorItem();
	MyMenu->AddItem(new BMenuItem("Quit", new BMessage(MENU_APP_QUIT), 'Q',
		B_COMMAND_KEY));
	bar->AddItem(MyMenu);

	MyMenu = new BMenu("Task");
	MyMenu->AddItem(new BMenuItem("Start/Stop task",
		new BMessage(MENU_TASK_START_STOP), 'S', B_COMMAND_KEY));
	MyMenu->AddItem(new BMenuItem("Reset time", new BMessage(MENU_TASK_RESET),
		0, 0));
	MyMenu->AddSeparatorItem();
	MyMenu->AddItem(new BMenuItem("New task", new BMessage(MENU_APP_NEW_TASK),
		'N',B_COMMAND_KEY));
	MyMenu->AddItem(new BMenuItem("Delete task",
		new BMessage(MENU_TASK_DELETE), 0, 0));
	bar->AddItem(MyMenu);

	m_ListView = new BListView("Tasks", B_SINGLE_SELECTION_LIST, B_WILL_DRAW);
	m_ListView->SetInvocationMessage(new BMessage(MENU_TASK_START_STOP));

	m_ScrollView = new BScrollView("ScrollTask", m_ListView,
		B_WILL_DRAW, false, true);

	m_TaskView = new NewTaskView();
	m_TaskView->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, B_SIZE_UNLIMITED));

	m_CardLayout = new BCardLayout();
	new BView("card", B_SUPPORTS_LAYOUT, m_CardLayout);

	m_CardLayout->AddView(m_ScrollView);	// item 0
	m_CardLayout->AddView(m_TaskView);		// item 1
	m_CardLayout->SetVisibleItem((int32)0);

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(bar)
		.AddGroup(B_VERTICAL)
			.SetInsets(B_USE_WINDOW_SPACING)
			.Add(m_CardLayout)
			.End();

	LoadTasks();

	m_Thread = spawn_thread(Function, "Ticker", B_NORMAL_PRIORITY,
		(void*)m_ListView);
	resume_thread(m_Thread);
}


void
TimeTrackerWindow::MessageReceived(BMessage* message)
{
	switch(message->what)
	{
	case MENU_APP_NEW_TASK:
	{
		m_Frame = Bounds();
		suspend_thread(m_Thread);	//Time doesn't go when I add a new task
		m_CardLayout->SetVisibleItem((int32)1);
		InvalidateLayout();
		break;
	}
	case MENU_APP_QUIT:
	{
		be_app->PostMessage(B_QUIT_REQUESTED);
		break;
	}
	case BUTTON_NEW_TASK_CANCEL:
	{
		m_CardLayout->SetVisibleItem((int32)0);
		InvalidateLayout();
		ResizeTo(m_Frame.Width(), m_Frame.Height());
		resume_thread(m_Thread);	//Okay back to work ;)
		break;
	}
	case BUTTON_NEW_TASK_OK:
	{
		BString	TaskName;
		TaskName.SetTo(message->FindString("Title"));	
		m_CardLayout->SetVisibleItem((int32)0);
		m_ListView->AddItem(new TaskListItem(TaskName)); //Hey a new task. DOH!
		InvalidateLayout();
		ResizeTo(m_Frame.Width(), m_Frame.Height());
		resume_thread(m_Thread);	//Okay back to work ;)
		break;
	}
	case MENU_TASK_START_STOP:
	{
		ToggleTask();
		break;
	}
	case MENU_TASK_DELETE:
	{
		int32 selection = m_ListView->CurrentSelection();
		if (selection >= 0) {
			BListItem* temp = m_ListView->RemoveItem(selection);
			delete(temp);
		}
		break;
	}
	case MENU_TASK_RESET:
	{
		int32 selection = m_ListView->CurrentSelection();
		if (selection >= 0) {
			TaskListItem* temp = (TaskListItem*)m_ListView->ItemAt(selection);
			temp->ResetTime();
			m_ListView->Invalidate();
		}
		break;
	}
	default:
		BWindow::MessageReceived(message);
		break;
	}
}


void
TimeTrackerWindow::ToggleTask()
{
	int32 selection = m_ListView->CurrentSelection();
	if (selection >= 0) {
		TaskListItem*	temp = (TaskListItem*)m_ListView->ItemAt(selection);
		temp->ToggleTaskStatus();
		m_ListView->Invalidate();
	}
}


bool
TimeTrackerWindow::QuitRequested()
{
	Lock();
	SaveTasks();
	Unlock();
	be_app->PostMessage(new BMessage(B_QUIT_REQUESTED));
	return(true);
}


void
TimeTrackerWindow::FrameMoved(BPoint origin)
{
	ApplicationPreferences.ReplaceRect("Window", Frame());
}


void
TimeTrackerWindow::FrameResized(float width, float height)
{
	ApplicationPreferences.ReplaceRect("Window", Frame());
}


void
TimeTrackerWindow::LoadTasks()
{
	BPath	MyPath;

	if (find_directory(B_USER_SETTINGS_DIRECTORY, &MyPath, false) == B_OK) {
		BString	filename(MyPath.Path());
		filename.Append("/TimeTrackerTasks");

		BFile prefsfile(filename.String(), B_READ_ONLY);
		if (prefsfile.InitCheck() == B_OK) {
			BMessage Tasks;
			Tasks.Unflatten(&prefsfile);

			int pos = 0;
			while (true)
			{
				char Buffer[255];
				sprintf(Buffer, "Task%d", pos);
				const char*	TaskName;
				TaskName = Tasks.FindString(Buffer, 0L);
				if (TaskName != NULL) {
					sprintf(Buffer, "TaskTime%d", pos);
					bigtime_t* time;
					ssize_t size;
					Tasks.FindData(Buffer, B_RAW_TYPE, (const void**)&time,
						&size);	
					sprintf(Buffer, "TaskStatus%d", pos);
					bool active = Tasks.FindBool(Buffer);

					BString Name(TaskName);
					TaskListItem* t = new TaskListItem(Name);
					t->SetStatus(active);
					t->SetTime(*time);
					m_ListView->AddItem(t);
					pos += 1;
				 } else
				 	return;
			}
		}
	}
}


void
TimeTrackerWindow::SaveTasks()
{
	BPath MyPath;

	if (find_directory(B_USER_SETTINGS_DIRECTORY,&MyPath,false) == B_OK) {
		BString	filename(MyPath.Path());
		filename.Append("/TimeTrackerTasks");
		BFile prefsfile(filename.String(), B_WRITE_ONLY | B_CREATE_FILE |
			B_ERASE_FILE);	

		BMessage Tasks;

		for (int i = 0; i < m_ListView->CountItems(); i++)
		{
			char Buffer[255];
			TaskListItem* temp = (TaskListItem*)m_ListView->ItemAt(i);

			sprintf(Buffer, "Task%d", i);
			Tasks.AddString(Buffer, temp->GetTaskName().String());
			sprintf(Buffer, "TaskTime%d",i);
			bigtime_t time = temp->GetTime();
			Tasks.AddData(Buffer, B_RAW_TYPE, (void*)&time, sizeof(bigtime_t));
			sprintf(Buffer, "TaskStatus%d", i);
			Tasks.AddBool(Buffer, temp->GetStatus());
		}		
		Tasks.Flatten(&prefsfile);
	}
}
