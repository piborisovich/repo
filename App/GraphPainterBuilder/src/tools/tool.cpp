#include "tool.hpp"

#include "graphicsscene.hpp"
#include "toolsettingswidgetcreator.hpp"

static QSize ICON_SIZE(24, 24);

Tool::Tool(const QString &name,
           std::shared_ptr<ToolSettings> settings,
           const QString &iconPath,
           QObject *parent) : ITool(parent)

    , m_settings(settings)
    , m_name(name)
    , m_settingsWidget( ToolSettingsWidgetCreator::create(settings) )
    , m_button( new QToolButton() )
    , m_scene(nullptr)
{
    m_button->setToolTip(m_name);
    m_button->setCheckable(true);
    m_button->setIcon( QIcon(iconPath) );
    m_button->setIconSize(ICON_SIZE);

    auto result = connect(m_button,
                          &QToolButton::clicked,
                          this,
                          &Tool::on_buttonClicked);
    Q_ASSERT(result);

    result = connect(m_button,
                     &QToolButton::toggled,
                     this,
                     &Tool::on_buttonToggled);
    Q_ASSERT(result);
}

GraphicsScene *Tool::scene()
{
    return m_scene;
}

void Tool::setScene(GraphicsScene *scene)
{
    m_scene = scene;
}

const QCursor &Tool::getCursor() const
{
    static QCursor cursor(Qt::ArrowCursor);

    return cursor;
}

QToolButton *Tool::button()
{
    return m_button;
}

QWidget *Tool::settingsWidget()
{
    return m_settingsWidget;
}

std::shared_ptr<ToolSettings> Tool::settings()
{
    return m_settings;
}

void Tool::select()
{
    if ( m_scene ) {

        const auto allItems = m_scene->items();

        // clazy:excludeall=range-loop-detach
        for ( QGraphicsItem * item : allItems ) {
            item->setFlag(QGraphicsItem::ItemIsSelectable, false);
            item->setFlag(QGraphicsItem::ItemIsMovable, false);

            item->setSelected(false);
        }
    }
}

void Tool::unselect()
{
}

bool Tool::vertexOnScene(const QPointF &scenePos) const
{
    if ( m_settings && m_scene ) {

        auto width = m_settings->penSize();

        auto width_div2 = width / 2.0;

        QRectF vertexRect( scenePos - QPointF(width_div2, width_div2), QSizeF(width, width) );

        return m_scene->sceneRect().contains(vertexRect);
    }

    return false;
}

void Tool::on_buttonClicked(bool checked)
{
    if ( m_scene ) {
        GraphicsScene *gs = scene();
        if ( gs ) {
            if ( checked ) {
                gs->changeCurrentTool(this);
            } else {
                gs->changeCurrentTool(nullptr);
            }
        }
    }
}

void Tool::on_buttonToggled(bool checked)
{
    if (checked) {
        select();
    } else {
        unselect();
    }
}
