#ifndef NEW_TASK_VIEW
#define NEW_TASK_VIEW

#include <View.h>
#include <TextControl.h>
#include <Button.h>

#include "Defines.h"

class NewTaskView : public BView
{
public:
	NewTaskView(BRect Frame);

	void	Draw(BRect	updateRect);	
	void	AttachedToWindow();

	void	MessageReceived(BMessage* message);
private:
	BTextControl*	m_TextControl;	
	BButton*		m_Ok;
	BButton*		m_Cancel;
};

#endif