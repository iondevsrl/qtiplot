/***************************************************************************
    File                 : RectangleWidget.cpp
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2008 by Ion Vasilief
    Email (use @ for *)  : ion_vasilief*yahoo.fr
    Description          : A widget displaying rectangles in 2D plots

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
#include "RectangleWidget.h"
#include "../PatternBox.h"

#include <QPainter>
#include <QPaintEngine>
#include <QPalette>
 
RectangleWidget::RectangleWidget(Graph *plot):FrameWidget(plot)
{
	setFrameStyle(Line);	
	setSize(0, 0);
}

void RectangleWidget::clone(RectangleWidget* r)
{
	d_frame = r->frameStyle();
	setFramePen(r->framePen());
	setBackgroundColor(r->backgroundColor());
	setBrush(r->brush());
	setCoordinates(r->xValue(), r->yValue(), r->right(), r->bottom());
}

QString RectangleWidget::saveToString()
{
	QString s = "<Rectangle>\n";
	s += FrameWidget::saveToString();
	s += "<FrameWidth>" + QString::number(d_frame_pen.width()) + "</FrameWidth>\n";
	QColor bc = backgroundColor();
	s += "<Background>" + bc.name() + "</Background>\n";
	s += "<Alpha>" + QString::number(bc.alpha()) + "</Alpha>\n";
	s += "<BrushColor>" + d_brush.color().name() + "</BrushColor>\n";
	s += "<BrushStyle>" + QString::number(PatternBox::patternIndex(d_brush.style())) + "</BrushStyle>\n";
	return s + "</Rectangle>\n";
}

void RectangleWidget::restore(Graph *g, const QStringList& lst)
{
	double x = 0.0, y = 0.0, right = 0.0, bottom = 0.0;
	QStringList::const_iterator line;
	QColor backgroundColor = Qt::white;
	QBrush brush = QBrush();
	RectangleWidget *r = new RectangleWidget(g);
	if (!r)
		return;
	
	for (line = lst.begin(); line != lst.end(); line++){
        QString s = *line;
        if (s.contains("<Frame>"))
			r->setFrameStyle(s.remove("<Frame>").remove("</Frame>").toInt());
		else if (s.contains("<Color>"))
			r->setFrameColor(QColor(s.remove("<Color>").remove("</Color>")));
		else if (s.contains("<x>"))
			x = s.remove("<x>").remove("</x>").toDouble();
		else if (s.contains("<y>"))
			y = s.remove("<y>").remove("</y>").toDouble();
		else if (s.contains("<right>"))
			right = s.remove("<right>").remove("</right>").toDouble();
		else if (s.contains("<bottom>"))
			bottom = s.remove("<bottom>").remove("</bottom>").toDouble();
		else if (s.contains("<FrameWidth>"))
			r->setFrameWidth(s.remove("<FrameWidth>").remove("</FrameWidth>").toInt());
		else if (s.contains("<Background>"))
			backgroundColor = QColor(s.remove("<Background>").remove("</Background>"));
		else if (s.contains("<Alpha>"))
			backgroundColor.setAlpha(s.remove("<Alpha>").remove("</Alpha>").toInt());
		else if (s.contains("<BrushColor>"))
			brush.setColor(QColor(s.remove("<BrushColor>").remove("</BrushColor>")));
		else if (s.contains("<BrushStyle>"))
			brush.setStyle(PatternBox::brushStyle((s.remove("<BrushStyle>").remove("</BrushStyle>")).toInt()));
	}

	r->setBackgroundColor(backgroundColor);
	r->setBrush(brush);
	r->setCoordinates(x, y, right, bottom);
	g->add(r, false);
}