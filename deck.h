#ifndef DECK_H
#define DECK_H

#include <QObject>
#include <QVector>
#include "Card.h"

class Deck : public QObject {
    Q_OBJECT

public:
    explicit Deck(QObject *parent = nullptr);
    void shuffle();
    Card* dealCard();
    int currentIndex;

private:
    QVector<Card*> cards;


    void initializeDeck();
};

#endif // DECK_H
