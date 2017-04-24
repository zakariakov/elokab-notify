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

#include "elokabnotification.h"
#include <QtGui>
#include <math.h>



ElokabNotification::ElokabNotification()
{

    mGui=new NotificationGui;

}

void ElokabNotification::Notify(const QString &app_name, uint replaces_id, const QString &app_icon,
                                const QString &summary, const QString &body, const QStringList &actions,
                                const QVariantMap &hints, int expire_timeout)
{
    Q_UNUSED(replaces_id)
    Q_UNUSED(actions)
    Q_UNUSED(hints)
    if(expire_timeout<3000)
        expire_timeout=3000;

    qDebug()<<"this is a message :"<<app_name<<summary<<body<<app_icon;
    mGui->setNotify(app_name,summary,body,app_icon,  expire_timeout);
    mGui->show();
    QTimer::singleShot(10,this,SLOT(resizeGui()));

}
void ElokabNotification::resizeGui()
{
    //mGui->resize(250,85);
    mGui->adjustSize();
}

void ElokabNotification::loadIconThemes()
{
    QString icnThem=QIcon::themeName();

    QByteArray sS=qgetenv("DESKTOP_SESSION");

    if(sS=="elokab-session"){
        QSettings setting("elokab","elokabsettings");
        setting.beginGroup("Themes");
        icnThem=  setting.value("iconsTheme",QIcon::themeName()).toString();
        setting.endGroup();

    }

    if(icnThem=="hicolor"||icnThem.isEmpty()){

        QStringList failback;
        failback << "oxygen"<< "Mint-X"<< "Humanity"<< "Tango"<< "Prudence-icon"<< "elementary"<< "gnome"<<"Adwaita";

        QDir dir("/usr/share/icons/");
        foreach (QString s, failback)
        {
            if (dir.exists(s))
            {
                icnThem=s;

                break;
            }
        }
    }

    if(icnThem.isEmpty())
        icnThem="hicolor";

    qDebug()<<"icon theme :"<< icnThem;
    QIcon::setThemeName(icnThem);




}


