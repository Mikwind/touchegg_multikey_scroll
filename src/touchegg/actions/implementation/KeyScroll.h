/**
 * @file /src/touchegg/actions/implementation/KeyScroll.h
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
#ifndef KEYSCROLL_H
#define KEYSCROLL_H

#include "src/touchegg/util/Include.h"
#include "src/touchegg/actions/implementation/Action.h"

/**
 * Emulates the mouse whell.
 */
class KeyScroll : public Action
{

public:

    KeyScroll(const QString &settings, Window window);

    void executeStart(const QHash<QString, QVariant>& attrs);

    void executeUpdate(const QHash<QString, QVariant>& attrs);

    void executeFinish(const QHash<QString, QVariant>& attrs);

private:

    /**
     * KeyScroll speed. More is slower.
     */
    int horizontalSpeed;

    /**
     * KeyScroll speed. More is slower.
     */
    int verticalSpeed;

    //------------------------------------------------------------------------------------------------------------------

    /**
     * Accumulated space to scrolling up.
     */
    int upKeyScrollSpace;

    /**
     * Accumulated space to scrolling down.
     */
    int downKeyScrollSpace;

    /**
     * Accumulated space to scrolling left.
     */
    int leftKeyScrollSpace;

    /**
     * Accumulated space to scrolling right.
     */
    int rightKeyScrollSpace;

    //------------------------------------------------------------------------------------------------------------------

    /**
     * Button of the mouse to scroll up.
     */
    int buttonUp;

    /**
     * Button of the mouse to scroll down.
     */
    int buttonDown;

    /**
     * Button of the mouse to scroll left.
     */
    int buttonLeft;

    /**
     * Button of the mouse to scroll right.
     */
    int buttonRight;

};

#endif // KEYSCROLL_H
