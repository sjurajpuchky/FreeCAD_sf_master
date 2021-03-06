/***************************************************************************
 *   Copyright (c) 2009 J�rgen Riegel <juergen.riegel@web.de>              *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/


#include "PreCompiled.h"

#ifndef _PreComp_
#endif

#include "ui_TaskSketcherGeneral.h"
#include "TaskSketcherGeneral.h"
#include <Gui/Application.h>
#include <Gui/Document.h>
#include <Gui/BitmapFactory.h>
#include <Gui/ViewProvider.h>
#include <Gui/WaitCursor.h>
#include <Base/UnitsApi.h>

#include "ViewProviderSketch.h"

using namespace SketcherGui;
using namespace Gui::TaskView;

SketcherGeneralWidget::SketcherGeneralWidget(QWidget *parent)
  : QWidget(parent), ui(new Ui_TaskSketcherGeneral)
{
    ui->setupUi(this);

    // connecting the needed signals
    connect(ui->checkBoxShowGrid, SIGNAL(toggled(bool)),
            this, SLOT(toggleGridView(bool)));
    connect(ui->checkBoxGridSnap, SIGNAL(stateChanged(int)),
            this, SLOT(toggleGridSnap(int)));
    connect(ui->comboBoxGridSize, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(setGridSize(QString)));
    connect(ui->checkBoxAutoconstraints, SIGNAL(stateChanged(int)),
            this, SIGNAL(emitToggleAutoconstraints(int)));
}

SketcherGeneralWidget::~SketcherGeneralWidget()
{
    delete ui;
}

void SketcherGeneralWidget::saveSettings()
{
    Base::Reference<ParameterGrp> hGrp = App::GetApplication().GetUserParameter()
        .GetGroup("BaseApp")->GetGroup("Preferences")->GetGroup("Mod/Sketcher/General");
    hGrp->SetBool("ShowGrid", ui->checkBoxShowGrid->isChecked());

    QString size = ui->comboBoxGridSize->currentText();
    hGrp->SetASCII("GridSize", (const char*)size.toAscii());

    hGrp->SetBool("GridSnap", ui->checkBoxGridSnap->isChecked());
    hGrp->SetBool("AutoConstraints", ui->checkBoxAutoconstraints->isChecked());
}

void SketcherGeneralWidget::loadSettings()
{
    Base::Reference<ParameterGrp> hGrp = App::GetApplication().GetUserParameter()
        .GetGroup("BaseApp")->GetGroup("Preferences")->GetGroup("Mod/Sketcher/General");
    ui->checkBoxShowGrid->setChecked(hGrp->GetBool("ShowGrid", true));

    fillGridCombo();
    QString size = ui->comboBoxGridSize->currentText();
    size = QString::fromAscii(hGrp->GetASCII("GridSize", (const char*)size.toAscii()).c_str());
    int it = ui->comboBoxGridSize->findText(size);
    if (it != -1) ui->comboBoxGridSize->setCurrentIndex(it);

    ui->checkBoxGridSnap->setChecked(hGrp->GetBool("GridSnap", ui->checkBoxGridSnap->isChecked()));
    ui->checkBoxAutoconstraints->setChecked(hGrp->GetBool("AutoConstraints", ui->checkBoxAutoconstraints->isChecked()));
}

void SketcherGeneralWidget::fillGridCombo(void)
{
    if (Base::UnitsApi::getSchema() == Base::Imperial1) {
        ui->comboBoxGridSize->addItem(QString::fromUtf8("1/1000 [thou] \""));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("1/128 \""));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("1/100 \""));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("1/64 \""));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("1/32 \""));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("1/16 \""));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("1/8 \""));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("1/4 \""));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("1/2 \""));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("1 \""));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("2 \""));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("4 \""));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("8 \""));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("12 \" [foot]"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("16 \""));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("36 \" [yard]"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("72 \" [2 yards]"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("144 \" [4 yards]"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("396 \" [half chain]"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("792 \" [chain]"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("792 \" [2 chains]"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("1584 \" [4 chains]"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("3960 \" [half furlong]"));

        ui->comboBoxGridSize->setCurrentIndex(ui->comboBoxGridSize->findText(QString::fromUtf8("1/4 \"")));
    }
    else {
        ui->comboBoxGridSize->addItem(QString::fromUtf8("1 \xC2\xB5m"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("2 \xC2\xB5m"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("5 \xC2\xB5m"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("10 \xC2\xB5m"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("20 \xC2\xB5m"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("50 \xC2\xB5m"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("0.1 mm"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("0.2 mm"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("0.5 mm"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("1 mm"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("2 mm"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("5 mm"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("10 mm"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("20 mm"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("50 mm"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("100 mm"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("200 mm"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("500 mm"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("1 m"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("2 m"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("5 m"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("10 m"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("20 m"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("50 m"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("100 m"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("200 m"));
        ui->comboBoxGridSize->addItem(QString::fromUtf8("500 m"));

        ui->comboBoxGridSize->setCurrentIndex(ui->comboBoxGridSize->findText(QString::fromUtf8("10 mm")));
    }
}

void SketcherGeneralWidget::toggleGridView(bool on)
{
    ui->label->setEnabled(on);
    ui->comboBoxGridSize->setEnabled(on);
    ui->checkBoxGridSnap->setEnabled(on);
    emitToggleGridView(on);
}

void SketcherGeneralWidget::setGridSize(const QString& val)
{
    emitSetGridSize(val);
}

void SketcherGeneralWidget::toggleGridSnap(int state)
{
    setGridSize(ui->comboBoxGridSize->currentText()); // Ensure consistency
    emitToggleGridSnap(state);
}

void SketcherGeneralWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    if (e->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
}

// ----------------------------------------------------------------------------

TaskSketcherGeneral::TaskSketcherGeneral(ViewProviderSketch *sketchView)
    : TaskBox(Gui::BitmapFactory().pixmap("document-new"),tr("Edit controls"),true, 0)
    , sketchView(sketchView)
{
    // we need a separate container widget to add all controls to
    widget = new SketcherGeneralWidget(this);
    this->groupLayout()->addWidget(widget);

    // connecting the needed signals
    QObject::connect(
        widget, SIGNAL(emitToggleGridView(bool)),
        this  , SLOT  (toggleGridView(bool))
        );
    QObject::connect(
        widget, SIGNAL(emitToggleGridSnap(int)),
        this  , SLOT  (toggleGridSnap(int))
       );

    QObject::connect(
        widget, SIGNAL(emitSetGridSize(QString)),
        this  , SLOT  (setGridSize(QString))
       );

    QObject::connect(
        widget, SIGNAL(emitToggleAutoconstraints(int)),
        this  , SLOT  (toggleAutoconstraints(int))
       );
    
    Gui::Selection().Attach(this);
    widget->loadSettings();
}

TaskSketcherGeneral::~TaskSketcherGeneral()
{
    widget->saveSettings();
    Gui::Selection().Detach(this);
}

void TaskSketcherGeneral::toggleGridView(bool on)
{
    sketchView->ShowGrid.setValue(on);
}

void TaskSketcherGeneral::setGridSize(const QString& val)
{
    float gridSize = (float) Base::Quantity::parse(val).getValue();
    if (gridSize > 0)
        sketchView->GridSize.setValue(gridSize);
}

void TaskSketcherGeneral::toggleGridSnap(int state)
{
    sketchView->GridSnap.setValue(state == Qt::Checked);
}

void TaskSketcherGeneral::toggleAutoconstraints(int state)
{
    sketchView->Autoconstraints.setValue(state == Qt::Checked);
}

/// @cond DOXERR
void TaskSketcherGeneral::OnChange(Gui::SelectionSingleton::SubjectType &rCaller,
                              Gui::SelectionSingleton::MessageType Reason)
{
    //if (Reason.Type == SelectionChanges::AddSelection ||
    //    Reason.Type == SelectionChanges::RmvSelection ||
    //    Reason.Type == SelectionChanges::SetSelection ||
    //    Reason.Type == SelectionChanges::ClrSelection) {
    //}
}
/// @endcond DOXERR

#include "moc_TaskSketcherGeneral.cpp"
