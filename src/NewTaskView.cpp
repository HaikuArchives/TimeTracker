/*
 * Copyright Michael Wulff Nielsen <Naish@worldonline.dk>
 * All rights reserved. Distributed under the terms of the GPL license.
 * Contributers:
 *		Humdinger, humdingerb@gmail.com
 */


#include <ControlLook.h>
#include <LayoutBuilder.h>
#include <Window.h>

#include "NewTaskView.h"


NewTaskView::NewTaskView()
	:
	BView("NewTask", B_WILL_DRAW)
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	m_TextControl = new BTextControl("TaskName", "Enter task name:", "",
		NULL);
	m_Ok = new BButton("OK", "OK", new BMessage(BUTTON_NEW_TASK_OK));
	m_Cancel = new BButton("Cancel", "Cancel",
		new BMessage(BUTTON_NEW_TASK_CANCEL));

	static const float spacing = be_control_look->DefaultItemSpacing();
	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(spacing, spacing, spacing, spacing)
		.Add(m_TextControl)
		.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.Add(m_Ok)
			.Add(m_Cancel)
			.End()
		.AddGlue();
}


void
NewTaskView::MessageReceived(BMessage* message)
{
	switch(message->what)
	{
	case BUTTON_NEW_TASK_OK:
	{
		BMessage* Temp;
		if (m_TextControl->Text() == "")
			Temp = new BMessage(BUTTON_NEW_TASK_OK);
		else {
			Temp = new BMessage(BUTTON_NEW_TASK_CANCEL);
			Temp->AddString("Title", m_TextControl->Text());
		}
		Window()->PostMessage(Temp);
		break;
	}
	case BUTTON_NEW_TASK_CANCEL:
	{
		BMessage* Temp = new BMessage(BUTTON_NEW_TASK_CANCEL);
		Window()->PostMessage(Temp);
		break;
	}
	default:
		BView::MessageReceived(message);
		break;
	}
}


void
NewTaskView::AttachedToWindow()
{
	m_Ok->SetTarget(this);
	m_Cancel->SetTarget(this);
}
