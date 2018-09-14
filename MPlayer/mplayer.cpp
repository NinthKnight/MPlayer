#include "mplayer.h"
#include <QTableWidgetItem>


MPlayer::MPlayer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//设置标题
	QStringList longerList = (QStringList() << QStringLiteral("歌名") << QStringLiteral("歌手名") << QStringLiteral("专辑名") << QStringLiteral("时长"));
	ui.tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	ui.tableWidget->setHorizontalHeaderLabels(longerList);
	ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	

	//设置tableWidget右键菜单
	m_popMenu = new QMenu(ui.tableWidget);
	m_actLyc = new QAction(QStringLiteral("歌词"), this);
	m_actMv = new QAction("MV", this);
	m_actDownLoad = new QAction(QStringLiteral("下载"), this);

	m_popMenu->addAction(m_actLyc);
	m_popMenu->addAction(m_actMv);
	m_popMenu->addAction(m_actDownLoad);

	//添加搜索按钮
	m_musicGroup = new QButtonGroup;
	m_musicGroup->addButton(ui.musicRadioButton);
	m_musicGroup->addButton(ui.mvRadioButton);
	ui.musicRadioButton->setChecked(true);

	//关联各种信号-槽
	//播放音乐&mv
	connect(&m_net, SIGNAL(sig_reqSongStatus(ItemResult, SearchStatus)), this, SLOT(slot_requestSong(ItemResult, SearchStatus)));
	connect(this, SIGNAL(sig_requestMv(QString)), &m_net, SLOT(requestMv(QString)));
	connect(&m_net, SIGNAL(sig_requestMvfinished(QString)), this, SLOT(slot_showMvWidget(QString)));
	
	//时长
	connect(&m_ffplayer, SIGNAL(sig_PositionChange(qint64)), this, SLOT(slot_positionChange(qint64)));

	//菜单
	connect(ui.tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slot_LMenu(QPoint)));
	connect(m_actLyc, SIGNAL(triggered()), this, SLOT(slot_tableWidget_Lyc()));
	connect(m_actMv, SIGNAL(triggered()), this, SLOT(slot_tableWidget_Mv()));
	connect(m_actDownLoad, SIGNAL(triggered()), this, SLOT(slot_tableWidget_DownLoad()));

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

	//搜索音乐
	if (ui.musicRadioButton->isChecked()) {
		m_net.requestSong(strSongName);
	}
	else {
		m_net.requestNewMv(strSongName);
	}
	
}

void  MPlayer::slot_play_music(int nRow, int nColumn)
{
	//获取对应的歌曲的url，然后播放即可。
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