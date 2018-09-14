#include "mplayer.h"
#include <QTableWidgetItem>
#include "MvWidget.h"


MPlayer::MPlayer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//���ñ���
	QStringList longerList = (QStringList() << QStringLiteral("����") << QStringLiteral("������") << QStringLiteral("ר����") << QStringLiteral("ʱ��"));
	ui.tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	ui.tableWidget->setHorizontalHeaderLabels(longerList);
	ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	

	//����tableWidget�Ҽ��˵�
	m_popMenu = new QMenu(ui.tableWidget);
	m_actLyc = new QAction(QStringLiteral("���"), this);
	m_actMv = new QAction("MV", this);
	m_actDownLoad = new QAction(QStringLiteral("����"), this);

	m_popMenu->addAction(m_actLyc);
	m_popMenu->addAction(m_actMv);
	m_popMenu->addAction(m_actDownLoad);

	//���������ť
	m_musicGroup = new QButtonGroup;
	m_musicGroup->addButton(ui.musicRadioButton);
	m_musicGroup->addButton(ui.mvRadioButton);
	ui.musicRadioButton->setChecked(true);

	//���������ź�-��
	//��������&mv
	connect(&m_net, SIGNAL(sig_reqSongStatus(ItemResult, SearchStatus)), this, SLOT(slot_requestSong(ItemResult, SearchStatus)));
	connect(this, SIGNAL(sig_requestMv(QString)), &m_net, SLOT(requestMv(QString)));
	connect(&m_net, SIGNAL(sig_requestMvfinished(QString)), this, SLOT(slot_showMvWidget(QString)));
	
	//ʱ��
	connect(&m_ffplayer, SIGNAL(sig_PositionChange(qint64)), this, SLOT(slot_positionChange(qint64)));

	//�˵�
	connect(ui.tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slot_LMenu(QPoint)));
	connect(m_actLyc, SIGNAL(triggered()), this, SLOT(slot_tableWidget_Lyc()));
	connect(m_actMv, SIGNAL(triggered()), this, SLOT(slot_tableWidget_Mv()));
	connect(m_actDownLoad, SIGNAL(triggered()), this, SLOT(slot_tableWidget_DownLoad()));

	//����һ����ģ̬��Mv Widget
	m_pWidget = new MvWidget(this);
	if (m_pWidget != NULL) {
		connect(&m_ffplayer, SIGNAL(sig_CurImageChange(QImage)), m_pWidget->m_pFrame, SLOT(slot_imageMV(QImage)));
	}

	m_pWidget->hide();

}



void MPlayer::slot_LMenu(QPoint pos)
{
	if (ui.tableWidget->rowCount() > 0 && ui.tableWidget->currentRow() >= 0) {
		m_popMenu->exec(QCursor::pos());
	}
}

void MPlayer::slot_tableWidget_Lyc()
{

}

void MPlayer::slot_tableWidget_Mv()
{
	if (ui.tableWidget->rowCount() > 0 && ui.tableWidget->currentRow() >= 0) {
		
		int row = ui.tableWidget->currentRow();
		QString mvHash = m_MvHashmap[row];

		//slot_doublick(m_prebgItem, 1, true); //mv
		emit sig_requestMv(mvHash);
	}
}

void MPlayer::slot_tableWidget_DownLoad()
{

}



void MPlayer::slot_play()
{
	m_ffplayer.stop();
	QString url;

	int nRow = ui.tableWidget->currentRow();
	if (nRow >= 0) {
		const ItemResult &result = GetItemByIndex(nRow);

		if (!result.strUrl.isEmpty())
		{
			m_ffplayer.setMedia(result.strUrl);
			m_ffplayer.play();
		}
	}

}

void MPlayer::slot_stop()
{
	m_ffplayer.stop();
}

void MPlayer::slot_requestSong(const ItemResult &result, SearchStatus status)
{
	if (SearchStatus::Started == status)
	{
		m_songlist.clear();
		m_hashmap.clear();
		m_MvHashmap.clear();

		int count = ui.tableWidget->rowCount();
		for (int i = 0; i<count; i++)
		{
			ui.tableWidget->removeRow(0); 
		}
		//ui.tableWidget->clear();
	}
	else if (SearchStatus::Searching == status)
	{

		int row = ui.tableWidget->rowCount();

		m_songlist.append(result);
		m_hashmap.insert(row, result.strHash);
		m_MvHashmap.insert(row, result.strMvHash);

		ui.tableWidget->insertRow(row);
		ui.tableWidget->setItem(row, 0, new QTableWidgetItem(result.strMusicName));
		ui.tableWidget->setItem(row, 1, new QTableWidgetItem(result.strSinger));
		ui.tableWidget->setItem(row, 2, new QTableWidgetItem(result.strAlbum));
		ui.tableWidget->setItem(row, 3, new QTableWidgetItem(result.strDur));
	}
	else if (SearchStatus::Finished == status)
	{
		//m_maskwid.hide();
	}
}

void MPlayer::slot_search() {

	QString strSongName = ui.lineEdit->text();

	if (strSongName.isEmpty()){
		return;
	}

	//��������
	if (ui.musicRadioButton->isChecked()) {
		m_net.requestSong(strSongName);
	}
	else {
		m_net.requestNewMv(strSongName);
	}
	
}

void  MPlayer::slot_play_music(int nRow, int nColumn)
{
	//��ȡ��Ӧ�ĸ�����url��Ȼ�󲥷ż��ɡ�
	m_ffplayer.stop();
	QString url;

	const ItemResult &result = GetItemByIndex(nRow);

	if (!result.strUrl.isEmpty())
	{
		m_ffplayer.setMedia(result.strUrl);
		m_ffplayer.play();
	}
}

void MPlayer::slot_positionChange(qint64 length)
{
	int pos = length / 1000;
	int dur = m_ffplayer.getDuration() / 1000000;

	QTime time;
	time.setHMS(0, dur / 60, dur % 60);
	QString durstr = time.toString("mm:ss");

	time.setHMS(0, pos / 60, pos % 60);
	QString posstr = time.toString("mm:ss");

	ui.beginTime->setText(posstr );
	ui.endTime->setText(durstr);

    ui.timeSlider->setRange(0, dur);
	ui.timeSlider->setValue(pos);
}

void MPlayer::closeEvent(QCloseEvent *event)
{
	m_ffplayer.stop();
}

void MPlayer::slot_showMvWidget(const QString& url)
{
	if (!url.isEmpty())
	{
		m_ffplayer.setMedia(url, true);
		m_ffplayer.play();

		m_pWidget->show();
	}

}