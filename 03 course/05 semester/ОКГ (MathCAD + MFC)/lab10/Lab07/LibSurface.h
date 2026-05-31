using namespace std;

typedef vector<CMatrix> CVecMatrix;
typedef vector<CVecMatrix> CMasMatrix;

typedef vector<CPoint> CVecPoint;
typedef vector<CVecPoint> CMatrPoint;

typedef double(*pfunc2)(double, double); // указатель на функцию f(x, y)

class CPlot3D
{
private:
	pfunc2 pFunc;          // указатель на функцию f(x,y), задающую поверхность
	CRectD SpaceRect;      // прямоугольник в пространстве, по которому строится поверхность
	CMasMatrix MatrF;      // массив матриц: однородные координаты точек поверхности (x,y,z,1)
	CMasMatrix MatrView;   // массив матриц: координаты точек (x,y,1) в плоскости XY системы наблюдателя
	CRectD ViewRect;       // прямоугольник в плоскости наблюдателя, охватывающий проекцию поверхности
	CRect WinRect;         // прямоугольник окна (в пикселях) для вывода
	CMatrix ViewPoint;     // вектор (3x1) с параметрами точки наблюдателя в сферических координатах (r, fi, q)
	CMatrPoint MatrWindow; // массив экранных точек (координаты в окне)

public:
	CPlot3D();
	~CPlot3D() { pFunc = NULL; }
	void SetFunction(pfunc2 pF, CRectD RS, double dx, double dy);
	void SetViewPoint(double r, double fi, double q);

	CMatrix GetViewPoint();                 // возвращает текущие параметры точки наблюдателя
	void SetWinRect(CRect Rect);            // задаёт прямоугольник окна для вывода
	void CreateMatrF(double dx, double dy); // формирует MatrF (сетку значений поверхности)
	void SetMatrF(CMasMatrix& Matr);        // задаёт MatrF извне (готовым набором точек)
	void CreateMatrView();                  // формирует MatrView – проекцию в систему координат наблюдателя
	void CreateMatrWindow();                // формирует MatrWindow – координаты в окне
	int GetNumberRegion();                  // определяет номер области (квадранта) точки наблюдателя
	void Draw(CDC& dc);
};



double Function1(double x, double y)
{
	return x*x + y*y;
}
double Function2(double x, double y)
{
	return x*x - y*y;
}
double Function3(double x, double y)
{
	//return sqrt(1 - x*x - y*y);
	double r = sqrt(x*x + y*y);
	double z;
	if (r < 0.001)
		z = 10;
	else
		z = 10 * sin(r) / r;
	return z;
}


CPlot3D::CPlot3D()
{
	pFunc = NULL;
	ViewPoint.RedimMatrix(3);
	WinRect.SetRect(0, 0, 200, 200);
	ViewPoint(0) = 10, ViewPoint(1) = 30, ViewPoint(2) = 45;
}
void CPlot3D::SetFunction(pfunc2 pF, CRectD RS, double dx, double dy)
// Инициализирует указатель на функцию f(x,y) и область построения.
// pFunc – указатель на функцию f(x,y), задающую поверхность.
// RS   – прямоугольник в мировых координатах (xmin, ymax, xmax, ymin) для построения.
// dx, dy – шаги по осям x и y для дискретизации поверхности.
{
	pFunc = pF;
	SpaceRect.SetRectD(RS.left, RS.top, RS.right, RS.bottom);
	MatrF.clear();
	MatrView.clear();
	MatrWindow.clear();

	CreateMatrF(dx, dy);
	CreateMatrView();
	CreateMatrWindow();
}
void CPlot3D::SetViewPoint(double r, double fi, double q)
// Задаёт параметры точки наблюдателя в сферических координатах.
// r  – расстояние до начала координат (радиус-вектор).
// fi – азимутальный угол (от оси OX вокруг оси OZ, в градусах).
// q  – угол наклона относительно оси OZ (0–180 градусов).
{
	ViewPoint(0) = r;
	ViewPoint(1) = fi;
	ViewPoint(2) = q;

	MatrView.clear();
	CreateMatrView();
	MatrWindow.clear();
	CreateMatrWindow();
}

CMatrix CPlot3D::GetViewPoint()
{
	CMatrix P = ViewPoint;
	return P;
}
void CPlot3D::SetWinRect(CRect Rect)
{
	WinRect = Rect;
	MatrWindow.clear();
	CreateMatrWindow();
}
void CPlot3D::CreateMatrF(double dx, double dy)
// Формирует MatrF – набор точек поверхности f(x,y) в мировых координатах.
// dx – шаг по оси X.
// dy – шаг по оси Y.
{
	double xL = SpaceRect.left;
	double xH = SpaceRect.right;
	double yL = SpaceRect.bottom;
	double yH = SpaceRect.top;

	CVecMatrix VecMatrix;
	CMatrix V(4);
	V(3) = 1; // однородная координата

	for (double x = xL; x <= xH; x += dx)
	{
		VecMatrix.clear();
		for (double y = yL; y <= yH; y += dy)
		{
			V(0) = x;
			V(1) = y;
			V(2) = pFunc(x, y);
			VecMatrix.push_back(V);
		}
		MatrF.push_back(VecMatrix);
	}
}

void CPlot3D::SetMatrF(CMasMatrix& Matr)
// Задаёт MatrF готовыми данными извне.
// Matr – внешний двумерный массив однородных координат (x,y,z,1).
// По данным пересчитывается SpaceRect, MatrView и MatrWindow.
{
	CVecMatrix VecMatrix;
	CMatrix V(4);
	double xmin, xmax, ymin, ymax;

	pFunc = NULL;         // функция не используется, поверхность задана явно
	MatrF.clear();
	MatrView.clear();
	MatrWindow.clear();

	V = Matr[0][0];
	xmin = xmax = V(0);
	ymin = ymax = V(1);

	for (int i = 0; i < Matr.size(); i++)
	{
		VecMatrix.clear();
		for (int j = 0; j < Matr[i].size(); j++)
		{
			V = Matr[i][j];
			VecMatrix.push_back(V);

			double x = V(0);
			double y = V(1);
			if (x < xmin) xmin = x;
			if (x > xmax) xmax = x;
			if (y < ymin) ymin = y;
			if (y > ymax) ymax = y;
		}
		MatrF.push_back(VecMatrix);
	}
	SpaceRect.SetRectD(xmin, ymax, xmax, ymin);
	CreateMatrView();
	CreateMatrWindow();
}

int CPlot3D::GetNumberRegion()
// Определяет, в какой из четырёх областей относительно прямоугольника SpaceRect
// находится точка наблюдателя, чтобы выбрать порядок обхода полигонов.
{
	CMatrix CartPoint = SphereToCart(ViewPoint);
	double xView = CartPoint(0);
	double yView = CartPoint(1);
	double zView = CartPoint(2); // пока не используется

	double xL = SpaceRect.left;
	double xH = SpaceRect.right;
	double yL = SpaceRect.bottom;
	double yH = SpaceRect.top;

	// Находим две диагонали прямоугольника как функции y1(x), y2(x)
	double y1 = yL + (yH - yL) * (xView - xL) / (xH - xL);
	double y2 = yH + (yH - yL) * (xView - xL) / (xH - xL);

	if ((yView <= y1) && (yView <= y2))
		return 1;
	if ((yView > y2) && (yView < y1))
		return 2;
	if ((yView >= y1) && (yView >= y2))
		return 3;
	if ((yView > y1) && (yView < y2))
		return 4;

	return 1; // на всякий случай
}

void CPlot3D::CreateMatrView()
// Формирует MatrView – координаты точек поверхности в системе координат наблюдателя
// (проекция в плоскость XY вида) и вычисляет ViewRect – прямоугольник проекции.
{
	CMatrix MV = CreateViewCoord(ViewPoint(0), ViewPoint(1), ViewPoint(2));
	CVecMatrix VecMatrix;
	CMatrix VX(4), V(3);
	V(2) = 1; // однородная координата

	double xmin = DBL_MAX;
	double xmax = -DBL_MAX;
	double ymin = DBL_MAX;
	double ymax = -DBL_MAX;

	for (int i = 0; i < MatrF.size(); i++)
	{
		VecMatrix.clear();
		for (int j = 0; j < MatrF[i].size(); j++)
		{
			VX = MatrF[i][j];
			VX = MV * VX;
			V(0) = VX(0); // x в системе наблюдателя
			V(1) = VX(1); // y в системе наблюдателя
			VecMatrix.push_back(V);

			double x = V(0);
			double y = V(1);
			if (x < xmin) xmin = x;
			if (x > xmax) xmax = x;
			if (y < ymin) ymin = y;
			if (y > ymax) ymax = y;  // у тебя тут был баг: стояло xmin = y
		}
		MatrView.push_back(VecMatrix);
	}
	ViewRect.SetRectD(xmin, ymax, xmax, ymin);
}

void CPlot3D::CreateMatrWindow()
// Формирует MatrWindow – координаты точек поверхности в окне (пиксели).
{
	CMatrix MW = SpaceToWindow(ViewRect, WinRect);

	CVecPoint VecPoint;
	CMatrix V(3);
	for (int i = 0; i < MatrView.size(); i++)
	{
		VecPoint.clear();
		for (int j = 0; j < MatrView[i].size(); j++)
		{
			V = MatrView[i][j];
			V = MW * V;
			CPoint P((int)V(0), (int)V(1));
			VecPoint.push_back(P);
		}
		MatrWindow.push_back(VecPoint);
	}
}

void CPlot3D::Draw(CDC& dc)
{
	if (MatrWindow.empty())
	{
		TCHAR* error = TEXT("Ошибка: нет данных для построения поверхности");
		MessageBox(NULL, error, TEXT("Ошибка"), MB_ICONSTOP);
		return;
	}
	CPoint pt[4];
	int kRegion = GetNumberRegion();
	int nRows = MatrWindow.size();
	int nCols = MatrWindow[0].size();

	switch (kRegion)
	{
	case 1:
	{
		for (int j = nCols - 1; j > 0; j--)
			for (int i = 0; i < nRows - 1; i++)
			{
				pt[0] = MatrWindow[i][j];
				pt[1] = MatrWindow[i][j - 1];
				pt[2] = MatrWindow[i + 1][j - 1];
				pt[3] = MatrWindow[i + 1][j];
				dc.Polygon(pt, 4);
			}
	} break;
	case 2:
	{
		for (int i = 0; i < nRows - 1; i++)
			for (int j = 0; j < nCols - 1; j++)
			{
				pt[0] = MatrWindow[i][j];
				pt[1] = MatrWindow[i][j + 1];
				pt[2] = MatrWindow[i + 1][j + 1];
				pt[3] = MatrWindow[i + 1][j];
				dc.Polygon(pt, 4);
			}
	} break;
	case 3:
	{
		for (int j = 0; j < nCols - 1; j++)   // у тебя было j < nCols, но используется j+1
			for (int i = 0; i < nRows - 1; i++)
			{
				pt[0] = MatrWindow[i][j];
				pt[1] = MatrWindow[i][j + 1];
				pt[2] = MatrWindow[i + 1][j + 1];
				pt[3] = MatrWindow[i + 1][j];
				dc.Polygon(pt, 4);
			}
	} break;
	case 4:
	{
		for (int i = nRows - 1; i > 0; i--)
			for (int j = 0; j < nCols - 1; j++) // у тебя было условие по i вместо j
			{
				pt[0] = MatrWindow[i][j];
				pt[1] = MatrWindow[i][j + 1];
				pt[2] = MatrWindow[i - 1][j + 1];
				pt[3] = MatrWindow[i - 1][j];
				dc.Polygon(pt, 4);
			}
	} break;
	}
}

void DrawLightSphere(CDC& dc, double Radius, CMatrix& PView, CMatrix& PSourceLight,
	CRect RW, COLORREF Color, int Index)
	// Index = 0 – диффузионная модель отражения света
	// Index = 1 – зеркальная модель отражения света
{
	BYTE red = GetRValue(Color);
	BYTE green = GetGValue(Color);
	BYTE blue = GetBValue(Color);

	CMatrix VSphere(3), VSphereNorm(3), V0(3), PV(4);
	COLORREF Col;
	double df = 1.2;
	double dq = 1.2;
	double kLight;
	double p = 105.0;

	VSphere(0) = 2;  // радиус в сферических координатах (пример)
	CMatrix VR = SphereToCart(PView);
	CMatrix VS = SphereToCart(PSourceLight);

	// Прямоугольник в пространстве наблюдателя
	CRectD RV(-Radius, Radius, Radius, -Radius);
	CMatrix MW = SpaceToWindow(RV, RW);
	CMatrix MV = CreateViewCoord(PView(0), PView(1), PView(2));

	V0(0) = RV.left;    // xL 
	V0(1) = RV.bottom;  // yL
	///V0(2) = 0;        // z в плоскости наблюдателя (для ОСК)

	for (double fi = 0; fi < 360.0; fi += df)
		for (double q = 0; q <= 180.0; q += dq)
		{
			VSphere(1) = fi;
			VSphere(2) = q;
			CMatrix VCart = SphereToCart(VSphere);      // точка сферы в декартовых координатах
			VSphereNorm = VCart;                        // вектор нормали к поверхности сферы
			double cos_RN = CosV1V2(VR, VSphereNorm);   // cos(вектора наблюдения, нормали)

			if (cos_RN > 0) // если точка видима наблюдателю
			{
				// Преобразование координат точки в оконные
				PV(0) = VCart(0);
				PV(1) = VCart(1);
				PV(2) = VCart(2);
				PV(3) = 1;

				PV = MV * PV;        // точка сферы -> ВСК
				VCart(0) = PV(0);    // Xv
				VCart(1) = PV(1);    // Yv
				VCart(2) = 1;
				VCart = VCart - V0;  // Xv - xL, Yv - yL: перевод в ОСК
				VCart = MW * VCart;  // перевод в оконные координаты

				CMatrix VP = VS - VR; // направление на источник света

				// Вычисление освещенности
				double cos_PN = CosV1V2(VP, VSphereNorm); // cos угла падения луча

				if (cos_PN > 0) // если точка сферы освещается
				{
					if (Index == 0)              // диффузное освещение
						kLight = cos_PN;
					else if (Index == 1)        // зеркальное освещение
					{
						double cos_val = CosV1V2(VSphereNorm, VS);
						if (cos_val < 0) cos_val = 0;
						kLight = pow(cos_val, p);
					}

					Col = RGB(kLight * red, kLight * green, kLight * blue);
					dc.SetPixel((int)VCart(0), (int)VCart(1), Col);
				}
				else
				{
					Col = RGB(0, 0, 0);
					dc.SetPixel((int)VCart(0), (int)VCart(1), Col);
				}
			}
		}
}
