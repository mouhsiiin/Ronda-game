#include "ClickableLabel.h"

ClickableLabel::ClickableLabel(QWidget *parent) : QLabel(parent) {}

void ClickableLabel::mousePressEvent(QMouseEvent *event) {
    // Emit the clicked signal when the label is clicked
    emit clicked();
    QLabel::mousePressEvent(event); // Pass the event to the base class implementation
}

