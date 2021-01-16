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

#include "qstepcountersensor.h"
#include "qstepcountersensor_p.h"

QT_BEGIN_NAMESPACE

IMPLEMENT_READING(QStepCounterReading)

/*!
    \class QStepCounterReading
    \ingroup sensors_reading
    \inmodule QtSensors
    \since 5.1

    \brief The QStepCounterReading class represents one reading from the
           step counter.

    \section2 QHemReading Units
    The step counter returns the user's steps since boot.
*/

/*!
    \property QStepCounterReading::steps
    \brief the steps since boot.

    \sa {QStepCounterReading Units}
*/

int QStepCounterReading::steps() const
{
    return d->steps;
}

/*!
    Sets the steps since boot to \a steps.
*/
void QStepCounterReading::setSteps(int steps)
{
    d->steps = steps;
}

// =====================================================================

/*!
    \class QStepCounterFilter
    \ingroup sensors_filter
    \inmodule QtSensors
    \since 5.1

    \brief The QStepCounterFilter class is a convenience wrapper around QSensorFilter.

    The only difference is that the filter() method features a pointer to QStepCounterReading
    instead of QSensorReading.
*/

/*!
    \fn QStepCounterFilter::filter(QStepCounterReading *reading)

    Called when \a reading changes. Returns false to prevent the reading from propagating.

    \sa QSensorFilter::filter()
*/

bool QStepCounterFilter::filter(QSensorReading *reading)
{
    return filter(static_cast<QStepCounterReading*>(reading));
}

char const * const QStepCounterSensor::type("QStepCounterSensor");

/*!
    \class QStepCounterSensor
    \ingroup sensors_type
    \inmodule QtSensors
    \since 5.1

    \brief The QStepCounterSensor class is a convenience wrapper around QSensor.

    The only behavioural difference is that this class sets the type properly.

    This class also features a reading() function that returns a QStepCounterReading instead of a QSensorReading.

    For details about how the sensor works, see \l QStepCounterReading.

    \sa QStepCounterReading
*/

/*!
    Construct the sensor as a child of \a parent.
*/
QStepCounterSensor::QStepCounterSensor(QObject *parent)
    : QSensor(QStepCounterSensor::type, parent)
{
}

/*!
    Destroy the sensor. Stops the sensor if it has not already been stopped.
*/
QStepCounterSensor::~QStepCounterSensor()
{
}

/*!
    \fn QStepCounterSensor::reading() const

    Returns the reading class for this sensor.

    \sa QSensor::reading()
*/

QStepCounterReading *QStepCounterSensor::reading() const
{
    return static_cast<QStepCounterReading*>(QSensor::reading());
}

#include "moc_qstepcountersensor.cpp"
QT_END_NAMESPACE

