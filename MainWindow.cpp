#include <QPushButton>
#include <QMediaPlaylist>
#include <QMediaPlayer>

#include "ScribbleArea.h"
#include "MainWindow.h"

MainWindow::MainWindow()
{
//    m_scribbleArea = new ScribbleArea(this);
//    QMainWindow::setCentralWidget(m_scribbleArea);

    for(int i = 0; i < Emotions::Count; i++){
        QMediaPlaylist* playlist = new QMediaPlaylist(this);
        playlist->addMedia(QUrl("qrc:/sounds/recall.mp3"));
        playlist->setPlaybackMode(QMediaPlaylist::Loop);
        m_playlists[i] = playlist;
    }

    m_player = new QMediaPlayer(this);
    m_player->setPlaylist(m_playlists[0]);
//    m_player->play();
}
