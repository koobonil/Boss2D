#include <platform/boss_platform.hpp>
#include "boss_parasource.hpp"

namespace BOSS
{
    class ContactPool
    {
    public:
        typedef Map<ContactPool> ContactPoolMap;
        typedef Map<ContactPoolMap> ContactPoolMapMap;
        class ST
        {
            static ContactPoolMapMap _;
        public:
            static ContactPool& GetContact(chars domain, uint16 port) {return _(domain)[port];}
            static bool RemoveContact(chars domain, uint16 port)
            {
                if(auto Domain = _.Access(domain))
                if(Domain->Remove(port))
                {
                    if(Domain->Count() == 0)
                        _.Remove(domain);
                    return true;
                }
                return false;
            }
        };

    public:
        ContactPool() {mConnected = false; mSocket = nullptr; mPort = 0; mTimeOut = 0;}
        ~ContactPool() {Platform::Socket::Close(mSocket);}

    public:
        void InitForFirstTime(chars domain, uint16 port, sint32 timeout)
        {
            if(!mSocket)
            {
                mConnected = false;
                mSocket = Platform::Socket::OpenForTcp();
                mDomain = domain;
                mPort = port;
            }
            mTimeOut = timeout;
        }
        bool Connect()
        {
            if(!mConnected)
            {
                mConnected = Platform::Socket::Connect(mSocket, mDomain, mPort, mTimeOut);
                if(!mConnected) return false;
            }
            return true;
        }
        void Disconnect()
        {
            mConnected = false;
            Platform::Socket::Disconnect(mSocket, mTimeOut);
        }
        bool Send(chars data, sint32 size)
        {
            if(!Connect()) return false;
            while(mConnected && 0 < size)
            {
                auto Result = Platform::Socket::Send(mSocket, (bytes) data, Math::Min(4096, size), mTimeOut);
                if(Result < 0)
                    mConnected = false;
                else
                {
                    data += Result;
                    size -= Result;
                }
            }
            return mConnected;
        }
        bool Recv(chararray& data)
        {
            if(!Connect()) return false;
            sint32 Result1 = Platform::Socket::RecvAvailable(mSocket);
            if(Result1 < 0)
                mConnected = false;
            else
            {
                char* NewData = data.AtDumpingAdded(Result1);
                while(mConnected && 0 < Result1)
                {
                    const sint32 Result2 = Platform::Socket::Recv(mSocket, (uint08*) NewData, Result1, mTimeOut);
                    if(Result2 == -10035) continue;
                    if(Result2 < 0)
                        mConnected = false;
                    else
                    {
                        NewData += Result2;
                        Result1 -= Result2;
                    }
                }
            }
            return mConnected;
        }

    public:
        inline chars domain() const {return mDomain;}

    private:
        bool mConnected;
        id_socket mSocket;
        String mDomain;
        uint16 mPort;
        sint32 mTimeOut;

    public:
        class Cookie
        {
        public:
            String mName;
            String mValue;
        };
        Array<Cookie> mCookies;
        Map<Cookie*> mCookieMap;
    };
    ContactPool::ContactPoolMapMap ContactPool::ST::_;

    class CachePool
    {
    public:
        typedef Array<CachePool> CachePoolArray;
        class ST
        {
            static CachePoolArray _;
        public:
            static CachePool& Add() {return _.AtAdding();}
            static CachePool& Get(sint32 i) {return _.AtWherever(i);}
            static void Remove(sint32 i) {_.SubtractionSection(i);}
            static sint32 Count() {return _.Count();}
        };

    public:
        CachePool() {mSize = 0; mDate = 0;}
        ~CachePool() {}

    public:
        String mPath;
        uint64 mSize;
        uint64 mDate;
    };
    CachePool::CachePoolArray CachePool::ST::_;

    ParaSource::ParaSource()
    {
        mLastContact = nullptr;
    }

    ParaSource::~ParaSource()
    {
    }

    void ParaSource::SetContact(chars domain, uint16 port, sint32 timeout)
    {
        mLastContact = &ContactPool::ST::GetContact(domain, port);
        mLastContact->InitForFirstTime(domain, port, timeout);
    }

    bool ParaSource::GetFile(uint08s& file, chars path, chars args)
    {
        if(!HTTPQuery(path, args)) return false;
        const sint32 BinarySize = mResponseText.Count() - 1;
        Memory::Copy(file.AtDumpingAdded(BinarySize), &mResponseText[0], BinarySize);
        return true;
    }

    bool ParaSource::GetJson(Contexts& jsons, chars path, chars args)
    {
        if(!HTTPQuery(path, args)) return false;
        static const String JsonBeginText = "#json begin";
        static const String JsonEndText = "#json end";

        const String TextData(mResponseText);
        sint32 OldPos = 0, BeginPos = 0;
        while((BeginPos = TextData.Find(OldPos, JsonBeginText)) != -1)
        {
            BeginPos += 11; // "#json begin"
            const sint32 EndPos = TextData.Find(BeginPos, JsonEndText);
            if(BeginPos < EndPos)
            {
                String FileName;
                while(mResponseText[++BeginPos] != ')') // "("
                    FileName += mResponseText[BeginPos];
                BOSS_TRACE("ParaSource.GetJson(%s)", (chars) FileName);
                auto& NewJson = jsons.AtAdding();
                NewJson.LoadJson(SO_NeedCopy, &mResponseText[BeginPos + 1], EndPos - (BeginPos + 1));
                OldPos = EndPos + 9; // "#json end"
            }
        }
        return true;
    }

    bool ParaSource::HTTPQuery(chars path, chars args)
    {
        if(!mLastContact)
        {
            mLastError = "You need to call by SetContact() first.";
            return false;
        }

        // 쿠키모음
        String AllCookies;
        for(sint32 i = 0, iend = mLastContact->mCookies.Count(); i < iend; ++i)
        {
            AllCookies += "Cookie: ";
            AllCookies += mLastContact->mCookies[i].mName;
            AllCookies += mLastContact->mCookies[i].mValue;
            AllCookies += "\r\n";
        }
        AllCookies += "\r\n";

        // 쿼리제작
        const bool HasArgs = (args && *args);
        const String RequestText = String::Format(
            "GET /%s%s%s HTTP/1.1\r\n"
            "Accept: text/html, application/xhtml+xml, */*\r\n"
            "Accept-Language: ko-KR\r\n"
            "Accept-Encoding: deflate\r\n"
            "User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0)\r\n"
            "Host: %s\r\n"
            "Connection: Keep-Alive\r\n"
            "%s", path, (HasArgs)? "?" : "", (HasArgs)? args : "", mLastContact->domain(), (chars) AllCookies);

        // 쿼리전송
        if(!mLastContact->Send(RequestText, RequestText.Length()))
        {
            mLastError = "Failed sending for HTTP request.";
            return false;
        }
        else Platform::Utility::Sleep(0, false);

        sint32 EndCodePos = -1;
        sint32 ContentLength = 0;
        bool KeepAlive = false;

        // HTTP헤더수신
        static chararray HttpData;
        HttpData.SubtractionAll();
        while(mLastContact->Recv(HttpData))
        {
            static const String EndCodeText = "\r\n\r\n";
            static const String ContentLengthText = "Content-Length:";
            static const String ConnectionText = "Connection: keep-alive";
            static const String SetCookieText = "Set-Cookie:";
            HttpData.AtAdding() = '\0';
            const String TextData(HttpData);
            EndCodePos = TextData.Find(0, EndCodeText);
            if(EndCodePos != -1)
            {
                EndCodePos += 4; // "\r\n\r\n"
                // ContentLength체크
                auto ContentLengthPos = TextData.Find(0, ContentLengthText);
                if(ContentLengthPos != -1)
                {
                    ContentLengthPos += 15; // "Content-Length:"
                    while(HttpData[ContentLengthPos] == ' ') ContentLengthPos++;
                    ContentLength = Parser::GetInt(&HttpData[ContentLengthPos]);
                }
                // Connection체크
                auto ConnectionPos = TextData.Find(0, ConnectionText);
                if(ConnectionPos != -1)
                    KeepAlive = true;
                // SetCookie체크
                sint32 CurCookiePos = 0, NextCookiePos = 0;
                while((NextCookiePos = TextData.Find(CurCookiePos, SetCookieText)) != -1)
                {
                    CurCookiePos = NextCookiePos + 11; // "Set-Cookie:"
                    String NewName, NewValue;
                    while(TextData[CurCookiePos] == ' ') CurCookiePos++;
                    while(TextData[CurCookiePos] != '=') NewName += TextData[CurCookiePos++];
                    while(TextData[CurCookiePos] != '\r') NewValue += TextData[CurCookiePos++];
                    if(auto OldCookie = mLastContact->mCookieMap.Access(NewName))
                        (*OldCookie)->mValue = NewValue;
                    else
                    {
                        auto& NewCookie = mLastContact->mCookies.AtAdding();
                        NewCookie.mName = NewName;
                        NewCookie.mValue = NewValue;
                        mLastContact->mCookieMap(NewName) = &NewCookie;
                    }
                }
                HttpData.SubtractionOne();
                break;
            }
            HttpData.SubtractionOne();
            Platform::Utility::Sleep(1, false);
        }
        if(EndCodePos == -1)
        {
            mLastError = "Failed receive HTTP header.";
            return false;
        }

        // HTTP데이터수신
        bool Success = false;
        mResponseText.SubtractionAll();
        const sint32 CopyLen = HttpData.Count() - EndCodePos;
        Memory::Copy(mResponseText.AtDumping(0, CopyLen), &HttpData.At(EndCodePos), CopyLen);
        while(mLastContact->Recv(mResponseText))
        {
            if(mResponseText.Count() == ContentLength)
            {
                Success = true;
                break;
            }
            else if(5 <= mResponseText.Count() && !Memory::Compare(&mResponseText[-5], "0\r\n\r\n", 5))
            {
                mResponseText.SubtractionSection(mResponseText.Count() - 5, 5);
                Success = true;
                break;
            }
            Platform::Utility::Sleep(1, false);
        }

        if(!KeepAlive) mLastContact->Disconnect();
        if(Success) mResponseText.AtAdding() = '\0';
        else mLastError = "Failed receive HTTP data.";
        return Success;
    }
}
