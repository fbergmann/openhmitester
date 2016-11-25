// -*- mode: c++; c-basic-offset: 4; c-basic-style: bsd; -*-
/*
 *   This program is free software; you can redistribute it and/or
 *   modify
 *   it under the terms of the GNU Lesser General Public License as
 *   published by the Free Software Foundation; either version 3.0 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *   02111-1307 USA
 *
 *   This file is part of the Open-HMI Tester,
 *   http://openhmitester.sourceforge.net
 *
 */

#include "qteventexecutor.h"
#include <debug.h>
#include <qwidgetutils.h>
#include <ohtbaseconfig.h>
#include <QWidget>

QtEventExecutor::QtEventExecutor()
    : EventExecutor()
    , MOUSE_MOVE_DELAY_MS(400)
    , MOUSE_MOVE_STEPS(18)

{
    //update flags
    f_executing_ = false;
    f_paused_ = false;
}

///
/// this method is called to install the event executor
///
void QtEventExecutor::install()
{
    //it does not an installation process
    DEBUG(D_EXECUTOR,"(QtEventExecutor::install) Event Executor installed.");
}

///
///execution process control methods
///

void QtEventExecutor::startExecution()
{
    //update flags
    f_executing_ = true;
    f_paused_ = false;

    // execution starts with mouse at 0,0
    _last_mouse_pos = QApplication::activeWindow()->mapToGlobal(QPoint(0,0));
    QCursor::setPos(_last_mouse_pos);
}

void QtEventExecutor::pauseExecution()
{
    //update flags
    f_executing_ = true;
    f_paused_ = true;
}

void QtEventExecutor::resumeExecution()
{
    //update flags
    f_executing_ = true;
    f_paused_ = true;
}

void QtEventExecutor::stopExecution()
{
    //update flags
    f_executing_ = false;
    f_paused_ = false;
}

///
/// this method is called when a new testItem arrives
///
void QtEventExecutor::handleNewTestItemReceived(DataModel::TestItem* ti)
{
    ///
    /// process control
    ///

    //if process is stoped...
    if (f_executing_ == false)
    {
        //no event executing
        DEBUG(D_EXECUTOR,"(QtEventExecutor::handleNewTestItemReceived) Stop state. No event executing.");
        return ;
    }
    //if process is paused...
    else if (f_executing_ && f_paused_)
    {
        //no event executing
        DEBUG(D_EXECUTOR,"(QtEventExecutor::handleNewTestItemReceived) Pause state. No event executing.");
        return ;
    }
    //if process is executing...
    else
    {
        //execute the event...
    }

    ///
    ///depending on the type..
    ///
    //window events
    if (ti->type() == QOE::QOE_WINDOW_CLOSE)
    {
        //executeCloseEvent(dynamic_cast<QOE::QOE_WindowClose*>(ti));
        QOE::QOE_WindowClose qoe;
        qoe.copy(ti);
        executeCloseEvent(&qoe);
    }
    //mouse events
    else if (ti->type() == QOE::QOE_MOUSE_PRESS)
    {
        //executeMousePressEvent(dynamic_cast<QOE::QOE_MousePress*>(ti));
        QOE::QOE_MousePress qoe;
        qoe.copy(ti);
        executeMousePressEvent(&qoe);
    }
    else if (ti->type() == QOE::QOE_MOUSE_RELEASE)
    {
        //executeMouseReleaseEvent(dynamic_cast<QOE::QOE_MouseRelease*>(ti));
        QOE::QOE_MouseRelease qoe;
        qoe.copy(ti);
        executeMouseReleaseEvent(&qoe);
    }
    else if (ti->type() == QOE::QOE_MOUSE_DOUBLE)
    {
        //executeMouseDoubleEvent(dynamic_cast<QOE::QOE_MouseDouble*>(ti));
        QOE::QOE_MouseDouble qoe;
        qoe.copy(ti);
        executeMouseDoubleEvent(&qoe);
    }
    else if (ti->type() == QOE::QOE_MOUSE_WHEEL)
    {
        //executeWheelEvent(dynamic_cast<QOE::QOE_MouseWheel*>(ti));
        QOE::QOE_MouseWheel qoe;
        qoe.copy(ti);
        executeWheelEvent(&qoe);
    }
    //keyboard events
    else if (ti->type() == QOE::QOE_KEY_PRESS)
    {
        //executeKeyPressEvent(dynamic_cast<QOE::QOE_KeyPress*>(ti));
        QOE::QOE_KeyPress qoe;
        qoe.copy(ti);
        executeKeyPressEvent(&qoe);
    }
}

///
/// event executors
///

void QtEventExecutor::executeMousePressEvent(QOE::QOE_MousePress* qoe)
{
    DEBUG(D_EXECUTOR,"(QtEventExecutor::executeMousePressEvent)");
    assert(qoe);

    QWidget* widget = _getWidget(qoe);
    if ( widget == NULL )
    {
        DEBUG(D_ERROR,"(QtEventExecutor::executeMousePressEvent) Missing Widget: "
              << qoe->widget());

    }

    /// Note that the widget might be NULL. Please, be careful!!

    DEBUG(D_EXECUTOR,"(QtEventExecutor::executeMousePressEvent) Pre execution");
    _preExecutionWithMouseMove(qoe, widget);
    DEBUG(D_EXECUTOR,"(QtEventExecutor::executeMousePressEvent) Execute.");
    qoe->execute(widget);
    DEBUG(D_EXECUTOR,"(QtEventExecutor::executeMousePressEvent) Post execution.");
    _postExecution(qoe, widget);

    DEBUG(D_EXECUTOR,"(QtEventExecutor::executeMousePressEvent) Exit");
}

void QtEventExecutor::executeMouseReleaseEvent(QOE::QOE_MouseRelease* qoe)
{
    DEBUG(D_EXECUTOR,"(QtEventExecutor::executeMouseReleaseEvent)");

    //get the widget
    QWidget* widget = _getWidget(qoe);
    if ( widget == NULL )
    {
        DEBUG(D_ERROR,"(QtEventExecutor::executeMouseReleaseEvent) Missing Widget: "
              << qoe->widget());
    }

    _preExecutionWithMouseMove(qoe, widget);
    qoe->execute(widget);
    _postExecution(qoe, widget);
}

void QtEventExecutor::executeMouseDoubleEvent(QOE::QOE_MouseDouble* qoe)
{
    DEBUG(D_EXECUTOR,"(QtEventExecutor::executeMouseDoubleEvent)");
    //get the widget
    QWidget* widget = _getWidget(qoe);
    if ( widget == NULL )
    {
        DEBUG(D_ERROR,"(QtEventExecutor::executeMouseDoubleEvent) Missing Widget: "
              << qoe->widget());
    }


    _preExecutionWithMouseMove(qoe, widget);
    qoe->execute(widget);
    _postExecution(qoe, widget);
}

void QtEventExecutor::executeKeyPressEvent(QOE::QOE_KeyPress* qoe)
{
    DEBUG(D_EXECUTOR,"(QtEventExecutor::executeKeyPressEvent)");
    //get the widget
    QWidget* widget = _getWidget(qoe);
    if ( widget == NULL )
    {
        DEBUG(D_ERROR,"(QtEventExecutor::executeKeyPressEvent) Missing Widget: "
              << qoe->widget());
    }

    _preExecutionNoMouse(qoe, widget);
    qoe->execute(widget);
    _postExecution(qoe, widget);
}

void QtEventExecutor::executeCloseEvent(QOE::QOE_WindowClose* qoe)
{
    DEBUG(D_EXECUTOR,"(QtEventExecutor::executeCloseEvent)");
    //get the widget
    QWidget* widget = _getWidget(qoe);
    if ( widget == NULL )
    {
        DEBUG(D_ERROR,"(QtEventExecutor::executeWindowCloseEvent) Missing Widget: "
              << qoe->widget());
    }

    _preExecutionNoMouse(qoe, widget);
    qoe->execute(widget);
    _postExecution(qoe, widget);
}

void QtEventExecutor::executeWheelEvent(QOE::QOE_MouseWheel* qoe)
{
    DEBUG(D_EXECUTOR,"(QtEventExecutor::executeWheelEvent)");
    //get the widget
    QWidget* widget = _getWidget(qoe);
    if ( widget == NULL )
    {
        DEBUG(D_ERROR,"(QtEventExecutor::executeMouseWheelEvent) Missing Widget: "
              << qoe->widget());
    }

    _preExecutionWithMouseMove(qoe, widget);
    qoe->execute(widget);
    _postExecution(qoe, widget);
}

//void executeKeyReleaseEvent ();
//void executeShowEvent ();
//void executeSpecialShowEvent ();
//void executeFocusInEvent ();
//void executeFocusOutEvent ();

///
///
///
/// private methods
///
///
///

///
/// mouse simulation
///

/*
void QtEventExecutor::_simulateMouseMove ( const QPoint& pBegin, const QPoint& pEnd )
{
    int ix = pBegin.x();
    int iy = pBegin.y();
    int dx = pEnd.x();
    int dy = pEnd.y();

    //if no moving...
    if ( ix == dx && iy == dy ) return;

    //Initial position: QCursor::pos()
    //End position: pEnd
    //Movements: MOUSE_MOVE_STEPS
    //Wait between movements: w


    //adapt n and w values
    uint t = qMax ( qAbs ( dx - ix ), qAbs ( dy - iy ) );//longest movement
    uint w = MOUSE_MOVE_DELAY_MS / MOUSE_MOVE_STEPS;
    assert ( w > 0 );

    //go moving
    QPoint old ( ix, iy );
    const int STEP_X = int ( double ( dx - ix ) / double ( MOUSE_MOVE_STEPS ) );
    const int STEP_Y = int ( double ( dy - iy ) / double ( MOUSE_MOVE_STEPS ) );
    for ( uint i = 1; i < n; i++ )
    {
        QPoint current( STEP_X * i + ix, STEP_Y * i + iy );
        QCursor::setPos(current);
        QTest::qWait(w);
    }

    //end moving
    QCursor::setPos ( pEnd );
}

void QtEventExecutor::_simulateMouseHover ( QWidget *widget,
                                            const QPoint &pBegin,
                                            const QPoint &pEnd )
{
    int ix = pBegin.x();
    int iy = pBegin.y();
    int dx = pEnd.x();
    int dy = pEnd.y();

    //if no moving...
    if ( ix == dx && iy == dy ) return;

    //Initial position: QCursor::pos()
    //End position: pEnd
    //Movements: MOUSE_MOVE_STEPS
    //Wait between movements: w


    //adapt n and w values
    uint t = qMax ( qAbs ( dx - ix ), qAbs ( dy - iy ) );//longest movement
    uint w = MOUSE_MOVE_DELAY_MS / MOUSE_MOVE_STEPS;
    assert ( w > 0 );

    //go moving...
    QPoint old ( ix, iy );
    const int STEP_X = int ( double ( dx - ix ) / double ( MOUSE_MOVE_STEPS ) );
    const int STEP_Y = int ( double ( dy - iy ) / double ( MOUSE_MOVE_STEPS ) );
    for ( uint i = 1; i <= MOUSE_MOVE_STEPS; i++ )
    {
        //calculate current position
        int cx = STEP_X * i  + ix;
        int cy = STEP_Y * i  + iy;
        QPoint current ( cx, cy );

        //set the cursor
        QCursor::setPos ( current );
        QMouseEvent me ( QEvent::MouseMove,
                         widget->mapFromGlobal ( current ),
                         Qt::NoButton, Qt::LeftButton,
                         QApplication::keyboardModifiers() );
        qApp->notify ( dynamic_cast<QObject*> ( widget ),
                       dynamic_cast<QEvent*> ( &me ) );

        //wait
        QTest::qWait ( w );
    }

    //end moving...
    QCursor::setPos ( pEnd );
    QMouseEvent me ( QEvent::MouseMove,
                     widget->mapFromGlobal ( pEnd ),
                     Qt::NoButton, Qt::LeftButton,
                     QApplication::keyboardModifiers() );
    qApp->notify ( ( QObject* ) widget, ( QEvent* ) &me );
}*/



void QtEventExecutor::_simulateMouseMove(const QPoint &pBegin,
                                         const QPoint &pEnd,
                                         QWidget* hoverOnWidget)
{
    int ix = pBegin.x();
    int iy = pBegin.y();
    int dx = pEnd.x();
    int dy = pEnd.y();

    //if no moving...
    if ( ix == dx && iy == dy ) return;

    /*
     Initial position: QCursor::pos()
     End position: pEnd
     Movements: MOUSE_MOVE_STEPS
     Wait between movements: w
   */

    //adapt n and w values
    //uint t = qMax ( qAbs ( dx - ix ), qAbs ( dy - iy ) );//longest movement
    uint w = MOUSE_MOVE_DELAY_MS / MOUSE_MOVE_STEPS;
    assert ( w > 0 );

    //go moving...
    //QPoint old ( ix, iy );
    const int STEP_X = int ( double ( dx - ix ) / double ( MOUSE_MOVE_STEPS ) );
    const int STEP_Y = int ( double ( dy - iy ) / double ( MOUSE_MOVE_STEPS ) );
    for ( uint i = 1; i <= (uint)MOUSE_MOVE_STEPS; ++i )
    {
        //calculate current position
        int cx = STEP_X * i  + ix;
        int cy = STEP_Y * i  + iy;
        QPoint current ( cx, cy );

        //set the cursor
        QCursor::setPos ( current );

        // hover on widget? (for drag and drop)
        if (hoverOnWidget){
            QMouseEvent me ( QEvent::MouseMove,
                             hoverOnWidget->mapFromGlobal ( current ),
                             Qt::NoButton, Qt::LeftButton,
                             QApplication::keyboardModifiers() );
            qApp->notify ( dynamic_cast<QObject*> ( hoverOnWidget ),
                           dynamic_cast<QEvent*> ( &me ) );
        }

        //wait
        QTest::qWait ( w );
    }

    //end moving...
    QCursor::setPos ( pEnd );
    if (hoverOnWidget){
        QMouseEvent me ( QEvent::MouseMove,
                         hoverOnWidget->mapFromGlobal ( pEnd ),
                         Qt::NoButton, Qt::LeftButton,
                         QApplication::keyboardModifiers() );
        qApp->notify ( ( QObject* ) hoverOnWidget, ( QEvent* ) &me );
    }
}

///
/// execution support
///

QWidget* QtEventExecutor::_getWidget(QOE::QOE_Base* qoe)
{
    assert(qoe);
    QString wname = QString(qoe->widget().c_str());
    QStringList wpath = wname.split ( PATH_SEPARATOR );
    return QWidgetUtils::getAWidget(&wpath);
}


void QtEventExecutor::_preExecutionNoMouse(QOE::QOE_Base* qoe, QWidget* widget)
{
    assert(qoe);

    //wait elapsed time for this item
    int sleep_ms = qoe->timestamp();
    sleep_ms = sleep_ms > 0? sleep_ms : 0;
    QTest::qWait(sleep_ms);

    //start simulation
    if (widget != NULL){
        widget->setUpdatesEnabled(false);
    }
}

void QtEventExecutor::_preExecutionWithMouseMove(QOE::QOE_Base* qoe, QWidget* widget)
{
    assert(qoe);

    //wait elapsed time for this item
    int sleep_ms = qoe->timestamp() - MOUSE_MOVE_DELAY_MS;
    sleep_ms = sleep_ms > 0? sleep_ms : 0;
    QTest::qWait(sleep_ms);

    // do mouse move
    if (widget != NULL){

        //QPoint target = widget->mapToGlobal(qoe->position());
        QPoint target = qoe->adaptedGlobalPosition(widget);
        _simulateMouseMove(_last_mouse_pos, target);
        _last_mouse_pos = target;
        //_simulateMouseMove(_last_mouse_pos, );

    }

    //start simulation
    if (widget != NULL){
        widget->setUpdatesEnabled(false);
    }
}

void QtEventExecutor::_preExecutionWithMouseHover(QOE::QOE_Base* qoe, QWidget* widget)
{
    assert(qoe);

    //wait elapsed time for this item
    int sleep_ms = qoe->timestamp() - MOUSE_MOVE_DELAY_MS;
    sleep_ms = sleep_ms > 0? sleep_ms : 0;
    QTest::qWait(sleep_ms);

    // do mouse hover
    if (widget != NULL){
        //_simulateMouseMove(_last_mouse_pos, widget->mapToGlobal(qoe->position()), widget);
        //_last_mouse_pos = widget->mapToGlobal ( qoe->position() );
        QPoint target = qoe->adaptedGlobalPosition(widget);
        _simulateMouseMove(_last_mouse_pos, target, widget);
        _last_mouse_pos = target;
    }

    //start simulation
    if (widget != NULL){
        widget->setUpdatesEnabled(false);
    }
}

void QtEventExecutor::_postExecution(QOE::QOE_Base* qoe, QWidget* widget)
{
    if (qoe && widget){

        //if sensitive -> set  sensitive data
        if ( qoe->isSensitive() )
        {
            QWA::QWidgetAdapter* qwa = qwaManager_.isSensitive(widget);
            qwa->applySensitiveValue(qoe->sensitiveValue());
        }

        //set focus
        QWidgetUtils::setFocusOnWidget(widget);

        //end simulation
        widget->setUpdatesEnabled ( true );
        widget->update();
    }
}

