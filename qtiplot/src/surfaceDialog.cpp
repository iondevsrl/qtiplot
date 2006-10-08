/***************************************************************************
    File                 : surfaceDialog.cpp
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2006 by Ion Vasilief
    Email                : ion_vasilief@yahoo.fr
    Description          : Define surface plot dialog
                           
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor,                    *
 *   Boston, MA  02110-1301  USA                                           *
 *                                                                         *
 ***************************************************************************/
#include "surfaceDialog.h"
#include "parser.h"
#include "application.h"

#include <QMessageBox>
#include <QGridLayout>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton.h>
#include <QLabel.h>
#include <QLineEdit.h>
#include <QComboBox.h>

SurfaceDialog::SurfaceDialog( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "SurfaceDialog" );
	setWindowTitle(tr("QtiPlot - Define surface plot"));
    setSizeGripEnabled( true );

	QLabel *l1 = new QLabel( tr("f(x,y)="));
	boxFunction = new QComboBox();
	boxFunction->setEditable(true);
	
	QBoxLayout *bl1 = new QBoxLayout (QBoxLayout::LeftToRight);
	bl1->addWidget(l1, 1);
	bl1->addWidget(boxFunction, 10);

    QGroupBox *gb1 = new QGroupBox(tr("X - axis"));
    QLabel *l2 = new QLabel( tr("From"));
	boxXFrom = new QLineEdit();
	boxXFrom->setText(tr("-1"));
	
	QLabel *l3 = new QLabel(tr("To"));
	boxXTo = new QLineEdit();
	boxXTo->setText(tr("1"));
	
    QGridLayout *gl1 = new QGridLayout();
    gl1->addWidget(l2, 0, 0);
    gl1->addWidget(boxXFrom, 0, 1);
    gl1->addWidget(l3, 1, 0);
    gl1->addWidget(boxXTo, 1, 1);
    gb1->setLayout(gl1);

    QGroupBox *gb2 = new QGroupBox(tr("Y - axis"));
    QLabel *l4 = new QLabel( tr("From"));
	boxYFrom = new QLineEdit();
	boxYFrom->setText(tr("-1"));
	
	QLabel *l5 = new QLabel(tr("To"));
	boxYTo = new QLineEdit();
	boxYTo->setText(tr("1"));
	
    QGridLayout *gl2 = new QGridLayout();
    gl2->addWidget(l4, 0, 0);
    gl2->addWidget(boxYFrom, 0, 1);
    gl2->addWidget(l5, 1, 0);
    gl2->addWidget(boxYTo, 1, 1);
    gb2->setLayout(gl2);
    
    QGroupBox *gb3 = new QGroupBox(tr("Z - axis"));
    QLabel *l6 = new QLabel( tr("From"));
	boxZFrom = new QLineEdit();
	boxZFrom->setText(tr("-1"));
	
	QLabel *l7 = new QLabel(tr("To"));
	boxZTo = new QLineEdit();
	boxZTo->setText(tr("1"));
	
    QGridLayout *gl3 = new QGridLayout();
    gl3->addWidget(l6, 0, 0);
    gl3->addWidget(boxZFrom, 0, 1);
    gl3->addWidget(l7, 1, 0);
    gl3->addWidget(boxZTo, 1, 1);
    gb3->setLayout(gl3);
    
    QBoxLayout *bl3 = new QBoxLayout ( QBoxLayout::LeftToRight);
	bl3->addWidget(gb1);
	bl3->addWidget(gb2);
	bl3->addWidget(gb3);

	buttonClear = new QPushButton();
    buttonClear->setText( tr( "Clear &list" ) );
	
	buttonOk = new QPushButton();
    buttonOk->setText( tr( "&OK" ) );
    buttonOk->setDefault( true );
   
    buttonCancel = new QPushButton();
    buttonCancel->setText( tr( "&Cancel" ) );
	
    QBoxLayout *bl2 = new QBoxLayout ( QBoxLayout::LeftToRight);
    bl2->addStretch();
	bl2->addWidget(buttonClear);
	bl2->addWidget(buttonOk);
	bl2->addWidget(buttonCancel);
	bl2->addStretch();
	
	QVBoxLayout* vl = new QVBoxLayout();
    vl->addLayout(bl1);
	vl->addLayout(bl3);
	vl->addLayout(bl2);
	setLayout(vl);
   
    // signals and slots connections
	connect( buttonClear, SIGNAL( clicked() ), this, SLOT(clearList() ) );
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );

	setFocusProxy(boxFunction);
}

void SurfaceDialog::clearList()
{
boxFunction->clear();
emit clearFunctionsList();
}

void SurfaceDialog::setFunction(const QString& s)
{
boxFunction->setCurrentText(s);
}

void SurfaceDialog::setLimits(double xs, double xe, double ys, double ye, double zs, double ze)
{
	boxXFrom->setText(QString::number(xs));
	boxXTo->setText(QString::number(xe));
	boxYFrom->setText(QString::number(ys));
	boxYTo->setText(QString::number(ye));
	boxZFrom->setText(QString::number(zs));
	boxZTo->setText(QString::number(ze));
}

void SurfaceDialog::accept()
{
QString Xfrom=boxXFrom->text().lower();
QString Xto=boxXTo->text().lower();
QString Yfrom=boxYFrom->text().lower();
QString Yto=boxYTo->text().lower();
QString Zfrom=boxZFrom->text().lower();
QString Zto=boxZTo->text().lower();

double fromX, toX, fromY,toY, fromZ,toZ;
try
	{
	MyParser parser;	
	parser.SetExpr(Xfrom.ascii());
	fromX=parser.Eval();
	}
catch(mu::ParserError &e)
	{
	QMessageBox::critical(0, tr("QtiPlot - X Start limit error"), QString::fromStdString(e.GetMsg()));
	boxXFrom->setFocus();
	return;
	}
try
	{
	MyParser parser;
	parser.SetExpr(Xto.ascii());
	toX=parser.Eval();
	}
catch(mu::ParserError &e)
	{
	QMessageBox::critical(0, tr("QtiPlot - X End limit error"), QString::fromStdString(e.GetMsg()));
	boxXTo->setFocus();
	return;
	}	

try
	{
	MyParser parser;
	parser.SetExpr(Yfrom.ascii());
	fromY=parser.Eval();
	}
catch(mu::ParserError &e)
	{
	QMessageBox::critical(0, tr("QtiPlot - Y Start limit error"), QString::fromStdString(e.GetMsg()));
	boxYFrom->setFocus();
	return;
	}	
try
	{
	MyParser parser;	
	parser.SetExpr(Yto.ascii());
	toY=parser.Eval();
	}
catch(mu::ParserError &e)
	{
	QMessageBox::critical(0, tr("QtiPlot - Y End limit error"), QString::fromStdString(e.GetMsg()));
	boxYTo->setFocus();
	return;
	}	
try
	{
	MyParser parser;
	parser.SetExpr(Zfrom.ascii());
	fromZ=parser.Eval();
	}
catch(mu::ParserError &e)
	{
	QMessageBox::critical(0, tr("QtiPlot - Z Start limit error"), QString::fromStdString(e.GetMsg()));
	boxZFrom->setFocus();
	return;
	}	
try
	{
	MyParser parser;
	parser.SetExpr(Zto.ascii());
	toZ=parser.Eval();
	}
catch(mu::ParserError &e)
	{
	QMessageBox::critical(0, tr("QtiPlot - Z End limit error"), QString::fromStdString(e.GetMsg()));
	boxZTo->setFocus();
	return;
	}

if (fromX >= toX || fromY >= toY || fromZ >= toZ)
	{
	QMessageBox::critical(0, tr("QtiPlot - Input error"),
				tr("Please enter limits that satisfy: from < end!"));
	boxXTo->setFocus();
	return;
	}

double x,y;
QString formula=boxFunction->currentText();
bool error=false;
try
	{
	MyParser parser;
	parser.DefineVar("x", &x);	
	parser.DefineVar("y", &y);		
	parser.SetExpr(formula.ascii());
		
	x=fromX; y=fromY;
	parser.Eval();
	x=toX; y=toY;
	parser.Eval();
	}
catch(mu::ParserError &e)
	{
	QMessageBox::critical(0, tr("QtiPlot - Input function error"), QString::fromStdString(e.GetMsg()));
	boxFunction->setFocus();
	error=true;	
	}
	
if (!error)
	{
	emit options(boxFunction->currentText(),fromX, toX, fromY, toY, fromZ, toZ);
	emit custom3DToolBar();

	ApplicationWindow *app = (ApplicationWindow *)this->parent();
	app->updateSurfaceFuncList(boxFunction->currentText());
	close();
	}
}

void SurfaceDialog::insertFunctionsList(const QStringList& list)
{
boxFunction->insertStringList (list, 1);
}

SurfaceDialog::~SurfaceDialog()
{
}
