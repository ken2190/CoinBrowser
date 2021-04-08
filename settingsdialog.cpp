#include "settingsdialog.h"
#include "mainwindow.h"
#include "ui_settingsdialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QtCore>
#include <QDate>
#include </usr/local/include/QtSpell-qt5/QtSpell.hpp>
#include "string.h"
#include <QKeyEvent>
#include <QUrl>
#include <QtWidgets>


settingsDialog::settingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settingsDialog)
{
    ui->setupUi(this);
    //MainWindow main;
    int index=0;
    QStringList blacklist,cryptolist;
    QStringList exchanges={"Binance","Bittrex"}, maincoins={"BTC","ETH","USTD"};
    ui->exchanges->clear();
    ui->exchanges->addItems(exchanges);

    ui->maincoins->clear();
    cryptolist = loadsettings("cryptolist").toStringList();
    if (cryptolist.isEmpty()) cryptolist = maincoins;
    else ui->maincoins->addItems(cryptolist);
    //qDebug() << ui->exchanges->currentText();
    QString crypt = loadsettings("crypt").toString();
    ui->stake_coin->setText(crypt+" Price usd");
    ui->message->setText("");
    if (ui->exchanges->currentText()=="Binance") blacklist = loadsettings("binance_blacklist").toStringList();
    if (ui->exchanges->currentText()=="Bittrex") blacklist = loadsettings("bittrex_blacklist").toStringList();
     for(auto & a : blacklist) ui->blacklist->append(a);
     for(auto & a : cryptolist) {
         ui->maincoinslist->append(a);
         if (a==crypt) ui->maincoins->setCurrentIndex(index);
         index++;
    }

    //ui->blacklist->setText(blacklist);
    QString json_path = loadsettings("json_path").toString();
    ui->jsonpathstring->setText(json_path);
    QString cryptolistwrite = loadsettings("cryptolistwrite").toString();
    ui->cryptolistwrite->setText(cryptolistwrite);
    QString cryptolistread = loadsettings("cryptolistread").toString();
    ui->cryptolistread->setText(cryptolistread);
    QString reportPath = loadsettings("reportpath").toString();
    ui->reportPath->setText(reportPath);
    double btc_price = loadsettings("stake_coin_price").toDouble();
    ui->stake_coin_price->setValue(btc_price);
}
settingsDialog::~settingsDialog()
{


    //qDebug() << ui->blacklist->toPlainText();
    QString ex=ui->exchanges->currentText();
    QStringList blacklist=ui->blacklist->toPlainText().split("\n"),cryptolist=ui->maincoinslist->toPlainText().split("\n");
    QSettings appsettings("QTinman",appgroup);
    appsettings.beginGroup(appgroup);
    appsettings.setValue("cryptolist", QVariant::fromValue(cryptolist));
    if (ui->exchanges->currentText()=="Binance") appsettings.setValue("binance_blacklist", QVariant::fromValue(blacklist));
    if (ui->exchanges->currentText()=="Bittrex") appsettings.setValue("bittrex_blacklist", QVariant::fromValue(blacklist));

    appsettings.endGroup();

    //for(auto & a : ui->blacklist) blacklist.append(a);

    savesettings("json_path",ui->jsonpathstring->text());
    savesettings("cryptolistread",ui->cryptolistread->text());
    savesettings("cryptolistwrite",ui->cryptolistwrite->text());
    savesettings("reportpath",ui->reportPath->text());
    savesettings("crypt",ui->maincoins->currentText());
    savesettings("stake_coin_price",ui->stake_coin_price->value());

    delete ui;
}


QVariant settingsDialog::loadsettings(QString settings)
{
    QVariant returnvar;
    QSettings appsettings("QTinman",appgroup);
    appsettings.beginGroup(appgroup);
    returnvar = appsettings.value(settings);
    appsettings.endGroup();
    return returnvar;
}

void settingsDialog::savesettings(QString settings, QVariant attr)
{
    QSettings appsettings("QTinman",appgroup);
    appsettings.beginGroup(appgroup);
    appsettings.setValue(settings,QVariant::fromValue(attr));
    appsettings.endGroup();
}

void settingsDialog::on_selectJsonFile_clicked()
{
    QString json_path = QFileDialog::getExistingDirectory(this,"Select Json file from CoinMarkedCap",".");
    ui->jsonpathstring->setText(json_path);
}

void settingsDialog::on_toolcryptolistwrite_clicked()
{

    QString cryptolistpath = QFileDialog::getExistingDirectory(this,"Select outfile where pairs are stored",".");
    ui->cryptolistwrite->setText(cryptolistpath);

}

void settingsDialog::on_toolcryptolistread_clicked()
{
    QString cryptolistpath = QFileDialog::getExistingDirectory(this,"Select infile a list of pairs from exhange",".");
    ui->cryptolistread->setText(cryptolistpath);
}

void settingsDialog::on_toolReportPath_clicked()
{
    QString reportpath = QFileDialog::getExistingDirectory(this,"Select reports path",".");
    ui->reportPath->setText(reportpath);
}

void settingsDialog::on_exchanges_activated(int index)
{
    QStringList blacklist;
    //MainWindow main;
    if (ui->exchanges->currentText()=="Binance") blacklist = loadsettings("binance_blacklist").toStringList();
    if (ui->exchanges->currentText()=="Bittrex") blacklist = loadsettings("bittrex_blacklist").toStringList();
    ui->blacklist->clear();
    for(auto & a : blacklist) ui->blacklist->append(a);
}

void settingsDialog::on_pushButton_clicked()
{
    QMessageBox::about(this,"Donate","If you find this program useful please donate to.\nPaypal to jonssofh@hotmail.com\nBTC 1HJ5xJmePkfrYwixbZJaMUcXosiJhYRLbo\nADA addr1q9h424fgyqw3y0zer34myqn9lyr303nxcyvzttk8nyqmr7r0242jsgqazg79j8rtkgpxt7g8zlrxdsgcykhv0xgpk8uqh49hnw\nVET 0x136349A99A5a56617e7E7AdbE8c55a0712B0068F\nSupport is most appreciated.");
}
