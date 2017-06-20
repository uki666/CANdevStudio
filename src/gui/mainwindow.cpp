#include <QtWidgets/QMdiArea>
#include <QtWidgets/QTableView>
#include <QtGui/QStandardItemModel>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QPushButton>

#include "mainwindow.h"
#include "candevice/candevice.h"
#include "canrawview/canrawview.h"
#include "cansignalview/cansignalview.h"
#include "canrawsender/canrawsender.h"
#include "cansignalsender/cansignalsender.cpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mdi(std::make_unique<QMdiArea>()),
    canDevice(std::make_unique<CanDevice>()),
    canRawView(std::make_unique<CanRawView>()),
    canSignalView(std::make_unique<CanSignalView>()),
    canRawSender(std::make_unique<CanRawSender>()),
    canSignalSender(std::make_unique<CanSignalSender>())
{
    mdi->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdi->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    setCentralWidget(mdi.get());

    canRawView->setWindowTitle("Can Raw View");
    mdi->addSubWindow(canRawView.get());
    canSignalView->setWindowTitle("Can Signal View");
    mdi->addSubWindow(canSignalView.get());
    canSignalSender->setWindowTitle("Can Signal Sender");
    mdi->addSubWindow(canSignalSender.get());
    canRawSender->setWindowTitle("Can Raw Sender");
    mdi->addSubWindow(canRawSender.get());
    mdi->tileSubWindows();

    connect(canDevice.get(), &CanDevice::frameReceived, canRawView.get(), &CanRawView::frameReceived);
    connect(canDevice.get(), &CanDevice::frameSent, canRawView.get(), &CanRawView::frameSent);

    connect(canRawSender.get(), &CanRawSender::sendFrame, canDevice.get(), &CanDevice::sendFrame);

    canDevice->init("socketcan", "can0");
    canDevice->start();
}

MainWindow::~MainWindow()
{
}
