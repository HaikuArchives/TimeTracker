/*
 * Copyright Michael Wulff Nielsen <Naish@worldonline.dk>
 * All rights reserved. Distributed under the terms of the GPL license.
 * Contributers:
 *		Humdinger, humdingerb@gmail.com
 */

#include <InterfaceDefs.h>

#include "TaskListItem.h"

TaskListItem::TaskListItem(BString Taskname)
{
	m_TaskName = Taskname;
	m_ActiveTask = false;
	m_SpentTime = 0;
}


void
TaskListItem::ToggleTaskStatus()
{
	m_ActiveTask = !m_ActiveTask;
}


BString
TaskListItem::GetTaskName()
{
	return(m_TaskName);
}


void
TaskListItem::TimeTick()
{
	if (m_ActiveTask)
		m_SpentTime += 1;
}


bigtime_t
TaskListItem::GetTime()
{
	return(m_SpentTime);
}


void
TaskListItem::DrawItem(BView* owner, BRect itemRect,
	bool drawEverything = false)
{
	// colors
	rgb_color background = tint_color(ui_color(B_LIST_BACKGROUND_COLOR), 1.08);
	rgb_color text = tint_color(ui_color(B_LIST_ITEM_TEXT_COLOR), 0.8);

	if (m_ActiveTask) {
		text = ui_color(B_SUCCESS_COLOR);
		background = ui_color(B_LIST_BACKGROUND_COLOR);
	}
	if (IsSelected()) {
		background = ui_color(B_LIST_SELECTED_BACKGROUND_COLOR);
		text = ui_color(B_LIST_SELECTED_ITEM_TEXT_COLOR);
		if (m_ActiveTask)
			text = ui_color(B_SUCCESS_COLOR);
	} else if (drawEverything)
		background = ui_color(B_LIST_BACKGROUND_COLOR);

	owner->SetHighColor(background);
	owner->FillRect(itemRect);

	owner->SetLowColor(background);
	owner->SetHighColor(text);

	// text
	BFont font;
	font.SetFace(B_REGULAR_FACE);
	owner->SetFont(&font);
	font_height	fheight;
	font.GetHeight(&fheight);

	BString	temp = m_TaskName;
	temp.Append(" - ");
	temp.Append(GetStringTime());

	owner->DrawString(temp.String(), BPoint(4,
		itemRect.top + fheight.ascent + fheight.descent + fheight.leading));

	// draw lines
	owner->SetHighColor(tint_color(ui_color(B_CONTROL_BACKGROUND_COLOR),
		B_DARKEN_2_TINT));
	owner->StrokeLine(itemRect.LeftBottom(), itemRect.RightBottom());
}


void
TaskListItem::SetStatus(bool state)
{
	m_ActiveTask = state;
}


void
TaskListItem::SetTime(bigtime_t time)
{
	m_SpentTime = time;
}


void
TaskListItem::ResetTime()
{
	m_SpentTime = 0;
}


bool
TaskListItem::GetStatus()
{
	return(m_ActiveTask);
}


BString
TaskListItem::GetStringTime()
{
	char buffer[255];

	int	Day = 0;
	int Hours = 0;
	int Minutes = 0;
	int Seconds = 0;
	
	Seconds = m_SpentTime % 60;
	Minutes = (m_SpentTime / 60) % 60;
	Hours = (m_SpentTime / 60 / 60) % 24;
	Day = (m_SpentTime / 60 / 60 / 24);

	sprintf(buffer, "%d days, %d:%d:%d", Day, Hours, Minutes, Seconds);

	BString temp;
	temp.SetTo(buffer);
	return(temp);
}


void
TaskListItem::Update(BView* owner, const BFont* font)
{
	BListItem::Update(owner, font);

	font_height	fheight;
	font->GetHeight(&fheight);

	SetHeight(ceilf(fheight.ascent + 2 + fheight.leading / 2
		+ fheight.descent) + 5);
}
