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

#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QQueue>

#include "downloadmanager.h"

class Downloader : public QObject
{
    Q_OBJECT
public:
    static Downloader* instance();

    Q_INVOKABLE void updateDB();
    Q_INVOKABLE void updateMissingImages();
    Q_INVOKABLE void updateImage(const QString &id);

    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)

    bool busy();

signals:
    void busyChanged();
    void downloadImgProgress(float value);
    void finished();
    void updatedImage(const QString &id);

private slots:
    void foreignSetsDownloaded();
    void foreignCardsDownloaded();
    void foreignImgDownloaded();
    void cleanUp();

private:
    explicit Downloader(QObject *parent = 0);
    void setBusy(bool busy);

    bool _busy;
    QQueue<QString> _filenames;
    QQueue<QString> _cardIds;
    DownloadManager _manager;
    int _totalImgs;
};

#endif // DOWNLOADER_H
