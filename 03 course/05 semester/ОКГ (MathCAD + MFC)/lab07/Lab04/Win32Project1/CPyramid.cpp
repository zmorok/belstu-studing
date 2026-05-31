#include "CPyramid.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#define pi 3.14159265;
using namespace std;

CPyramid::CPyramid()
{
	this->Vertices.RedimMatrix(4, 6);	
	
	// создание матрицы с координатами точек пирамиды
	// 
	// 
	//	A	A1	B	B1	C	C1	
	//	3	1	0	0	0	0
	//	0	0	0	0	3	1
	//	0	3	0	3	0	3
	//	1	1	1	1	1	1
	this->Vertices(0, 0) = 3;		// A (ось х)
	this->Vertices(0, 1) = 1;		// A1 (ось х)
	this->Vertices(2, 1) = 3;		// A1 (ось z)
	this->Vertices(2, 3) = 3;		// B1 (ось z)
	this->Vertices(1, 4) = 3;		// C (ось y)
	this->Vertices(1, 5) = 1;		// C1 (ось у)
	this->Vertices(2, 5) = 3;		// C1 (ось z)
	for (int i = 0; i < 6; i++)
	{
		this->Vertices(3, i) = 1;
	}
}

// МСК -> ВСК
CMatrix CreateViewCoord(double r, double fi, double q)			
{
	double fi_sphere = (fi / 180) * pi;
	double q_sphere = (q / 180) * pi;		

	//матрица пересчета в ВСК
	CMatrix K(4, 4); 
	K(0, 0) = -sin(fi_sphere);
	K(0, 1) = cos(fi_sphere);
	K(1, 0) = -cos(q_sphere) * cos(fi_sphere);
	K(1, 1) = -cos(q_sphere) * sin(fi_sphere);
	K(1, 2) = sin(q_sphere);
	K(2, 0) = -sin(q_sphere) * cos(fi_sphere);
	K(2, 1) = -sin(q_sphere) * sin(fi_sphere);
	K(2, 2) = -cos(q_sphere);
	K(2, 3) = r;
	K(3, 3) = 1;
	return K;
}

// с удалением ребер
void CPyramid::Draw1(CDC& dc, CMatrix& PView, CRect& RW)
{
	//матрица из МСК в ВСК
	CMatrix XV = CreateViewCoord(PView(0), PView(1), PView(2));

	//координаты в ВСК (для камеры)
	CMatrix ViewVert = XV * this->Vertices;

	// область просмотра в ОСК
	CRectD RectView;
	GetRect(this->Vertices, RectView);
	CMatrix MW = SpaceToWindow(RectView, RW);

	CPoint	MasVert[6],	//для координат
			A1B1C1[3],	//верхняя грань
			ABC[3];		//нижняя грань
	
	CMatrix V(3);		//для преобразования точек
	V(2) = 1;

	// преобразование координат вершин в оконные координаты
	for (int i = 0; i < 6; i++)
	{
		//берём координаты точки ВСК
		V(0) = ViewVert(0, i);
		V(1) = ViewVert(1, i);
		V = MW * V;	//получаем координаты в ОСК и запись их в массив
		MasVert[i].x = (int)V(0);
		MasVert[i].y = (int)V(1);
	}

	// определение граней
	ABC[0] = MasVert[0];
	ABC[1] = MasVert[2];
	ABC[2] = MasVert[4];
	A1B1C1[0] = MasVert[1];
	A1B1C1[1] = MasVert[3];
	A1B1C1[2] = MasVert[5];

	//вывод углов фи и тета
	char buf[50] = "";
	sprintf(buf, "%.*f", 0, PView(1));
	dc.TextOut(10, 10, buf);
	sprintf(buf, "%.*f", 0, PView(2));
	dc.TextOut(10, 30, buf);

	CPen Pen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* pOldPen = dc.SelectObject(&Pen);

	CBrush BottomBrush(RGB(0, 255, 0));
	CBrush TopBrush(RGB(150, 0, 0));
	CBrush BaseBrush(RGB(255, 255, 255));

	CBrush* pOldBrush = dc.SelectObject(&BottomBrush);
	dc.SelectObject(&BaseBrush);

	// отрисовка граней ABB1A1, BCB1C1, ACC1A1
	CPoint ABB1A1[4];
	ABB1A1[0] = ABC[0];
	ABB1A1[1] = ABC[1];
	ABB1A1[2] = A1B1C1[1];
	ABB1A1[3] = A1B1C1[0];

	CPoint BCB1C1[4];
	BCB1C1[0] = ABC[1];
	BCB1C1[1] = ABC[2];
	BCB1C1[2] = A1B1C1[2];
	BCB1C1[3] = A1B1C1[1];

	CPoint ACC1A1[4];
	ACC1A1[0] = ABC[2];
	ACC1A1[1] = ABC[0];
	ACC1A1[2] = A1B1C1[0];
	ACC1A1[3] = A1B1C1[2];

	

	if (PView(1) < 90)
	{
		dc.Polygon(ABB1A1, 4);
		dc.Polygon(ACC1A1, 4);
	}
	else
	{
		if (PView(1) < 180)
		{
			dc.Polygon(ACC1A1, 4);
			dc.Polygon(BCB1C1, 4);
		}
		else if (PView(1) < 270)
		{
			dc.Polygon(ACC1A1, 4);
			dc.Polygon(ABB1A1, 4);
			dc.Polygon(BCB1C1, 4);
		}
		else
		{
			dc.Polygon(ACC1A1, 4);
			dc.Polygon(ABB1A1, 4);
		}
	}

	// отрисовка граней в зависимости от 0
	if (PView(2) <= 10 && PView(1) > 180 && PView(1) < 270)
	{
		dc.Polygon(ACC1A1, 4);
		dc.Polygon(ABB1A1, 4);
		dc.Polygon(BCB1C1, 4);
	}

	if (PView(2) <= 90)
	{
		dc.SelectObject(&TopBrush);
		dc.Polygon(A1B1C1, 3);
	}
	else
	{
		dc.SelectObject(&BottomBrush);
		dc.Polygon(ABC, 3);
	}

	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
}

// без удаления граней
void CPyramid::Draw(CDC& dc, CMatrix& PView, CRect& RW)			
{
	CMatrix XV = CreateViewCoord(PView(0), PView(1), PView(2));

	CMatrix ViewVert = XV * this->Vertices;
	CRectD RectView;

	GetRect(Vertices, RectView);
	CMatrix MW = SpaceToWindow(RectView, RW);

	//массивы точек для вершин граней пирамиды
	CPoint MasVert[6], a1b1c1[3], abc[3]; 
	CMatrix V(3);
	V(2) = 1;

	for (int i = 0; i < 6; i++)
	{
		V(0) = ViewVert(0, i);
		V(1) = ViewVert(1, i);
		V = MW * V;
		MasVert[i].x = (int)V(0);
		MasVert[i].y = (int)V(1);
	}
	abc[0] = MasVert[0];
	abc[1] = MasVert[2];
	abc[2] = MasVert[4];

	a1b1c1[0] = MasVert[1];
	a1b1c1[1] = MasVert[3];
	a1b1c1[2] = MasVert[5];
	CPen Pen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* pOldPen = dc.SelectObject(&Pen);
	char buf[50] = "";

	sprintf(buf, "%.*f", 0, PView(1));
	dc.TextOut(10, 10, buf);
	sprintf(buf, "%.*f", 0, PView(2));
	dc.TextOut(10, 30, buf);

	dc.MoveTo(abc[0]);
	dc.LineTo(abc[1]);
	dc.MoveTo(abc[0]);
	dc.LineTo(abc[2]);
	dc.MoveTo(abc[2]);
	dc.LineTo(abc[1]);

	dc.MoveTo(a1b1c1[0]);
	dc.LineTo(a1b1c1[1]);
	dc.MoveTo(a1b1c1[0]);
	dc.LineTo(a1b1c1[2]);
	dc.MoveTo(a1b1c1[2]);
	dc.LineTo(a1b1c1[1]);

	dc.MoveTo(abc[0]);
	dc.LineTo(a1b1c1[0]);
	dc.MoveTo(abc[1]);
	dc.LineTo(a1b1c1[1]);
	dc.MoveTo(abc[2]);
	dc.LineTo(a1b1c1[2]);

	dc.SelectObject(pOldPen);

}

//ограничивающий прямоугольник для координат вершин пирамиды
void  CPyramid::GetRect(CMatrix& Vert, CRectD&RectView)
{
	RectView.top = Vert.GetRow(2).MinElement();
	RectView.bottom = Vert.GetRow(2).MaxElement();
	RectView.left = Vert.GetRow(0).MinElement();
	RectView.right = Vert.GetRow(0).MaxElement();
}

//векторное произведение
CMatrix VectorMult(CMatrix& V1, CMatrix& V2)
{
	if (V1.rows() != V2.rows() || V1.cols() > 1 || V2.cols() > 1) // Число столбцов больше одного 
	{
		char* error = "CMatrix VectorMult(CMatrix& V1, CMatrix& V2) объект не вектор - число столбцов больше 1 ";
		MessageBoxA(NULL, error, "Ошибка", MB_ICONSTOP);
		exit(1);
	}

	CMatrix Temp = V1;
	Temp(0) = V1(1)*V2(2) - V1(2)*V2(1);
	Temp(1) = V1(2)*V2(0) - V1(0)*V2(2);
	Temp(2) = V1(0)*V2(1) - V1(1)*V2(0);

	return Temp;
}

double ScalarMult(CMatrix& V1, CMatrix& V2)
{
	if (V1.rows() != V2.rows() || V1.cols() > 1 || V2.cols() > 1) // Число столбцов больше одного 
	{
		char* error = "double ScalarMult(CMatrix& V1, CMatrix& V2) объект не вектор - число столбцов больше 1 ";
		MessageBoxA(NULL, error, "Ошибка", MB_ICONSTOP);
		exit(1);
	}

	return V1(0)*V2(0) + V1(1)*V2(1) + V1(2)*V2(2);
}

// с удалением ребер
void CPyramid::Draw11(CDC& dc, CMatrix& PView, CRect& RW)
{
	CMatrix XV = CreateViewCoord(PView(0), PView(1), PView(2));
	CMatrix ViewVert = XV * this->Vertices;
	CRectD RectView;
	GetRect(this->Vertices, RectView);
	CMatrix MW = SpaceToWindow(RectView, RW);

	CPoint MasVert[6], A1B1C1[3], ABC[3];
	CMatrix V(3);
	V(2) = 1;

	for (int i = 0; i < 6; i++)
	{
		V(0) = ViewVert(0, i);
		V(1) = ViewVert(1, i);
		V = MW * V;
		MasVert[i].x = (int)V(0);
		MasVert[i].y = (int)V(1);
	}

	ABC[0] = MasVert[0];
	ABC[1] = MasVert[2];
	ABC[2] = MasVert[4];
	A1B1C1[0] = MasVert[1];
	A1B1C1[1] = MasVert[3];
	A1B1C1[2] = MasVert[5];

	//нормали (при использовании векторного произведения)
	CMatrix NormalABB1A1 = VectorMult(ViewVert.GetCol(1) - ViewVert.GetCol(0), ViewVert.GetCol(2) - ViewVert.GetCol(0));
	CMatrix NormalBCB1C1 = VectorMult(ViewVert.GetCol(2) - ViewVert.GetCol(1), ViewVert.GetCol(3) - ViewVert.GetCol(1));
	CMatrix NormalACC1A1 = VectorMult(ViewVert.GetCol(3) - ViewVert.GetCol(2), ViewVert.GetCol(0) - ViewVert.GetCol(2));

	//параллельно оси Z, направлен вниз
	CMatrix ObserverVector(3);
	ObserverVector(0) = 0;
	ObserverVector(1) = 0;
	ObserverVector(2) = -1; // Вектор наблюдателя

	//стиль пера и кисти
	CPen Pen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* pOldPen = dc.SelectObject(&Pen);
	CBrush BottopBrush(RGB(34, 245, 206));
	CBrush TopBrush(RGB(5, 192, 34));
	CBrush BaseBrush(RGB(255, 255, 255));
	CBrush* pOldBrush = dc.SelectObject(&BottopBrush);
	dc.SelectObject(&BaseBrush);

	//отрисовка граней
	CPoint ABB1A1[4] = { ABC[0], ABC[1], A1B1C1[1], A1B1C1[0] };
	CPoint BCB1C1[4] = { ABC[1], ABC[2], A1B1C1[2], A1B1C1[1] };
	CPoint ACC1A1[4] = { ABC[2], ABC[0], A1B1C1[0], A1B1C1[2] };

	//если грань видима (нормаль направлена к наблюдателю), отрисовывает её.
	if (ScalarMult(NormalABB1A1, ObserverVector) > 0)
	{
		dc.Polygon(ABB1A1, 4);
	}

	if (ScalarMult(NormalBCB1C1, ObserverVector) > 0)
	{
		dc.Polygon(BCB1C1, 4);
	}

	if (ScalarMult(NormalACC1A1, ObserverVector) > 0)
	{
		dc.Polygon(ACC1A1, 4);
	}

	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
}