#include "TaskListItem.h"

TaskListItem::TaskListItem(BString Taskname)
{
	m_View=NULL;
	m_Font=NULL;
	m_TaskName=Taskname;
	m_ActiveTask=false;
	m_SpentTime=0;
}

void	TaskListItem::ToggleTaskStatus()
{
	m_ActiveTask=!m_ActiveTask;
}

BString	TaskListItem::GetTaskName()
{
	return(m_TaskName);
}

void	TaskListItem::TimeTick()
{
	if(m_ActiveTask)
		m_SpentTime+=1;
}

bigtime_t	TaskListItem::GetTime()
{
	return(m_SpentTime);
}

void	TaskListItem::DrawItem(BView *owner,BRect itemRect,bool drawEverything=false)
{
	rgb_color	selected={255,255,255};

	if(IsSelected())
	{
		selected.red=240;
		selected.green=240;
		selected.blue=240;
		owner->SetHighColor(selected);
		owner->FillRect(itemRect);
	}
	else if(drawEverything)
	{
		selected.red=255;
		selected.green=255;
		selected.blue=255;
		owner->SetHighColor(selected);
		owner->FillRect(itemRect);
	}
	
	rgb_color	Text={255,0,0};

	if(m_ActiveTask)
	{
		Text.red=0;
		Text.green=0;
		Text.blue=0;
	}
	
	owner->SetLowColor(selected);
	owner->SetHighColor(Text);

	owner->MovePenTo(itemRect.left+4,itemRect.bottom-2);
	
	BString	temp=m_TaskName;
	temp.Append(" - ");
	temp.Append(GetStringTime());
	
	owner->DrawString(temp.String());
}

void		TaskListItem::SetStatus(bool state)
{
	m_ActiveTask=state;
}

void		TaskListItem::SetTime(bigtime_t time)
{
	m_SpentTime=time;
}

void		TaskListItem::ResetTime()
{
	m_SpentTime=0;
}

bool	TaskListItem::GetStatus()
{
	return(m_ActiveTask);
}

BString		TaskListItem::GetStringTime()
{
	char buffer[255];

	int	Day=0;
	int Hours=0;
	int Minutes=0;
	int Seconds=0;
	
	Seconds=m_SpentTime%60;
	Minutes=(m_SpentTime/60)%60;
	Hours=(m_SpentTime/60/60)%24;
	Day=(m_SpentTime/60/60/24);

	sprintf(buffer,"%d days, %d:%d:%d",Day,Hours,Minutes,Seconds);
	
	BString temp;
	temp.SetTo(buffer);
	return(temp);
}

void	TaskListItem::Update(BView *owner, const BFont *font)
{
	m_View=owner;
	m_Font=font;
	BListItem::Update(owner,font);
}