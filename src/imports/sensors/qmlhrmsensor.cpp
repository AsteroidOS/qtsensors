/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSensors module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qmlhrmsensor.h"

/*!
    \qmltype HrmSensor
    \instantiates QmlHrmSensor
    \ingroup qml-sensors_type
    \inqmlmodule QtSensors
    \since QtSensors 5.0
    \inherits Sensor
    \brief The HrmSensor element reports on the user's heart rate in bpm.

    The HrmSensor element reports on the user's heart rate in bpm.

    This element wraps the QHrmSensor class. Please see the documentation for
    QHrmSensor for details.

    \sa HrmReading
*/

QmlHrmSensor::QmlHrmSensor(QObject *parent)
    : QmlSensor(parent)
    , m_sensor(new QHrmSensor(this))
{
    connect(m_sensor, SIGNAL(statusChanged(QHrmSensor::Status)),
            this, SIGNAL(statusChanged(QHrmSensor::Status)));
}

QmlHrmSensor::~QmlHrmSensor()
{
}

QmlSensorReading *QmlHrmSensor::createReading() const
{
    return new QmlHrmSensorReading(m_sensor);
}

QSensor *QmlHrmSensor::sensor() const
{
    return m_sensor;
}

/*!
    \qmlproperty qreal HrmSensor::status
    This property holds a value indicating the status of the heart rate monitor.

    Please see QHrmSensor::status for information about this property.
*/

QHrmSensor::Status QmlHrmSensor::status() const
{
    return m_sensor->status();
}

/*!
    \qmltype HrmReading
    \instantiates QmlHrmSensorReading
    \ingroup qml-sensors_reading
    \inqmlmodule QtSensors
    \since QtSensors 5.0
    \inherits SensorReading
    \brief The HrmReading element holds the most recent HrmSensor reading.

    The HrmReading element holds the most recent HrmSensor reading.

    This element wraps the QHrmReading class. Please see the documentation for
    QHrmReading for details.

    This element cannot be directly created.
*/

QmlHrmSensorReading::QmlHrmSensorReading(QHrmSensor *sensor)
    : QmlSensorReading(sensor)
    , m_sensor(sensor)
{
}

QmlHrmSensorReading::~QmlHrmSensorReading()
{
}

/*!
    \qmlproperty qreal HrmReading::bpm
    This property holds the number of beats per minute.

    Please see QHrmReading::bpm for information about this property.
*/

int QmlHrmSensorReading::bpm() const
{
    return m_bpm;
}

QSensorReading *QmlHrmSensorReading::reading() const
{
    return m_sensor->reading();
}

void QmlHrmSensorReading::readingUpdate()
{
    int bpm = m_sensor->reading()->bpm();
    if (m_bpm != bpm) {
        m_bpm = bpm;
        Q_EMIT bpmChanged();
    }
}
