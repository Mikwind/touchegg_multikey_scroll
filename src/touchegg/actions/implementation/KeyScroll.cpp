/**
 * @file /src/touchegg/actions/implementation/KeyScroll.cpp
 *
 * This file is part of Touchégg.
 *
 * Touchégg is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License  as  published by  the  Free Software
 * Foundation,  either version 2 of the License,  or (at your option)  any later
 * version.
 *
 * Touchégg is distributed in the hope that it will be useful,  but  WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the  GNU General Public License  for more details.
 *
 * You should have received a copy of the  GNU General Public License along with
 * Touchégg. If not, see <http://www.gnu.org/licenses/>.
 *
 * @author José Expósito <jose.exposito89@gmail.com> (C) 2011 - 2012
 * @class  KeyScroll
 */
#include "KeyScroll.h"

// ****************************************************************************************************************** //
// **********                                  CONSTRUCTORS AND DESTRUCTOR                                 ********** //
// ****************************************************************************************************************** //

KeyScroll::KeyScroll(const QString &settings, Window window)
    : Action(settings, window),
      horizontalSpeed(30),
      verticalSpeed(30),
      upKeyScrollSpace(0),
      downKeyScrollSpace(0),
      leftKeyScrollSpace(0),
      rightKeyScrollSpace(0),
      buttonUp(4),
      buttonDown(5),
      buttonLeft(6),
      buttonRight(7)
{
    bool error = false;

    QStringList mainStr = settings.split(":");
    if (mainStr.length() != 2)
        error = true;

    // Speed
    QStringList mainStrSpeed = ((QString)mainStr.at(0)).split("=");
    if (!error && mainStrSpeed.length() == 2 && mainStrSpeed.at(0) == "SPEED") {
        bool ok;
        int configSpeed = QString(mainStrSpeed.at(1)).toInt(&ok);
        if (ok && configSpeed >= 1 && configSpeed <= 10) {
            this->verticalSpeed   = 40 - 2 * configSpeed;
            this->horizontalSpeed = 40 - 2 * configSpeed;
        } else {
            error = true;
        }
    } else {
        error = true;
    }

    // Inverted
    QStringList mainStrInv = mainStr.at(1).split("=");
    if (!error && mainStrInv.length() == 2 && mainStrInv.at(0) == "INVERTED") {
        if (mainStrInv.at(1) == "true") {
            this->buttonUp         = 5;
            this->buttonDown       = 4;
            this->buttonLeft       = 7;
            this->buttonRight      = 6;
        }
    } else {
        error = true;
    }


    if (error) {
        qWarning() << "Error reading KEYSCROLL settings, using the default settings";
    }
}


// ****************************************************************************************************************** //
// **********                                        PUBLIC METHODS                                        ********** //
// ****************************************************************************************************************** //

void KeyScroll::executeStart(const QHash<QString, QVariant>& /*attrs*/) {}

void KeyScroll::executeUpdate(const QHash<QString, QVariant>& attrs)
{
    float deltaX = attrs.value(GEIS_GESTURE_ATTRIBUTE_DELTA_X).toFloat();
    float deltaY = attrs.value(GEIS_GESTURE_ATTRIBUTE_DELTA_Y).toFloat();

    // Vertical scroll
    if (deltaY > 0) {
        this->downKeyScrollSpace += (int)deltaY;

        while (this->downKeyScrollSpace >= this->verticalSpeed) {
            this->downKeyScrollSpace -= this->verticalSpeed;
            XTestFakeButtonEvent(QX11Info::display(), this->buttonDown, true, 0);
            XTestFakeButtonEvent(QX11Info::display(), this->buttonDown, false, 0);
            XFlush(QX11Info::display());
        }

    } else {
        this->upKeyScrollSpace -= (int)deltaY;

        while (this->upKeyScrollSpace >= this->verticalSpeed) {
            this->upKeyScrollSpace -= this->verticalSpeed;
            XTestFakeButtonEvent(QX11Info::display(), this->buttonUp, true, 0);
            XTestFakeButtonEvent(QX11Info::display(), this->buttonUp, false, 0);
            XFlush(QX11Info::display());
        }
    }

    // Horizontal scroll
    if (deltaX > 0) {
        this->rightKeyScrollSpace += (int)deltaX;

        while (this->rightKeyScrollSpace >= this->horizontalSpeed) {
            this->rightKeyScrollSpace -= this->horizontalSpeed;
            XTestFakeButtonEvent(QX11Info::display(), this->buttonRight, true, 0);
            XTestFakeButtonEvent(QX11Info::display(), this->buttonRight, false, 0);
            XFlush(QX11Info::display());
        }

    } else {
        this->leftKeyScrollSpace -= (int)deltaX;

        while (this->leftKeyScrollSpace >= this->horizontalSpeed) {
            this->leftKeyScrollSpace -= this->horizontalSpeed;
            XTestFakeButtonEvent(QX11Info::display(), this->buttonLeft, true, 0);
            XTestFakeButtonEvent(QX11Info::display(), this->buttonLeft, false, 0);
            XFlush(QX11Info::display());
        }
    }
}

void KeyScroll::executeFinish(const QHash<QString, QVariant>& /*attrs*/) {}
