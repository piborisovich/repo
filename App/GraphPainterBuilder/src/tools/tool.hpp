#ifndef TOOL_HPP
#define TOOL_HPP

#include "itool.hpp"

/*!
 * \brief Базовый класс инструмента
 */
class Tool : public ITool
{
    Q_OBJECT
public:

    explicit Tool(const QString &name,
                  std::shared_ptr<ToolSettings> settings,
                  const QString &iconPath = QString(),
                  QObject* parent = nullptr);

    GraphicsScene *scene() override;
    void setScene(GraphicsScene *scene) override;

    const QCursor &getCursor() const override;

    QToolButton *button() override;

    QWidget* settingsWidget() override;

    std::shared_ptr<ToolSettings> settings() override;

    void select() override;
    void unselect() override;

private Q_SLOTS:
    void on_buttonClicked(bool checked);
    void on_buttonToggled(bool checked);

private:

    std::shared_ptr<ToolSettings> m_settings;
    QString m_name;
    QWidget *m_settingsWidget;

    QToolButton *m_button;
    GraphicsScene *m_scene;
};

#endif // TOOL_HPP
