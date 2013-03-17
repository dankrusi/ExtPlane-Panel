#include "menubutton.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QSettings>
#include <QDebug>
#include <QCoreApplication>
#include <QCheckBox>
#include "panelitems/panelitem.h"
#include "dialogs/settingsdialog.h"
#include "dialogs/edititemdialog.h"

MenuButton::MenuButton(QWidget *parent) : QObject(parent), side(20) {
    // Init
    panelWindow = parent;
#ifdef MOBILE_DEVICE
    side = 50;
#endif
    editMode = false;
    currentMenu = NULL;
}

void MenuButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(Qt::NoBrush);
    painter->setPen(Qt::yellow);
    //painter->drawRect(0,0,side,side);
    painter->drawLine(0,side,side,0);
}

QRectF MenuButton::boundingRect() const {
    return QRectF(0, 0 , side, side);
}

void MenuButton::mousePressEvent ( QGraphicsSceneMouseEvent * event ) {
    QGraphicsItem::mousePressEvent(event);
    if(event->button() != Qt::LeftButton) return;
    if(currentMenu) {
        currentMenu->setFocus(); //BUG: doesnt seem to work
        return;
    }

    QDialog *menu = new QDialog(panelWindow);
    menu->move(event->screenPos().x(), event->screenPos().y());
    QVBoxLayout *layout = new QVBoxLayout();

    QCheckBox *editModeCheck = new QCheckBox("Edit Panel", menu);
    editModeCheck->setChecked(editMode);
    connect(editModeCheck, SIGNAL(clicked(bool)), panelWindow, SLOT(setEditMode(bool)));
    connect(editModeCheck, SIGNAL(clicked(bool)), this, SLOT(setEditMode(bool)));

    layout->addWidget(editModeCheck);
    QPushButton *addButton = new QPushButton("Add Item", menu);
    connect(addButton, SIGNAL(clicked()), panelWindow, SLOT(showAddItemDialog()));
    layout->addWidget(addButton);

    QPushButton *saveButton = new QPushButton("Save Panel", menu);
    connect(saveButton, SIGNAL(clicked()), panelWindow, SLOT(savePanel()));
    layout->addWidget(saveButton);

    QPushButton *loadButton = new QPushButton("Load Panel", menu);
    connect(loadButton, SIGNAL(clicked()), panelWindow, SLOT(loadPanel()));
    layout->addWidget(loadButton);

    QPushButton *newButton = new QPushButton("New Panel", menu);
    connect(newButton, SIGNAL(clicked()), panelWindow, SLOT(newPanel()));
    layout->addWidget(newButton);

    QPushButton *hardwareButton = new QPushButton("Hardware", menu);
    connect(hardwareButton, SIGNAL(clicked()), panelWindow, SLOT(showHardware()));
    layout->addWidget(hardwareButton);

    QPushButton *settingsButton = new QPushButton("App Settings", menu);
    connect(settingsButton, SIGNAL(clicked()), panelWindow, SLOT(showSettings()));
    connect(settingsButton, SIGNAL(clicked()), this, SLOT(closeCurrentMenu()));
    layout->addWidget(settingsButton);

    QPushButton *closeButton = new QPushButton("Close", menu);
    connect(closeButton, SIGNAL(clicked()), this, SLOT(closeCurrentMenu()));
    layout->addWidget(closeButton);

    QPushButton *quitButton = new QPushButton("Quit", menu);
    connect(quitButton, SIGNAL(clicked()), panelWindow, SLOT(quit()));
    layout->addWidget(quitButton);

    currentMenu = menu;
    connect(currentMenu, SIGNAL(finished(int)), this, SLOT(closeCurrentMenu()));

    menu->setLayout(layout);
    menu->setModal(false);
    menu->show();
}

void MenuButton::setEditMode(bool em) {
    editMode = em; // @todo Set the checkbox in UI
}

void MenuButton::closeCurrentMenu() {
    if(currentMenu) {
        currentMenu->deleteLater();
        currentMenu = NULL;
    }
}

