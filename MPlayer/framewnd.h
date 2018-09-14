#pragma once

#include <QWidget>
#include "ui_framewnd.h"

class FrameWnd : public QWidget
{
	Q_OBJECT

public:
	FrameWnd(QWidget *parent = Q_NULLPTR);
	~FrameWnd();


public slots:
	void slot_imageMV(QImage);

protected:
	virtual void paintEvent(QPaintEvent*);

private:
	Ui::FrameWnd ui;
	QImage m_img;
};
