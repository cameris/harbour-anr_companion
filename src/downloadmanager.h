/*
 * Copyright (C) 2014 cameris
 *
 * This file is part of ANR Companion.
 *
 * ANR Companion is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ANR Companion is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ANR Companion.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QByteArray>
#include <QUrl>
#include <QStringList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QQueue>
#include <QTimer>

class DownloadManager : public QObject
{
    Q_OBJECT
public:
    explicit DownloadManager(QObject *parent = 0);

    void append(const QStringList &urlList);
    void append(const QString &url);
    void append(const QUrl &url);

    QByteArray downloadedData() const;

signals:
    void downloadItemFinished();
    void finished();

private slots:
    void downloadReady();
    void startNextDownload();

private:
    QNetworkAccessManager _manager;
    QNetworkReply* _currentDownload;
    QQueue<QUrl> _queue;
    QByteArray _data;

};

#endif // DOWNLOADMANAGER_H
