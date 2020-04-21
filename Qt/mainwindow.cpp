#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QByteArray>
#include <QTimer>
#include <QDesktopServices>

#include <QApplication>
#include <QtCharts>
#include <QChartView>
#include <QPieSlice>
#include <QPieSeries>


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    man = new QNetworkAccessManager(this);
    QUrl uni("http://www.ivl.disco.unimib.it/minisites/cpp/List_of_Universal_artists.txt");
    reply_u = man->get(QNetworkRequest(uni));
    connect(reply_u, SIGNAL(readyRead()), this, SLOT(httpReadyRead_u()));
    connect(reply_u, SIGNAL(finished()), this, SLOT(downloadFinished_Uni()));

    QUrl emi("http://www.ivl.disco.unimib.it/minisites/cpp/List_of_EMI_artists.txt");
    reply_e = man->get(QNetworkRequest(emi));
    connect(reply_e, SIGNAL(readyRead()), this, SLOT(httpReadyRead_e()));
    connect(reply_e, SIGNAL(finished()), this, SLOT(downloadFinished_Emi()));
}

MainWindow::~MainWindow() {
    delete ui;
    man->deleteLater();
}

void MainWindow::httpReadyRead_u(){
    text_u += QString(reply_u->readAll());

}

void MainWindow::httpReadyRead_e(){
    text_e += QString(reply_e->readAll());

}



void MainWindow::downloadFinished_Uni(){

    if(reply_u->error()){
        QMessageBox replyError;
        replyError.setText("<font color=\"red\">url errato</font>");
        replyError.exec();
    }else{

        QRegExp rx("\"");
        QStringList split = text_u.split(rx);

        for(int i=0; i<27; ++i)
            alph_uni << 0;

        uni_count= 0;
        //creazione dei link e delle label e creazione della lista contenente i numeri per ogni lettera
        for(QStringList::iterator i = (split.begin()+1); i!= split.end(); i+=2){
            QString f = *(i+1);
            f = f.trimmed().toLower();
            uni_count++;
            QString u = *i;
            if(u.at(0) == 'h'){

                int b = ((int)f[0].toLatin1() -97);
                if(b>=0 && b<26)
                    alph_uni[b]++;
                else alph_uni[alph_uni.size()-1]++;

               l_u << new QLabel("<a href=\"" + u +"\">"+*(i+1)+"</a>");

            }else if(!(f == "")){

                int b = ((int)f[0].toLatin1() -97);
                if(b>=0 && b<26)
                    alph_uni[b]++;
                else alph_uni[alph_uni.size()-1]++;

                l_u << new QLabel("<a href=\"https://it.wikipedia.org"+u+"\">"+*(i+1)+"</a>");


            }else{

                alph_uni[alph_uni.size()-1]++;
                l_u << new QLabel("???");
            }
        }

        split.clear();

        for(int i=0; i<l_u.size(); ++i){
            l_u[i]->setTextFormat(Qt::RichText);
            l_u[i]->setTextInteractionFlags(Qt::TextBrowserInteraction);
            l_u[i]->setOpenExternalLinks(true);

            QListWidgetItem *item = new QListWidgetItem("");
            item->setSizeHint(QSize(0,25));

            ui->listWidget_Uni->addItem(item);
            ui->listWidget_Uni->setItemWidget(item, l_u[i]);
        }

        bar_chart_uni();
        pi_chart();

        reply_u->deleteLater();
  }
}

void MainWindow::downloadFinished_Emi(){
    if(reply_e->error()){
        QMessageBox replyError;
        replyError.setText("<font color=\"red\">url errato</font>");
        replyError.exec();
    }else{
        QRegExp rx("\"");
        QStringList split = text_e.split(rx);

        for(int i=0; i<27; ++i)
            alph_emi << 0;

        emi_count = 0;
        //creazione dei link e delle label e creazione della lista contenente i numeri per ogni lettera
        for(QStringList::iterator i = (split.begin()+1); i!= split.end(); i+=2){
            emi_count++;
            QString f = *(i+1);
            f = f.trimmed().toLower();
            QString u = *i;
            if(u != "" && u.at(0) == 'h'){
                int b = ((int)f[0].toLatin1() -97);
                if(b>=0 && b<26)
                    alph_emi[b]++;
                else alph_emi[alph_emi.size()-1]++;

                l_e << new QLabel("<a href=\"" + u +"\">"+*(i+1)+"</a>");


            }else if(f != ""){
                int b = ((int)f[0].toLatin1() -97);
                if(b>=0 && b<26)
                    alph_emi[b]++;
                else alph_emi[alph_emi.size()-1]++;

                l_e << new QLabel("<a href=\"https://it.wikipedia.org"+u+"\">"+*(i+1)+"</a>");;

            }else{
                alph_emi[alph_emi.size()-1]++;

                l_e << new QLabel("???");

            }
        }

        for(int i=0; i<l_e.size(); ++i){
            l_e[i]->setTextFormat(Qt::RichText);
            l_e[i]->setTextInteractionFlags(Qt::TextBrowserInteraction);
            l_e[i]->setOpenExternalLinks(true);

            QListWidgetItem *item = new QListWidgetItem("");
            item->setSizeHint(QSize(0,25));

            ui->listWidget_Emi->addItem(item);
            ui->listWidget_Emi->setItemWidget(item, l_e[i]);


        }
        split.clear();

        bar_chart_emi();

        //delete della reply e del manager
        reply_e->deleteLater();
    }
}

void MainWindow::bar_chart_uni(){

    QBarSet *set0 = new QBarSet("Numero artisti per lettera");
    for(int i=0; i<alph_uni.size(); ++i)
        *set0 << alph_uni[i];

    QBarSeries *series = new QBarSeries();
    series->setLabelsVisible(true);
    series->append(set0);
    series->setBarWidth(0.9);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Numero artisti Emi per lettera");
    chart->setAnimationOptions(QChart::AllAnimations);

    QStringList categories;
    categories << "a" << "b" << "c" << "d" << "e" << "f" << "g" << "h" << "i" << "j" << "k" << "l" << "m" << "n" << "o" << "p"
                  << "q" << "r" << "s" << "t" << "u" << "v" << "w" << "x" << "y" << "z";

    categories << "others";
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    QFont f("Times", 9);
    axis->setLabelsFont(f);
    chart->addAxis(axis, Qt::AlignBottom);
    series->attachAxis(axis);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0,100);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->hide();

    ui->graphicsView_u->setChart(chart);
}

void MainWindow::bar_chart_emi(){
    QBarSet *set0 = new QBarSet("Numero artisti per lettera");
    for(int i=0; i<alph_emi.size(); ++i)
        *set0 << alph_emi[i];

    QBarSeries *series = new QBarSeries();
    series->setLabelsVisible(true);
    series->append(set0);
    series->setBarWidth(0.9);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Numero artisti Emi per lettera");
    chart->setAnimationOptions(QChart::AllAnimations);

    QStringList categories;
    categories << "a" << "b" << "c" << "d" << "e" << "f" << "g" << "h" << "i" << "j" << "k" << "l" << "m" << "n" << "o" << "p"
                  << "q" << "r" << "s" << "t" << "u" << "v" << "w" << "x" << "y" << "z";

    categories << "others";
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    QFont f("Times", 9);
    axis->setLabelsFont(f);
    chart->addAxis(axis, Qt::AlignBottom);
    series->attachAxis(axis);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0,100);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->hide();

    ui->graphicsView_e->setChart(chart);
}

void MainWindow::pi_chart(){
    QPieSeries *series = new QPieSeries();
    QString u = "Universal: " + QString::number(uni_count);
    QString e = "Emi: " + QString::number(emi_count);
    series->append(u, uni_count);
    series->append(e, emi_count);

    series->setLabelsVisible();

    QChart *chart = new QChart();
    chart->setTheme(QChart::ChartThemeLight);
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->addSeries(series);
    chart->setTitle("Numero di artisti pre etichetta");
    chart->legend()->hide();

    ui->graphicsView->setChart(chart);

}

