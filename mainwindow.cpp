#include "MainWindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), deck(nullptr) {
    //backgroud music

    media = new QMediaPlayer;
    QAudioOutput* out = new QAudioOutput;
    media->setAudioOutput(out);
    media->setSource(QUrl("qrc:/soundeffcts/audio/touch-and-sound-113676.mp3"));
    out->setVolume(.005);
    media->play();

    //playing card sound

    cardSound = new QMediaPlayer;
    QAudioOutput* effect = new QAudioOutput;
    cardSound->setAudioOutput(effect);
    cardSound->setSource(QUrl("qrc:/soundeffcts/audio/flipcard-91468 (mp3cut.net).mp3"));
    effect->setVolume(.35);

    qDebug()<<media->mediaStatus();
    setupUI();
    startNewGame();


}

void MainWindow::setupUI() {

    //initialisation of layouts and labels
    playerLayout = new QHBoxLayout;
    opponentLayout = new QHBoxLayout;
    gameBoardlayout = new QGridLayout;
    Scorelabel = new QLabel;
    deckLabel = new QLabel;
    opponentScoreLabel = new QLabel;

    // Create QFrames for styling
    playerFrame = new QFrame(this);
    playerFrame->setLayout(playerLayout);

    opponentFrame = new QFrame(this);
    opponentFrame->setLayout(opponentLayout);

    sideFrame = new QFrame(this);
    QVBoxLayout *sideLayout = new QVBoxLayout;

    boardFram = new QFrame(this);
    boardFram->setLayout(gameBoardlayout);

    sideLayout->addWidget(deckLabel);
    sideFrame->setLayout(sideLayout);



    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(playerLayout);

    //drop down for difficultty
    QComboBox* difficultyComboBox = new QComboBox(this);
    difficultyComboBox->addItem("Easy");
    difficultyComboBox->addItem("Medium");
    difficultyComboBox->addItem("Hard");
    connect(difficultyComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onDifficultyChanged(int)));



    QPushButton *newGameButton = new QPushButton("New Game", this);
    connect(newGameButton, &QPushButton::clicked, this, &MainWindow::startNewGame);

    QSpacerItem* spacer = new QSpacerItem(20, 200, QSizePolicy::Fixed, QSizePolicy::Minimum);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setContentsMargins(30,40,0,0);
    mainLayout->addWidget(newGameButton,0,0);
    mainLayout->addWidget(difficultyComboBox,1,0);
    mainLayout->addItem(spacer,0,0);


    mainLayout->addWidget(opponentFrame,0,1,2,3);
    mainLayout->addWidget(opponentScoreLabel,0,4,2,1);

    mainLayout->addWidget(boardFram,2,1,1,3);
    mainLayout->addWidget(sideFrame,2,0);

    mainLayout->addWidget(Scorelabel,3,4);
    mainLayout->addWidget(centralWidget,3,1,1,3);


    QWidget *mainWidget = new QWidget(this);
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
    // Set style for QFrames
    //playerFrame->setStyleSheet("QFrame { background-color: lightblue; border: 2px solid navy; }");
    //opponentFrame->setStyleSheet("background-color: lightpink; border: 2px solid maroon;height: 500px;");
    sideFrame->setStyleSheet("QFrame { background-color: rgba(0,0,255,0); }");
    boardFram->setStyleSheet("background-color: lightgray; border-radius: 10%");
    Scorelabel->setStyleSheet("color: #fff;"
                              "font-size: 35px;");
    opponentScoreLabel->setStyleSheet("color: #fff;"
                                      "font-size: 35px;");
    mainWidget->setStyleSheet("background-color : #175519;");
    newGameButton->setStyleSheet("QPushButton{background-color: white; font-size: 25px; border-radius: 5% ; height: 50px;}"
                                 "QPushButton:hover { background-color: #17a510; border : 1px solid white; }"
                                 "QPushButton:pressed { background-color: #177710; }");

    difficultyComboBox->setStyleSheet("QComboBox, QAbstractItemView {background-color: white; font-size: 25px; border-radius: 5% ; height: 50px;}"
                                      "QComboBox::down-arrow{background-color: white; font-size: 25px; border-radius: 10% ;}");

    sideFrame->setFixedHeight(400);



}

void MainWindow::startNewGame() {

    // Clean up existing cards
    playerHand.clear();
    opponentHand.clear();
    playerLabels.clear();
    opponentLabels.clear();
    gameBoard.clear();
    boardlabels.clear();
    //reset the score and turn
    playerScore = 0;
    opponentScore = 0;
    turn = TURN::player;

    gameover = false;


    // Create a new deck
    if (deck) {
        delete deck;
    }
    deck = new Deck(this);
    deck->shuffle();

    // Deal new cards
    for (int i = 0; i<3 ;i++)
    {
        Card *playerCard = deck->dealCard();
        Card *opponentCard = deck->dealCard();

        playerHand.append(playerCard);
        opponentHand.append(opponentCard);
    }
    for (int i = 0; i < 4; ++i) {
        try{

            Card *card = deck->dealCard();

            gameBoard.append(card);

        }catch(std::runtime_error){
            qDebug() << "deck is empty";
            // backCard->clear();
        }
    }

    // Update the UI
    updateUI();

}

void MainWindow::handleCardClicked() {
    ClickableLabel *clickedLabel = qobject_cast<ClickableLabel*>(sender());
    if (clickedLabel && turn == TURN::player) {

        //play sound
        cardSound->play();

        int indexOfClickedCard = playerLabels.indexOf(clickedLabel);
        int result = lookForPoints(playerHand[indexOfClickedCard]->getValue(),1);
        if (result == 0){
            gameBoard.append(playerHand[indexOfClickedCard]);
        }
        playerScore += result;

        playerHand.removeAt(indexOfClickedCard);

        // check for new round
        if (opponentHand.isEmpty() && playerHand.isEmpty())
            try{
                newRound();
            }catch(std::runtime_error){
                qDebug() << "deck is empty";
                if (turn == TURN::player){
                    playerScore += gameBoard.size();
                }else if(turn == TURN::opponent){
                    opponentScore += gameBoard.size();
                }
                gameover = true;
                foreach (Card* card, gameBoard) {
                    playerHand.append(card);
                    gameBoard.remove(gameBoard.indexOf(card));
                }

            }

        updateUI();
        delay(500,[=](){
            checkwinner();
        });
        turn = TURN::opponent;
        if (opponentFrame)      //check if computer hand in not empty
            delay(600,[=](){
                 computerMove();
            });


    }
}

void MainWindow::updateUI() {
    // Clear existing layouts
    playerLabels.clear();
    opponentLabels.clear();
    boardlabels.clear();


    QLayoutItem *child;
    while ((child = playerLayout->takeAt(0)) != nullptr) {
        child->widget()->deleteLater();
        delete child;
    }

    while ((child = opponentLayout->takeAt(0)) != nullptr) {
        child->widget()->deleteLater();
        delete child;
    }

    while ((child = gameBoardlayout->takeAt(0)) != nullptr) {
        child->widget()->deleteLater();
        delete child;
    }


    // Update the player layout
    for (int i = 0; i < playerHand.size(); ++i) {
        ClickableLabel *playerLabel = new ClickableLabel(this);
        QPixmap cardPixmap = QPixmap(playerHand[i]->getImagePath()).scaledToHeight(CARDHEIGHT);
        playerLabel->setPixmap(cardPixmap);
        connect(playerLabel, &ClickableLabel::clicked, this, &MainWindow::handleCardClicked);
        playerLabel->setFixedWidth(cardPixmap.width());
        playerLayout->addWidget(playerLabel);

        playerLabels.append(playerLabel);
    }
    //update opponent layout
    qDebug() << opponentHand.size();
    for (int i = 0; i < opponentHand.size(); ++i) {
        ClickableLabel *opponentLabel = new ClickableLabel(this);
        // back of the card image for the opponenet (copmputer)
        QPixmap cardPixmap = QPixmap(":/cards/cards/back_card.png").scaledToHeight(CARDHEIGHT);
        opponentLabel->setPixmap(cardPixmap);
        opponentLabel->setFixedWidth(cardPixmap.width());
        opponentLayout->addWidget(opponentLabel);

        opponentLabels.append(opponentLabel);
    }
    // update the board layout
    int row=0,col=0;
    foreach (Card* card, gameBoard) {

        ClickableLabel* cardLabel = new ClickableLabel(this);

        QPixmap cardPixmap = QPixmap(card->getImagePath()).scaledToHeight(CARDHEIGHT);
        cardLabel->setPixmap(cardPixmap);
        cardLabel->setFixedWidth(cardPixmap.width());

        if(col != 4){
            gameBoardlayout->addWidget(cardLabel,row,col++);
        }else {
            col = 0;
            row++;
            gameBoardlayout->addWidget(cardLabel,row,col++);
        }
        boardlabels.append(cardLabel);


    }

    //update scores
    QString score =  QString::number(playerScore);
    Scorelabel->setText("Your Score : " + score);
    opponentScoreLabel->setText("Opponent \n Score : " + QString::number(opponentScore));


    resizeEvent();

}

void MainWindow::newRound(){
    for (int i = 0; i < 3; ++i) {
            Card *playerCard = deck->dealCard();
            Card *opponentCard = deck->dealCard();

            playerHand.append(playerCard);
            opponentHand.append(opponentCard);
    }

}

int MainWindow::lookForPoints(int CardValue, bool delet){
    int score = 0;
    foreach (Card* card, gameBoard) {

        if (card->getValue() == CardValue){
            if (delet)
                gameBoard.removeAt(gameBoard.indexOf(card));
            score++;
            updateUI();
            score = score + lookForPoints(CardValue + 1);

        }
    }

    return score;
}

void MainWindow::computerMove(){
    if(turn != opponent){
        return;
    }

    //play as the assigned difficulty
    qDebug()<<"the diff: "<<diff;
    Card* AIchoice = opponentHand.back();
    switch (diff) {
    case easy:
        AIchoice = easyAI();
        break;
    case meduim:
        AIchoice = meduimAI();
        break;
    case hard:
        AIchoice = hardAI();
        break;
    }

    int result = lookForPoints(AIchoice->getValue(),1);
    qDebug()<<result<<"  :ress";
    if (result == 0){
        gameBoard.append(AIchoice);
    }
    opponentScore += result;
    opponentHand.removeAt(opponentHand.indexOf(AIchoice));

    cardSound->play();
    updateUI();

    //check if the game is over
    delay(500,[=](){
        checkwinner();
    });

    //check for new round
    if (opponentHand.isEmpty() && playerHand.isEmpty()){
        try{
        newRound();
        }catch(std::runtime_error){
            qDebug() << "deck is empty";
            if (turn == TURN::player){
                playerScore += gameBoard.size();
            }else if(turn == TURN::opponent){
                opponentScore += gameBoard.size();
            }
            gameover = true;
            foreach (Card* card, gameBoard) {
                gameBoard.removeAt(gameBoard.indexOf(card));
            }

        }
    delay(500,[=](){
        updateUI();
    });
    }
    //switch the turn to player
    turn = TURN::player;

}

Card* MainWindow::easyAI(){

    srand(time(0));

    int index = rand()  % opponentHand.size();
    qDebug()<< "easy :"<<index;
    return opponentHand[index];
}

Card* MainWindow::meduimAI(){

    foreach (Card* card1, opponentHand){
        foreach (Card* card2, gameBoard) {
            if (card1->getValue() == card2->getValue())
                return card1;
        }
    }
    return easyAI();
}

Card* MainWindow::hardAI(){
    int maxPoints = 0;
    Card* chosenCard = nullptr;
    foreach (Card* card, opponentHand) {
        int points = lookForPoints(card->getValue(),0);
        if (maxPoints < points){
            maxPoints = points;
            chosenCard = card;
        }
    }
    if (chosenCard)
        return chosenCard;
    return meduimAI();
}

void MainWindow::checkwinner(){
    if (gameover){
        //missa
        if (turn == TURN::player){
            playerScore += gameBoard.size();
        }else if(turn == TURN::opponent){
            opponentScore += gameBoard.size();
        }
        gameBoard.clear();
        delay(2000,[=](){
            updateUI();
        });

        if(opponentScore > playerScore){
            QMessageBox::information(this,"Game over","YOU LOST!!   ");
        }else if(opponentScore < playerScore){
            QMessageBox::information(this,"Game over","YOU WON!!   ");

        }else {
            QMessageBox::information(this,"Game over","it's a tie");

        }
        startNewGame();


    }
}

void MainWindow::delay(int milliseconds, const std::function<void()>& callback)
{
    QTimer::singleShot(milliseconds, this, callback);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);

    // Calculate the new size for the cards based on the window size
    int newCardHeight = static_cast<int>(height() * 0.23);

    // Update the size of existing cards in player hand
    for (ClickableLabel *playerLabel : playerLabels) {
        QPixmap cardPixmap = QPixmap(playerHand[playerLabels.indexOf(playerLabel)]->getImagePath()).scaledToHeight(newCardHeight);
        playerLabel->setPixmap(cardPixmap);
        playerLabel->setFixedWidth(cardPixmap.width());
    }

    // Update the size of existing cards in game board
    int row = 0, col = 0;
    foreach (ClickableLabel *cardLabel , boardlabels) {
        QPixmap cardPixmap = QPixmap(gameBoard[boardlabels.indexOf(cardLabel)]->getImagePath()).scaledToHeight(newCardHeight*.8);
        cardLabel->setPixmap(cardPixmap);
        cardLabel->setFixedWidth(cardPixmap.width());
        gameBoardlayout->addWidget(cardLabel, row, col++);

        if (col == 4) {
            col = 0;
            row++;
        }
    }

    // Update the size of existing cards in opponent hand
    for (ClickableLabel *opponentLabel : opponentLabels) {
        QPixmap cardPixmap = QPixmap(":/cards/cards/back_card.png").scaledToHeight(newCardHeight);
        opponentLabel->setPixmap(cardPixmap);
        opponentLabel->setFixedWidth(cardPixmap.width());
    }

    if(deck->currentIndex != 40){
    QPixmap cardPixmap = QPixmap(":/cards/cards/back_card.png").scaledToHeight(newCardHeight*1.1);
    deckLabel->setPixmap(cardPixmap);
    deckLabel->setFixedWidth(cardPixmap.width());
    }else {
        deckLabel->clear();
    }

}

void MainWindow::onDifficultyChanged(int index) {

    switch (index) {
    case 0:
        diff = DIFFICULTY::easy;
        break;
    case 1:
        diff = DIFFICULTY::meduim;
        break;
    case 2:
        diff = DIFFICULTY::hard;
        break;
    default:
        qDebug() << "difficulty celection error";
        diff = DIFFICULTY::meduim;
        break;
    }
    qDebug() << "Selected difficulty: " << diff;
    startNewGame();


}
