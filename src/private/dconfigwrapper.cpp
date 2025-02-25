// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dconfigwrapper_p.h"

#include <QDebug>
#include <QCoreApplication>
#include <QLoggingCategory>
#include <QQmlEngine>
#include <QTimer>
#include <private/qqmlopenmetaobject_p.h>

#include <DConfig>
#include <DThreadUtils>

#ifndef QT_DEBUG
Q_LOGGING_CATEGORY(cfLog, "dtk.dsg.config" , QtInfoMsg);
#else
Q_LOGGING_CATEGORY(cfLog, "dtk.dsg.config");
#endif

DCORE_USE_NAMESPACE;

// the properties and previous values.
using DefalutProperties = QMap<QByteArray, QVariant>;
static DefalutProperties propertyAndValues(const QObject* obj)
{
    QMap<QByteArray, QVariant> properties;
    const QMetaObject *mo = obj->metaObject();
    const int offset = mo->propertyOffset();
    const int count = mo->propertyCount();
    static const QStringList ReservedPropertyNames {
        "name",
        "subpath",
        "async"
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
        owner->setValue(proName, value);
        // Pre judgment returns the set value first.
        // If the value is different, `valueChanged` will be triggered again to update the value,
        // there are problems when the service is unavailable.
        // But If `impl->value(proName)` is returned, blocking has a performance problem.
        return value;
    }
    int metaCall(QObject *o, QMetaObject::Call _c, int _id, void **_a) override
    {
        if (_c == QMetaObject::ResetProperty) {
            const auto key = name(_id - type()->propertyOffset());
            owner->resetValue(key);
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
    if (mo) {
        qCWarning(cfLog) << name << ": This name can't be changed after initialized";
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
    if (mo) {
        qCWarning(cfLog) << subpath << ": This subpath can't be changed after initialized";
        return;
    }

    m_subpath = subpath;
}

/*!
 * \brief \sa DConfig keyList()
 * \return
 */
QStringList DConfigWrapper::keyList() const
{
    if (!impl)
        return QStringList();

    return configKeyList;
}

/*!
 * \brief \sa DConfig isValid()
 * \return
 */
bool DConfigWrapper::isValid() const
{
    if (!impl)
        return false;

    // If is invalid, will delete the impl object
    return true;
}

bool DConfigWrapper::isDefaultValue(const QString &key) const
{
    return !nonDefaultValueKeyList.contains(key);
}

/*!
 * \brief \sa DConfig value()
 * \return
 */
QVariant DConfigWrapper::value(const QString &key, const QVariant &fallback) const
{
    const auto &value = property(key.toLatin1().constData());
    return value.isValid() ? value : fallback;
}

/*!
 * \brief \sa DConfig setValue()
 * \return
 */
void DConfigWrapper::setValue(const QString &key, const QVariant &value)
{
    if (!impl)
        return;

    if (m_async) {
        QMetaObject::invokeMethod(impl.get(), [this, key, value] {
            impl->setValue(key, value);
        });
    } else {
        impl->setValue(key, value);
    }
}

void DConfigWrapper::resetValue(const QString &key)
{
    if (!impl)
        return;

    if (m_async) {
        QMetaObject::invokeMethod(impl.get(), [this, key] {
            impl->reset(key);
        });
    } else {
        impl->reset(key);
    }
}

void DConfigWrapper::classBegin()
{

}

class Q_DECL_HIDDEN DConfigWrapperThread : public QThread {
public:
    DConfigWrapperThread()
        : QThread()
    {
        setObjectName("DConfigWrapperThread");
        moveToThread(this);
    }
    ~DConfigWrapperThread() override
    {
        quit();
        wait();
    }
};

static QThread *globalThread() {
    static QThread *thread = nullptr;
    if (!thread) {
        thread = new DConfigWrapperThread();
        thread->start();
    }
    return thread;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
DTK_CORE_NAMESPACE::DThreadUtils *globalThreadUtils() {
    static DTK_CORE_NAMESPACE::DThreadUtils *threadUtils = nullptr;
    if (!threadUtils) {
        threadUtils = new DTK_CORE_NAMESPACE::DThreadUtils(globalThread());
    }
    return threadUtils;
}
#endif

/*!
    \brief Initialize `DConfig` and redirect method of property's get and set.
    `DConfig` can only be initialized after \property name and \property subpath initialization
    is completed.
    \return
 */
void DConfigWrapper::componentComplete()
{
    Q_ASSERT(!impl);

    // Get the dynamic properties and previous values defined in qml.
    // Muse before new DConfigWrapperMetaObject
    initializeConfigs = propertyAndValues(this);
    qCDebug(cfLog) << "Initialize Properties:" << initializeConfigs;

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    auto objectType = new QQmlOpenMetaObjectType(&DConfigWrapper::staticMetaObject);
#else
    auto objectType = new QQmlOpenMetaObjectType(&DConfigWrapper::staticMetaObject, qmlEngine(this));
#endif

    mo = new DConfigWrapperMetaObject(this, objectType);
    mo->setCached(true);

    if (m_async) {
        // Init properties
        for (auto iter = initializeConfigs.begin(); iter != initializeConfigs.end(); iter++) {
            mo->setValue(iter.key(), iter.value());
        }

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        globalThreadUtils()->run(this, &DConfigWrapper::initializeProperties);
#else
        QMetaObject::invokeMethod(globalThread(), [this] {
            initializeProperties();
        });
#endif
    } else {
        initializeProperties();
    }
}

template<typename Fun, typename... Args>
typename std::result_of<typename std::decay<Fun>::type(Args...)>::type
callInGuiThread(DConfigWrapper *wrapper, Fun fun, Args&&... args) {
    if (QThread::currentThread() == qApp->thread()) {
        return fun(std::forward<Args>(args)...);
    }

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    return DThreadUtils::gui().exec(wrapper, fun, std::forward<Args>(args)...);
#else
    return DThreadUtil::runInMainThread(wrapper, fun, std::forward<Args>(args)...);
#endif
}

// in config thread, don't set the member variable directly.
// Must ensure the member variable is set in the main thread.
// So there have a "const" flag.
void DConfigWrapper::initializeProperties() const
{
    auto impl = new DTK_CORE_NAMESPACE::DConfig(m_name, m_subpath);

    if (!impl->isValid()) {
        qCWarning(cfLog) << QString("create dconfig failed, valid:%1, name:%2, subpath:%3, backend:%4")
                                .arg(impl->isValid())
                                .arg(impl->name())
                                .arg(impl->subpath())
                                .arg(impl->backendName());
        delete impl;
        impl = nullptr;
        return;
    }

    qInfo() << QString("create dconfig successful, valid:%1, name:%2, subpath:%3, backend:%4")
                   .arg(impl->isValid())
                   .arg(impl->name())
                   .arg(impl->subpath())
                   .arg(impl->backendName());

    const auto keyList = impl->keyList();
    QStringList nonDefaultValueKeyList;
    for (const auto &key : keyList) {
        if (!impl->isDefaultValue(key)) {
            nonDefaultValueKeyList.append(key);
        }
    }

    auto wrapper = const_cast<DConfigWrapper*>(this);
    callInGuiThread(wrapper, [wrapper, keyList, nonDefaultValueKeyList, impl] {
        wrapper->impl.reset(impl);
        wrapper->configKeyList = keyList;
        wrapper->nonDefaultValueKeyList = nonDefaultValueKeyList;
    });

    for (const auto &key : keyList) {
        const QVariant currentValue = callInGuiThread(wrapper, [wrapper, key] {
            return wrapper->property(key.toLocal8Bit());
        });

        const auto initialValue = initializeConfigs.value(key.toLocal8Bit());
        if (currentValue.isValid() && currentValue != initialValue) {
            // This key has been set value in QML by user, so we should update the value to DConfig.
            qCDebug(cfLog) << "Update value from user on initialize, key:" << key
                           << "value:" << currentValue << "initialize value:" << initialValue
                           << "config side value:" << impl->value(key, QVariant());
            impl->setValue(key, currentValue);
        } else {
            // Must fallback to the initial value, in the sync mode, the DConfigWrapperMetaObject's
            // properties is not initialize.
            const auto value = impl->value(key, initialValue);
            callInGuiThread(wrapper, [wrapper, key, value] {
                if (value.isValid())
                    wrapper->mo->setValue(key.toLocal8Bit(), value);
            });
        }
    }

    // Using QueuedConnection because impl->setValue maybe emit sync signal in `propertyWriteValue`.
    connect(impl, &DTK_CORE_NAMESPACE::DConfig::valueChanged, wrapper, [wrapper, impl](const QString &key) {
        const QByteArray &propName = key.toLocal8Bit();

        qCDebug(cfLog) << "update value from DConfig by 'valueChanged', key:" << propName;
        const auto value = impl->value(propName, QVariant());
        const bool isDefault = impl->isDefaultValue(propName);
        callInGuiThread(wrapper, [wrapper, propName, value, isDefault] {
            if (isDefault) {
                wrapper->nonDefaultValueKeyList.removeOne(propName);
            } else if (!wrapper->nonDefaultValueKeyList.contains(propName)) {
                wrapper->nonDefaultValueKeyList.append(propName);
            }
            if (value.isValid())
                wrapper->mo->setValue(propName, value);
        });

        QMetaObject::invokeMethod(wrapper, [wrapper, key] {
            Q_EMIT wrapper->valueChanged(key);
        });
    }, Qt::DirectConnection);

    QMetaObject::invokeMethod(wrapper, &DConfigWrapper::initialized);
}

bool DConfigWrapper::async() const
{
    return m_async;
}

void DConfigWrapper::setAsync(bool newAsync)
{
    m_async = newAsync;
    if (mo)
        qCWarning(cfLog) << "Async can't be changed after initialized";
}
