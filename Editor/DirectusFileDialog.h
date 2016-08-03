/*
Copyright(c) 2016 Panos Karabelas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

//= INCLUDES ==================
#include <QFileDialog>
#include "DirectusAssetLoader.h"
#include "Core/Socket.h"
#include "DirectusCore.h"
//=============================

class DirectusFileDialog : public QFileDialog
{
    Q_OBJECT
public:
    explicit DirectusFileDialog(QWidget *parent = 0);
    void Initialize(QWidget* mainWindow, DirectusCore* directusCore);
    void ResetFilePath();
    bool FilePathExists();
    void LoadModel();
    void LoadScene();
    void SaveSceneAs();
    void SaveScene();

private:
    QString m_lastSceneFilePath;
    QString m_assetOperation;

    DirectusAssetLoader* m_assetLoader;
    QWidget* m_mainWindow;
    Socket* m_socket;
    DirectusCore* m_directusCore;

signals:
    void AssetLoaded();

private slots:
    void AssetLoadedSurrogate();

public slots:
    void FileDialogAccepted(QString);

};
