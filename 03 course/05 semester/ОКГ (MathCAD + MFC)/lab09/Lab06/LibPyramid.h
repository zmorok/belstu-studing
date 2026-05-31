#pragma once
double cosv1v2(CMatrix& one, CMatrix& two);
class CPyramid
{
private:
	CMatrix Vertices; // Координаты вершин
	void GetRect(CMatrix& Vert, CRectD& RectView);

public:
	CPyramid();
	void Draw(CDC& dc, CMatrix& P, CRect& RW);
	void Draw1(CDC& dc, CMatrix& P, CRect& RW);
	void ColorDraw(CDC& dc, CMatrix& PView, CMatrix& PLight, CRect& RW, COLORREF color);
};

CPyramid::CPyramid()
{
	Vertices.RedimMatrix(4, 6);	// ABC    — нижнее основание
								// A'B'C' — верхнее основание
	/*       A                   B                    C                   A'                  B'                  C'      */
	Vertices(0, 0) = 0;	Vertices(0, 1) = 0;  Vertices(0, 2) = 3; Vertices(0, 3) = 0; Vertices(0, 4) = 0; Vertices(0, 5) = 1;
	Vertices(1, 0) = 3; Vertices(1, 1) = 0;  Vertices(1, 2) = 0; Vertices(1, 3) = 1; Vertices(1, 4) = 0; Vertices(1, 5) = 0;
	Vertices(2, 0) = 0; Vertices(2, 1) = 0;  Vertices(2, 2) = 0; Vertices(2, 3) = 6; Vertices(2, 4) = 6; Vertices(2, 5) = 6;
	Vertices(3, 0) = 1; Vertices(3, 1) = 1;  Vertices(3, 2) = 1; Vertices(3, 3) = 1; Vertices(3, 4) = 1; Vertices(3, 5) = 1;
}

void CPyramid::GetRect(CMatrix& Vert, CRectD& RectView) //коорд прям-ка, охват проекцию пирамиды на XY ВСК
{
	CMatrix V = Vert.GetRow(0);               // x - координаты
	double xMin = V.MinElement();
	double xMax = V.MaxElement();
	V = Vert.GetRow(1);                       // y - координаты
	double yMin = V.MinElement();
	double yMax = V.MaxElement();
	RectView.SetRectD(xMin, yMax, xMax, yMin);
}

void CPyramid::Draw(CDC& dc, CMatrix& PView, CRect& RW)
// Рисует пирамиду БЕЗ удаления невидимых ребер
// Самостоятельный пересчет координат из мировых в оконные (MM_TEXT)
// dc - ссылка на класс CDC MFC
// P - координаты точки наблюдения в мировой сферической системе
// координат
// (r,fi(град.), q(град.))
// RW - область в окне для отображения	
{
	CMatrix MV = CreateViewCoord(PView(0), PView(1), PView(2)); // Матрица (4x4)
	CMatrix ViewVert = MV * Vertices; // Координаты вершин пирамиды в видовой СК
	CRectD RectView; // проекция - охватывающий прямоугольник
	GetRect(ViewVert, RectView); // Получаем охватывающий прямоугольник по к-там вершин
	CMatrix MW = SpaceToWindow(RectView, RW); // Матрица (3x3) для пересчета координат из мировых в оконные

	CPoint MasVert[6]; // Масив оконных координат вершин,
	CMatrix V(3);
	V(2) = 1;
	// Цикл по количеству вершин – вычисляем оконные коодинаты
	for (int i = 0; i < 6; i++)
	{
		V(0) = ViewVert(0, i); // x в видовой
		V(1) = ViewVert(1, i); // y в видовой
		V = MW * V;            // Оконные координаты точки
		MasVert[i].x = (int)V(0); // оконная к-ты х
		MasVert[i].y = (int)V(1); // оконная к-та у
	}

	// Рисуем
	CPen Pen(PS_SOLID, 2, RGB(0, 0, 255));
	CPen* pOldPen = dc.SelectObject(&Pen);
	dc.MoveTo(MasVert[3]);
	// Ребра НИЖНЕЙ грани
	for (int i = 0; i < 3; i++)
	{
		dc.LineTo(MasVert[i]);
	}
	dc.LineTo(MasVert[0]);
	dc.MoveTo(MasVert[5]);
	// Ребра ВЕРХНЕЙ грани
	for (int i = 3; i < 6; i++)
	{
		dc.LineTo(MasVert[i]);
	}
	// Ребра БОКОВЫХ граней
	for (int i = 0; i < 3; i++)
	{
		dc.MoveTo(MasVert[i]);
		dc.LineTo(MasVert[i + 3]);
	}
	dc.SelectObject(pOldPen);
}


// Главная функция освещения и закраски пирамиды.
// Выполняет: переход МСК-ВСК-ОСК, определение видимости, расчёт нормали,
// расчёт освещения по модели Ламберта, закраску грани.
void CPyramid::ColorDraw(CDC& dc, CMatrix& PView, CMatrix& PLight, CRect& RW, COLORREF color)
{
    // 1) Разбор цвета источника света
    BYTE Red = GetRValue(color);
    BYTE Green = GetGValue(color);
    BYTE Blue = GetBValue(color);

    // 2) Перевод камеры и света из сферических координат в декартовы.
    //    Это положение наблюдателя и источника света в МСК.
    CMatrix ViewCart = SphereToCart(PView);   // камера
    CMatrix LightCart = SphereToCart(PLight);  // источник света

    // 3) Матрица перехода: МСК - ВСК
    CMatrix MV = CreateViewCoord(PView(0), PView(1), PView(2));

    // 4) Перенос всех вершин пирамиды в ВСК
    CMatrix ViewVert = MV * Vertices;

    // 5) Поиск прямоугольника, который охватывает фигуру в ВСК.
    CRectD RectView;
    GetRect(ViewVert, RectView);

    // 6) Построение матрицы перехода ВСК - ОСК
    CMatrix MW = SpaceToWindow(RectView, RW);

    // 7) Пересчет координат 6 вершин пирамиды в оконные координаты.
    CPoint MasVert[6];
    CMatrix V(3);
    V(2) = 1;

    for (int i = 0; i < 6; i++)
    {
        V(0) = ViewVert(0, i);  // X в ВСК
        V(1) = ViewVert(1, i);  // Y в ВСК

        V = MW * V;             // ВСК → ОСК

        MasVert[i].x = (int)V(0);
        MasVert[i].y = (int)V(1);
    }

    // 8) Закраска боковых граней пирамиды
    //    Каждая грань определяется:
    //    - R1, R2 : две вершины основания
    //    - VE     : соответствующая вершина верхнего основания
    //    Нормаль вычисляется как крестовое произведение.
    CMatrix R1(3), R2(3), VN(3);
    double sm;

    for (int i = 0; i < 3; i++)
    {
        // Индекс следующей точки основания
        int k = (i == 2 ? 0 : i + 1);

        // Вершины основания и соответствующая вершина верха
        R1 = Vertices.GetCol(i, 0, 2);
        R2 = Vertices.GetCol(k, 0, 2);
        CMatrix VE = Vertices.GetCol(i + 3, 0, 2);

        // Два ребра грани:
        // V1 — вдоль основания, V2 — вверх к вершине
        CMatrix V1 = R2 - R1;
        CMatrix V2 = VE - R1;

        // Вектор нормали
        VN = VectorMult(V2, V1);

        // Проверка видимости грани:
        // cos между нормалью и направлением на ТН (ViewCart)
        // Если cos >= 0 - грань обращена к наблюдателю
        sm = cosv1v2(ViewCart, VN);

        if (sm >= 0)
        {
            // Освещённость: косинус между нормалью и направлением на источник света
            // Диффузная модель Ламберта: I = I0 * cos(φ)
            sm = cosv1v2(LightCart, VN);

            if (sm >= 0)
            {
                // Закрашиваем цветом источника, умножая на коэффициент cos(φ)
                CBrush Brus(RGB(sm * Red, sm * Green, sm * Blue));
                CBrush* pOldBrush = dc.SelectObject(&Brus);

                // 4 точки боковой грани
                CPoint MasVertr[4]{ MasVert[i], MasVert[k], MasVert[k + 3], MasVert[i + 3] };

                CPen Pen(PS_SOLID, 2, RGB(0, 0, 0));
                CPen* pOldPen = dc.SelectObject(&Pen);

                dc.Polygon(MasVertr, 4);

                dc.SelectObject(pOldPen);
                dc.SelectObject(pOldBrush);
            }
            else
            {
                // Грань в тени - закрашиваем чёрным
                CBrush Brus(RGB(0, 0, 0));
                CBrush* pOldBrush = dc.SelectObject(&Brus);

                CPoint MasVertr[4]{ MasVert[i], MasVert[k], MasVert[k + 3], MasVert[i + 3] };

                CPen Pen(PS_SOLID, 2, RGB(0, 0, 0));
                CPen* pOldPen = dc.SelectObject(&Pen);

                dc.Polygon(MasVertr, 4);

                dc.SelectObject(pOldPen);
                dc.SelectObject(pOldBrush);
            }
        }
    }

    // 9) Закраска нижнего и верхнего основания
    VN = VectorMult(R1, R2);  // нормаль к основанию
    sm = cosv1v2(VN, ViewCart);

    // VP — вектор от наблюдателя к источнику (не используется далее)
    CMatrix VP = LightCart - ViewCart;

    // ----- Нижнее основание

    if (sm >= 0)
    {
        sm = cosv1v2(LightCart, VN);

        if (sm >= 0)
        {
            CBrush* topBrush = new CBrush(RGB(sm * Red, sm * Green, sm * Blue));
            dc.SelectObject(topBrush);
            dc.Polygon(MasVert, 3);
        }
        else
        {
            CBrush* topBrush = new CBrush(RGB(0, 0, 0));
            dc.SelectObject(topBrush);
            dc.Polygon(MasVert, 3);
        }
    }
    // ----- Верхнее основание -----
    else
    {
        sm = cosv1v2(LightCart, -VN);

        if (sm >= 0)
        {
            CBrush* topBrush = new CBrush(RGB(sm * Red, sm * Green, sm * Blue));
            dc.SelectObject(topBrush);
            dc.Polygon(MasVert + 3, 3);
        }
        else
        {
            CBrush* topBrush = new CBrush(RGB(0, 0, 0));
            dc.SelectObject(topBrush);
            dc.Polygon(MasVert + 3, 3);
        }
    }
}


// Функция вычисляет косинус угла между двумя 3-мерными векторами V1 и V2.
// Используется для:
// 1) определения видимости грани (камеры → грань);
// 2) расчёта освещённости грани (свет → грань) по формуле Ламберта.
double cosv1v2(CMatrix& V1, CMatrix& V2)
{
    // 1. Вычисляем модуль (длину) каждого вектора
    double modV1 = ModVec(V1);
    double modV2 = ModVec(V2);

    // 2. Проверка: если длина хотя бы одного вектора ≈ 0,
    int b = (modV1 < 1e-7) || (modV2 < 1e-7);
    if (b)
    {
        TCHAR* error = L"CosV1V2: модуль одного или обоих векторов < 1e-7!";
        MessageBox(NULL, error, L"Ошибка", MB_ICONSTOP);
        exit(1);
    }

    // 3. Проверка размерности входных векторов:
    int b1 = (V1.cols() == 1) && (V1.rows() == 3);
    int b2 = (V2.cols() == 1) && (V2.rows() == 3);
    b = b1 && b2;

    if (!b)
    {
        TCHAR* error = L"CosV1V2: неправильные размерности векторов!";
        MessageBox(NULL, error, L"Ошибка", MB_ICONSTOP);
        exit(1);
    }

    // 4. Расчёт косинуса угла между двумя векторами:
    double cos_f = ScalarMult(V1, V2) / (modV1 * modV2);

    return cos_f;
}
