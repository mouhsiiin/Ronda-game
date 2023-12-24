#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include "Deck.h"
#include "ClickableLabel.h"
#include "Card.h"
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QEventLoop>
#include <QTimer>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QComboBox>


const int CARDHEIGHT = 150;


enum TURN{
    player,opponent
};

enum DIFFICULTY{
    easy, meduim ,hard
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event = nullptr) override;


private slots:
    void handleCardClicked();
    void startNewGame();
    void onDifficultyChanged(int index);


private:
    void setupUI();
    void updateUI();
    void newRound();
    int  lookForPoints(int CardValue, bool dele = true);
    void computerMove();
    void checkwinner();
    Card* easyAI();
    Card* meduimAI();
    Card* hardAI();
    void delay(int milliseconds, const std::function<void()>& callback);

    Deck *deck;
    TURN turn;
    DIFFICULTY diff;
    bool gameover = false;
    int playerScore = 0;
    int opponentScore = 0;
    QLabel *Scorelabel;
    QLabel *opponentScoreLabel;
    QLabel *deckLabel;
    //cards vectors
    QVector<Card*> playerHand;
    QVector<Card*> opponentHand;
    QVector<Card*> gameBoard;
    //labels of cards
    QVector<ClickableLabel*> playerLabels;
    QVector<ClickableLabel*> opponentLabels;
    QVector<ClickableLabel*> boardlabels;
    //game layouts
    QHBoxLayout *playerLayout;
    QHBoxLayout *opponentLayout;
    QGridLayout *gameBoardlayout;

    QFrame *playerFrame;
    QFrame *opponentFrame;
    QFrame *sideFrame;
    QFrame *boardFram;

    QMediaPlayer *media;
    QMediaPlayer *cardSound;


};

#endif // MAINWINDOW_H
