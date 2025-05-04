#include "simplecontext.hpp"
#include "load_context_exception.hpp"

namespace Core
{

namespace ApplicationContext
{

SimpleContext::~SimpleContext()
{
}

void SimpleContext::load()
{
    m_isValid = false;

    if ( m_contextPath.isEmpty() ) {
        throw load_context_exception("Load context error: path is empty");
    }

    QFile contextFile(m_contextPath);
    if ( !contextFile.exists() ) {
        throw load_context_exception("Load context error: file not exists");
    }

    if ( !contextFile.open(QIODevice::ReadOnly) ) {
        throw load_context_exception("Load context error: file cannot be opened");
    }

    QTextStream in(&contextFile);

    QStringList data = in.readAll().split("|");

    if ( data.size() != int(m_fields.size()) ) {
        throw load_context_exception(
                    QString("Load context error: number of fields is %1 instead of %2").arg(data.size())
                                                                                       .arg(m_fields.size()).toStdString());
    }

    auto field_it = m_fields.begin();

    for ( auto it = data.constBegin(); it != data.constEnd(); ++it) {
        auto &data_pair = (field_it++)->second;
        auto &check = data_pair.second;

        data_pair.first = check(*it) ? *it : QVariant();
    }

    contextFile.close();

    m_isValid = true;
}

void SimpleContext::save()
{
    if ( m_contextPath.isEmpty() ) {
        throw load_context_exception("Save context error: path is empty");
    }

    QFile contextFile(m_contextPath);
    if ( !contextFile.open(QIODevice::WriteOnly) ) {
        throw load_context_exception("Save context error: open file error");
    }

    QTextStream out(&contextFile);

    QStringList paramsList;

    for ( auto it = m_fields.cbegin(); it != m_fields.cend(); ++it ) {
        paramsList << it->second.first.toString();
    }

    out << paramsList.join("|");

    contextFile.close();
}

QVariant SimpleContext::data(int field_id) noexcept
{
    return m_fields.count(field_id) ? m_fields.at(field_id).first : QVariant();
}

void SimpleContext::setData(int field_id, const QVariant &data)
{
    if ( m_fields.count(field_id) ) {
        m_fields[field_id].first = data;
    }
}

bool SimpleContext::isValid() const noexcept
{
    return m_isValid;
}

void SimpleContext::setContext_path(const QString &value)
{
    m_contextPath = value;
}

} //ApplicationContext
} //Core
