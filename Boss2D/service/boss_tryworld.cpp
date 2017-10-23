#include <boss.hpp>
#include "boss_tryworld.hpp"

namespace BOSS
{
    ////////////////////////////////////////////////////////////////////////////////
    // TryWorld::Util
    ////////////////////////////////////////////////////////////////////////////////
    const Point* TryWorld::Util::GetDotByLineCross(const Point& DstB, const Point& DstE, const Point& SrcB, const Point& SrcE)
    {
        static Point Result;
		if(Math::MaxF(SrcB.x, SrcE.x) < Math::MinF(DstB.x, DstE.x)
		|| Math::MaxF(DstB.x, DstE.x) < Math::MinF(SrcB.x, SrcE.x)
		|| Math::MaxF(SrcB.y, SrcE.y) < Math::MinF(DstB.y, DstE.y)
		|| Math::MaxF(DstB.y, DstE.y) < Math::MinF(SrcB.y, SrcE.y))
			return nullptr;
		const int ResultA1 = GetClockwiseValue(DstB, DstE, SrcB);
		const int ResultB1 = GetClockwiseValue(DstB, DstE, SrcE);
		const int ResultA2 = GetClockwiseValue(SrcB, SrcE, DstB);
		const int ResultB2 = GetClockwiseValue(SrcB, SrcE, DstE);
		if((ResultA1 == 0 || ResultB1 == 0 || (ResultA1 < 0 && 0 < ResultB1) || (0 < ResultA1 && ResultB1 < 0))
			&& (ResultA2 == 0 || ResultB2 == 0 || (ResultA2 < 0 && 0 < ResultB2) || (0 < ResultA2 && ResultB2 < 0)))
		{
			// 수직
			if(DstB.x == DstE.x && SrcB.x == SrcE.x)
			{
				Result.x = DstB.x;
				if(DstB.y < DstE.y)
				{
					const bool IsInSrcB = (DstB.y <= SrcB.y && SrcB.y <= DstE.y);
					const bool IsInSrcE = (DstB.y <= SrcE.y && SrcE.y <= DstE.y);
					if(IsInSrcB && IsInSrcE)
						Result.y = (SrcB.y < SrcE.y)? SrcB.y : SrcE.y;
					else if(IsInSrcB) Result.y = SrcB.y;
					else if(IsInSrcE) Result.y = SrcE.y;
					else Result.y = (SrcB.y < SrcE.y)? DstB.y : DstE.y;
				}
				else
				{
					const bool IsInSrcB = (DstE.y <= SrcB.y && SrcB.y <= DstB.y);
					const bool IsInSrcE = (DstE.y <= SrcE.y && SrcE.y <= DstB.y);
					if(IsInSrcB && IsInSrcE)
						Result.y = (SrcB.y < SrcE.y)? SrcE.y : SrcB.y;
					else if(IsInSrcB) Result.y = SrcB.y;
					else if(IsInSrcE) Result.y = SrcE.y;
					else Result.y = (SrcB.y < SrcE.y)? DstE.y : DstB.y;
				}
			}
			// 수평
			else if(DstB.y == DstE.y && SrcB.y == SrcE.y)
			{
				Result.y = DstB.y;
				if(DstB.x < DstE.x)
				{
					const bool IsInSrcB = (DstB.x <= SrcB.x && SrcB.x <= DstE.x);
					const bool IsInSrcE = (DstB.x <= SrcE.x && SrcE.x <= DstE.x);
					if(IsInSrcB && IsInSrcE)
						Result.x = (SrcB.x < SrcE.x)? SrcB.x : SrcE.x;
					else if(IsInSrcB) Result.x = SrcB.x;
					else if(IsInSrcE) Result.x = SrcE.x;
					else Result.x = (SrcB.x < SrcE.x)? DstB.x : DstE.x;
				}
				else
				{
					const bool IsInSrcB = (DstE.x <= SrcB.x && SrcB.x <= DstB.x);
					const bool IsInSrcE = (DstE.x <= SrcE.x && SrcE.x <= DstB.x);
					if(IsInSrcB && IsInSrcE)
						Result.x = (SrcB.x < SrcE.x)? SrcE.x : SrcB.x;
					else if(IsInSrcB) Result.x = SrcB.x;
					else if(IsInSrcE) Result.x = SrcE.x;
					else Result.x = (SrcB.x < SrcE.x)? DstE.x : DstB.x;
				}
			}
			// 직교
			else if(DstB.x == DstE.x && SrcB.y == SrcE.y)
			{
				Result.x = DstB.x;
				Result.y = SrcB.y;
			}
			else if(SrcB.x == SrcE.x && DstB.y == DstE.y)
			{
				Result.x = SrcB.x;
				Result.y = DstB.y;
			}
			// 꼭지점닿음
			else if((DstB.x == SrcB.x && DstB.y == SrcB.y) || (DstE.x == SrcE.x && DstE.y == SrcE.y)
				|| (DstB.x == SrcE.x && DstB.y == SrcE.y) || (DstE.x == SrcB.x && DstE.y == SrcB.y))
				return nullptr;
			else
			{
				// 직선A
				float A_A = DstB.y - DstE.y;
				float A_B = DstE.x - DstB.x;
				const float A_Distance = Math::Sqrt(Math::Pow(A_A) + Math::Pow(A_B));
				A_A /= A_Distance;
				A_B /= A_Distance;
				const float A_C = A_A * DstB.x + A_B * DstB.y;
				// 직선B
				float B_A = SrcB.y - SrcE.y;
				float B_B = SrcE.x - SrcB.x;
				const float B_Distance = Math::Sqrt(Math::Pow(B_A) + Math::Pow(B_B));
				B_A /= B_Distance;
				B_B /= B_Distance;
				const float B_C = B_A * SrcB.x + B_B * SrcB.y;
				// 교점구하기
				const float Rate = A_B * B_A - A_A * B_B;
				Result.x = (A_B * B_C - B_B * A_C) / Rate;
				Result.y = (A_A * B_C - B_A * A_C) / -Rate;
			}
			return &Result;
		}
		return nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // TryWorld::Map
    ////////////////////////////////////////////////////////////////////////////////
    TryWorld::Map::Map()
	{
	}

	TryWorld::Map::~Map()
	{
	}

	void TryWorld::Map::Release(Map*& map)
	{
		delete map;
        map = nullptr;
	}

	TryWorld::Path* TryWorld::Map::BuildPath(const Point& beginPos, const Point& endPos, const int step, int* score)
	{
		Triangle* ClearNode = &Top;
		while(ClearNode = ClearNode->Next)
		{
			ClearNode->WayDot = Point(0, 0);
			ClearNode->WayBack = nullptr;
			ClearNode->DistanceSum = 0;
		}
		Triangle* Begin = FIND_PICK_TRIANGLE(beginPos);
		Triangle* End = FIND_PICK_TRIANGLE(endPos);
		if(Begin)
		{
			Begin->WayDot = beginPos;
			Begin->WayBack = (Triangle*) -1;
			bool IsSuccess = PATH_FIND(Begin, End, endPos);
			Begin->WayBack = nullptr;
			if(IsSuccess)
			{
				Path* Result = new Path(step);
				Result->Dots.AtAdding() = endPos;
				Triangle* CurTriangle = End;
				while(CurTriangle)
				{
					// 중간정점 삽입
					if(0 < step)
					{
						const Point DotA = Result->Dots[-1];
						const Point DotB = CurTriangle->WayDot;
						const int Distance = Math::Sqrt(Math::Pow(DotB.x - DotA.x) + Math::Pow(DotB.y - DotA.y));
						for(int f = step; f < Distance; f += step)
							Result->Dots.AtAdding() = Point((DotA.x * (Distance - f) + DotB.x * f) / Distance, (DotA.y * (Distance - f) + DotB.y * f) / Distance);
					}
					Result->Dots.AtAdding() = CurTriangle->WayDot;
					CurTriangle = CurTriangle->WayBack;
				}
                if(score) *score = End->DistanceSum;
				return Result;
			}
		}
		return nullptr;
	}

	void TryWorld::Map::CREATE_TRIANGLES(const Rect& boundBox, PolygonList& list)
	{
		for(int i = 0; i < list.Count(); ++i)
		for(int j = 0, jend = list[i].Count(); j < jend; ++j)
		{
			const int CurIndex = Dots.Count();
			Dots.AtAdding() = list[i][j];
			Lines.AtAdding().Set((i == list.Count() - 1)? linetype_bound : linetype_wall, CurIndex, CurIndex + ((j + 1) % jend) - j);
		}
		// 맵구성
		MAPPING(Top.INSERT_FIRST(), nullptr, linetype_bound, 0, 1);
	}

	void TryWorld::Map::MAPPING(Triangle* focus, Triangle* parent, linetype type, int dotA, int dotB)
	{
		for(int dotC = 0; dotC < Dots.Count(); dotC++)
		{
			// 점C의 조건
			if(!parent) {if(dotC == dotA || dotC == dotB) continue;}
			else if(dotC == parent->DotA || dotC == parent->DotB || dotC == parent->DotC)
				continue;
			// 삼각형구성의 조건
			if(IS_INCLUDE_ANY_DOT_BY(dotA, dotB, dotC)) continue;
			int LineAC = FIND_LINE_ID(dotA, dotC);
			int LineBC = FIND_LINE_ID(dotB, dotC);
			if((LineAC != -1 || !IS_CROSSING_ANY_LINE_BY(dotA, dotC)) && (LineBC != -1 || !IS_CROSSING_ANY_LINE_BY(dotB, dotC)))
			{
				// 삼각형추가
				focus->TypeAB = type;
				focus->TypeAC = (LineAC == -1)? linetype_space : Lines[LineAC].Type;
				focus->TypeBC = (LineBC == -1)? linetype_space : Lines[LineBC].Type;
				focus->DotA = dotA;
				focus->DotB = dotB;
				focus->DotC = dotC;
				// 선추가
				if(LineAC == -1) Lines.AtAdding().Set(linetype_space, focus->DotA, focus->DotC);
				if(LineBC == -1) Lines.AtAdding().Set(linetype_space, focus->DotB, focus->DotC);
				// 연결되는 삼각형정보
				focus->LinkAB = parent;
				if(focus->TypeAC != linetype_bound && !(focus->LinkAC = FIND_SAME_TRIANGLE(focus->DotA, focus->DotC, focus)))
					MAPPING(focus->LinkAC = Top.INSERT_FIRST(), focus, focus->TypeAC, focus->DotA, focus->DotC);
				if(focus->TypeBC != linetype_bound && !(focus->LinkBC = FIND_SAME_TRIANGLE(focus->DotB, focus->DotC, focus)))
					MAPPING(focus->LinkBC = Top.INSERT_FIRST(), focus, focus->TypeBC, focus->DotB, focus->DotC);
				return;
			}
		}
	}

	bool TryWorld::Map::IS_INCLUDE_ANY_DOT_BY(int dotA, int dotB, int dotC)
	{
		if(R_SIDE1(dotA, dotB, dotC) < 0)
		{
			int Swap = dotB;
			dotB = dotC;
			dotC = Swap;
		}
		for(int i = 0; i < Dots.Count(); i++)
		{
			if(i == dotA || i == dotB || i == dotC) continue;
			if(R_SIDE1(dotA, dotB, i) < 0 || R_SIDE1(dotB, dotC, i) < 0 || R_SIDE1(dotC, dotA, i) < 0)
				continue;
			return true;
		}
		return false;
	}

	bool TryWorld::Map::IS_CROSSING_ANY_LINE_BY(int dotA, int dotB)
	{
		for(int i = 0; i < Lines.Count(); ++i)
		{
			const Line* Node = &Lines[i];
			if(dotA == Node->DotA || dotA == Node->DotB
				|| dotB == Node->DotA || dotB == Node->DotB)
				continue;
			if(Math::MaxF(Dots[Node->DotA].x, Dots[Node->DotB].x) < Math::MinF(Dots[dotA].x, Dots[dotB].x)
				|| Math::MaxF(Dots[dotA].x, Dots[dotB].x) < Math::MinF(Dots[Node->DotA].x, Dots[Node->DotB].x)
				|| Math::MaxF(Dots[Node->DotA].y, Dots[Node->DotB].y) < Math::MinF(Dots[dotA].y, Dots[dotB].y)
				|| Math::MaxF(Dots[dotA].y, Dots[dotB].y) < Math::MinF(Dots[Node->DotA].y, Dots[Node->DotB].y))
				continue;
			int ResultA1 = R_SIDE1(dotA, dotB, Node->DotA);
			int ResultB1 = R_SIDE1(dotA, dotB, Node->DotB);
			int ResultA2 = R_SIDE1(Node->DotA, Node->DotB, dotA);
			int ResultB2 = R_SIDE1(Node->DotA, Node->DotB, dotB);
			if((ResultA1 == 0 || ResultB1 == 0 || (ResultA1 < 0 && 0 < ResultB1) || (0 < ResultA1 && ResultB1 < 0))
				&& (ResultA2 == 0 || ResultB2 == 0 || (ResultA2 < 0 && 0 < ResultB2) || (0 < ResultA2 && ResultB2 < 0)))
				return true;
		}
		return false;
	}

	int TryWorld::Map::FIND_LINE_ID(int dotA, int dotB)
	{
		int Count = 0;
		for(int i = 0; i < Lines.Count(); ++i)
		{
			const Line* Node = &Lines[i];
			if((dotA == Node->DotA && dotB == Node->DotB)
				|| (dotA == Node->DotB && dotB == Node->DotA))
				return Count;
			Count++;
		}
		return -1;
	}

	TryWorld::Map::Triangle* TryWorld::Map::FIND_PICK_TRIANGLE(const Point& pos)
	{
		Triangle* Node = &Top;
		while(Node = Node->Next)
		{
			if(R_SIDE1(Node->DotA, Node->DotB, Node->DotC) < 0)
			{
				if(0 < R_SIDE2(Node->DotA, Node->DotB, pos)
					|| 0 < R_SIDE2(Node->DotB, Node->DotC, pos)
					|| 0 < R_SIDE2(Node->DotC, Node->DotA, pos))
					continue;
			}
			else if(R_SIDE2(Node->DotA, Node->DotB, pos) < 0
				|| R_SIDE2(Node->DotB, Node->DotC, pos) < 0
				|| R_SIDE2(Node->DotC, Node->DotA, pos) < 0)
				continue;
			return Node;
		}
		return nullptr;
	}

	TryWorld::Map::Triangle* TryWorld::Map::FIND_SAME_TRIANGLE(int dotA, int dotB, Triangle* parent)
	{
		Triangle* Node = &Top;
		while(Node = Node->Next)
		{
			if(Node == parent) continue;
			if((dotA == Node->DotA || dotA == Node->DotB || dotA == Node->DotC)
				&& (dotB == Node->DotA || dotB == Node->DotB || dotB == Node->DotC))
				return Node;
		}
		return nullptr;
	}

	bool TryWorld::Map::PATH_FIND(Triangle* focus, const Triangle* target, const Point& endPos)
	{
		if(target == focus) return true;
		bool Result = false;
		const Point DotAB = (DISTANCE(focus->WayDot, Dots[focus->DotA]) < DISTANCE(focus->WayDot, Dots[focus->DotB]))? DOT_AB_SIDE_A(*focus) : DOT_AB_SIDE_B(*focus);
		const Point DotAC = (DISTANCE(focus->WayDot, Dots[focus->DotA]) < DISTANCE(focus->WayDot, Dots[focus->DotC]))? DOT_AC_SIDE_A(*focus) : DOT_AC_SIDE_C(*focus);
		const Point DotBC = (DISTANCE(focus->WayDot, Dots[focus->DotB]) < DISTANCE(focus->WayDot, Dots[focus->DotC]))? DOT_BC_SIDE_B(*focus) : DOT_BC_SIDE_C(*focus);
		const int DistanceAB = DISTANCE(focus->WayDot, DotAB) + ((target == focus->LinkAB)? DISTANCE(DotAB, endPos) : 0);
		const int DistanceAC = DISTANCE(focus->WayDot, DotAC) + ((target == focus->LinkAC)? DISTANCE(DotAC, endPos) : 0);
		const int DistanceBC = DISTANCE(focus->WayDot, DotBC) + ((target == focus->LinkBC)? DISTANCE(DotBC, endPos) : 0);
		if(focus->TypeAB == linetype_space && (!focus->LinkAB->WayBack || focus->DistanceSum + DistanceAB < focus->LinkAB->DistanceSum))
		{
			focus->LinkAB->WayDot = DotAB;
			focus->LinkAB->WayBack = focus;
			focus->LinkAB->DistanceSum = focus->DistanceSum + DistanceAB;
			Result |= PATH_FIND(focus->LinkAB, target, endPos);
		}
		if(focus->TypeAC == linetype_space && (!focus->LinkAC->WayBack || focus->DistanceSum + DistanceAC < focus->LinkAC->DistanceSum))
		{
			focus->LinkAC->WayDot = DotAC;
			focus->LinkAC->WayBack = focus;
			focus->LinkAC->DistanceSum = focus->DistanceSum + DistanceAC;
			Result |= PATH_FIND(focus->LinkAC, target, endPos);
		}
		if(focus->TypeBC == linetype_space && (!focus->LinkBC->WayBack || focus->DistanceSum + DistanceBC < focus->LinkBC->DistanceSum))
		{
			focus->LinkBC->WayDot = DotBC;
			focus->LinkBC->WayBack = focus;
			focus->LinkBC->DistanceSum = focus->DistanceSum + DistanceBC;
			Result |= PATH_FIND(focus->LinkBC, target, endPos);
		}
		return Result;
	}

    ////////////////////////////////////////////////////////////////////////////////
    // TryWorld::Hurdle
    ////////////////////////////////////////////////////////////////////////////////
    TryWorld::Hurdle::Hurdle() : ObjectBeginID(-1)
	{
	}

	TryWorld::Hurdle::~Hurdle()
	{
	}

	TryWorld::Hurdle* TryWorld::Hurdle::Create(Hurdle* hurdle)
	{
		Hurdle* Result = new Hurdle();
		if(hurdle) Result->List = hurdle->List;
		return Result;
	}

	void TryWorld::Hurdle::Release(Hurdle*& hurdle)
	{
		delete hurdle;
        hurdle = nullptr;
	}

	void TryWorld::Hurdle::Add(Points& polygon, const bool isBoundLine)
	{
		BOSS_ASSERT("Object가 추가된 Hurdle은 Add를 지원하지 않습니다", ObjectBeginID == -1);
		Rect Bound(polygon[0].x, polygon[0].y, polygon[0].x, polygon[0].y);
		for(int i = 1, iend = polygon.Count(); i < iend; ++i)
		{
			Bound.l = Math::MinF(Bound.l, polygon[i].x);
			Bound.t = Math::MinF(Bound.t, polygon[i].y);
			Bound.r = Math::MaxF(Bound.r, polygon[i].x);
			Bound.b = Math::MaxF(Bound.b, polygon[i].y);
		}
		for(int i = List.Count() - 1; 0 <= i; --i)
		{
			const Points* Result = MERGE_POLYGON(List[i], polygon, Bound, isBoundLine);
			if(Result)
			{
                polygon = *Result;
				List.SubtractionSection(i);
			}
		}
		List.AtAdding() = polygon;
	}

	void TryWorld::Hurdle::AddWithoutMerging(const Points& polygon)
	{
		if(ObjectBeginID == -1) ObjectBeginID = List.Count();
		List.AtAdding() = polygon;
	}

	TryWorld::Map* TryWorld::Hurdle::BuildMap(const Rect& boundBox)
	{
		BOSS_ASSERT("Object가 추가된 Hurdle은 BuildMap을 지원하지 않습니다", ObjectBeginID == -1);
		Points BoundPolygon;
		BoundPolygon.AtAdding() = Point(boundBox.l, boundBox.t);
		BoundPolygon.AtAdding() = Point(boundBox.l, boundBox.b);
		BoundPolygon.AtAdding() = Point(boundBox.r, boundBox.b);
		BoundPolygon.AtAdding() = Point(boundBox.r, boundBox.t);
		Add(BoundPolygon, true);
		Map* Result = new Map();
		Result->CREATE_TRIANGLES(boundBox, List);
		return Result;
	}

	const Points* TryWorld::Hurdle::MERGE_POLYGON(const Points& Dst, const Points& Src, const Rect SrcBound, const bool IsBoundLine)
	{
		// 한계검사
		bool IsL = true, IsT = true, IsR = true, IsB = true;
		int BestDstI = -1;
		for(int i = 0; i < Dst.Count(); ++i)
		{
			if(IsL) IsL &= (Dst[i].x < SrcBound.l);
			if(IsT) IsT &= (Dst[i].y < SrcBound.t);
			if(IsR) IsR &= (Dst[i].x > SrcBound.r);
			if(IsB) IsB &= (Dst[i].y > SrcBound.b);
			if(!IsBoundLine && BestDstI == -1 && (Dst[i].x < SrcBound.l || Dst[i].y < SrcBound.t))
				BestDstI = i;
		}
		BOSS_ASSERT("Src는 Dst의 우측하단에 위치하여야 안전합니다", IsBoundLine || BestDstI != -1);
		if(IsL || IsT || IsR || IsB) return nullptr;

		// 교차검사 수집데이터(최소한 Dst의 0번 정점은 Src에 포함되지 않아야 함)
		static Points CollectDstB;
		static Points CollectDstE;
		int CollectDstLength = Dst.Count();
        Memory::Copy(CollectDstB.AtDumping(0, CollectDstLength), &Dst[0], sizeof(Point) * CollectDstLength);
		Memory::Copy(CollectDstE.AtDumping(0, CollectDstLength), &Dst[1], sizeof(Point) * (CollectDstLength - 1));
		CollectDstE.At(CollectDstLength - 1) = Dst[0];
		static Points CollectSrcB;
		static Points CollectSrcE;
		int CollectSrcLength = Src.Count();
		Memory::Copy(CollectSrcB.AtDumping(0, CollectSrcLength), &Src[0], sizeof(Point) * CollectSrcLength);
		Memory::Copy(CollectSrcE.AtDumping(0, CollectSrcLength), &Src[1], sizeof(Point) * (CollectSrcLength - 1));
		CollectSrcE.At(CollectSrcLength - 1) = Src[0];
		for(int d = 0; d < CollectDstLength; ++d)
		for(int s = 0; s < CollectSrcLength; ++s)
		{
			const Point* CrossDot = nullptr;
			if(CrossDot = Util::GetDotByLineCross(CollectDstB[d], CollectDstE[d], CollectSrcB[s], CollectSrcE[s]))
			{
				if((CrossDot->x != CollectDstB[d].x || CrossDot->y != CollectDstB[d].y)
					&& (CrossDot->x != CollectDstE[d].x || CrossDot->y != CollectDstE[d].y))
				{
					CollectDstE.AtWherever(CollectDstLength) = CollectDstE[d];
					CollectDstB.AtWherever(CollectDstLength++) = (CollectDstE.At(d) = *CrossDot);
				}
				if((CrossDot->x != CollectSrcB[s].x || CrossDot->y != CollectSrcB[s].y)
					&& (CrossDot->x != CollectSrcE[s].x || CrossDot->y != CollectSrcE[s].y))
				{
					CollectSrcE.AtWherever(CollectSrcLength) = CollectSrcE[s];
					CollectSrcB.AtWherever(CollectSrcLength++) = (CollectSrcE.At(s) = *CrossDot);
				}
			}
		}
		if(CollectDstLength == Dst.Count())
			return nullptr;

        static Points Result;
        Result.SubtractionAll();
		if(IsBoundLine)
		{
			Result.AtAdding() = Point(0, 0);
			for(int d = 0; d < CollectDstLength; ++d)
			{
				if(CollectDstB[d].x <= SrcBound.l) continue;
				if(CollectDstB[d].y <= SrcBound.t) continue;
				if(CollectDstB[d].x >= SrcBound.r) continue;
				if(CollectDstB[d].y >= SrcBound.b) continue;
				Result.At(0) = CollectDstB[d];
				break;
			}
			BOSS_ASSERT("계산상 오류입니다", Result[0].x != 0 || Result[0].y != 0);
		}
		else Result.At(0) = Dst[BestDstI];
		do
		{
			int DstIndex = -1;
			for(int d = 0; d < CollectDstLength; ++d)
				if(Result[-1].x == CollectDstB[d].x && Result[-1].y == CollectDstB[d].y
					&& (CollectDstB[d].x != CollectDstE[d].x || CollectDstB[d].y != CollectDstE[d].y))
				{
					DstIndex = d;
					break;
				}
			int SrcIndex = -1;
			for(int s = 0; s < CollectSrcLength; ++s)
				if(Result[-1].x == CollectSrcB[s].x && Result[-1].y == CollectSrcB[s].y
					&& (CollectSrcB[s].x != CollectSrcE[s].x || CollectSrcB[s].y != CollectSrcE[s].y))
				{
					SrcIndex = s;
					break;
				}
			BOSS_ASSERT("계산상 오류입니다", DstIndex != -1 || SrcIndex != -1);
			Point SelectedDot;
			if(DstIndex != -1 && SrcIndex != -1 && 1 < Result.Count())
			{
				const float PrevAngle = Math::Atan(Result[-2].x - Result[-1].x, Result[-2].y - Result[-1].y);
				const float NextAngleDst = Math::Atan(CollectDstE[DstIndex].x - Result[-1].x, CollectDstE[DstIndex].y - Result[-1].y);
				const float NextAngleSrc = Math::Atan(CollectSrcE[SrcIndex].x - Result[-1].x, CollectSrcE[SrcIndex].y - Result[-1].y);
				if(NextAngleDst < NextAngleSrc)
				{
					if(NextAngleDst < PrevAngle && PrevAngle < NextAngleSrc)
						SelectedDot = CollectSrcE[SrcIndex];
					else SelectedDot = CollectDstE[DstIndex];
				}
				else
				{
					if(NextAngleSrc < PrevAngle && PrevAngle < NextAngleDst)
						SelectedDot = CollectDstE[DstIndex];
					else SelectedDot = CollectSrcE[SrcIndex];
				}
			}
			else if(DstIndex != -1)
				SelectedDot = CollectDstE[DstIndex];
			else SelectedDot = CollectSrcE[SrcIndex];
			if(1 < Result.Count() && ((Result[-2].x == SelectedDot.x && Result[-1].x == SelectedDot.x)
				|| (Result[-2].y == SelectedDot.y && Result[-1].y == SelectedDot.y)))
				Result.At(-1) = SelectedDot;
			else Result.AtAdding() = SelectedDot;
		}
		while(Result[-1].x != Result[0].x || Result[-1].y != Result[0].y);
        Result.SubtractionOne();
		return &Result;
	}

    ////////////////////////////////////////////////////////////////////////////////
    // TryWorld::GetPosition
    ////////////////////////////////////////////////////////////////////////////////
    TryWorld::GetPosition::GetPosition()
	{
	}

	TryWorld::GetPosition::~GetPosition()
	{
	}

	const Point TryWorld::GetPosition::SubTarget(const Hurdle* hurdle, Path* path, const Point& curPos)
	{
        if(hurdle && path)
        {
            const int CurDot = path->Dots.Count() - 1 - path->DotFocus;
            int DotBegin = Math::Max(0, CurDot - 2 * (1 + path->Step));
			for(int p = DotBegin; p < path->Dots.Count(); ++p)
			{
				bool IsFindHurdle = false;
				const Point& CurTarget = path->Dots[p];
				for(int h = 0; !IsFindHurdle && h < hurdle->List.Count(); ++h)
				{
					// curPos를 둘러싼 오브젝트는 검사에서 제외
					if(0 <= hurdle->ObjectBeginID && hurdle->ObjectBeginID <= h)
					{
						bool IsInHurdle = true;
						for(int l = 0, lend = hurdle->List[h].Count(); IsInHurdle && l < lend; ++l)
							if(0 < Util::GetClockwiseValue(hurdle->List[h][l], hurdle->List[h][(l + 1) % lend], curPos))
								IsInHurdle = false;
						if(IsInHurdle) continue;
					}
					for(int l = 0, lend = hurdle->List[h].Count(); !IsFindHurdle && l < lend; ++l)
					{
						IsFindHurdle = (nullptr != Util::GetDotByLineCross(hurdle->List[h][l], hurdle->List[h][(l + 1) % lend], curPos, CurTarget));
						if(IsFindHurdle && !Util::GetClockwiseValue(hurdle->List[h][l], hurdle->List[h][(l + 1) % lend], curPos))
							IsFindHurdle = false;
					}
				}
				if(!IsFindHurdle)
                {
                    path->DotFocus = path->Dots.Count() - 1 - p;
                    return CurTarget;
                }
			}
        }
		return curPos;
	}

	const Point TryWorld::GetPosition::ValidNext(const Hurdle* hurdle, const Point& curPos, const Point& nextPos)
	{
		for(int h = 0; h < hurdle->List.Count(); ++h)
		for(int l = 0, lend = hurdle->List[h].Count(); l < lend; ++l)
		{
			const Point* Result = Util::GetDotByLineCross(hurdle->List[h][l], hurdle->List[h][(l + 1) % lend], curPos, nextPos);
			if(Result && Util::GetClockwiseValue(hurdle->List[h][l], hurdle->List[h][(l + 1) % lend], curPos))
				return *Result;
		}
		return nextPos;
	}
}
