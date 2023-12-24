#include "Card.h"

Card::Card(int value, const QString &suit, const QString &imagePath, QObject *parent)
    : QObject(parent), value(value), suit(suit), imagePath(imagePath) {}

int Card::getValue() const {
    return value;
}

QString Card::getSuit() const {
    return suit;
}

QString Card::getImagePath() const {
    return imagePath;
}
