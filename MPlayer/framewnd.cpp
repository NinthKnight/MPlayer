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
	painter.drawRect(0, 0, this->width(), this->height()); //�Ȼ��ɺ�ɫ

	if (m_img.width() <= 0)
		return;

	///��ͼ�񰴱������ųɺʹ���һ����С
	QImage img = m_img.scaled(this->size(), Qt::KeepAspectRatio);

	int x = (width() - img.width()) / 2;
	int y = (height() - img.height()) / 2;
	painter.setRenderHint(QPainter::SmoothPixmapTransform);
	painter.drawImage(x, y, img); //����ͼ��
}

void FrameWnd::slot_imageMV(QImage img) {
	m_img = img;
	update();
}