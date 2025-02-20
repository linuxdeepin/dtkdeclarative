// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dconfigwrapper_p.h"

#include <QDebug>
#include <QCoreApplication>
#include <QLoggingCategory>
#include <QQmlEngine>
#include <QTimer>
#include <QQmlInfo>
#include <private/qqmlopenmetaobject_p.h>

#include <DConfig>

#ifndef QT_DEBUG
Q_LOGGING_CATEGORY(cfLog, "dtk.dsg.config" , QtInfoMsg);
#else
Q_LOGGING_CATEGORY(cfLog, "dtk.dsg.config");
#endif

DCORE_USE_NAMESPACE;

// the properties and previous values.
static DConfigWrapper::DefalutProperties propertyAndValues(const QObject* obj)
{
    QMap<QByteArray, QVariant> properties;
    const QMetaObject *mo = obj->metaObject();
    const int offset = mo->propertyOffset();
    const int count = mo->propertyCount();
    static const QStringList ReservedPropertyNames {
        "name",
        "subpath"
    };

    for (int i = offset; i < count; ++i) {
        const QMetaProperty &property = mo->property(i);
        if (ReservedPropertyNames.contains(property.name())) {
            qCWarning(cfLog()) << property.name() << " is keyword for Config." << property.isUser();
            continue;
        }

        const QVariant &previousValue = property.read(obj);
        properties[property.name()] = previousValue;
    }
    return properties;
}

class DConfigWrapperMetaObject : public QQmlOpenMetaObject {
public:
    DConfigWrapperMetaObject(DConfigWrapper *o, QQmlOpenMetaObjectType *type)
        : QQmlOpenMetaObject(o, type)
        , owner(o)
    {
    }
    ~DConfigWrapperMetaObject() override;

    DConfigWrapper *owner;
    // QQmlOpenMetaObject interface
protected:
    virtual QVariant propertyWriteValue(int index, const QVariant &value) override
    {
        const QByteArray &proName = name(index);
        qCDebug(cfLog) << "propertyWriteValue" << proName << value;
        owner->impl->setValue(proName, value);
        // Pre judgment returns the set value first.
        // If the value is different, `valueChanged` will be triggered again to update the value,
        // there are problems when the service is unavailable.
        // But If `impl->value(proName)` is returned, blocking has a performance problem.
        return value;
    }
    int metaCall(QObject *o, QMetaObject::Call _c, int _id, void **_a) override
    {
        if (_c == QMetaObject::ResetProperty) {
            owner->impl->reset(name(_id - type()->propertyOffset()));
        }

        return QQmlOpenMetaObject::metaCall(o, _c, _id, _a);
    }
};

DConfigWrapperMetaObject::~DConfigWrapperMetaObject()
{
}

/*!
    \class DTK::Quick::DConfigWrapper
    \inmodule dtkdeclarative

    \brief Import some function about reading and writing DSG configuration.
*/
DConfigWrapper::DConfigWrapper(QObject *parent)
    : QObject(parent)
    , impl(nullptr)
{
}

DConfigWrapper::~DConfigWrapper()
{
}

/*!
 * \brief \sa DConfig name()
 * \return
 */
QString DConfigWrapper::name() const
{
    return m_name;
}

void DConfigWrapper::setName(const QString &name)
{
    if (!m_name.isEmpty()) {
        qWarning() << "name is existed." << m_name;
        return;
    }

    m_name = name;
}

/*!
 * \brief \sa DConfig subpath()
 * \return
 */
QString DConfigWrapper::subpath() const
{
    return m_subpath;
}

void DConfigWrapper::setSubpath(const QString &subpath)
{
    if (!m_subpath.isEmpty()) {
        qWarning() << "subpath is existed." << m_subpath;
        return;
    }

    m_subpath = subpath;
}

QObject *DConfigWrapper::proxyTarget() const
{
    return m_proxyTarget;
}

void DConfigWrapper::setProxyTarget(QObject *newProxyTarget)
{
    if (m_proxyTarget == newProxyTarget)
        return;

    if (m_proxyTargetValueChangedConnection) {
        disconnect(m_proxyTargetValueChangedConnection);
    }

    m_proxyTarget = newProxyTarget;

    if (m_componentCompleted) {
        if (m_proxyTarget)
            initWithProxyTarget();
        else
            initWithDConfig();
    }

    Q_EMIT proxyTargetChanged();
}

/*!
 * \brief \sa DConfig keyList()
 * \return
 */
QStringList DConfigWrapper::keyList() const
{
    if (m_proxyTarget) {
        const QMetaObject *mo = m_proxyTarget->metaObject();
        const int offset = mo->propertyOffset();
        const int count = mo->propertyCount();

        QStringList keyList = {};
        keyList.reserve(count);

        for (int i = offset; i < count; ++i) {
            const QMetaProperty &property = mo->property(i);
            keyList << property.name();
        }
        return keyList;
    }

    if (!impl)
        return QStringList();

    return impl->keyList();
}

/*!
 * \brief \sa DConfig isValid()
 * \return
 */
bool DConfigWrapper::isValid() const
{
    if (m_proxyTarget) {
        int index = m_proxyTarget->metaObject()->indexOfMethod("isInitializeSucceed()");
        if (index < 0) {
            qmlEngine(this)->throwError(QString("Can't know whether the DConfig is valid, "
                                                "because the proxyTarget doesn't have "
                                                "isInitializeSucceed method"));
            return false;
        }

        return m_proxyTarget->metaObject()->method(index).invoke(m_proxyTarget);
    }

    if (!impl)
        return false;

    return impl->isValid();
}

/*!
 * \brief \sa DConfig value()
 * \return
 */
QVariant DConfigWrapper::value(const QString &key, const QVariant &fallback) const
{
    if (m_proxyTarget) {
        if (isDefaultValue(key))
            return fallback;

        return m_proxyTarget->property(key.toUtf8().constData());
    }

    if (!impl)
        return fallback;

    return impl->value(key, fallback);
}

/*!
 * \brief \sa DConfig setValue()
 * \return
 */
void DConfigWrapper::setValue(const QString &key, const QVariant &value)
{
    if (m_proxyTarget) {
        m_proxyTarget->setProperty(key.toUtf8().constData(), value);
        return;
    }

    if (!impl)
        return;

    impl->setValue(key, value);
}

void DConfigWrapper::resetValue(const QString &key)
{
    if (m_proxyTarget) {
        int index = m_proxyTarget->metaObject()->indexOfProperty(key.toUtf8().constData());
        if (index >= 0)
            m_proxyTarget->metaObject()->property(index).reset(m_proxyTarget);
        return;
    }

    if (!impl)
        return;

    impl->reset(key);
}

void DConfigWrapper::classBegin()
{
    m_componentCompleted = false;
}

/*!
    \brief Initialize `DConfig` and redirect method of property's get and set.
    `DConfig` can only be initialized after \property name and \property subpath initialization
    is completed.
    \return
 */
void DConfigWrapper::componentComplete()
{
    m_componentCompleted = false;
    // Get the dynamic properties and previous values defined in qml.
    m_properties = propertyAndValues(this);
    qCDebug(cfLog) << "properties" << m_properties;

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    auto objectType = new QQmlOpenMetaObjectType(&DConfigWrapper::staticMetaObject, qmlEngine(this));
#else
    auto objectType = new QQmlOpenMetaObjectType(&DConfigWrapper::staticMetaObject);
#endif
    m_metaObject = new DConfigWrapperMetaObject(this, objectType);
    m_metaObject->setCached(true);

    if (m_proxyTarget) {
        initWithProxyTarget();
    } else {
        initWithDConfig();
    }
}

bool DConfigWrapper::isDefaultValue(const QString &key) const {
    Q_ASSERT(m_proxyTarget);
    if (!f_isDefaultValue.enclosingMetaObject())
        return false;

    return f_isDefaultValue.invoke(m_proxyTarget, Qt::DirectConnection, Q_ARG(QString, key));
}

void DConfigWrapper::initWithProxyTarget()
{
    Q_ASSERT(m_proxyTarget);
    if (impl) {
        impl->deleteLater();
        impl = nullptr;
    }

    qCInfo(cfLog) << QString("Initialize dconfig with proxy object:") << m_proxyTarget;

    auto mo = m_proxyTarget->metaObject();
    int index = mo->indexOfMethod(QMetaObject::normalizedSignature("isDefaultValue(const QString&)"));

    if (index < 0) {
        qmlWarning(this) << "Can't know whether the value is default, because the proxyTarget doesn't have isDefaultValue method.";
    } else {
        f_isDefaultValue = mo->method(index);
    }

    for (auto iter = m_properties.begin(); iter != m_properties.end(); iter++) {
        // it's need to emit signal, because other qml object maybe read the old value
        // when binding the property before the component completed, also it has a performance problem.
        // sync backend's value to `Wrapper`, we only use Wrapper's value(defined in qml) as fallback value.
        if (isDefaultValue(iter.key()))
            m_metaObject->setValue(iter.key(), iter.value());
        else
            m_metaObject->setValue(iter.key(), m_proxyTarget->property(iter.key()));
    }

    Q_ASSERT(!m_proxyTargetValueChangedConnection);
    m_proxyTargetValueChangedConnection = connect(m_proxyTarget,
                                                  SIGNAL(valueChanged(QString, QVariant)),
                                                  this, SLOT(onProxyTargetValueChanged(QString,QVariant)));

    if (!m_proxyTargetValueChangedConnection) {
        if (auto engine = qmlEngine(this))
            engine->throwError(QString(QLatin1String("Can't connect to valueChanged signal, proxyTarget is invalid.")));
        else
            qCWarning(cfLog) << "Can't connect to valueChanged signal, proxyTarget is invalid.";
    }
}

void DConfigWrapper::initWithDConfig()
{
    Q_ASSERT(!impl);
    impl = new DTK_CORE_NAMESPACE::DConfig(m_name, m_subpath, this);
    Q_ASSERT(!m_proxyTarget);

    if (!impl->isValid()) {
        qCWarning(cfLog) << QString("create dconfig failed, valid:%1, name:%2, subpath:%3, backend:%4")
        .arg(impl->isValid())
            .arg(impl->name())
            .arg(impl->subpath())
            .arg(impl->backendName());
        impl->deleteLater();
        impl = nullptr;
        return;
    }

    qCInfo(cfLog) << QString("create dconfig successful, valid:%1, name:%2, subpath:%3, backend:%4")
                         .arg(impl->isValid())
                         .arg(impl->name())
                         .arg(impl->subpath())
                         .arg(impl->backendName());

    for (auto iter = m_properties.begin(); iter != m_properties.end(); iter++) {
        // it's need to emit signal, because other qml object maybe read the old value
        // when binding the property before the component completed, also it has a performance problem.
        // sync backend's value to `Wrapper`, we only use Wrapper's value(defined in qml) as fallback value.
        m_metaObject->setValue(iter.key(), impl->value(iter.key(), iter.value()));
    }

    // Using QueuedConnection because impl->setValue maybe emit sync signal in `propertyWriteValue`.
    connect(impl, &DTK_CORE_NAMESPACE::DConfig::valueChanged, this, [this](const QString &key){
        const QByteArray &proName = key.toLocal8Bit();
        if (m_properties.contains(proName)) {
            qCDebug(cfLog) << "update value from DConfig by 'valueChanged', key:" << proName;
            m_metaObject->setValue(proName, impl->value(proName, m_properties.value(proName)));
        }
        Q_EMIT valueChanged(key);
    }, Qt::QueuedConnection);
}

void DConfigWrapper::onProxyTargetValueChanged(const QString &key, const QVariant &value)
{
    const QByteArray &propName = key.toLocal8Bit();
    if (m_properties.contains(propName)) {
        qCDebug(cfLog) << "update value from:" << m_proxyTarget << "by valueChanged', key:" << propName
                       << "value:" << value;
        if (isDefaultValue(propName))
            m_metaObject->setValue(propName, m_properties.value(propName));
        else
            m_metaObject->setValue(propName, value);
    }
    Q_EMIT valueChanged(key);
}
