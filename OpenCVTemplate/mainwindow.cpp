#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pdi = new PDI();
}



MainWindow::~MainWindow()
{
    delete ui;
    delete pdi;
}

void MainWindow::on_actionAbrir_Imagem_triggered()
{
    //abre uma caixa de dialogo para selecao de uma imagem
    QString filename = QFileDialog::getOpenFileName(
    this, "Open File", "/home",
    tr("Images(*.png *.xpm *.jpg)")
    );
    pdi->carregarImagem(filename.toStdString());
}

void MainWindow::on_actionHistograma_triggered()
{
    pdi->histograma();
}

void MainWindow::on_actionNegativo_triggered()
{
    pdi->negativo();
}

void MainWindow::on_actionAjuste_de_Contraste_triggered()
{
    pdi->ajusteContraste();
}

void MainWindow::on_actionTransformacao_Logaritmica_triggered()
{
    pdi->logaritmica();
}

void MainWindow::on_actionTransformacao_de_Potencia_triggered()
{
    pdi->potencia();
}

void MainWindow::on_actionEqualizacaoHistograma_triggered(){
    pdi->equalizacaoHistograma();
}

void MainWindow::on_actionMediana_triggered(){
    pdi->filtroMediana();
}

void MainWindow::on_actionGaussiana_triggered(){
    pdi->filtroGaussiano();
}

void MainWindow::on_actionMaximo_triggered(){
    pdi->filtroMaximo();
}

void MainWindow::on_actionMinimo_triggered(){
    pdi->filtroMinimo();
}

void MainWindow::on_actionMedia_triggered()
{
    pdi->filtroMedia();
}

void MainWindow::on_actionBrilho_triggered()
{
    pdi->brilhoHSV();
}

void MainWindow::on_actionMedianaColor_triggered(){
    pdi->medianaHSV();
}

void MainWindow::on_actionEqualizaColor_triggered(){
    pdi->equalizacaoHistogramaHSV();
}

void MainWindow::on_actionGaussianaColor_triggered(){
    pdi->gaussianaHSV();
}
