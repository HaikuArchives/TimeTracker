/*
 * Copyright Michael Wulff Nielsen <Naish@worldonline.dk>
 * All rights reserved. Distributed under the terms of the GPL license.
 */


#ifndef NEW_TASK_VIEW
#define NEW_TASK_VIEW

#include <Button.h>
#include <TextControl.h>
#include <View.h>

#include "Defines.h"

class NewTaskView : public BView
{
public:
			NewTaskView();

	void	AttachedToWindow();
	void	MessageReceived(BMessage* message);
private:
	BTextControl*	m_TextControl;	
	BButton*		m_Ok;
	BButton*		m_Cancel;
};

#endif
