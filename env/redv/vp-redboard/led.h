#ifndef LED_H
#define LED_H

#include "gpioutils.h"

#include <QGraphicsItem>
#include <QPainter>

using namespace gpioutil;

class LED : public QGraphicsItem
{
public:
    //! default colorMask is for rgb
    LED(QPoint f_point, GpioClient** f_gpio)
        : m_size(20, 20)
        , m_colorMask(0xff)
        , m_gpio(f_gpio)
    {
        setPos(f_point);
    }

    //! set color mask lsb for rgb.
    void setColorMask(quint8 f_mask)
    {
        m_colorMask = f_mask;
    }

    QRectF boundingRect() const override
    {
        return QRectF(-m_size.width()/2, -m_size.height()/2, m_size.width(), m_size.height());
    }

    void paint(QPainter *f_painter, const QStyleOptionGraphicsItem *f_option, QWidget *f_widget) override
    {
        if (!(*m_gpio))
            return;
        uint8_t map = translatePinNumberToRGBLed(translateGpioToExtPin((*m_gpio)->state));
        map &= m_colorMask;
        m_color = QColor(map & 1 ? 255 : 0, map & (1 << 1) ? 255 : 0, map & (1 << 2) ? 255 : 0, 0xC0);

        f_painter->setBrush(m_color);
        f_painter->drawEllipse(-m_size.width()/2, -m_size.height()/2, m_size.width(), m_size.height());
    }

    void advance(int step) override
    {
        if (! step)
            return;
        if (!(*m_gpio))
            return;

        if ((*m_gpio)->update())
        {
           update();
        }
    }


private:
    QSize m_size;
    QColor m_color;
    quint8 m_colorMask;

    GpioClient** m_gpio;
};

#endif // LED_H
