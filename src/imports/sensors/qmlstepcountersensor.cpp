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

#include "qmlstepcountersensor.h"

/*!
    \qmltype StepCounterSensor
    \instantiates QmlStepCounterSensor
    \ingroup qml-sensors_type
    \inqmlmodule QtSensors
    \since QtSensors 5.0
    \inherits Sensor
    \brief The StepCounterSensor element reports on the user's steps since boot.

    The StepCounterSensor element reports on the user's steps since boot.

    This element wraps the QStepCounterSensor class. Please see the documentation for
    QStepCounterSensor for details.

    \sa StepCounterReading
*/

QmlStepCounterSensor::QmlStepCounterSensor(QObject *parent)
    : QmlSensor(parent)
    , m_sensor(new QStepCounterSensor(this))
{
}

QmlStepCounterSensor::~QmlStepCounterSensor()
{
}

QmlSensorReading *QmlStepCounterSensor::createReading() const
{
    return new QmlStepCounterSensorReading(m_sensor);
}

QSensor *QmlStepCounterSensor::sensor() const
{
    return m_sensor;
}

/*!
    \qmltype StepCounterReading
    \instantiates QmlStepCounterSensorReading
    \ingroup qml-sensors_reading
    \inqmlmodule QtSensors
    \since QtSensors 5.0
    \inherits SensorReading
    \brief The StepCounterReading element holds the most recent StepCounterSensor reading.

    The StepCounterReading element holds the most recent StepCounterSensor reading.

    This element wraps the QStepCounterReading class. Please see the documentation for
    QStepCounterReading for details.

    This element cannot be directly created.
*/

QmlStepCounterSensorReading::QmlStepCounterSensorReading(QStepCounterSensor *sensor)
    : QmlSensorReading(sensor)
    , m_sensor(sensor)
{
}

QmlStepCounterSensorReading::~QmlStepCounterSensorReading()
{
}

/*!
    \qmlproperty qreal StepCounterReading::steps
    This property holds the number of steps since boot.

    Please see QStepCounterReading::steps for information about this property.
*/

int QmlStepCounterSensorReading::steps() const
{
    return m_steps;
}

QSensorReading *QmlStepCounterSensorReading::reading() const
{
    return m_sensor->reading();
}

void QmlStepCounterSensorReading::readingUpdate()
{
    int steps = m_sensor->reading()->steps();
    if (m_steps != steps) {
        m_steps = steps;
        Q_EMIT stepsChanged();
    }
}
