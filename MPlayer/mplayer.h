#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mplayer.h"
#include "PlayCore\FFmpegPlayer.h"
#include "netWork\mynetwork.h"

class MPlayer : public QMainWindow
{
	Q_OBJECT

public:
	MPlayer(QWidget *parent = Q_NULLPTR);

	const ItemResult& GetItemByIndex(int index) { return m_songlist.at(index); }

protected:
	void closeEvent(QCloseEvent *event);

public slots:
	void slot_stop();
	void slot_play();
	void slot_search();
	void slot_requestSong(const ItemResult&, SearchStatus);
	void slot_positionChange(qint64);

	void  slot_play_music(int, int);

	void slot_LMenu(QPoint pos);
	void slot_tableWidget_Lyc();
	void slot_tableWidget_Mv();
	void slot_tableWidget_DownLoad();


private:
	Ui::MPlayerClass ui;

	FFmpegPlayer m_ffplayer;
	MyNetWork m_net;

	QList<ItemResult> m_songlist;
	QMap<int, QString> m_hashmap;

	QMenu* m_popMenu;
	QAction* m_actLyc; //∏Ë¥ 
	QAction* m_actMv; //mv
	QAction* m_actDownLoad; //œ¬‘ÿ
};
