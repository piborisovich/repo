#ifndef GRAPHTOOLSETTINGSWIDGET_HPP
#define GRAPHTOOLSETTINGSWIDGET_HPP

#include "toolsettings.hpp"
#include "toolsettingswidget.hpp"

#include <QSpinBox>
#include <QPushButton>

/*!
 * \brief Настройки для построения графика
 */
class GraphToolSettingsWidget : public ToolSettingsWidget
{
    Q_OBJECT
public:
    GraphToolSettingsWidget(std::shared_ptr<ToolSettings> settings,
                            QWidget* parent = nullptr,
                            Qt::WindowFlags f = Qt::WindowFlags());

Q_SIGNALS:
    void startPlot();

private Q_SLOTS:
    void on_pointsCntChanged(int value);

private:
    QSpinBox *m_pointsCnt;
    QPushButton *m_plotButton;
};

#endif // GRAPHTOOLSETTINGSWIDGET_HPP
