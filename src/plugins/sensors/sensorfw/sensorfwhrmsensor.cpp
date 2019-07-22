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

#include "sensorfwhrmsensor.h"

char const * const SensorfwHrmSensor::id("sensorfw.hrmsensor");

SensorfwHrmSensor::SensorfwHrmSensor(QSensor *sensor)
    : SensorfwSensorBase(sensor)
    , m_initDone(false)
{
    init();
    setReading<QHrmReading>(&m_reading);
    m_sensor = (QHrmSensor *)sensor;
}

void SensorfwHrmSensor::slotDataAvailable(const HeartRate& data)
{
    m_reading.setBpm(data.heartRateData().bpm_);
    m_reading.setTimestamp(data.heartRateData().timestamp_);
    switch(data.heartRateData().status_) {
        case HrmUnreliable:
            m_sensor->setStatus(QHrmSensor::Unreliable);
            break;
        case HrmAccuracyLow:
            m_sensor->setStatus(QHrmSensor::AccuracyLow);
            break;
        case HrmAccuracyMedium:
            m_sensor->setStatus(QHrmSensor::AccuracyMedium);
            break;
        case HrmAccuracyHigh:
            m_sensor->setStatus(QHrmSensor::AccuracyHigh);
            break;
        case HrmNoContact:
            m_sensor->setStatus(QHrmSensor::NoContact);
    }
    newReadingAvailable();
}

bool SensorfwHrmSensor::doConnect()
{
    Q_ASSERT(m_sensorInterface);
    return QObject::connect(m_sensorInterface, SIGNAL(HeartRateChanged(HeartRate)),
                            this, SLOT(slotDataAvailable(HeartRate)));
}

QString SensorfwHrmSensor::sensorName() const
{
    return "hrmsensor";
}

void SensorfwHrmSensor::init()
{
    m_initDone = false;
    initSensor<HrmSensorChannelInterface>(m_initDone);
}

void SensorfwHrmSensor::start()
{
    if (reinitIsNeeded)
        init();
    SensorfwSensorBase::start();
}
