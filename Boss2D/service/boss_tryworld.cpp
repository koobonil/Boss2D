#include <boss.hpp>
#include "boss_tryworld.hpp"

namespace BOSS
{
    ////////////////////////////////////////////////////////////////////////////////
    // TryWorld::Util
    ////////////////////////////////////////////////////////////////////////////////
    const TryWorld::Dot* TryWorld::Util::GetDotByLineCross(const Dot& DstB, const Dot& DstE, const Dot& SrcB, const Dot& SrcE)
    {
        static Dot Result;
        // 최대최소평가
        const float DstMinX = Math::MinF(DstB.x, DstE.x), SrcMaxX = Math::MaxF(SrcB.x, SrcE.x);
		if(DstMinX > SrcMaxX) return nullptr;
        const float DstMaxX = Math::MaxF(DstB.x, DstE.x), SrcMinX = Math::MinF(SrcB.x, SrcE.x);
		if(DstMaxX < SrcMinX) return nullptr;
        const float DstMinY = Math::MinF(DstB.y, DstE.y), SrcMaxY = Math::MaxF(SrcB.y, SrcE.y);
		if(DstMinY > SrcMaxY) return nullptr;
        const float DstMaxY = Math::MaxF(DstB.y, DstE.y), SrcMinY = Math::MinF(SrcB.y, SrcE.y);
		if(DstMaxY < SrcMinY) return nullptr;

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
                if(DstMinY <= SrcB.y && SrcB.y <= DstMaxY)
                    Result = SrcB;
                else if(DstMinY <= SrcE.y && SrcE.y <= DstMaxY)
                    Result = SrcE;
                else Result = DstB;
			}
			// 수평
			else if(DstB.y == DstE.y && SrcB.y == SrcE.y)
			{
                if(DstMinX <= SrcB.x && SrcB.x <= DstMaxX)
                    Result = SrcB;
                else if(DstMinX <= SrcE.x && SrcE.x <= DstMaxX)
                    Result = SrcE;
                else Result = DstB;
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
			// 꼭지점
            else if((DstB.x == SrcB.x && DstB.y == SrcB.y) || (DstB.x == SrcE.x && DstB.y == SrcE.y))
                Result = DstB;
            else if((DstE.x == SrcE.x && DstE.y == SrcE.y) || (DstE.x == SrcB.x && DstE.y == SrcB.y))
                Result = DstE;
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

    bool TryWorld::Util::PtInPolygon(const DotList& Polygon, const Dot& Pt)
    {
        int LeftSideCount = 0;
        for(int i = 0, iend = Polygon.Count(); i < iend; ++i)
        {
            const auto& LineB = Polygon[i];
            const auto& LineE = Polygon[(i + 1) % iend];
            if(LineB.y == LineE.y) // 수평선처리
            {
                if(LineB.y == Pt.y && Math::MinF(LineB.x, LineE.x) <= Pt.x && Pt.x <= Math::MaxF(LineB.x, LineE.x))
                    return true;
            }
            else if(Math::MinF(LineB.y, LineE.y) < Pt.y && Pt.y <= Math::MaxF(LineB.y, LineE.y))
            {
                const int CWValue = GetClockwiseValue(LineB, LineE, Pt);
                if(CWValue == 0) return true;
                else if(((LineB.y < LineE.y)? CWValue : -CWValue) < 0)
                    LeftSideCount++;
            }
        }
        return LeftSideCount & 1; // Pt보다 좌측점이 홀수면 폴리곤안쪽
    }

    bool TryWorld::Util::PtInPolyLine(const DotList& Polygon, const Dot& Pt)
    {
        for(int i = 0, iend = Polygon.Count(); i < iend; ++i)
        {
            const auto& LineB = Polygon[i];
            const auto& LineE = Polygon[(i + 1) % iend];
            if(GetClockwiseValue(LineB, LineE, Pt) == 0)
            {
                if(Pt.x < Math::MinF(LineB.x, LineE.x)) continue;
                if(Pt.x > Math::MaxF(LineB.x, LineE.x)) continue;
                if(Pt.y < Math::MinF(LineB.y, LineE.y)) continue;
                if(Pt.y > Math::MaxF(LineB.y, LineE.y)) continue;
                return true;
            }
        }
        return false;
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

    TryWorld::Path* TryWorld::Map::CreatePath(const int step)
    {
        return new Path(step);
    }

    TryWorld::Path* TryWorld::Map::BuildPath(const Dot& beginPos, const Dot& endPos, const int step, int* score, ScoreCB cb)
	{
		Triangle* ClearNode = &Top;
		while(ClearNode = ClearNode->Next)
		{
			ClearNode->WayDot = Dot(0, 0);
			ClearNode->WayBack = nullptr;
            ClearNode->ObjectScore = -1;
			ClearNode->DistanceSum = 0;
		}
        Triangle* Begin = FIND_PICK_TRIANGLE(beginPos);
        Triangle* End = FIND_PICK_TRIANGLE(endPos);
		if(Begin && End)
		{
			Begin->WayDot = beginPos;
			Begin->WayBack = (Triangle*) -1;
            bool IsSuccess = PATH_FIND(Begin, End, endPos, cb);
			Begin->WayBack = nullptr;
			if(IsSuccess)
			{
                Path* Result = CreatePath(step);
				Result->Dots.AtAdding() = endPos;
				Triangle* CurTriangle = End;
				while(CurTriangle)
				{
					// 중간정점 삽입
					if(0 < step)
					{
						const Dot DotA = Result->Dots[-1];
						const Dot DotB = CurTriangle->WayDot;
						const int Distance = Math::Sqrt(Math::Pow(DotB.x - DotA.x) + Math::Pow(DotB.y - DotA.y));
						for(int f = step; f < Distance; f += step)
							Result->Dots.AtAdding() = Dot((DotA.x * (Distance - f) + DotB.x * f) / Distance, (DotA.y * (Distance - f) + DotB.y * f) / Distance);
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

    bool TryWorld::Map::MAPPING(PolygonList& list)
	{
		for(int i = 0; i < list.Count(); ++i)
        for(int j = 0, jend = list[i].DotArray.Count(); j < jend; ++j)
        {
            const int CurIndex = Dots.Count();
            Dots.AtAdding() = list[i].DotArray[j];
            if(!list[i].Enable)
                Lines.AtAdding().Set(linetype_space, CurIndex, CurIndex + ((j + 1) % jend) - j);
            else if(i == list.Count() - 1)
                Lines.AtAdding().Set(linetype_bound, CurIndex, CurIndex + ((j + 1) % jend) - j);
            else Lines.AtAdding().Set(linetype_wall, CurIndex, CurIndex + ((j + 1) % jend) - j);
        }
		if(!CREATE_TRIANGLES(Top.INSERT_FIRST(), nullptr, linetype_bound, Dots.Count() - 2, Dots.Count() - 1))
        {
            Top.DELETE_FIRST();
            return false;
        }
        return true;
	}

	bool TryWorld::Map::CREATE_TRIANGLES(Triangle* focus, Triangle* parent, linetype type, int dotA, int dotB)
	{
		for(int dotC = 0; dotC < Dots.Count(); ++dotC)
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
                int IndexLineAC = -1;
                int IndexLineBC = -1;
				if(LineAC == -1)
                {
                    IndexLineAC = Lines.Count();
                    Lines.AtAdding().Set(linetype_space, focus->DotA, focus->DotC);
                }
				if(LineBC == -1)
                {
                    IndexLineBC = Lines.Count();
                    Lines.AtAdding().Set(linetype_space, focus->DotB, focus->DotC);
                }
				// 연결되는 삼각형정보
				focus->LinkAB = parent;
				if(focus->TypeAC != linetype_bound && !(focus->LinkAC = FIND_SAME_TRIANGLE(focus->DotA, focus->DotC, focus)))
                {
                    if(!CREATE_TRIANGLES(focus->LinkAC = Top.INSERT_FIRST(), focus, focus->TypeAC, focus->DotA, focus->DotC))
                    {
                        focus->TypeAC = linetype_wall;
                        focus->LinkAC = Top.DELETE_FIRST();
                        if(IndexLineAC != -1)
                            Lines.At(IndexLineAC).Set(linetype_wall, focus->DotA, focus->DotC);
                    }
                }
				if(focus->TypeBC != linetype_bound && !(focus->LinkBC = FIND_SAME_TRIANGLE(focus->DotB, focus->DotC, focus)))
                {
					if(!CREATE_TRIANGLES(focus->LinkBC = Top.INSERT_FIRST(), focus, focus->TypeBC, focus->DotB, focus->DotC))
                    {
                        focus->TypeBC = linetype_wall;
                        focus->LinkBC = Top.DELETE_FIRST();
                        if(IndexLineBC != -1)
                            Lines.At(IndexLineBC).Set(linetype_wall, focus->DotB, focus->DotC);
                    }
                }
				return true;
			}
		}
        return false;
	}

    bool TryWorld::Map::IS_INCLUDE_ANY_DOT_BY(int dotA, int dotB, int dotC) const
	{
		if(R_SIDE1(dotA, dotB, dotC) < 0)
		{
			int Swap = dotB;
			dotB = dotC;
			dotC = Swap;
		}
		for(int i = 0; i < Dots.Count(); ++i)
		{
			if(i == dotA || i == dotB || i == dotC) continue;
			if(R_SIDE1(dotA, dotB, i) < 0 || R_SIDE1(dotB, dotC, i) < 0 || R_SIDE1(dotC, dotA, i) < 0)
				continue;
			return true;
		}
		return false;
	}

    bool TryWorld::Map::IS_CROSSING_ANY_LINE_BY(int dotA, int dotB) const
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

    int TryWorld::Map::FIND_LINE_ID(int dotA, int dotB) const
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

    TryWorld::Map::Triangle* TryWorld::Map::FIND_PICK_TRIANGLE(const Dot& pos)
	{
        Triangle* Node = &Top;
		while(Node = Node->Next)
		{
            BOSS_ASSERT("잘못된 시나리오입니다", Node->DotA != -1 && Node->DotB != -1 && Node->DotC != -1);
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
            BOSS_ASSERT("잘못된 시나리오입니다", Node->DotA != -1 && Node->DotB != -1 && Node->DotC != -1);
			if(Node == parent) continue;
			if((dotA == Node->DotA || dotA == Node->DotB || dotA == Node->DotC)
				&& (dotB == Node->DotA || dotB == Node->DotB || dotB == Node->DotC))
				return Node;
		}
		return nullptr;
	}

    bool TryWorld::Map::PATH_FIND(Triangle* focus, const Triangle* target, const Dot& endPos, ScoreCB cb) const
	{
        if(target == focus) return true;
        else if(focus->ObjectScore == -1)
        {
            if(cb) focus->ObjectScore = cb(Dots[focus->DotA], Dots[focus->DotB], Dots[focus->DotC]);
            else focus->ObjectScore = 0;
        }

		bool Result = false;
		const Dot DotAB = (DISTANCE(focus->WayDot, Dots[focus->DotA]) < DISTANCE(focus->WayDot, Dots[focus->DotB]))? DOT_AB_SIDE_A(*focus) : DOT_AB_SIDE_B(*focus);
		const Dot DotAC = (DISTANCE(focus->WayDot, Dots[focus->DotA]) < DISTANCE(focus->WayDot, Dots[focus->DotC]))? DOT_AC_SIDE_A(*focus) : DOT_AC_SIDE_C(*focus);
		const Dot DotBC = (DISTANCE(focus->WayDot, Dots[focus->DotB]) < DISTANCE(focus->WayDot, Dots[focus->DotC]))? DOT_BC_SIDE_B(*focus) : DOT_BC_SIDE_C(*focus);
        const int DistanceAB = DISTANCE(focus->WayDot, DotAB) + ((target == focus->LinkAB)? DISTANCE(DotAB, endPos) : 0) + focus->ObjectScore;
        const int DistanceAC = DISTANCE(focus->WayDot, DotAC) + ((target == focus->LinkAC)? DISTANCE(DotAC, endPos) : 0) + focus->ObjectScore;
        const int DistanceBC = DISTANCE(focus->WayDot, DotBC) + ((target == focus->LinkBC)? DISTANCE(DotBC, endPos) : 0) + focus->ObjectScore;
		if(focus->TypeAB == linetype_space && (!focus->LinkAB->WayBack || focus->DistanceSum + DistanceAB < focus->LinkAB->DistanceSum))
		{
			focus->LinkAB->WayDot = DotAB;
			focus->LinkAB->WayBack = focus;
			focus->LinkAB->DistanceSum = focus->DistanceSum + DistanceAB;
            Result |= PATH_FIND(focus->LinkAB, target, endPos, cb);
		}
		if(focus->TypeAC == linetype_space && (!focus->LinkAC->WayBack || focus->DistanceSum + DistanceAC < focus->LinkAC->DistanceSum))
		{
			focus->LinkAC->WayDot = DotAC;
			focus->LinkAC->WayBack = focus;
			focus->LinkAC->DistanceSum = focus->DistanceSum + DistanceAC;
            Result |= PATH_FIND(focus->LinkAC, target, endPos, cb);
		}
		if(focus->TypeBC == linetype_space && (!focus->LinkBC->WayBack || focus->DistanceSum + DistanceBC < focus->LinkBC->DistanceSum))
		{
			focus->LinkBC->WayDot = DotBC;
			focus->LinkBC->WayBack = focus;
			focus->LinkBC->DistanceSum = focus->DistanceSum + DistanceBC;
            Result |= PATH_FIND(focus->LinkBC, target, endPos, cb);
		}
		return Result;
	}

    ////////////////////////////////////////////////////////////////////////////////
    // TryWorld::Hurdle
    ////////////////////////////////////////////////////////////////////////////////
    TryWorld::Hurdle::Hurdle()
	{
        BuildFlag = false;
	}

	TryWorld::Hurdle::~Hurdle()
	{
	}

	TryWorld::Hurdle* TryWorld::Hurdle::Create(Hurdle* hurdle)
	{
		Hurdle* Result = new Hurdle();
		if(hurdle)
        {
            Result->BuildFlag = hurdle->BuildFlag;
            Result->List = hurdle->List;
            if(Result->BuildFlag)
            {
                Result->BuildFlag = false;
                Result->List.SubtractionOne();
            }
        }
		return Result;
	}

	void TryWorld::Hurdle::Release(Hurdle*& hurdle)
	{
		delete hurdle;
        hurdle = nullptr;
	}

    bool TryWorld::Hurdle::Add(DotList& polygon, bool error_test)
	{
		bool TestResult = true;
		for(int i = List.Count() - 1; 0 <= i; --i)
		{
            bool GetError = false;
            const DotList* Result = MERGE_POLYGON(List[i].DotArray, polygon, (error_test)? &GetError : nullptr);
			if(Result)
			{
                polygon = *Result;
				List.SubtractionSection(i);
			}
            TestResult &= !GetError;
		}
        auto& NewPolygon = List.AtAdding();
        NewPolygon.Enable = (2 < polygon.Count());
        NewPolygon.DotArray = polygon;
        return TestResult;
	}

    void TryWorld::Hurdle::AddWithoutMerging(const DotList& polygon)
	{
        auto& NewPolygon = List.AtAdding();
        NewPolygon.Enable = (2 < polygon.Count());
        NewPolygon.DotArray = polygon;
	}

	TryWorld::Map* TryWorld::Hurdle::BuildMap(const Rect& boundBox)
	{
        if(BuildFlag)
            List.SubtractionOne();
        else BuildFlag = true;

		DotList BoundPolygon;
		BoundPolygon.AtAdding() = Dot(boundBox.l, boundBox.t);
		BoundPolygon.AtAdding() = Dot(boundBox.l, boundBox.b);
		BoundPolygon.AtAdding() = Dot(boundBox.r, boundBox.b);
		BoundPolygon.AtAdding() = Dot(boundBox.r, boundBox.t);
        AddWithoutMerging(BoundPolygon);
		Map* Result = new Map();
        if(!Result->MAPPING(List))
        {
            delete Result;
            return nullptr;
        }
		return Result;
	}

	const TryWorld::DotList* TryWorld::Hurdle::MERGE_POLYGON(const DotList& dst, const DotList& src, bool* error)
	{
        const Dot* Dst = &dst[0];
        const Dot* Src = &src[0];
        // Src의 바깥에 존재하는 Dst점 찾기
        int DstBegin = -1;
        for(int d = 0, dend = dst.Count(); d < dend; ++d)
        {
            if(!Util::PtInPolygon(src, Dst[d]))
            {
                DstBegin = d;
                break;
            }
        }
        if(DstBegin == -1)
            return nullptr;

        // 교점캐시
        struct CrossDotCache
        {
            int status; // 0: 미실시, -1: 교점없음, 1:교점있음
            float x;
            float y;
            Dot ToDot() const {return Dot(x, y);}
            bool SameTest(const CrossDotCache& other) {return SameTest(x, y, other.x, other.y);}
            static bool SameTest(float x1, float y1, float x2, float y2)
            {
                if(x1 == x2 && y1 == y2) return true;
                const float dist_x = x1 - x2;
                const float dist_y = y1 - y2;
                if(dist_x > 0.0001) return false;
                if(dist_x < -0.0001) return false;
                if(dist_y > 0.0001) return false;
                if(dist_y < -0.0001) return false;
                return true;
            }
        };
        static Array<CrossDotCache, datatype_pod_canmemcpy_zeroset> CrossDotCaches;
        CrossDotCaches.SubtractionAll();
        auto CrossDots = CrossDotCaches.AtDumpingAdded(dst.Count() * src.Count());
        #define READY_CROSSDOT(DOTNAME, DSTB, DSTE, SRCB, SRCE) \
            do { \
                if(DOTNAME.status == 0) \
                { \
                    const Dot* CrossResult = Util::GetDotByLineCross(DSTB, DSTE, SRCB, SRCE); \
                    if(CrossResult) \
                    { \
                        DOTNAME.status = 1; \
                        DOTNAME.x = CrossResult->x; \
                        DOTNAME.y = CrossResult->y; \
                    } \
                    else DOTNAME.status = -1; \
                } \
            } while(false)

        // 시작교차점 검색
        for(int d = DstBegin, dend = dst.Count(); d < dend; ++d)
        {
            const Dot& DstB = Dst[d];
            const Dot& DstE = Dst[(d + 1) % dend];
		    for(int s = 0, send = src.Count(); s < send; ++s)
		    {
                const Dot& SrcB = Src[s];
                const Dot& SrcE = Src[(s + 1) % send];
                CrossDotCache& CrossDot = CrossDots[d + s * dend];
                READY_CROSSDOT(CrossDot, DstB, DstE, SrcB, SrcE);
			    if(CrossDot.status == 1)
                {
                    // 한 선분에 다수의 교차점이 발생시 대처
                    bool TestResult = true;
                    float DstDistance = -1, SrcDistance = -1;
                    for(int s2 = s + 1; s2 < send; ++s2)
                    {
                        const Dot& Src2B = Src[s2];
                        const Dot& Src2E = Src[(s2 + 1) % send];
                        CrossDotCache& CrossDotForDist = CrossDots[d + s2 * dend];
                        READY_CROSSDOT(CrossDotForDist, DstB, DstE, Src2B, Src2E);
                        if(CrossDotForDist.status == 1)
                        {
                            if(CrossDot.SameTest(CrossDotForDist))
                            {
                                if(SrcDistance < 0)
                                    SrcDistance = Math::Distance(SrcB.x, SrcB.y, CrossDot.x, CrossDot.y);
                                const float SrcDistance2 = Math::Distance(Src2B.x, Src2B.y, CrossDotForDist.x, CrossDotForDist.y);
                                if(SrcDistance > SrcDistance2)
                                {
                                    TestResult = false;
                                    break;
                                }
                            }
                            else
                            {
                                if(DstDistance < 0)
                                    DstDistance = Math::Distance(DstB.x, DstB.y, CrossDot.x, CrossDot.y);
                                const float DstDistance2 = Math::Distance(DstB.x, DstB.y, CrossDotForDist.x, CrossDotForDist.y);
                                if(DstDistance > DstDistance2)
                                {
                                    TestResult = false;
                                    break;
                                }
                            }
                        }
                    }
                    if(!TestResult) continue;

                    // 수집시작
                    static DotList Collector;
                    Collector.SubtractionAll();
                    for(int i = DstBegin; i <= d; ++i)
                        Collector.AtAdding() = Dst[i];
                    Collector.AtAdding() = CrossDot.ToDot();

                    // 마감교차점 검색
                    for(int _s2 = 1; _s2 < send; ++_s2)
                    {
                        const int s2 = (s + _s2) % send;
                        const Dot& Src2B = Src[s2];
                        const Dot& Src2E = Src[(s2 + 1) % send];
                        Collector.AtAdding() = Src2B; // 수집
                        for(int d2 = 0; d2 < dend; ++d2)
                        {
                            // 한번 교차된 두 선분짝은 다시 사용불가
                            if(d2 == d && s2 == s) continue;
                            const Dot& Dst2B = Dst[d2];
                            const Dot& Dst2E = Dst[(d2 + 1) % dend];
                            CrossDotCache& CrossDot2 = CrossDots[d2 + s2 * dend];
                            READY_CROSSDOT(CrossDot2, Dst2B, Dst2E, Src2B, Src2E);
                            if(CrossDot2.status == 1)
                            {
                                Collector.AtAdding() = CrossDot2.ToDot(); // 수집
                                if(DstBegin == 0) // 시작점이 변동되지 않은 경우에만 마무리수집이 필요
                                for(int i = d2 + 1; i < dend; ++i)
                                    Collector.AtAdding() = Dst[i]; // 마무리수집
                                // 중복점 정리
                                for(int i = Collector.Count() - 1; 0 <= i; --i)
                                {
                                    const Dot& DotA = Collector[i];
                                    const Dot& DotB = Collector[(i + 1) % Collector.Count()];
                                    if(CrossDotCache::SameTest(DotA.x, DotA.y, DotB.x, DotB.y))
                                        Collector.SubtractionSection(i);
                                }

                                // 이후 Src에 교차점이 더 있다면 에러체크
                                if(error)
                                {
                                    for(int d3 = 0; d3 < dend; ++d3)
                                    {
                                        const Dot& Dst3B = Dst[d3];
                                        const Dot& Dst3E = Dst[(d3 + 1) % dend];
		                                for(int s3 = 0; s3 < send; ++s3)
		                                {
                                            const Dot& Src3B = Src[s3];
                                            const Dot& Src3E = Src[(s3 + 1) % send];
                                            CrossDotCache& CrossDot3 = CrossDots[d3 + s3 * dend];
                                            READY_CROSSDOT(CrossDot3, Dst3B, Dst3E, Src3B, Src3E);
			                                if(CrossDot3.status == 1)
                                            {
                                                bool SameTest1 = true;
                                                if(d3 != d || s3 != s)
                                                    SameTest1 = CrossDot3.SameTest(CrossDots[d + s * dend]);
                                                bool SameTest2 = true;
                                                if(d3 != d2 || s3 != s2)
                                                    SameTest2 = CrossDot3.SameTest(CrossDots[d2 + s2 * dend]);
                                                if(!SameTest1 && !SameTest2)
                                                if(!Util::PtInPolyLine(Collector, CrossDot3.ToDot()))
                                                {
                                                    *error = true;
                                                    return &Collector;
                                                }
                                            }
                                        }
                                    }
                                    *error = false;
                                }
                                return &Collector;
                            }
                        }
                    }
                }
            }
        }
        return nullptr;
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

    bool TryWorld::GetPosition::SubTarget(const Hurdle* hurdle, Path* path, const Point& curPos, Point& targetPos)
	{
        if(hurdle && path)
        {
            const int CurDot = path->Dots.Count() - 1 - path->DotFocus;
            int DotBegin = Math::Max(0, CurDot - 2 * (1 + path->Step));
			for(int p = DotBegin; p < path->Dots.Count(); ++p)
			{
				bool IsFindHurdle = false;
				const Dot& CurTarget = path->Dots[p];
				for(int h = 0; !IsFindHurdle && h < hurdle->List.Count(); ++h)
				{
                    if(!hurdle->List[h].Enable) continue;
                    const auto& CurDots = hurdle->List[h].DotArray;
					for(int l = 0, lend = CurDots.Count(); !IsFindHurdle && l < lend; ++l)
					{
                        IsFindHurdle = (nullptr != Util::GetDotByLineCross(CurDots[l], CurDots[(l + 1) % lend], curPos, CurTarget));
                        if(IsFindHurdle && 0 < Util::GetClockwiseValue(CurDots[l], CurDots[(l + 1) % lend], curPos))
							IsFindHurdle = false;
					}
				}
				if(!IsFindHurdle)
                {
                    path->DotFocus = path->Dots.Count() - 1 - p;
                    targetPos = Point(CurTarget.x, CurTarget.y);
                    return true;
                }
			}
        }
        return false;
	}

    const TryWorld::Dot** TryWorld::GetPosition::GetValidNext(const Hurdle* hurdle, const Point& curPos, const Point& nextPos,
        Point& resultPos, Point& reflectPos, float distanceMin)
	{
        static const Dot* ResultArray[2] = {nullptr, nullptr};
        const Dot** Result = nullptr;
        Dot ResultPos = nextPos;
        float ResultDistance = distanceMin;
		for(int h = 0; h < hurdle->List.Count(); ++h)
        {
            if(!hurdle->List[h].Enable) continue;
            const auto& CurDots = hurdle->List[h].DotArray;
            for(int i = 0, iend = CurDots.Count(); i < iend; ++i)
            {
                const Dot& LineBegin = CurDots[i];
                const Dot& LineEnd = CurDots[(i + 1) % iend];
                if(Util::GetClockwiseValue(LineBegin, LineEnd, curPos) < 0)
                if(const Dot* CrossPos = Util::GetDotByLineCross(LineBegin, LineEnd, curPos, nextPos))
                {
                    const float CurDistance = Math::Distance(curPos.x, curPos.y, CrossPos->x, CrossPos->y);
                    if(ResultDistance < 0 || CurDistance < ResultDistance)
                    {
                        ResultArray[0] = &LineBegin;
                        ResultArray[1] = &LineEnd;
                        Result = ResultArray;
                        ResultPos = *CrossPos;
                        ResultDistance = CurDistance;

                        const float LineDx = LineEnd.x - LineBegin.x;
                        const float LineDy = LineEnd.y - LineBegin.y;
                        const float TValue = ((curPos.x - LineBegin.x) * LineDx + (curPos.y - LineBegin.y) * LineDy)
                            / (LineDx * LineDx + LineDy * LineDy);
                        const float NearX = LineBegin.x + TValue * LineDx;
                        const float NearY = LineBegin.y + TValue * LineDy;
                        reflectPos.x = curPos.x + (ResultPos.x - NearX) * 2;
                        reflectPos.y = curPos.y + (ResultPos.y - NearY) * 2;
                    }
                }
            }
        }
        resultPos = Point(ResultPos.x, ResultPos.y);
        return Result;
	}
}
