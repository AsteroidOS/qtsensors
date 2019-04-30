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

#include "qhrmsensor.h"
#include "qhrmsensor_p.h"

QT_BEGIN_NAMESPACE

IMPLEMENT_READING(QHrmReading)

/*!
    \class QHrmReading
    \ingroup sensors_reading
    \inmodule QtSensors
    \since 5.1

    \brief The QHrmReading class represents one reading from the
           heart rate monitor.

    \section2 QHemReading Units
    The heart rate monitor returns the user's heart rate in beats per minute.
*/

/*!
    \property QHrmReading::bpm
    \brief the beats per minute.

    Measured as Hz.
    \sa {QHrmReading Units}
*/

int QHrmReading::bpm() const
{
    return d->bpm;
}

/*!
    Sets the beats per minute to \a bpm.
*/
void QHrmReading::setBpm(int bpm)
{
    d->bpm = bpm;
}

// =====================================================================

/*!
    \class QHrmFilter
    \ingroup sensors_filter
    \inmodule QtSensors
    \since 5.1

    \brief The QHrmFilter class is a convenience wrapper around QSensorFilter.

    The only difference is that the filter() method features a pointer to QHrmReading
    instead of QSensorReading.
*/

/*!
    \fn QHrmFilter::filter(QHrmReading *reading)

    Called when \a reading changes. Returns false to prevent the reading from propagating.

    \sa QSensorFilter::filter()
*/

bool QHrmFilter::filter(QSensorReading *reading)
{
    return filter(static_cast<QHrmReading*>(reading));
}

char const * const QHrmSensor::type("QHrmSensor");

/*!
    \class QHrmSensor
    \ingroup sensors_type
    \inmodule QtSensors
    \since 5.1

    \brief The QHrmSensor class is a convenience wrapper around QSensor.

    The only behavioural difference is that this class sets the type properly.

    This class also features a reading() function that returns a QHrmReading instead of a QSensorReading.

    For details about how the sensor works, see \l QHrmReading.

    \sa QHrmReading
*/

/*!
    Construct the sensor as a child of \a parent.
*/
QHrmSensor::QHrmSensor(QObject *parent)
    : QSensor(QHrmSensor::type, *new QHrmSensorPrivate, parent)
{
}

/*!
    Destroy the sensor. Stops the sensor if it has not already been stopped.
*/
QHrmSensor::~QHrmSensor()
{
}

/*!
    \fn QHrmSensor::reading() const

    Returns the reading class for this sensor.

    \sa QSensor::reading()
*/

QHrmReading *QHrmSensor::reading() const
{
    return static_cast<QHrmReading*>(QSensor::reading());
}

/*!
    \property QHrmSensor::status
    \brief a value indicating the status of the heart rate monitor.

    This is an enum that represents the status of the sensor.
*/

QHrmSensor::Status QHrmSensor::status() const
{
    Q_D(const QHrmSensor);
    return d->status;
}

/*!
    \since 5.1

    Sets the status to \a status. This is to be called from the backend.
*/
void QHrmSensor::setStatus(QHrmSensor::Status status)
{
    Q_D(QHrmSensor);
    if (d->status != status) {
        d->status = status;
        emit statusChanged(status);
    }
}

#include "moc_qhrmsensor.cpp"
QT_END_NAMESPACE

