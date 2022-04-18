﻿// CMyGraph.cpp: файл реализации
//

#include "pch.h"
#include "coursework.h"
#include "CMyGraph.h"


// CMyGraph

IMPLEMENT_DYNAMIC(CMyGraph, CStatic)

void CMyGraph::draw_axis(CDC& dc) {
}

CMyGraph::CMyGraph()
	:CStatic(),
	scale_x(), scale_y() {
}

CMyGraph::~CMyGraph() {
}






BEGIN_MESSAGE_MAP(CMyGraph, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// Обработчики сообщений CMyGraph


void CMyGraph::OnPaint() {
	CPaintDC dc(this);
	CPen gr(BS_SOLID, 1, RGB(0, 0, 0));
	HGDIOBJ oldpen = dc.SelectObject(gr);

	RECT r;
	GetWindowRect(&r);
	r = { 0,0,r.right - r.left,r.bottom - r.top };
	CRect rforf(r);
	rforf.left += shift.x;
	rforf.bottom -= shift.y;
	dc.FillSolidRect(&r, bg_color);

	dc.IntersectClipRect(&rforf);
	for (MathFunction* f : functions) {
		bool is_first = true;
		CPen gr(BS_SOLID, 1, f->color);
		dc.SelectObject(gr);
		for (POINT dot : f->get_points()) {
			if (hist) {
				dc.MoveTo(dot.x, r.bottom - shift.y);
				dc.LineTo(dot);
			} else {
				if (is_first) { dc.MoveTo(dot); is_first = false; } else {
					dc.LineTo(dot);
				}
			}
		}
	}
	draw_axis(dc);

	dc.SelectObject(oldpen);
}

void CMyGraph::setScale(double x_from, double x_to, double y_from, double y_to) {
	if (x_from != scale_x.from || x_to != scale_x.to || y_from != scale_y.from || y_to != scale_y.to) {
		background_calculated = false;
	}
	this->scale_x = { x_from,x_to };
	this->scale_y = { y_from,y_to };
	for (MathFunction* f : functions) {
		f->set_scale(x_from, x_to, y_from, y_to);
	}
}

void CMyGraph::setStep(double step) {
	for (MathFunction* f : functions) {
		f->set_step(step);
	}
}

void CMyGraph::setRect(RECT r) {
	r.bottom -= shift.x;
	r.left += shift.x;
	for (MathFunction* f : functions) {
		f->set_rect(r);
	}
}

void CMyGraph::setLog(bool b) {
	if (b != is_log) {
		background_calculated = false;
	}
	this->is_log = b;
	for (MathFunction* f : functions) {
		f->set_log(b);
	}
}

void CMyGraph::setNotCalculated() {
	background_calculated = false;
	for (MathFunction* f : functions) {
		f->set_not_calculated();
	}
}

void CMyGraph::setBgColor(COLORREF col) {
	if (col != bg_color) {
		background_calculated = false;
		bg_color = col;
	}
}
