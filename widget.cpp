#include "widget.h"
#include "qtcpsocket.h"
#include <QImage>
#include <QPainter>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    server_ = new QTcpServer(this);
    connect(server_, &QTcpServer::newConnection, this, [&]() {
        qDebug()<<"connected.";
        while(server_->hasPendingConnections()) {
            QTcpSocket* socket = server_->nextPendingConnection();
            connect(socket, &QTcpSocket::readyRead, this, [this, socket](){
                socketRead(socket);
            });

            connect(socket, &QTcpSocket::disconnected, this, [&]() {
                qDebug()<<"disconnected.";
                memset(g_drawCache, 0x00, MID_DIS_BUFFER_SIZE);
                QWidget::update();
            });

            socketRead(socket);
        }
    });
    server_->listen(QHostAddress::Any, 50122);
    memset(g_drawCache, 0x00, MID_DIS_BUFFER_SIZE);
}

Widget::~Widget()
{
}

void Widget::paintEvent(QPaintEvent *) {
    int numRows = MID_DIS_DOTS_Y; // 行数
    int numCols = MID_DIS_DOTS_X; // 列数

//    qDebug()<<width()<<","<<height();
    int cellSize = qMin(width() / numCols, height() / numRows); // 计算每个单元格的大小
    QPainter painter(this);

    // 背景
    painter.setBrush(Qt::white);
    painter.drawRect(0, 0, numCols * cellSize, numRows * cellSize);

    // 绘制
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            if (MID_LCD_CacheGetBit(col, row, g_drawCache)) {
                painter.setBrush(Qt::darkGray);
            } else {
                painter.setBrush(Qt::lightGray);
            }
            painter.drawRect(col * cellSize, row * cellSize, cellSize, cellSize);
        }
    }
}

void Widget::socketRead(QTcpSocket* socket)
{
    QByteArray array = socket->readAll();
    if(array.size() > 0) {
//        qDebug()<<array.size()<<", receive data:"<<array;
        std::vector<char> buffer_receive_(array.begin(), array.end());
        std::vector<char> data;
        ipc::FormatErrorCode ret = message_.Deserialize(buffer_receive_, data);
        if(ret != ipc::FormatErrorCode::SUCESS) {
            qDebug()<<"receive error!!!, Deserialize";
        }

        if(data.size() == MID_DIS_BUFFER_SIZE) {
            for(int i = 0; i< data.size(); i++) {
                g_drawCache[i] = data.at(i);
            }
//            MID_LCD_CachePrint(g_drawCache);
            QWidget::update();
        } else {
            qDebug()<<"receive data lenght error: "<<data.size()<<"MID_DIS_BUFFER_SIZE:"<<MID_DIS_BUFFER_SIZE;
        }
    }
}
