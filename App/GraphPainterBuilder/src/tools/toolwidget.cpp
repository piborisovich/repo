#include "toolwidget.hpp"
#include "flowlayout.hpp"

ToolWidget::ToolWidget(const ToolList &tools,
                       QWidget *parent,
                       Qt::WindowFlags f) : QWidget(parent, f)
    , m_toollist (tools)
{
    FlowLayout* flowLayout = new FlowLayout(this, 0, 5, 5);

    for (const auto &tool : m_toollist) {
        if ( tool ) {
            auto *button = tool->button();

            if ( button ) {
                auto result = connect(button,
                                      &QToolButton::clicked,
                                      this,
                                      &ToolWidget::on_toolButtonClicked);
                Q_ASSERT(result);

                flowLayout->addWidget(button);
            }
            m_buttonToolMap.insert(button, tool.get());
        }
    }

    setLayout(flowLayout);
}

void ToolWidget::on_toolButtonClicked(bool checked)
{
    QToolButton *button = qobject_cast<QToolButton*>( sender() );

    if ( button ) {

        if ( checked ) {
            auto result = std::find_if(m_toollist.cbegin(),
                                       m_toollist.cend(),
                                       [button](const std::shared_ptr<ITool> &tool){
                                           return button != tool->button() && tool->button()->isChecked();
                                       });
            if ( result != m_toollist.cend() ) {
                (*result)->button()->setChecked(false);
            }

            auto it = m_buttonToolMap.find(button);

            if ( it != m_buttonToolMap.end() ) {
                emit toolSelected( *(it.value()) );
            }
        } else {
            emit toolDisabled();
        }
    }
}
