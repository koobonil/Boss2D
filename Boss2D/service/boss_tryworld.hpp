#pragma once
#include <platform/boss_platform.hpp>
#include <element/boss_point.hpp>
#include <element/boss_rect.hpp>

namespace BOSS
{
    //! \brief 길찾기
    class TryWorld
    {
        // Dot
        private: typedef Array<Point, datatype_class_canmemcpy, 4> DotList;
        // Line
        private: enum linetype {linetype_bound, linetype_space, linetype_wall};
	    private: struct Line
	    {
		    linetype Type;
		    int DotA;
		    int DotB;
		    void Set(linetype type, int dotA, int dotB)
		    {
			    Type = type;
			    DotA = dotA;
			    DotB = dotB;
		    }
        };
	    private: typedef Array<Line, datatype_pod_canmemcpy, 8> LineList;
        // Polygon
        private: class Polygon
        {
            public: Polygon() {Payload = -1;}
            public: ~Polygon() {}
            public: int Payload;
            public: DotList Dots;
        };
        private: typedef Array<Polygon, datatype_class_nomemcpy, 8> PolygonList;

        public: class Util;
        public: class Path;
        public: class Map;
        public: class Hurdle;
        public: class GetPosition;

        public: class Util
        {
            public: static inline const int GetClockwiseValue(const Point& Start, const Point& End, const Point& Src)
	        {return ((Start.x - End.x) * (Src.y - End.y) - (Start.y - End.y) * (Src.x - End.x) < 0)? -1 : 1;}
            public: static const Point* GetDotByLineCross(const Point& DstB, const Point& DstE, const Point& SrcB, const Point& SrcE);
        };

	    public: class Path
	    {
		    friend class Map;
            friend class GetPosition;
            private: const int Step;
            public: int DotFocus;
		    public: DotList Dots;

            private: Path(int step) : Step(step)
		    {
                DotFocus = 0;
		    }
		    public: ~Path()
		    {
		    }

		    public: static void Release(Path*& path)
		    {
			    delete path;
                path = nullptr;
		    }
	    };

	    public: class Map
	    {
		    friend class Hurdle;
		    public: DotList Dots;
		    public: LineList Lines;
		    private: class Triangle
		    {
			    friend class Map;
			    private: linetype TypeAB;
			    private: linetype TypeAC;
			    private: linetype TypeBC;
			    private: int DotA;
			    private: int DotB;
			    private: int DotC;
			    private: Triangle* LinkAB;
			    private: Triangle* LinkAC;
			    private: Triangle* LinkBC;
			    private: Point WayDot;
			    private: Triangle* WayBack;
			    private: int DistanceSum;
			    private: Triangle* Next;

			    private: Triangle()
			    {
				    TypeAB = linetype_bound;
				    TypeAC = linetype_bound;
				    TypeBC = linetype_bound;
				    DotA = -1;
				    DotB = -1;
				    DotC = -1;
				    LinkAB = nullptr;
				    LinkAC = nullptr;
				    LinkBC = nullptr;
				    WayDot = Point(0, 0);
				    WayBack = nullptr;
				    DistanceSum = 0;
				    Next = nullptr;
			    }
			    public: ~Triangle()
			    {
				    Triangle* DeleteNode = Next;
				    while(DeleteNode)
				    {
					    Triangle* DeleteNodeNext = DeleteNode->Next;
					    DeleteNode->Next = nullptr;
					    delete DeleteNode;
					    DeleteNode = DeleteNodeNext;
				    }
			    }

			    private: Triangle* INSERT_FIRST()
			    {
				    Triangle* Result = new Triangle();
				    Result->Next = Next;
				    return Next = Result;
			    }
		    } Top;

		    private: Map();
		    public: ~Map();
		    public: static void Release(Map*& map);
		    public: Path* BuildPath(const Point& beginPos, const Point& endPos, const int step = 0, int* score = nullptr);
		    private: void CREATE_TRIANGLES(const Rect& boundBox, PolygonList& list);
		    private: void MAPPING(Triangle* focus, Triangle* parent, linetype type, int dotA, int dotB);
		    private: bool IS_INCLUDE_ANY_DOT_BY(int dotA, int dotB, int dotC);
		    private: bool IS_CROSSING_ANY_LINE_BY(int dotA, int dotB);
		    private: int FIND_LINE_ID(int dotA, int dotB);
		    private: Triangle* FIND_PICK_TRIANGLE(const Point& pos);
		    private: Triangle* FIND_SAME_TRIANGLE(int dotA, int dotB, Triangle* parent);
		    private: bool PATH_FIND(Triangle* focus, const Triangle* target, const Point& endPos);

		    private: inline const int R_SIDE1(const int dotA, const int dotB, const int dotC)
		    {return ((Dots[dotA].x - Dots[dotB].x) * (Dots[dotC].y - Dots[dotB].y) - (Dots[dotA].y - Dots[dotB].y) * (Dots[dotC].x - Dots[dotB].x));}
		    private: inline const int R_SIDE2(const int dotA, const int dotB, const Point& C)
		    {return ((Dots[dotA].x - Dots[dotB].x) * (C.y - Dots[dotB].y) - (Dots[dotA].y - Dots[dotB].y) * (C.x - Dots[dotB].x));}
		    private: inline const int DISTANCE(const Point& A, const Point& B) {return Math::Sqrt(Math::Pow(A.x - B.x) + Math::Pow(A.y - B.y));}
		    private: inline const Point DOT_AB_SIDE_A(const Triangle& t) {return Point((Dots[t.DotA].x * 2 + Dots[t.DotB].x) / 3, (Dots[t.DotA].y * 2 + Dots[t.DotB].y) / 3);}
		    private: inline const Point DOT_AC_SIDE_A(const Triangle& t) {return Point((Dots[t.DotA].x * 2 + Dots[t.DotC].x) / 3, (Dots[t.DotA].y * 2 + Dots[t.DotC].y) / 3);}
		    private: inline const Point DOT_BC_SIDE_B(const Triangle& t) {return Point((Dots[t.DotB].x * 2 + Dots[t.DotC].x) / 3, (Dots[t.DotB].y * 2 + Dots[t.DotC].y) / 3);}
		    private: inline const Point DOT_AB_SIDE_B(const Triangle& t) {return Point((Dots[t.DotA].x + Dots[t.DotB].x * 2) / 3, (Dots[t.DotA].y + Dots[t.DotB].y * 2) / 3);}
		    private: inline const Point DOT_AC_SIDE_C(const Triangle& t) {return Point((Dots[t.DotA].x + Dots[t.DotC].x * 2) / 3, (Dots[t.DotA].y + Dots[t.DotC].y * 2) / 3);}
		    private: inline const Point DOT_BC_SIDE_C(const Triangle& t) {return Point((Dots[t.DotB].x + Dots[t.DotC].x * 2) / 3, (Dots[t.DotB].y + Dots[t.DotC].y * 2) / 3);}
	    };

	    public: class Hurdle
	    {
		    friend class GetPosition;
		    private: PolygonList List;
		    private: int ObjectBeginID;

		    private: Hurdle();
		    public: ~Hurdle();
		    public: static Hurdle* Create(Hurdle* hurdle = nullptr);
		    public: static void Release(Hurdle*& hurdle);
            public: void Add(int payload, Points& polygon, const bool isBoundLine = false);
            public: void AddWithoutMerging(int payload, const Points& polygon);
		    public: Map* BuildMap(const Rect& boundBox);
		    private: const Points* MERGE_POLYGON(const Points& Dst, const Points& Src, const Rect SrcBound, const bool IsBoundLine);
	    };

	    public: class GetPosition
	    {
		    private: GetPosition();
		    private: ~GetPosition();
		    public: static const Point SubTarget(const Hurdle* hurdle, Path* path, const Point& curPos);
            public: static const Polygon* GetValidNext(const Hurdle* hurdle, const Point& curPos, Point& nextPos,
                float distanceMin = -1, Point* reflectPos = nullptr);
	    };
    };
}
