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

#include "downloadmanager.h"

DownloadManager::DownloadManager(QObject *parent) :
    QObject(parent)
{
}

void DownloadManager::append(const QStringList &urlList)
{
    foreach (QString url, urlList)
        append(QUrl::fromEncoded(url.toLocal8Bit()));

    if (_queue.isEmpty())
        QTimer::singleShot(0, this, SIGNAL(finished()));
}

void DownloadManager::append(const QString &url)
{
    append(QUrl::fromEncoded(url.toLocal8Bit()));
}

void DownloadManager::append(const QUrl &url)
{
    if (_queue.isEmpty())
        QTimer::singleShot(0, this, SLOT(startNextDownload()));

    _queue.enqueue(url);
}

QByteArray DownloadManager::downloadedData() const
{
    QTimer::singleShot(0, this, SLOT(startNextDownload()));

    return _data;
}

void DownloadManager::downloadReady()
{
    _data = _currentDownload->readAll();
    _currentDownload->deleteLater();

    emit downloadItemFinished();
}

void DownloadManager::startNextDownload()
{
    if (_queue.isEmpty())
    {
        emit finished();
        return;
    }

    QUrl url = _queue.dequeue();

    QNetworkRequest request(url);
    _currentDownload = _manager.get(request);
    connect(_currentDownload, &QNetworkReply::finished, this, &DownloadManager::downloadReady);
}
