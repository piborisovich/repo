#ifndef GRAPHTOOLSETTINGSWIDGET_HPP
#define GRAPHTOOLSETTINGSWIDGET_HPP

#include "toolsettings.hpp"
#include "toolsettingswidget.hpp"

#include <QSpinBox>

/*!
 * \brief Настройки для построения графика
 */
class GraphToolSettingsWidget : public ToolSettingsWidget
{
public:
    GraphToolSettingsWidget(std::shared_ptr<ToolSettings> settings,
                            QWidget* parent = nullptr,
                            Qt::WindowFlags f = Qt::WindowFlags());
private Q_SLOTS:
    void on_pointsCntChanged(int value);

private:
    QSpinBox *m_pointsCnt;
};

#endif // GRAPHTOOLSETTINGSWIDGET_HPP
