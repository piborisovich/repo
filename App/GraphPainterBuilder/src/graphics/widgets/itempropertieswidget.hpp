#ifndef ITEMPROPERTIESWIDGET_HPP
#define ITEMPROPERTIESWIDGET_HPP

#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QPen>

class ItemPropertiesWidget : public QWidget
{
    Q_OBJECT
public:
    ItemPropertiesWidget(QWidget* parent = nullptr,
                         Qt::WindowFlags f = Qt::WindowFlags());

Q_SIGNALS:
    void penChanged(const QPen &pen);

protected:
    void setPen(const QPen &pen);
    QPen pen() const;

private Q_SLOTS:
    void on_penColorClicked();
    void on_showSliderTooltip(int value);

private:
    QPushButton *m_penColorButton;
    QSlider *m_penSizeSlider;
    QPen m_pen;

};

#endif // ITEMPROPERTIESWIDGET_HPP
