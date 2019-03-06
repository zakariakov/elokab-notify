#include "notificationgui.h"
#include "ui_notificationgui.h"
#include <QGraphicsDropShadowEffect>
#include <QX11Info>
#include <QDebug>
#include <QSettings>
#include <QTimer>
#include <QDesktopWidget>
#define TOP_LEFT 0
#define TOP_RIGHT 1
#define BOT_LEFT 2
#define BOT_RIGHT 3
#define TOP_CENTER 4
#define LEFT_CENTER 5
#define RIGHT_CENTER 6
#define BOT_CENTER 7

//____________________________________________________________________________________
NotificationGui::NotificationGui(bool til_wm, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NotificationGui)
{
    m_til_wm=til_wm;
     ui->setupUi(this);
     ui->widgetNavig->setVisible(false);
     setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint | Qt::X11BypassWindowManagerHint);
     mTimer=new QTimer;

     m_fileWatcher=new QFileSystemWatcher;
    QSettings setting;
     m_fileWatcher->addPath(setting.fileName());
     connect(m_fileWatcher, SIGNAL(fileChanged(QString)), this, SLOT(loadSettings()));

     connect(ui->toolButtonClose,SIGNAL(clicked()),this,SLOT(hide()));
     connect(ui->toolButtonPrev,SIGNAL(clicked()),this,SLOT(prevtNotification()));
     connect(ui->toolButtonNext,SIGNAL(clicked()),this,SLOT(nextNotification()));
     connect(QApplication::desktop(), SIGNAL(workAreaResized(int)),
             this, SLOT(   realign()));

     connect(mTimer,SIGNAL(timeout()),this,SLOT(hide()));
     QGraphicsDropShadowEffect *fx = new QGraphicsDropShadowEffect();
     //--------------------------------------------------------------------
     if(this->isRightToLeft())
          fx->setOffset(-3,3);
     else
          fx->setOffset(3,3);

     fx->setBlurRadius(10);
     fx->setColor(QColor(63, 63, 63, 200));
     //   m_isCompositing=QX11Info::isCompositingManagerRunning();
     this->setAttribute(Qt::WA_TranslucentBackground,true);
     setGraphicsEffect(fx);
     loadSettings();

}

 void NotificationGui::mousePressEvent(QMouseEvent *event)
 {
     Q_UNUSED(event)
     hide();
 }
//____________________________________________________________________________________
NotificationGui::~NotificationGui()
{
    delete ui;
}

//____________________________________________________________________________________
void NotificationGui::hideEvent(QHideEvent */*event*/)
{
    listNotifys.clear();
    ui->labelAppName->clear();
    ui->labelBody->clear();
    this->resize(200,50);
    qDebug()<<"<listNotifys.count()"<<listNotifys.count();
 //   QWidget::hide();
}

//____________________________________________________________________________________
void NotificationGui::setNotify(const QString & appname,const QString & title,
                                const QString &body,const QString &iconName, int expire_timeout)
{
    mTimer->stop();
    ui->labelAppName->clear();
    ui->labelBody->clear();
    this->resize(200,50);

    ui->labelAppName->setText(title);
    ui->labelBody->setText(body);

    //ui->widgetNavig->setVisible(ui->stackedWidget->count()>1);

    QString appIcon = iconName;
    if ( appIcon.isEmpty() )
    {
        appIcon = appname;
    }

    QPixmap pix=QIcon::fromTheme(appIcon).pixmap(48);
    ui->labelIcon->setPixmap(pix);
    listNotifys.append(title+"||"+appIcon+"||"+body);
    ui->widgetNavig->setVisible(listNotifys.count()-1);
    mCurIndex=listNotifys.count()-1;
    ui->toolButtonNext->setEnabled(false);
    ui->toolButtonPrev->setEnabled(true);
    mTimeout=   expire_timeout;
    qDebug()<<"mTimeout"<<mTimeout;
    mTimer->start(expire_timeout);
    realign();

}

//____________________________________________________________________________________
void  NotificationGui::nextNotification()
{
    mCurIndex++;
    ui->toolButtonNext->setEnabled(mCurIndex<listNotifys.count()-1);

     ui->toolButtonPrev->setEnabled(mCurIndex>0);
     showtNotification();
  //  qDebug()<<mCurIndex;
}

//____________________________________________________________________________________
void  NotificationGui::prevtNotification()
{
    mCurIndex--;
       ui->toolButtonNext->setEnabled(mCurIndex<listNotifys.count()-1);
        ui->toolButtonPrev->setEnabled(mCurIndex>0);
showtNotification();
    //  qDebug()<<mCurIndex;
}

//____________________________________________________________________________________
void  NotificationGui::showtNotification()
{
    if(mCurIndex<0||mCurIndex>listNotifys.count()-1)
        return;
QString text=listNotifys.at(mCurIndex);
ui->labelAppName->setText(text.section("||",0,0));
QString appIcon=text.section("||",1,1);
ui->labelBody->setText(text.section("||",2,2));
QPixmap pix=QIcon::fromTheme(appIcon).pixmap(48);
ui->labelIcon->setPixmap(pix);

   //   qDebug()<<mCurIndex;
}
QColor listToColor(QList<QVariant>list)
{
    int red=0,green=0, bleu=0,alpha=255;
    int count=list.count();
    if(count>0)red=list.at(0).toInt();
    if(count>1)green=list.at(1).toInt();
    if(count>2)bleu=list.at(2).toInt();
    if(count>3)alpha=list.at(3).toInt();
    return QColor(red,green,bleu,alpha);
}
//____________________________________________________________________________________
void  NotificationGui::loadSettings()
{
    QSettings setting;
    setting.beginGroup("Panel-Style");

    QList<QVariant> mcolorGlobale=setting.value("colorGlobale",QList<QVariant>()<<56<<56<< 56<< 150).toList();
    QList<QVariant> mcolorFont=setting.value("colorFont",QList<QVariant>()<<255<<255<<255<<150).toList();

    QColor colorGlobale=listToColor(mcolorGlobale);
    QColor colorFont=listToColor(mcolorFont);

    bool m_styleDefault=setting.value("styleDefault",true).toBool();
    setting.endGroup();
    setting.beginGroup("Notification");
    mPosition=setting.value("position",TOP_LEFT).toInt();

    setting.endGroup();

    //------------------------------------

    QString radius;
    if(QX11Info::isCompositingManagerRunning()){
        radius="5";
        ui->verticalLayout->setContentsMargins(5,5,5,5);
    }else{
        ui->verticalLayout->setContentsMargins(1,0,1,0);
        radius="0";
    }

    //*********************popup*************************
    QString myStylePopup=QString("\n/*syle for main menu background... */"
                                 "\n QWidget #widgetBgrmenu{"
                                 "\n   background-color: rgba(%1, %2, %3,%4);"
                                 "\n   border-radius:%5px;"
                                 "\n }"
                                 "\n QWidget{\n  color: rgb(%6, %7, %8);\n }"

                                 )
            .arg(colorGlobale.red())
            .arg(colorGlobale.green())
            .arg(colorGlobale.blue())
            .arg(colorGlobale.alpha())
            .arg(radius)
            .arg(colorFont.red())
            .arg(colorFont.green())
            .arg(colorFont.blue());
//    qDebug()<<colorFont.name();
    QString styleDefault=QString(" QWidget #widgetBgrmenu{"
                                 "  background:palette(window );"
                                 "   border-radius:%1px;"
                                 "    border:1px outset palette(dark);"
                                 " }").arg(radius) ;




    if(m_styleDefault==true){
        this->setStyleSheet(styleDefault);
    }else{
        this->setStyleSheet(myStylePopup);
    }

}

//____________________________________________________________________________________
void NotificationGui::realign()
{
loadSettings();
adjustSize();
   this->resize(this->sizeHint().width(),this->sizeHint().height());
 QRect screen;
 int pScr= QApplication::desktop()->primaryScreen();
if(m_til_wm)
    screen = QApplication::desktop()->screen(pScr)->geometry();
else
    screen = QApplication::desktop()->availableGeometry(0);

   QRect rect =this->rect();
   qDebug()<<"screen rect "<<screen;
 //------------------------------------
     switch (mPosition) {
     case TOP_LEFT:
         rect.moveTo(screen.topLeft());

         break;
     case TOP_RIGHT:
         rect.moveTo(screen.right()-rect.width(),screen.top());

         break;
     case BOT_LEFT:
         rect.moveTo(screen.left(),screen.bottom()-rect.height());

         break;
     case BOT_RIGHT:
         rect.moveTo(screen.right()-rect.width(),screen.bottom()-rect.height());

        break;
     case TOP_CENTER:
         rect.moveTo((screen.width()/2)-(rect.width()/2),screen.top());

         break;
     case LEFT_CENTER:
         rect.moveTo(screen.left(),(screen.height()/2)-(rect.height()/2));

         break;
     case RIGHT_CENTER:
         rect.moveTo(screen.right()-rect.height(),(screen.height()/2)-(rect.height()/2));

         break;
     case BOT_CENTER:
         rect.moveTo((screen.width()/2)-(rect.width()/2),screen.bottom()-rect.height());

         break;
     default:
         break;
     }

     if (rect.right() > screen.right())
         rect.moveRight(screen.right());


     if (rect.bottom() > screen.bottom())
         rect.moveBottom(screen.bottom());

     if (rect.left() < screen.left())
         rect.moveLeft(screen.left());

     if (rect.top() < screen.top())
         rect.moveTop(screen.top());


     move(rect.topLeft());
     qDebug()<<"rect.topLeft()"<<rect.topLeft();

}
