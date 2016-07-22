#include "NewTaskView.h"

NewTaskView::NewTaskView(BRect Frame) : BView(Frame,"NewTask",B_FOLLOW_ALL_SIDES,B_WILL_DRAW)
{
	SetViewColor(216,216,216);

	BRect	Temp=Bounds();
	
	Temp.top+=25;
	Temp.bottom=Temp.top+20;
	Temp.right-=3;
	m_TextControl=new BTextControl(Temp,"TaskName","","",NULL,B_FOLLOW_LEFT_RIGHT);
	m_TextControl->SetDivider(0);
	AddChild(m_TextControl);
	
	Temp=Bounds();
	Temp.top+=50;
	Temp.right-=60;
	Temp.left=Temp.right-50;
	Temp.bottom=Temp.top+20;
	
	m_Ok=new BButton(Temp,"Ok","Ok",new BMessage(BUTTON_NEW_TASK_OK),B_FOLLOW_RIGHT|B_FOLLOW_TOP);
	AddChild(m_Ok);

	Temp=Bounds();
	Temp.top+=50;
	Temp.right-=4;
	Temp.left=Temp.right-50;
	Temp.bottom=Temp.top+20;
	
	m_Cancel=new BButton(Temp,"Cancel","Cancel",new BMessage(BUTTON_NEW_TASK_CANCEL),B_FOLLOW_RIGHT|B_FOLLOW_TOP);
	AddChild(m_Cancel);

}

void	NewTaskView::MessageReceived(BMessage* message)
{
	switch(message->what)
	{
	case BUTTON_NEW_TASK_OK:
	{
		BMessage*	Temp=new BMessage(BUTTON_NEW_TASK_OK);
		Temp->AddString("Title",m_TextControl->Text());
		Window()->PostMessage(Temp);
	}
	break;
	case BUTTON_NEW_TASK_CANCEL:
	{
		BMessage*	Temp=new BMessage(BUTTON_NEW_TASK_CANCEL);
		Window()->PostMessage(Temp);
	}
	break;
	default:
		BView::MessageReceived(message);
		break;
	}
}


void	NewTaskView::AttachedToWindow()
{
	m_Ok->SetTarget(this);
	m_Cancel->SetTarget(this);
}

void	NewTaskView::Draw(BRect	updateRect)
{
	BPoint	P(5,18);
	DrawString("Enter taskname:",P);
	
}
