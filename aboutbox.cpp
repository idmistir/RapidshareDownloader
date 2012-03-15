#include "aboutbox.h"

AboutBox::AboutBox(QWidget *parent) :
    QDialog(parent)
{
    lbText = new QLabel(this);
    btAboutQt = new QPushButton(this);
    btOk = new QPushButton(this);
    layout = new QBoxLayout(QBoxLayout::TopToBottom);

    lbText->setText("Created by:\n" \
                    "Dimitris Preketes\n" \
                    "d.preketes@gmail.com\n\n" \
                    "Inspired by the ideas and requests of:\n" \
                    "Theodore Kypriotis\n");

    btAboutQt->setText("About Qt");
    btOk->setText("OK");
    btOk->setDefault(true);

    layout->addWidget(lbText, 1);
    layout->addWidget(btAboutQt);
    layout->addWidget(btOk);

    this->setLayout(layout);

    connect(btAboutQt, SIGNAL(clicked()), this, SLOT(aboutQt()));
    connect(btOk, SIGNAL(clicked()), this, SLOT(accept()));

    this->setWindowTitle("About");
}

void AboutBox::aboutQt( void ) {
    QApplication::aboutQt();
}
