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
      rightKeyScrollSpace(0)
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
            this->verticalSpeed   = 100 - 2 * configSpeed;
            this->horizontalSpeed = 100 - 2 * configSpeed;
        } else {
            error = true;
        }
    } else {
        error = true;
    }

    // Read the keys to send from te configuration
    QStringList keys = ((QString)mainStr.at(1)).split("+");
    size_t k = 0;

    foreach(QString key, keys) {
        if (key == "Control" || key == "Shift" || key == "Super" || key == "Alt") {
            key = key.append("_L");
            KeySym keySym = XStringToKeysym(key.toStdString().c_str());
            KeyCode keyCode = XKeysymToKeycode(QX11Info::display(), keySym);
            this->holdDownKeys.append(keyCode);

        } else if (key == "AltGr") {
            KeySym keySym = XStringToKeysym("Alt_R");
            KeyCode keyCode = XKeysymToKeycode(QX11Info::display(), keySym);
            this->holdDownKeys.append(keyCode);

        } else {
            KeySym keySym = XStringToKeysym(key.toStdString().c_str());
            KeyCode keyCode = XKeysymToKeycode(QX11Info::display(), keySym);
            if(k == 0){
                this->pressBetweenKeysUp.append(keyCode);
            }
            if(k == 1){
                this->pressBetweenKeysDown.append(keyCode);
            }
            k++;
        }
    }


    if (error) {
        qWarning() << "Error reading KEYSCROLL settings, using the default settings";
    }
    // Bring the window under the cursor to front, because only the window with the focus can receive keys
    XClientMessageEvent event;
    event.window = this-> window;
    event.type = ClientMessage;
    event.message_type = XInternAtom(QX11Info::display(), "_NET_ACTIVE_WINDOW", false);
    event.format = 32;
    event.data.l[0] = 2;
    event.data.l[1] = CurrentTime;
    event.data.l[2] = 0;

    XSendEvent(QX11Info::display(), QX11Info::appRootWindow(QX11Info::appScreen()), false,
            (SubstructureNotifyMask | SubstructureRedirectMask), (XEvent *)&event);
    XFlush(QX11Info::display());
}


// ****************************************************************************************************************** //
// **********                                        PUBLIC METHODS                                        ********** //
// ****************************************************************************************************************** //

void KeyScroll::executeStart(const QHash<QString, QVariant>& /*attrs*/) {
        for (int n = 0; n < this->holdDownKeys.length(); n++) {
        XTestFakeKeyEvent(QX11Info::display(), this->holdDownKeys.at(n), true, 0);
    }
    
}

void KeyScroll::executeUpdate(const QHash<QString, QVariant>& attrs)
{
    float deltaX = attrs.value(GEIS_GESTURE_ATTRIBUTE_DELTA_X).toFloat();
    float deltaY = attrs.value(GEIS_GESTURE_ATTRIBUTE_DELTA_Y).toFloat();

    // Vertical scroll
    if (deltaY > 0) {
        this->downKeyScrollSpace += (int)deltaY;

        while (this->downKeyScrollSpace >= this->verticalSpeed) {
            this->downKeyScrollSpace -= this->verticalSpeed;
            sendKeysDown();
            //DOWN
            XFlush(QX11Info::display());
        }

    } else {
        this->upKeyScrollSpace -= (int)deltaY;

        while (this->upKeyScrollSpace >= this->verticalSpeed) {
            this->upKeyScrollSpace -= this->verticalSpeed;
            sendKeysUp();
            //UP
            XFlush(QX11Info::display());
        }
    }   

    // Horizontal scroll
    if (deltaX > 0) {
        this->rightKeyScrollSpace += (int)deltaX;

        while (this->rightKeyScrollSpace >= this->horizontalSpeed) {
            this->rightKeyScrollSpace -= this->horizontalSpeed;
            //RIGHT
            XFlush(QX11Info::display());
        }

    } else {
        this->leftKeyScrollSpace -= (int)deltaX;

        while (this->leftKeyScrollSpace >= this->horizontalSpeed) {
            this->leftKeyScrollSpace -= this->horizontalSpeed;
            //LEFT
            XFlush(QX11Info::display());
        }
    }
}

void KeyScroll::executeFinish(const QHash<QString, QVariant>& /*attrs*/) {
        for (int n = 0; n < this->holdDownKeys.length(); n++) {
        XTestFakeKeyEvent(QX11Info::display(), this->holdDownKeys.at(n), false, 0);
    }   
}

void KeyScroll::sendKeysUp() {
    for (int n = 0; n < this->pressBetweenKeysUp.length(); n++) {
        XTestFakeKeyEvent(QX11Info::display(), this->pressBetweenKeysUp.at(n), true, 0);
        XTestFakeKeyEvent(QX11Info::display(), this->pressBetweenKeysUp.at(n), false, 0);
    }
}

void KeyScroll::sendKeysDown() {
    for (int n = 0; n < this->pressBetweenKeysDown.length(); n++) {
        XTestFakeKeyEvent(QX11Info::display(), this->pressBetweenKeysDown.at(n), true, 0);
        XTestFakeKeyEvent(QX11Info::display(), this->pressBetweenKeysDown.at(n), false, 0);
    }
}
