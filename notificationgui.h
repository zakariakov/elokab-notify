#ifndef NOTIFICATIONGUI_H
#define NOTIFICATIONGUI_H

#include <QWidget>
#include <QTimer>
#include <QFileSystemWatcher>
#include <QSettings>


namespace Ui {
class NotificationGui;
}
/**
 * @brief The NotificationGui class نافذة لعرض الاشعارات
 */
class NotificationGui : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief NotificationGui تكوين نافذة الاشعارات
     * @param parent
     */
    explicit NotificationGui(bool til_wm,QWidget *parent = nullptr);
    ~NotificationGui();

   void mousePressEvent(QMouseEvent *event);
protected:
    /**
     * @brief hideEvent عند اخفاء النافذة
     */
    void hideEvent(QHideEvent *);
    /**
     * @brief enterEvent عند ادخال مؤشر الفأرة
     * يتم توقيف المؤقت حتى لا تختفي النافذة
     */
    void enterEvent(QEvent * /*event*/)
    {
        if (mTimer)
            mTimer->stop();
    }
    /**
     * @brief leaveEvent عند اخراج المؤشر يتم استعادة المؤقت
     */
    void leaveEvent(QEvent * /*event*/)
    {
        if (mTimer)
            mTimer->start(mTimeout);
    }
    /**
     * @brief resizeEvent عند تغيير حجم النافذة
     * @param event اشارة التحجيم
     */
    void resizeEvent(QResizeEvent *event)
    {
        QWidget::resizeEvent(event);
        realign();
    }



public slots:
    /**
     * @brief setNotify تلقي امر العرض
     * @param appname اسم البرنامج
     * @param title العنوان
     * @param body الرسالة
     * @param iconName اسم الايقونة
     * @param expire_timeout الوقت
     */
    void setNotify(const QString & appname, const QString & title, const QString &body,
                   const QString &iconName, int expire_timeout);

private slots:
    /**
     * @brief loadSettings تحميل الاعدادات
     */
    void  loadSettings();
    /**
       * @brief realign تموقع النافذة
       */
      void realign();
      /**
       * @brief nextNotification الاشعار التالي
       */
      void nextNotification();
      /**
       * @brief prevtNotification الاشعار السابق
       */
      void prevtNotification();
      /**
       * @brief showtNotification اظهار النافذة
       */
      void showtNotification();
private:
    Ui::NotificationGui *ui;

    /*!< قائمة بالرسائل في حالة تلقي عدة رسائل في نفس الوقت */
     QStringList listNotifys;

    QTimer *mTimer;

    int mTimeout;

    int mPosition;
    /**
     * @brief m_fileWatcher مراقب تغير ملف الاعدادات
     */
    QFileSystemWatcher *m_fileWatcher;



    int mCurIndex;

    bool m_til_wm=false;
};

#endif // NOTIFICATIONGUI_H
