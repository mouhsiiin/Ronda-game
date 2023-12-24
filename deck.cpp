#include "Deck.h"
#include <QRandomGenerator>

Deck::Deck(QObject *parent) : QObject(parent), currentIndex(0) {
    initializeDeck();
}



void Deck::initializeDeck() {
    // Clear existing cards
    cards.clear();

    QVector<QString> suits = {"g", "p", "w", "s"};
    QVector<QString> values = {"1", "2", "3", "4", "5", "6", "7", "10", "11", "12"};

    // Create a new deck of cards
    for (const QString &suit : suits) {
        for (const QString &value : values) {
            //generating the image path
            QString imagePath = QString(":/cards/cards/%2%1.png").arg(value.toLower(), suit.toLower());
            Card *card = new Card(values.indexOf(value) + 1, suit, imagePath, this);
            cards.append(card);
        }
    }
}

void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(cards.begin(), cards.end(), g);
    currentIndex = 0;
}

Card* Deck::dealCard() {
    if (currentIndex < cards.size()) {
        return cards[currentIndex++];
    } else {
        throw std::runtime_error("Deck is empty");
    }
}
