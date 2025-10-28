#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QString>
#include <iostream>
#include <QTimer>
#include <vector>
#include <QKeyEvent>
#include <cmath>

using namespace std;

class Shooter : public QWidget
{
    protected:

        int HP = 10;

        static constexpr double worldX = 18;
        static constexpr double worldY = 10;

        double xBall1 = 9 - 1.5 - 0.5;
        double yBall1 = 2 + 0.5;

        double xBall2 = 9 + 1.5 + 0.5;
        double yBall2 = 2 + 0.5;

        double xBall3 = 9;
        double yBall3 = 2 + 1;

        double xBall1Speed = -0.2 / 2;
        double yBall1Speed = 0.1 / 2;

        double xBall2Speed = 0.2 / 2;
        double yBall2Speed = 0.1 / 2;

        double xBall3Speed = 0.009 / 2;
        double yBall3Speed = 0.1 / 2;

        double selfPosX1 = 9 - 0.25;
        double selfPosY1 = 9;

        double selfPosX2 = 9 + 0.25;
        double selfPosY2 = 10 - 0.3;

        bool Ball1 = false;
        bool Ball2 = false;
        bool Ball3 = false;

        bool isGameOver = false;

        double offsetx, offsety, scale;

        // Ball Positions (empty for now, filled later in constructor)
        std::vector<QPoint> ballPos;

        // Ball Velocities
        std::vector<std::pair<float, float>> ballVel = {
            {1.0f, 0.5f},  // Ball 1 moves right + down
            {-0.5f, 1.0f}, // Ball 2 moves left + up
            {0.2f, -0.3f}  // Ball 3 moves right + up
        };

        void paintEvent(QPaintEvent *) override
        {
            QPainter painter(this);
            painter.setRenderHints(QPainter::Antialiasing, true);

            // Middle shooter
            painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::SquareCap));
            painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
            painter.drawRect(QRectF(metertopixel(9 - 0.25, 2), metertopixel(9 + 0.25, 3)));

            // Right shooter
            painter.save();
            QPointF move = metertopixel(9 + 1.5, 2);
            painter.translate(move);
            painter.rotate(-45.0);
            painter.translate(- move);
            painter.drawRect(QRectF(metertopixel(9 + 1.5 - 0.25, 2 - 0.25), metertopixel(9 + 1.5 + 0.25, 2 + 0.75)));
            painter.restore();
        
            // Left shooter
            painter.save();
            move = metertopixel(9 - 1.5, 2);
            painter.translate(move);
            painter.rotate(45.0);
            painter.translate(- move);
            painter.drawRect(QRectF(metertopixel(9 - 1.5 - 0.25, 2 - 0.25), metertopixel(9 - 1.5 + 0.25, 2 + 0.75)));
            painter.restore();

            // SpaceShip Body
            painter.drawRect(QRectF(metertopixel(9 - 1.5, 1), metertopixel(9 + 1.5, 2)));

            // Player
            painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
            painter.drawRect(QRectF(metertopixel(selfPosX1, selfPosY1), metertopixel(selfPosX2, selfPosY2)));

            // HP
            QString hpText = QString::number(HP);
            painter.setPen(Qt::black);
            painter.setFont(QFont("Arial", 40, QFont::Bold));
            painter.drawText(QRectF(metertopixel(17, 0), metertopixel(17.8, 1)), Qt::AlignCenter, hpText);

            // Bullets
            painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::SquareCap));
            painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));

            painter.drawEllipse(metertopixel(xBall1, yBall1), 25, 25);
            painter.drawEllipse(metertopixel(xBall2, yBall2), 25, 25);
            painter.drawEllipse(metertopixel(xBall3, yBall3), 25, 25);
        }

        void keyPressEvent(QKeyEvent *event) {
            if (event->key() == Qt::Key_Left) {
                selfPosX1 -= 0.2;
                selfPosX2 -= 0.2;
                if (selfPosX1 < 0)
                {
                    selfPosX1 = 0;
                    selfPosX2 = 0.5;
                }
            }

            if (event->key() == Qt::Key_Right) {
                selfPosX1 += 0.2;
                selfPosX2 += 0.2;
                if (selfPosX2 > 18){
                    selfPosX2 = 18;
                    selfPosX1 = 18 - 0.5;
                }


            }

            update();
        }

        void ifHitBall1()
        {
            cout << Ball1 << Ball2 << Ball3 << endl;
            if (eucladianDist(selfPosX1 * scale, selfPosY1 * scale, xBall1 * scale, yBall1 * scale) <= 25 or
                eucladianDist((selfPosX1 * scale) + (0.5 * scale), selfPosY1 * scale, xBall1 * scale, yBall1 * scale) <= 25 or
                eucladianDist(selfPosX1 * scale, (selfPosY1 * scale) + (0.7 * scale), xBall1 * scale, yBall1 * scale) <= 25 or
                eucladianDist((selfPosX1 * scale) + (0.5 * scale), (selfPosY1 * scale) + (0.7 * scale), xBall1 * scale, yBall1 * scale) <= 25)
            {
                cout << "Ball 1 Hit" << endl; 
                if (Ball1 == false)
                {
                    Ball1 = true;
                    HP -= 1;
                }
            }
            else
            {
                Ball1 = false;
            }
            if (eucladianDist(selfPosX1 * scale, selfPosY1 * scale, xBall2 * scale, yBall2 * scale) <= 25 or
                eucladianDist((selfPosX1 * scale) + (0.5 * scale), selfPosY1 * scale, xBall2 * scale, yBall2 * scale) <= 25 or
                eucladianDist(selfPosX1 * scale, (selfPosY1 * scale) + (0.7 * scale), xBall2 * scale, yBall2 * scale) <= 25 or
                eucladianDist((selfPosX1 * scale) + (0.5 * scale), (selfPosY1 * scale) + (0.7 * scale), xBall2 * scale, yBall2 * scale) <= 25)
            {
                cout << "Ball 2 Hit" << endl;
                if (Ball2 == false)
                {
                    Ball2 = true;
                    HP -= 1;
                }
            }
            else
            {
                Ball2 = false;
            }
            if (eucladianDist(selfPosX1 * scale, selfPosY1 * scale, xBall3 * scale, yBall3 * scale) <= 25 or
                eucladianDist((selfPosX1 * scale) + (0.5 * scale), selfPosY1 * scale, xBall3 * scale, yBall3 * scale) <= 25 or
                eucladianDist(selfPosX1 * scale, (selfPosY1 * scale) + (0.7 * scale), xBall3 * scale, yBall3 * scale) <= 25 or
                eucladianDist((selfPosX1 * scale) + (0.5 * scale), (selfPosY1 * scale) + (0.7 * scale), xBall3 * scale, yBall3 * scale) <= 25)
            {
                cout << "Ball 3 Hit" << endl;
                if (Ball3 == false)
                {
                    Ball3 = true;
                    HP -= 1;
                }
            }
            else
            {
                Ball3 = false;
            }
            if (HP == -1)
            {
                isGameOver = true;
            }
        }

    public:
        Shooter(QWidget *parent = nullptr) : QWidget(parent)
        {
            std::cout << "Shooter window created!" << std::endl;

            transformation();

            QTimer *timer = new QTimer(this);
            connect(timer, &QTimer::timeout, this, [&]()
            {
                ifHitBall1();

                if (isGameOver == true) {
                    timer->stop();
                }

                if ((xBall1 * scale) + 25 > 18 * scale or (xBall1 * scale) - 25 < 0 * scale) 
                {
                    xBall1Speed *= -1;
                }

                if ((xBall2 * scale) + 25 > 18 * scale or (xBall2 * scale) - 25 < 0 * scale)
                {
                    xBall2Speed *= -1;
                }

                if ((xBall3 * scale) + 25 > 18 * scale or (xBall3 * scale) - 25 < 0 * scale)
                {
                    xBall3Speed *= -1;
                }

                if ((yBall1 * scale) + 25 > 10 * scale or (yBall1 * scale) - 25 < 0 * scale) 
                {
                    yBall1Speed *= -1;
                }

                if ((yBall2 * scale) + 25 > 10 * scale or (yBall2 * scale) - 25 < 0 * scale)
                {
                    yBall2Speed *= -1;
                }

                if ((yBall3 * scale) + 25 > 10 * scale or (yBall3 * scale) - 25 < 0 * scale)
                {
                    yBall3Speed *= -1;
                }

                xBall1 += xBall1Speed;
                yBall1 += yBall1Speed;

                xBall2 += xBall2Speed;
                yBall2 += yBall2Speed;

                xBall3 += xBall3Speed;
                yBall3 += yBall3Speed;

                update();
            });
            timer->start(16);
        }


        ~Shooter()
        {
            std::cout << "Shooter window destroyed!" << std::endl;
        }

        double eucladianDist(double x1, double y1, double x2, double y2)
        {
            return std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
        }

        void transformation()
        {
            double ratioX = width() / worldX;
            double ratioY = height() / worldY;

            if (ratioX > ratioY) {
                offsetx = ((ratioX - ratioY) * worldX) / 2;
                scale = ratioY;
                offsety = 0.0;
            } else {
                offsety = ((ratioY - ratioX) * worldY) / 2;
                scale = ratioX;
                offsetx = 0.0;
            }
        }

        QPointF metertopixel(double x, double y)
        {
            transformation();
            return QPointF(offsetx + (x * scale), offsety + (y * scale));
        }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Shooter window;
    window.showFullScreen();
    window.show();

    return app.exec();
}
