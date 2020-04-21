#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDesktopServices>
#include <QLabel>
#include <QListWidgetItem>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void bar_chart_uni();
    void bar_chart_emi();
    void pi_chart();

private slots:
    //slot per il segnale readyRead()
    void httpReadyRead_u();
    //slot per il segnale readyRead()
    void httpReadyRead_e();
    //slot per il segnale finished()
    void downloadFinished_Uni();
    //slot per il segnale finished()
    void downloadFinished_Emi();

private:
    Ui::MainWindow *ui;

    QNetworkAccessManager *man;

    QNetworkReply *reply_u;
    QNetworkReply *reply_e;

    QString text_u;
    QString text_e;

    QList<QLabel*> l_u;
    QList<QLabel*> l_e;

    QList<int> alph_uni;
    QList<int> alph_emi;

    int emi_count;
    int uni_count;

};
#endif // MAINWINDOW_H
