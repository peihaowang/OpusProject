#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QMediaPlayer;
class QMediaPlaylist;
class ScribbleArea;

class MainWindow : public QMainWindow
{

public:

    MainWindow();

protected:

    enum Emotions{
        Joy = 0
        , Sad
        , Tranquil
        , Angry
        , Count
    };

    ScribbleArea*       m_scribbleArea;
    QMediaPlayer*       m_player;
    QMediaPlaylist*     m_playlists[Emotions::Count];

};

#endif // MAINWINDOW_H
