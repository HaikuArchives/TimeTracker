/*
 * Copyright Michael Wulff Nielsen <Naish@worldonline.dk>
 * All rights reserved. Distributed under the terms of the GPL license.
 */


#include <KernelKit.h>
#include <ListView.h>
#include <Window.h>

#include <stdio.h>

#include "TaskListItem.h"

int32
Function(void* Data)
{
	BListView* ListView = (BListView*)Data;
	
	while (true)
	{
		snooze(1000000);
		for(int i = 0; i<ListView->CountItems(); i++)
		{
			TaskListItem* temp = (TaskListItem*)ListView->ItemAt(i);
			temp->TimeTick();
		}
		BWindow* temp = ListView->Window();
		temp->Lock();
		ListView->Invalidate();
		temp->Unlock();
	}
}
