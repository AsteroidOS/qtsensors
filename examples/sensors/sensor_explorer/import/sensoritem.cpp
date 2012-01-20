/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: http://www.qt-project.org/
**
** This file is part of the QtSensors module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "sensoritem.h"
#include <QtCore/QDebug>
#include <QtSensors>

QT_BEGIN_NAMESPACE

/*
    \class QSensorItem
    \brief The QSensorItem element provides information about the meta-datas from a sensors installed on the system.
*/

/*
    Construct a QSensorItem object with parent \a parent
*/
QSensorItem::QSensorItem(QObject* parent)
    : QObject(parent)
    , _qsensor(0)
{
}

/*
    Construct a QSensorItem object with QSensor \a sensor and parent \a parent
*/
QSensorItem::QSensorItem(QSensor* sensor, QObject* parent)
    : QObject(parent)
    , _qsensor(sensor)
{
}

/*
    Destructor of a QSensorItem
*/
QSensorItem::~QSensorItem()
{
}

/*
    \fn QSensorItem::startChanged()
    Notifies the client if the sensors has changed its start active status
*/

/*
    \property QSensorItem::start
    This property starts or stops the sensor
*/
bool QSensorItem::start()
{
    return (_qsensor ? _qsensor->isActive() : false);
}

void QSensorItem::setStart(bool run)
{
    if (_qsensor)
        _qsensor->setActive(run);
}

/*
    \property QSensorItem::id
    Returns the sensor id of the sensor item
*/
QString QSensorItem::id()
{
    return (_qsensor ? _qsensor->identifier() : "");
}

/*
    Set the new value \a val to the QPropertyInfo object \a property
*/
void QSensorItem::changePropertyValue(QPropertyInfo* property, const QString& val)
{
    if (_qsensor && _sensorProperties.contains(property)){
        if ( _qsensor->setProperty(property->name().toLatin1().constData(), QVariant(val)))
            updateSensorPropertyValues();
        else
            qWarning() << "new property value couldn't be set";
    }
}

/*
    Reading the meta-datas and activates the sensor.
*/
void QSensorItem::select()
{
    if (_sensorProperties.isEmpty()) {
        // Probe the reading using Qt's meta-object facilities
        //Read properties from reader
        QSensorReading *reading = _qsensor->reading();
        const QMetaObject *mo = reading->metaObject();
        int firstProperty = QSensorReading::staticMetaObject.propertyOffset();

        for (int i = firstProperty; i < mo->propertyCount(); ++i) {
            QString typeName = QLatin1String(mo->property(i).typeName());
            int crap = typeName.lastIndexOf("::");
            if (crap != -1)
                typeName = typeName.mid(crap + 2);

            QPropertyInfo* pi = new QPropertyInfo(mo->property(i).name()
                                                  , i
                                                  , isWriteable(mo->property(i).name())
                                                  , typeName
                                                  , "-"
                                                  , this);
            _readerProperties.append(pi);
        }

        //Read properties from sensor
        const QMetaObject *mo1 = _qsensor->metaObject();
        firstProperty = QSensorReading::staticMetaObject.propertyOffset();

        for (int i = firstProperty; i < mo1->propertyCount(); ++i) {
            QString propertyname = mo1->property(i).name();
            if (ignoreProperty(propertyname))
                continue;

            QString typeName = QLatin1String(mo1->property(i).typeName());
            int crap = typeName.lastIndexOf("::");
            if (crap != -1)
                typeName = typeName.mid(crap + 2);

            QPropertyInfo* pi = new QPropertyInfo(propertyname
                                                  , i
                                                  , isWriteable(propertyname)
                                                  , typeName
                                                  , "-"
                                                  , this);
            _sensorProperties.append(pi);
        }
        updateSensorPropertyValues();
        connect(_qsensor, SIGNAL(readingChanged()), this, SLOT(sensorReadingChanged()));
    }
    connect(_qsensor, SIGNAL(activeChanged()), SIGNAL(startChanged()));
}

/*
    Unselect the sensor by stopping the sensor.
*/
void QSensorItem::unSelect()
{
    _qsensor->stop();
    disconnect(_qsensor, SIGNAL(activeChanged()), this , SIGNAL(startChanged()));
}

/*
    Updates the property values from QSensor
*/
void QSensorItem::updateSensorPropertyValues()
{
    if (_qsensor){
        const QMetaObject *mo = _qsensor->metaObject();
        for (int i = 0; i < _sensorProperties.count(); i++){
            QVariant val = mo->property(_sensorProperties[i]->index()).read(_qsensor);
            _sensorProperties[i]->setValue(convertValue(_sensorProperties[i]->typeName(), val));
        }
    }
}

/*
    \fn QSensorItem::propertiesChanged()
    Notifies the client if the list of the properties was changed
*/

/*
    Updates the property values from the QSensorReader
*/
void QSensorItem::sensorReadingChanged()
{
    QSensorReading *reading = _qsensor->reading();
    const QMetaObject *mo = reading->metaObject();
    for (int i = 0; i < _readerProperties.count(); i++){
        QVariant val = mo->property(_readerProperties[i]->index()).read(reading);
        _readerProperties[i]->setValue(convertValue(_readerProperties[i]->typeName(), val));
    }
}

/*
    Returns true if the property with the name \a propertyname should be ignored
*/
bool QSensorItem::ignoreProperty(const QString& propertyname)
{
    if (propertyname == "reading" ||
        propertyname == "active" ||
        propertyname == "sensorid" ||
        propertyname == "connectedToBackend" ||
        propertyname == "busy")
        return true;

    return false;
}

/*
    Returns true if the property with the name \a propertyname is writeable
*/
bool QSensorItem::isWriteable(const QString& propertyname)
{
    if (_qsensor){
        const QMetaObject *mo = _qsensor->metaObject();
        int propertyindex = mo->indexOfProperty(propertyname.toLocal8Bit().constData());
        if (propertyindex >= 0){
            QMetaProperty prop = mo->property(propertyindex);
            return prop.isWritable();
        }
        else {
            QSensorReading *reading = _qsensor->reading();
            const QMetaObject *moreader = reading->metaObject();
            propertyindex = moreader->indexOfProperty(propertyname.toLocal8Bit().constData());
            if (propertyindex >= 0){
                QMetaProperty prop = mo->property(propertyindex);
                return prop.isWritable();
            }
        }
    }

    return false;
}

/*
    Convert the variant \a val dependend on the type \a type and returns the converted value as a QString
*/
QString QSensorItem::convertValue(const QString& type, const QVariant& val)
{
    if (type == "LightLevel"){
        switch (val.toInt()) {
            case 1: return "Dark";
            case 2: return "Twilight";
            case 3: return "Light";
            case 4: return "Bright";
            case 5: return "Sunny";
            default: return "Undefined";
        }
    }
    else if (type == "Orientation"){
        switch (val.toInt()) {
            case 1: return "TopUp";
            case 2: return "TopDown";
            case 3: return "LeftUp";
            case 4: return "RightUp";
            case 5: return "FaceUp";
            case 6: return "FaceDown";
            default: return "Undefined";
        }
    }
    else if (type == "qrangelist"){
        qrangelist rangelist = val.value<qrangelist>();
        QStringList ranges;
        foreach (const qrange &r, rangelist) {
            if (r.first == r.second)
                ranges << QString("%1 Hz").arg(r.first);
            else
                ranges << QString("%1-%2 Hz").arg(r.first).arg(r.second);
        }
        if (ranges.count() > 0)
            return ranges.join(", ");
        return "-";
    }
    else if (type == "qoutputrangelist") {
        qoutputrangelist rangelist = val.value<qoutputrangelist>();
        QStringList ranges;
        foreach (const qoutputrange &r, rangelist) {
            ranges << QString("(%1, %2) += %3").arg(r.minimum).arg(r.maximum).arg(r.accuracy);
        }
        if (ranges.count() > 0)
            return ranges.join(", ");
        return "-";
    }

    return val.toString();
}

/*
    \property QSensorItem::properties
    Returns a list of all properties from the sensor
*/
QDeclarativeListProperty<QPropertyInfo> QSensorItem::properties()
{
    _properties.clear();
    _properties.append(_sensorProperties);
    _properties.append(_readerProperties);
    return QDeclarativeListProperty<QPropertyInfo> (this, _properties);
}

QT_END_NAMESPACE