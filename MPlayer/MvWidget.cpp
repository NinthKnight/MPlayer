#include "MvWidget.h"

MvWidget::MvWidget(QMainWindow *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_pFrame = new FrameWnd(this);
	setCentralWidget(m_pFrame);
}

MvWidget::~MvWidget()
{
}

