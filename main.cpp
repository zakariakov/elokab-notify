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
#include "elokabnotify_adaptor.h"
#include <QApplication>

#include <QtDBus/QDBusConnection>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("elokab-notify");
    app.setOrganizationName("elokab");

   QDBusConnection connection = QDBusConnection::sessionBus();

    if (!  connection.registerService("org.freedesktop.Notifications"))
    {
        qWarning() << "Unable to register 'freedesktop.Notifications' service - is another instance of freedesktop.Notifications running?";
        QDBusInterface dbus("org.freedesktop.Notifications",
                            "/org/freedesktop/Notifications",
                            "org.freedesktop.Notifications");

        if (!dbus.isValid()) {
            qDebug() << "QDBusInterface is not valid!";
            return 1;
        }

        QList<QVariant> args;
        args.append("Elokab Notification");       // Application Name
        args.append(0123U);         // Replaces ID (0U)
        args.append("information");     // Notification Icon
        args.append("Elokab Notification");          // Body
        args.append( "Unable to register 'freedesktop.Notifications' service - is another instance of freedesktop.Notifications running?");       // Summary
        args.append(QStringList()); // Actions
        args.append(QVariantMap());
        args.append(3000);
        dbus.callWithArgumentList(QDBus::NoBlock, "Notify", args);

        return 1;
    }
    ElokabNotification *elokabNotification = new ElokabNotification;

    new ElokabInterfaceAdaptor(elokabNotification);
    connection.registerObject("/org/freedesktop/Notifications", elokabNotification);

    return app.exec();
}
