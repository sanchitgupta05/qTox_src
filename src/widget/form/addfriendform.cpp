/*
    Copyright (C) 2014 by Project Tox <https://tox.im>

    This file is part of qTox, a Qt-based graphical interface for Tox.

    This program is libre software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    See the COPYING file for more details.
*/

#include "addfriendform.h"

#include <QFont>
#include <QCoreApplication>
#include <QMessageBox>
#include <QErrorMessage>
#include <tox/tox.h>
#include "ui_mainwindow.h"
#include "src/nexus.h"
#include "src/core.h"
#include "src/misc/cdata.h"
#include "src/toxdns.h"
#include "src/misc/settings.h"
#include "src/widget/widget.h"

AddFriendForm::AddFriendForm()
{
    main = new QWidget(), head = new QWidget();
    QFont bold;
    bold.setBold(true);
    headLabel.setText(tr("Add Friends"));
    headLabel.setFont(bold);

    toxIdLabel.setText(tr("Tox ID","Tox ID of the person you're sending a friend request to"));
    messageLabel.setText(tr("Message","The message you send in friend requests"));
    sendButton.setText(tr("Send friend request"));

    main->setLayout(&layout);
    layout.addWidget(&toxIdLabel);
    layout.addWidget(&toxId);
    layout.addWidget(&messageLabel);
    layout.addWidget(&message);
    layout.addWidget(&sendButton);

    head->setLayout(&headLayout);
    headLayout.addWidget(&headLabel);

    connect(&sendButton, SIGNAL(clicked()), this, SLOT(onSendTriggered()));
    connect(Nexus::getCore(), &Core::usernameSet, this, &AddFriendForm::onUsernameSet);
}

AddFriendForm::~AddFriendForm()
{
    head->deleteLater();
    main->deleteLater();
}

void AddFriendForm::show(Ui::MainWindow &ui)
{
    ui.mainContent->layout()->addWidget(main);
    ui.mainHead->layout()->addWidget(head);
    main->show();
    head->show();
}

QString AddFriendForm::getMessage() const
{
    const QString msg = message.toPlainText();
    return !msg.isEmpty() ? msg : message.placeholderText();
}

void AddFriendForm::showWarning(const QString &message) const
{
    QMessageBox warning(main);
    warning.setWindowTitle("Tox");
    warning.setText(message);
    warning.setIcon(QMessageBox::Warning);
    warning.exec();
}

void AddFriendForm::onUsernameSet(const QString& username)
{
    message.setPlaceholderText(tr("%1 here! Tox me maybe?","Default message in friend requests if the field is left blank. Write something appropriate!").arg(username));
}

void AddFriendForm::onSendTriggered()
{
    QString id = toxId.text().trimmed();

    if (id.isEmpty()) {
        showWarning(tr("Please fill in a valid Tox ID","Tox ID of the friend you're sending a friend request to"));
    } else if (ToxID::isToxId(id)) {
        if (id.toUpper() == Core::getInstance()->getSelfId().toString().toUpper())
            showWarning(tr("You can't add yourself as a friend!","When trying to add your own Tox ID as friend"));
        else
            emit friendRequested(id, getMessage());
        this->toxId.clear();
        this->message.clear();
    } else {
        if (Settings::getInstance().getProxyType() != ProxyType::ptNone)
        {
            QMessageBox::StandardButton btn = QMessageBox::warning(main, "qTox", tr("qTox needs to use the Tox DNS, but can't do it through a proxy.\n\
Ignore the proxy and connect to the Internet directly?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
            if (btn != QMessageBox::Yes)
                return;
        }

        ToxID toxId = ToxDNS::resolveToxAddress(id, true);

        if (toxId.toString().isEmpty())
        {
            showWarning(tr("This Tox ID does not exist","DNS error"));
            return;
        }

        emit friendRequested(toxId.toString(), getMessage());
        this->toxId.clear();
        this->message.clear();
    }
}



// THE following files should be added to a WallForm.cpp TODO

WallForm::WallForm()
{
    main = new QWidget(), head = new QWidget();
    wall = new QListWidget;
    QFont bold;
    bold.setBold(true);
    headLabel.setText(tr("Wall"));
    headLabel.setFont(bold);

    wallLabel.setText(tr("Your Wall"));
    messageLabel.setText(tr("Message","The message you want to Post"));
    sendButton.setText(tr("Post"));

    main->setLayout(&layout);
    layout.addWidget(&wallLabel);
    layout.addWidget(wall);
    layout.addWidget(&messageLabel);
    layout.addWidget(&message);
    layout.addWidget(&sendButton);

    head->setLayout(&headLayout);
    headLayout.addWidget(&headLabel);

    connect(&sendButton, SIGNAL(clicked()), this, SLOT(onPostTriggered()));
    //connect(Nexus::getCore(), &Core::usernameSet, this, &AddFriendForm::onUsernameSet);

}

WallForm::~WallForm()
{
    head->deleteLater();
    main->deleteLater();
    delete wall;
}

void WallForm::show(Ui::MainWindow &ui)
{
    ui.mainContent->layout()->addWidget(main);
    ui.mainHead->layout()->addWidget(head);
    main->show();
    head->show();
}

void WallForm::onPostTriggered()
{
    QString msg = getMessage();

    // Post the message on the wall, UI stuff
    QListWidgetItem item;
    QColor offline(0,0,255);
    item.setText(msg);
    item.setTextColor(offline);
    wall->addItem(&item);

    qApp->processEvents();

    // Send the messages to your friends

    //traverse the friends list

        // Check if Online .. if online send the message

        // if not, then send the messages to the trusted nodes of the friend


}

QString WallForm::getMessage() const
{
    const QString msg = message.toPlainText();
    return !msg.isEmpty() ? msg : message.placeholderText();
}
