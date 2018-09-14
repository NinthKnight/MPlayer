#include "framewnd.h"
#include<QPainter>
#include<QDebug>


FrameWnd::FrameWnd(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

FrameWnd::~FrameWnd()
{
}

void FrameWnd::paintEvent(QPaintEvent *e)
{
	//baseWidget::paintEvent(e);
	QPainter painter(this);
	painter.setBrush(Qt::black);
	painter.drawRect(0, 0, this->width(), this->height()); //先画成黑色

	if (m_img.width() <= 0)
		return;

	///将图像按比例缩放成和窗口一样大小
	QImage img = m_img.scaled(this->size(), Qt::KeepAspectRatio);

	int x = (width() - img.width()) / 2;
	int y = (height() - img.height()) / 2;
	painter.setRenderHint(QPainter::SmoothPixmapTransform);
	painter.drawImage(x, y, img); //画出图像
}

void FrameWnd::slot_imageMV(QImage img) {
	m_img = img;
	update();
}