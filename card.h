#ifndef CARD_H
#define CARD_H

#include <QObject>
#include <QString>

class Card : public QObject {
    Q_OBJECT

public:
    Card(int value, const QString &suit, const QString &imagePath, QObject *parent = nullptr);

    int getValue() const;
    QString getSuit() const;
    QString getImagePath() const;

private:
    int value;
    QString suit;
    QString imagePath;
};

#endif // CARD_H
