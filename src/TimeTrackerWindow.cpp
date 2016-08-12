/*
 * Copyright Michael Wulff Nielsen <Naish@worldonline.dk>
 * All rights reserved. Distributed under the terms of the GPL license.
 */


#include "TimeTrackerWindow.h"

extern	int32 Function(void* Data);
extern	BMessage ApplicationPreferences;

TimeTrackerWindow::TimeTrackerWindow(BRect Frame)
	:
	BWindow(Frame, "TimeTracker V0.1", B_TITLED_WINDOW, B_FRAME_EVENTS)
{
	m_TasksSaved = false;

	BRect temp;
	temp = Bounds();
	temp.bottom = temp.top + 19;
	BMenuBar* bar = new BMenuBar(temp, "root menu");

	BMenu* MyMenu = new BMenu("File");
	
	MyMenu->AddItem(new BMenuItem("New Task", new BMessage(MENU_FILE_NEW_TASK),
		'N',B_COMMAND_KEY));
	MyMenu->AddSeparatorItem();
	MyMenu->AddItem(new BMenuItem("About", new BMessage(MENU_FILE_ABOUT), 'A',
		B_COMMAND_KEY));
	MyMenu->AddSeparatorItem();
	MyMenu->AddItem(new BMenuItem("Quit", new BMessage(MENU_FILE_QUIT), 'Q',
		B_COMMAND_KEY));
	bar->AddItem(MyMenu);

	MyMenu = new BMenu("Task");
	MyMenu->AddItem(new BMenuItem("Start/Stop Task",
		new BMessage(MENU_TASK_START_STOP), 'S', B_COMMAND_KEY));
	MyMenu->AddItem(new BMenuItem("Reset time", new BMessage(MENU_TASK_RESET),
		0, 0));
	MyMenu->AddSeparatorItem();
	MyMenu->AddItem(new BMenuItem("Delete Task",
		new BMessage(MENU_TASK_DELETE), 0, 0));
	bar->AddItem(MyMenu);
	
	
	BRect temp1 = Bounds();
	temp1.InsetBy(2, 2);
	temp1.top += 20;
	temp1.right -= B_V_SCROLL_BAR_WIDTH;
	
	m_ListView = new BListView(temp1, "Tasks", B_SINGLE_SELECTION_LIST,
		B_FOLLOW_ALL_SIDES);
	m_ListView->SetInvocationMessage(new BMessage(MENU_TASK_START_STOP));
	temp1 = Bounds();
	temp1.top += 20;

	m_TaskView = new NewTaskView(temp1);
 
	m_ScrollView = new BScrollView("ScrollTask", m_ListView,
		B_FOLLOW_ALL_SIDES, B_FULL_UPDATE_ON_RESIZE, false, true);
	AddChild(m_ScrollView);

	AddChild(bar);
	
	LoadTasks();
	
	m_Thread = spawn_thread(Function,"Ticker", B_NORMAL_PRIORITY,
		(void*)m_ListView);
	resume_thread(m_Thread);
}


void
TimeTrackerWindow::MessageReceived(BMessage* message)
{
	switch(message->what)
	{
	case MENU_FILE_ABOUT:
	{
		BAlert*	T = new BAlert("Information",
		"Time Tracker V1.0\n"
		"By Michael Wulff Nielsen\n"
		"This program is released under the Gnu Public License\n"
		"For information about this application write to Naish@worldonline.dk",
		"okay");
		T->Go();
	}
	break;
	case MENU_FILE_NEW_TASK:
	{
		suspend_thread(m_Thread);	//Time doesn't go when I add a new task
		RemoveChild(m_ScrollView);
		AddChild(m_TaskView);
		m_TaskView->ResizeTo(Bounds().Width(), (Bounds().Height()) - 20);
	}
	break;
	case MENU_FILE_QUIT:
		be_app->PostMessage(B_QUIT_REQUESTED);
	break;

	case BUTTON_NEW_TASK_CANCEL:
	{
		RemoveChild(m_TaskView);
		AddChild(m_ScrollView);
		m_ScrollView->ResizeTo(Bounds().Width(), (Bounds().Height()) - 20);
		resume_thread(m_Thread);	//Okay back to work ;)
	}
	break;
	case BUTTON_NEW_TASK_OK:
	{
		BString	TaskName;
		TaskName.SetTo(message->FindString("Title"));	
		RemoveChild(m_TaskView);
		AddChild(m_ScrollView);
		m_ListView->AddItem(new TaskListItem(TaskName)); //Hey a new task. DOH!
		m_ScrollView->ResizeTo(Bounds().Width(), (Bounds().Height()) - 20);
		resume_thread(m_Thread);	//Okay back to work ;)
	}
	break;
	
	case MENU_TASK_START_STOP:
		ToggleTask();
	break;
	case MENU_TASK_DELETE:
	{
		int32 selection = m_ListView->CurrentSelection();
		if (selection >= 0) {
			BListItem* temp = m_ListView->RemoveItem(selection);
			delete(temp);
		}
	}
	break;
	case MENU_TASK_RESET:
	{
		int32 selection = m_ListView->CurrentSelection();
		if (selection >= 0)
		{
			TaskListItem* temp = (TaskListItem*)m_ListView->ItemAt(selection);
			temp->ResetTime();
			m_ListView->Invalidate();
		}
	}
	break;
	
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
	
	if (find_directory(B_USER_SETTINGS_DIRECTORY,&MyPath,false) == B_OK) {
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
