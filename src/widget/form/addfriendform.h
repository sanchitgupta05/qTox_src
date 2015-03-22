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

#ifndef ADDFRIENDFORM_H
#define ADDFRIENDFORM_H

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QListWidget>
#include <QHash>

namespace Ui {class MainWindow;}

class AddFriendForm : public QObject
{
    Q_OBJECT
public:
    AddFriendForm();
    ~AddFriendForm();

    void show(Ui::MainWindow &ui);
    QString getMessage() const;

signals:
    void friendRequested(const QString& friendAddress, const QString& message);

protected:
    void showWarning(const QString& message) const;

public slots:
    void onUsernameSet(const QString& userName);

private slots:
    void onSendTriggered();

private:
    QLabel headLabel, toxIdLabel, messageLabel;
    QPushButton sendButton;
    QLineEdit toxId;
    QTextEdit message;
    QVBoxLayout layout, headLayout;
    QWidget *head, *main;
};

// Create a WallForm.h for the following class TODO

class WallForm : public QObject
{
    Q_OBJECT
public:
    WallForm();
    ~WallForm();

    QString getMessage() const;
    bool checkHashedMessages(const QString &message);

signals:
    //void friendRequested(const QString& friendAddress, const QString& message);

protected:
    //void showWarning(const QString& message) const;

public slots:
    //void onUsernameSet(const QString& userName);

private slots:
    void onPostTriggered();
    void postReceivedMessages(const QString & message);
    void show(Ui::MainWindow &ui);

private:
    QLabel headLabel, wallLabel, messageLabel;
    QPushButton sendButton;
    QListWidget* wall;
    QTextEdit message;
    QVBoxLayout layout, headLayout;
    QWidget *head, *main;

    QHash<QString, int> hashMessage;
};

#endif // ADDFRIENDFORM_H
