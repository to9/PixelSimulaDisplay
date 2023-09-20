#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include "ipc_helper.h"

#define HAL_DIS_DOTS_X           (14)
#define HAL_DIS_DOTS_Y           (8)
#define MID_DIS_DOTS_X           (HAL_DIS_DOTS_X * 2)
#define MID_DIS_DOTS_Y           (HAL_DIS_DOTS_Y * 2)
#define MID_DIS_BYTE_X           ((MID_DIS_DOTS_X + 7) >> 3)
#define MID_DIS_BUFFER_SIZE		 (MID_DIS_BYTE_X * MID_DIS_DOTS_Y)

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QSize sizeHint() const override {
        return QSize(674, 386);
    }

protected:
    void paintEvent(QPaintEvent *) override;

private:
    uint8_t g_drawCache[MID_DIS_BUFFER_SIZE];

    uint8_t MID_LCD_CacheGetBit(uint32_t x, uint32_t y, uint8_t *cache)
    {
        uint32_t i;
        uint8_t v;

        i = y * MID_DIS_BYTE_X + x / 8;
        v = (cache[i] & (1 << x % 8)) > 0 ? 1 : 0;
        return v;
    }

    void MID_LCD_CachePrint(uint8_t *drawBuff)
    {
        uint32_t i, j;
        for(i = 0; i < MID_DIS_DOTS_Y; i++)
        {
            for(j = 0; j < MID_DIS_DOTS_X; j++)
            {
                 printf("%c ", MID_LCD_CacheGetBit(j, i, drawBuff) + 0x30);
            }
             printf("\r\n");
        }
         printf("\r\n");
    }

public slots:
    void socketRead(QTcpSocket *socket);

private:
    QTcpServer *server_;
    ipc::BinaryFormat message_;
};
#endif // WIDGET_H
