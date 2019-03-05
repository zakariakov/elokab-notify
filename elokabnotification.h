/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef ELOKABNOTIFICATION_H
#define ELOKABNOTIFICATION_H

#include "notificationgui.h"

#include <QGraphicsObject>
#include <QBrush>
/**
 * @brief The ElokabNotification class اشعارات البرامج
 */
class ElokabNotification : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief ElokabNotification تكوين الاشعارات
     */
    ElokabNotification(bool til_wm);

public Q_SLOTS:
    /**
     * @brief Notify طلب اشعار
     * @param app_name اسم التطبيق
     * @param replaces_id معرف وحيد
     * @param app_icon ايقونة البرنامج
     * @param summary العنوان
     * @param body الرسالة
     * @param actions الاوامر
     * @param hints
     * @param expire_timeout الوقت قبل غلق الاشعار
     */
    void Notify(const QString &app_name, uint replaces_id, const QString &app_icon,
                const QString &summary, const QString &body, const QStringList &actions,
                    const QVariantMap &hints, int expire_timeout);
Q_SIGNALS:
    void crashed();

protected:
private slots:
   void resizeGui();
   void loadIconThemes();
private:
NotificationGui *mGui;

};

#endif // ELOKABNOTIFICATION_H
