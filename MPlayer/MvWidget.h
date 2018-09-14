#pragma once

#include <QWidget>
#include "ui_MvWidget.h"
#include "framewnd.h"

class MvWidget : public QMainWindow
{
	Q_OBJECT

public:
	MvWidget(QMainWindow *parent = Q_NULLPTR);
	~MvWidget();


	//”√”⁄ªÊ÷∆mv
	FrameWnd* m_pFrame;

private:
	Ui::MvWidget ui;
	


};
