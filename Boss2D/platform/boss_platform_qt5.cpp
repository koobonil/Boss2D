#include <boss.hpp>
#include "boss_platform_qt5.hpp"

#ifdef BOSS_PLATFORM_QT5

    #include <format/boss_bmp.hpp>

    MainData* g_data = nullptr;
    QMainWindow* g_window = nullptr;
    QWidget* g_view = nullptr;
    QGLFunctions* g_func = nullptr;

    SizePolicy::SizePolicy()
    {
        m_minwidth = 0;
        m_minheight = 0;
        m_maxwidth = 0;
        m_maxheight = 0;
    }
    SizePolicy::~SizePolicy() {}

    CanvasClass::CanvasClass() : mIsTypeSurface(true)
    {
        mIsSurfaceBinded = false;
        mSavedCanvas = nullptr;
        mSavedZoom = 1.0f;
        mPainter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);
        mMask = QPainter::CompositionMode_SourceOver;
    }
    CanvasClass::CanvasClass(QPaintDevice* device) : mIsTypeSurface(false)
    {
        mIsSurfaceBinded = false;
        mSavedCanvas = nullptr;
        mSavedZoom = 1.0f;
        mPainter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);
        mMask = QPainter::CompositionMode_SourceOver;
        BindCore(device);
    }
    CanvasClass::~CanvasClass()
    {
        if(!mIsTypeSurface)
            UnbindCore();
    }
    void CanvasClass::Bind(QPaintDevice* device)
    {
        BOSS_ASSERT("본 함수의 사용은 Surface타입에서만 허용합니다", mIsTypeSurface);
        if(!mIsSurfaceBinded)
        {
            mIsSurfaceBinded = true;
            BindCore(device);
        }
    }
    void CanvasClass::Unbind()
    {
        BOSS_ASSERT("본 함수의 사용은 Surface타입에서만 허용합니다", mIsTypeSurface);
        if(mIsSurfaceBinded)
        {
            mIsSurfaceBinded = false;
            UnbindCore();
        }
    }
    void CanvasClass::BindCore(QPaintDevice* device)
    {
        BOSS_ASSERT("mSavedCanvas는 nullptr이어야 합니다", !mSavedCanvas);
        if(mSavedCanvas = ST())
        {
            mSavedCanvas->mSavedZoom = mSavedCanvas->mPainter.matrix().m11();
            mSavedCanvas->mSavedFont = mSavedCanvas->mPainter.font();
            mSavedCanvas->mSavedClipRect = mSavedCanvas->mPainter.clipBoundingRect();
            mSavedCanvas->mPainter.end();
        }
        mPainter.begin(device);
        ST() = this;
    }
    void CanvasClass::UnbindCore()
    {
        BOSS_ASSERT("CanvasClass는 스택식으로 해제해야 합니다", ST() == this);
        mPainter.end();
        if(auto CurGLWidget = g_data->getGLWidget())
            CurGLWidget->makeCurrent();
        if(ST() = mSavedCanvas)
        {
            mSavedCanvas->mPainter.begin(mSavedCanvas->mPainter.device());
            Platform::Graphics::SetZoom(mSavedCanvas->mSavedZoom);
            mSavedCanvas->mPainter.setFont(mSavedCanvas->mSavedFont);
            mSavedCanvas->mPainter.setClipRect(mSavedCanvas->mSavedClipRect);
            mSavedCanvas = nullptr;
        }
    }

    #if BOSS_ANDROID
        static JavaVM* g_jvm = nullptr;
        jint JNI_OnLoad(JavaVM* vm, void*)
        {
            g_jvm = vm;
            BOSS_TRACE("g_jvm=0x%08X", g_jvm);
            return JNI_VERSION_1_6;
        }

        JNIEnv* GetAndroidJNIEnv()
        {
            static QAndroidJniEnvironment g_environment;
            return g_environment;
        }
        jobject GetAndroidApplicationActivity()
        {
            QAndroidJniObject Activity = QtAndroid::androidActivity();
            return GetAndroidJNIEnv()->NewGlobalRef(Activity.object());
        }
        void SetAndroidApplicationActivity(jobject activity)
        {
        }
        jobject GetAndroidApplicationContext()
        {
            QAndroidJniObject Context = QtAndroid::androidContext();
            return GetAndroidJNIEnv()->NewGlobalRef(Context.object());
        }
        void SetAndroidApplicationContext(jobject context)
        {
        }
    #elif BOSS_IPHONE
        void* GetIOSApplicationUIView()
        {
            return QGuiApplication::platformNativeInterface()->nativeResourceForWindow("uiview", QGuiApplication::focusWindow());
        }
    #endif

    #if BOSS_NEED_MAIN
        extern void PlatformInit();
        extern void PlatformQuit();

        int main(int argc, char* argv[])
        {
            int result = 0;
            {
                QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
                QApplication app(argc, argv);
                MainWindow mainWindow;
                g_window = &mainWindow;

                Platform::Option::SetFlag("AssertPopup", true);
                PlatformInit();
                #if BOSS_NEED_FULLSCREEN
                    mainWindow.showFullScreen();
                #else
                    mainWindow.show();
                #endif
                result = app.exec();
                PlatformQuit();
                Platform::Option::SetFlag("AssertPopup", false);

                g_window = nullptr;
            }

            // 스토리지(TLS) 영구제거
            Storage::ClearAll(SCL_SystemAndUser);
            return result;
        }
    #endif

    #if BOSS_WINDOWS
        #include <windows.h>
    #elif BOSS_ANDROID
        extern "C" int __android_log_print(int prio, const char *tag,  const char *fmt, ...);
    #endif

    extern "C" int boss_platform_assert(BOSS_DBG_PRM const char* name, const char* query)
    {
        String AssertInfo[4] = {
            String::Format("[QUERY] %s", query),
            String::Format("[METHOD] %s()", BOSS_DBG_FUNC),
            String::Format("[FILE/LINE] %s(%dLn)", BOSS_DBG_FILE, BOSS_DBG_LINE),
            String::Format("[THREAD_ID] %u", QThread::currentThreadId())};
        #if BOSS_ANDROID
            __android_log_print(7, "*******", "************************************************************");
            __android_log_print(7, "*******", name);
            __android_log_print(7, "*******", "------------------------------------------------------------");
            __android_log_print(7, "*******", (chars) AssertInfo[0]);
            __android_log_print(7, "*******", (chars) AssertInfo[1]);
            __android_log_print(7, "*******", (chars) AssertInfo[2]);
            __android_log_print(7, "*******", (chars) AssertInfo[3]);
            __android_log_print(7, "*******", "************************************************************");
        #else
            qDebug() << "************************************************************";
            qDebug() << name;
            qDebug() << "------------------------------------------------------------";
            qDebug() << (chars) AssertInfo[0];
            qDebug() << (chars) AssertInfo[1];
            qDebug() << (chars) AssertInfo[2];
            qDebug() << (chars) AssertInfo[3];
            qDebug() << "************************************************************";
        #endif

        if(Platform::Option::GetFlag("AssertPopup"))
        {
            #if BOSS_WINDOWS
                WString AssertMessage = WString::Format(
                    L"%s\n\n%s\t\t\n%s\t\t\n%s\t\t\n%s\t\t\n\n"
                    L"(YES is Break, NO is Ignore)\t\t",
                    (wchars) WString::FromChars(name),
                    (wchars) WString::FromChars(AssertInfo[0]),
                    (wchars) WString::FromChars(AssertInfo[1]),
                    (wchars) WString::FromChars(AssertInfo[2]),
                    (wchars) WString::FromChars(AssertInfo[3]));
                switch(MessageBoxW((g_window)? (HWND) g_window->winId() : NULL,
                    AssertMessage, L"ASSERT BREAK", MB_ICONWARNING | MB_ABORTRETRYIGNORE))
                {
                case IDABORT: return 0;
                case IDIGNORE: return 1;
                }
            #else
                QString AssertMessage;
                AssertMessage.sprintf(
                    "%s\t\t\n%s\t\t\n%s\t\t\n%s\t\t\n\n"
                    "(YES is Break, NO is Ignore)\t\t",
                    (chars) AssertInfo[0], (chars) AssertInfo[1],
                    (chars) AssertInfo[2], (chars) AssertInfo[3]);
                QMessageBox AssertBox(QMessageBox::Warning, "ASSERT BREAK", QString::fromUtf8(name),
                    QMessageBox::Yes | QMessageBox::No | QMessageBox::NoToAll);
                AssertBox.setInformativeText(AssertMessage);
                AssertBox.setDefaultButton(QMessageBox::Yes);
                switch(AssertBox.exec())
                {
                case QMessageBox::Yes: return 0;
                case QMessageBox::NoToAll: return 1;
                }
            #endif
        }
        return 2;
    }

    extern "C" void boss_platform_trace(const char* text, ...)
    {
        QString TraceMessage;
        va_list Args;
        va_start(Args, text);
        TraceMessage.vsprintf(text, Args);
        va_end(Args);

        #if BOSS_ANDROID
            __android_log_print(7, "#######", TraceMessage.toUtf8().constData());
        #else
            qDebug() << TraceMessage;
        #endif
    }

    namespace BOSS
    {
        ////////////////////////////////////////////////////////////////////////////////
        // PLATFORM
        ////////////////////////////////////////////////////////////////////////////////
        void Platform::InitForGL(bool frameless)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data);
            g_data->initForGL(frameless);
        }

        void Platform::InitForMDI(bool frameless)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data);
            g_data->initForMDI(frameless);
        }

        void Platform::SetViewCreator(View::CreatorCB creator)
        {
            PlatformImpl::Core::g_Creator = creator;
        }

        void Platform::SetWindowName(chars name)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_window);
            g_window->setWindowTitle(QString::fromUtf8(name));
        }

        h_view Platform::SetWindowView(chars viewclass)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_window);
            auto NewViewManager = PlatformImpl::Core::g_Creator(viewclass);
            g_data->getMainAPI()->changeViewManagerAndDestroy(NewViewManager);

            h_view NewViewHandle = h_view::create_by_ptr(BOSS_DBG g_data->getMainAPI());
            g_data->getMainAPI()->setViewAndCreateAndSize(NewViewHandle);
            return NewViewHandle;
        }

        void Platform::SetWindowPos(sint32 x, sint32 y)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_window);
            if(!Platform::Utility::IsFullScreen())
            {
                auto TitleBarHeight = QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight);
                auto WindowFrame = QApplication::style()->pixelMetric(QStyle::PM_MdiSubWindowFrameWidth);
                g_window->move(x - WindowFrame, y - TitleBarHeight - WindowFrame / 2);
            }
        }

        void Platform::SetWindowSize(sint32 width, sint32 height)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_window);
            if(!Platform::Utility::IsFullScreen())
                g_window->resize(width, height);
        }

        static rect128 g_LastNormalWindowRect = {-1, -1, -1, -1};
        void Platform::GetWindowRect(rect128& rect, bool normally)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_window);
            if(Platform::Utility::IsFullScreen() && normally)
                Memory::Copy(&rect, &g_LastNormalWindowRect, sizeof(rect128));
            else
            {
                rect.l = g_window->x();
                rect.t = g_window->y();
                rect.r = rect.l + g_window->width();
                rect.b = rect.t + g_window->height();
            }
        }

        void Platform::AddWindowProcedure(WindowEvent event, ProcedureCB cb, payload data)
        {
            PlatformImpl::Wrap::AddWindowProcedure(event, cb, data);
        }

        void Platform::SetStatusText(chars text, UIStack stack)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_window);
            if(stack == UIS_Push || stack == UIS_PushPop)
                StackMessage::Me().Push();
            g_window->statusBar()->showMessage(QString::fromUtf8(
                (StackMessage::Me().Count() == 0)? text : (chars) StackMessage::Me().Get(text)));
            if(stack == UIS_Pop || stack == UIS_PushPop)
                StackMessage::Me().Pop();
        }

        h_icon Platform::CreateIcon(chars filepath)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_window);
            buffer NewIcon = Buffer::AllocNoConstructorOnce<QIcon>(BOSS_DBG 1);
            BOSS_CONSTRUCTOR(NewIcon, 0, QIcon, QString(Platform::File::RootForAssets()) + QString::fromUtf8(filepath));
            return h_icon::create_by_buf(BOSS_DBG NewIcon);
        }

        h_action Platform::CreateAction(chars name, chars tip, h_view view, h_icon icon)
        {
            BOSS_DECLARE_BUFFERED_CLASS(BufferedQAction, QAction, nullptr);

            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_window);
            buffer NewAction = Buffer::AllocNoConstructorOnce<BufferedQAction>(BOSS_DBG 1);
            if(icon.get()) BOSS_CONSTRUCTOR(NewAction, 0, QAction, *((QIcon*) icon.get()), QString::fromUtf8(name), g_window);
            else BOSS_CONSTRUCTOR(NewAction, 0, QAction, QString::fromUtf8(name), g_window);

            ((QAction*) NewAction)->setStatusTip(QString::fromUtf8(tip));
            return h_action::create_by_buf(BOSS_DBG NewAction);
        }

        h_policy Platform::CreatePolicy(sint32 minwidth, sint32 minheight, sint32 maxwidth, sint32 maxheight)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_window);
            buffer NewPolicy = Buffer::Alloc<SizePolicy, datatype_class_canmemcpy>(BOSS_DBG 1);
            ((SizePolicy*) NewPolicy)->m_minwidth = minwidth;
            ((SizePolicy*) NewPolicy)->m_minheight = minheight;
            ((SizePolicy*) NewPolicy)->m_maxwidth = maxwidth;
            ((SizePolicy*) NewPolicy)->m_maxheight = maxheight;
            return h_policy::create_by_buf(BOSS_DBG NewPolicy);
        }

        h_view Platform::CreateView(chars name, sint32 width, sint32 height, h_policy policy, chars viewclass)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_window);
            auto NewViewManager = PlatformImpl::Core::g_Creator(viewclass);
            buffer NewGenericView = Buffer::AllocNoConstructorOnce<GenericView>(BOSS_DBG 1);
            BOSS_CONSTRUCTOR(NewGenericView, 0, GenericView, NewViewManager,
                QString::fromUtf8(name), width, height, (SizePolicy*) policy.get());

            h_view NewViewHandle = h_view::create_by_buf(BOSS_DBG (buffer) ((GenericView*) NewGenericView)->m_api);
            NewViewManager->SetView(NewViewHandle);
            return NewViewHandle;
        }

        void* Platform::SetNextViewClass(h_view view, chars viewclass)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_window);
            auto NewViewManager = PlatformImpl::Core::g_Creator(viewclass);
            ((ViewAPI*) view.get())->setNextViewManager(NewViewManager);
            return NewViewManager->GetClass();
        }

        bool Platform::SetNextViewManager(h_view view, View* viewmanager)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_window);
            ((ViewAPI*) view.get())->setNextViewManager(viewmanager);
            return true;
        }

        h_dock Platform::CreateDock(h_view view, UIAllow allow, UIAllow allowbase)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_window);
            UIAllow allows[2] = {allow, allowbase};
            Qt::DockWidgetArea Areas[2] = {Qt::NoDockWidgetArea, Qt::NoDockWidgetArea};
            for(sint32 i = 0; i < 2; ++i)
            {
                if(allows[i] & UIA_Left) Areas[i] = (Qt::DockWidgetArea) (Areas[i] | Qt::LeftDockWidgetArea);
                if(allows[i] & UIA_Top) Areas[i] = (Qt::DockWidgetArea) (Areas[i] | Qt::TopDockWidgetArea);
                if(allows[i] & UIA_Right) Areas[i] = (Qt::DockWidgetArea) (Areas[i] | Qt::RightDockWidgetArea);
                if(allows[i] & UIA_Bottom) Areas[i] = (Qt::DockWidgetArea) (Areas[i] | Qt::BottomDockWidgetArea);
            }

            QDockWidget* NewDock = new QDockWidget(GenericView::cast(view)->getName(), g_window);
            GenericView* RenewedView = new GenericView(view);
            ((QDockWidget*) NewDock)->setAllowedAreas(Areas[1]);
            ((QDockWidget*) NewDock)->setWidget(RenewedView->m_api->getWidget());
            g_window->addDockWidget(Areas[0], (QDockWidget*) NewDock);
            return h_dock::create_by_ptr(BOSS_DBG NewDock);
        }

        void Platform::AddAction(chars group, h_action action, UIRole role)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_window);
            if(role & UIR_Menu)
            {
                QMenu* CurMenu = g_data->getMenu(group);
                CurMenu->addAction((QAction*) action.get());
            }
            if(role & UIR_Tool)
            {
                QToolBar* CurToolBar = g_data->getToolbar(group);
                CurToolBar->addAction((QAction*) action.get());
            }
        }

        void Platform::AddSeparator(chars group, UIRole role)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_window);
            if(role & UIR_Menu)
            {
                QMenu* CurMenu = g_data->getMenu(group);
                CurMenu->addSeparator();
            }
            if(role & UIR_Tool)
            {
                QToolBar* CurToolBar = g_data->getToolbar(group);
                CurToolBar->addSeparator();
            }
        }

        void Platform::AddToggle(chars group, h_dock dock, UIRole role)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_window);
            if(role & UIR_Menu)
            {
                QMenu* CurMenu = g_data->getMenu(group);
                CurMenu->addAction(((QDockWidget*) dock.get())->toggleViewAction());
            }
            if(role & UIR_Tool)
            {
                QToolBar* CurToolBar = g_data->getToolbar(group);
                CurToolBar->addAction(((QDockWidget*) dock.get())->toggleViewAction());
            }
        }

        h_window Platform::OpenChildWindow(h_view view, h_icon icon)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_window);
            GenericView* RenewedView = new GenericView(view);
            QWidget* NewWidget = g_data->addWidget(RenewedView);
            NewWidget->resize(RenewedView->getFirstSize());
            NewWidget->setWindowTitle(RenewedView->getName());
            if(icon.get()) NewWidget->setWindowIcon(*((QIcon*) icon.get()));
            NewWidget->show();

            buffer NewBox = Buffer::Alloc<WidgetBox>(BOSS_DBG 1);
            ((WidgetBox*) NewBox)->setWidget(RenewedView, NewWidget);
            h_window NewWindowHandle = h_window::create_by_buf(BOSS_DBG NewBox);
            RenewedView->attachWindow(NewWindowHandle);
            return NewWindowHandle;
        }

        h_window Platform::OpenPopupWindow(h_view view, h_icon icon)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_window);
            return h_window::null();
        }

        void Platform::CloseWindow(h_window window)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_window);
            window.set_buf(nullptr);
        }

        id_cloned_share Platform::SendNotify(h_view view, chars topic, id_share in, bool needout)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_window);
            if(needout)
            {
                id_cloned_share Result;
                ((ViewAPI*) view.get())->sendNotify(topic, in, &Result);
                return Result;
            }
            ((ViewAPI*) view.get())->sendNotify(topic, in, nullptr);
            return nullptr;
        }

        void Platform::BroadcastNotify(chars topic, id_share in, chars viewclass)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_window);
            if(auto Views = View::Search(viewclass, SC_Search))
            {
                struct Payload {chars topic; id_share in;} Param = {topic, in};
                Views->AccessByCallback([](const MapPath*, h_view* view, payload param)->void
                {
                    const Payload* Param = (const Payload*) param;
                    ((ViewAPI*) view->get())->sendNotify(Param->topic, Param->in, nullptr);
                }, &Param);
            }
        }

        void Platform::PassAllViews(PassCB cb, payload data)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_window);
            BOSS_ASSERT("콜백함수가 nullptr입니다", cb);
            if(auto Views = View::Search(nullptr, SC_Search))
            {
                struct Payload {PassCB cb; payload data; bool canceled;} Param = {cb, data, false};
                Views->AccessByCallback([](const MapPath*, h_view* view, payload param)->void
                {
                    Payload* Param = (Payload*) param;
                    if(Param->canceled) return;
                    Param->canceled = Param->cb(view->get(), Param->data);
                }, &Param);
            }
        }

        void Platform::UpdateAllViews()
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_window);
            if(auto Views = View::Search(nullptr, SC_Search))
            {
                Views->AccessByCallback([](const MapPath*, h_view* view, payload param)->void
                {
                    ((ViewAPI*) view->get())->dirtyAndUpdate();
                }, nullptr);
            }
        }

        ////////////////////////////////////////////////////////////////////////////////
        // POPUP
        ////////////////////////////////////////////////////////////////////////////////
        bool Platform::Popup::TextDialog(String& text, chars title, chars topic, bool ispassword)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_window);

            bool IsOk = false;
            QString NewText = QInputDialog::getText(g_window,
                QString::fromUtf8(title),
                QString::fromUtf8(topic),
                (ispassword)? QLineEdit::Password : QLineEdit::Normal,
                QString::fromUtf8(text), &IsOk);
            if(IsOk) text = NewText.toUtf8().constData();
            return IsOk;
        }

        bool Platform::Popup::ColorDialog(uint08& r, uint08& g, uint08& b, uint08& a, chars title)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_window);

            QColor NewColor = QColorDialog::getColor(QColor(r, g, b, a), g_window,
                QString::fromUtf8(title), QColorDialog::ShowAlphaChannel);
            const bool IsOk = NewColor.isValid();
            if(IsOk)
            {
                r = NewColor.red();
                g = NewColor.green();
                b = NewColor.blue();
                a = NewColor.alpha();
            }
            return IsOk;
        }

        bool Platform::Popup::FileDialog(String& path, String* shortpath, chars title, bool isdir)
        {
            return PlatformImpl::Wrap::Popup_FileDialog(path, shortpath, title, isdir);
        }

        sint32 Platform::Popup::MessageDialog(chars title, chars text, DialogButtonType type)
        {
            return PlatformImpl::Wrap::Popup_MessageDialog(title, text, type);
        }

        void Platform::Popup::WebBrowserDialog(chars url)
        {
            #if BOSS_WINDOWS
                PlatformImpl::Wrap::Popup_WebBrowserDialog(url);
            #else
                QDesktopServices::openUrl(QUrl(url, QUrl::TolerantMode));
            #endif
        }

        bool Platform::Popup::OpenEditTracker(String& text, UIEditType type, sint32 l, sint32 t, sint32 r, sint32 b)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_view);
            if(r <= l || b <= t) return false;

            EditTracker* NewTracker = new EditTracker(type, QString::fromUtf8(text), g_view);
            const bool Result = NewTracker->Popup(l, t, r - l, b - t);
            if(Result) text = NewTracker->text().toUtf8().constData();
            delete NewTracker;
            return Result;
        }

        sint32 Platform::Popup::OpenListTracker(Strings textes, sint32 l, sint32 t, sint32 r, sint32 b)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_view);
            if(r <= l || b <= t) return -1;

            ListTracker* NewTracker = new ListTracker(textes, g_view);
            const sint32 Result = NewTracker->Popup(l, t, r - l, b - t);
            delete NewTracker;
            return Result;
        }

        void Platform::Popup::CloseAllTracker()
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_view);
            Tracker::CloseAll(g_view);
        }

        bool Platform::Popup::HasOpenedTracker()
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_view);
            return Tracker::HasAnyone(g_view);
        }

        void Platform::Popup::ShowToolTip(String text)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data);
            QToolTip::showText(QCursor::pos(), QString::fromUtf8(text));
        }

        void Platform::Popup::HideToolTip()
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data);
            QToolTip::hideText();
        }

        ////////////////////////////////////////////////////////////////////////////////
        // UTILITY
        ////////////////////////////////////////////////////////////////////////////////
        static bool g_NeedSetRandom = true;
        uint32 Platform::Utility::Random()
        {
            if(g_NeedSetRandom)
            {
                g_NeedSetRandom = false;
                qsrand((uint32) (CurrentTimeMsec() & 0xFFFFFFFF));
            }
            return (qrand() & 0xFFFF) | ((qrand() & 0xFFFF) << 16);
        }

        void Platform::Utility::Sleep(sint32 ms, bool process_input, bool process_socket)
        {
            QTime StartTime = QTime::currentTime();
            if(process_input || process_socket)
            {
                QEventLoop EventLoop(QThread::currentThread());
                sint32 Flag = QEventLoop::AllEvents;
                if(!process_input) Flag |= QEventLoop::ExcludeUserInputEvents;
                if(!process_socket) Flag |= QEventLoop::ExcludeSocketNotifiers;
                EventLoop.processEvents((QEventLoop::ProcessEventsFlag) Flag, ms);
            }
            sint32 SleepTime = ms - StartTime.msecsTo(QTime::currentTime());
            if(0 < SleepTime) QThread::msleep(SleepTime);
        }

        void Platform::Utility::SetMinimize()
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data);

            g_data->getMainWindow()->showMinimized();
        }

        void Platform::Utility::SetFullScreen()
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data);
            if(IsFullScreen()) return;

            Platform::GetWindowRect(g_LastNormalWindowRect);
            g_data->getMainWindow()->showFullScreen();
        }

        bool Platform::Utility::IsFullScreen()
        {
            return (g_LastNormalWindowRect.l != -1 || g_LastNormalWindowRect.t != -1
                || g_LastNormalWindowRect.r != -1 || g_LastNormalWindowRect.b != -1);
        }

        void Platform::Utility::SetNormalWindow()
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data);
            if(!IsFullScreen()) return;

            g_data->getMainWindow()->showNormal();
            Platform::SetWindowPos(g_LastNormalWindowRect.l, g_LastNormalWindowRect.t);
            Platform::SetWindowSize(g_LastNormalWindowRect.r - g_LastNormalWindowRect.l, g_LastNormalWindowRect.b - g_LastNormalWindowRect.t);
            g_LastNormalWindowRect.l = g_LastNormalWindowRect.t = g_LastNormalWindowRect.r = g_LastNormalWindowRect.b = -1;
        }

        void Platform::Utility::ExitProgram()
        {
            QApplication::quit();
        }

        bool Platform::Utility::GetScreenRect(rect128& rect)
        {
            sint32 NumScreens = QApplication::desktop()->numScreens();
            if(NumScreens == 0)
                rect.l = rect.t = rect.r = rect.b = 0;
            else
            {
                rect128 TotalRect = {10000, 10000, -10000, -10000};
                for(sint32 i = 0; i < NumScreens; ++i)
                {
                    QRect GeometryRect = QApplication::desktop()->availableGeometry(i);
                    TotalRect.l = Math::Min(TotalRect.l, GeometryRect.left());
                    TotalRect.t = Math::Min(TotalRect.t, GeometryRect.top());
                    TotalRect.r = Math::Max(TotalRect.r, GeometryRect.right());
                    TotalRect.b = Math::Max(TotalRect.b, GeometryRect.bottom());
                }
                rect.l = TotalRect.l;
                rect.t = TotalRect.t;
                rect.r = TotalRect.r;
                rect.b = TotalRect.b;
            }

            #if BOSS_ANDROID
                /*id_file_read Hdmi = Platform::File::OpenForRead("/sys/devices/virtual/switch/hdmi/state");
                if(!Hdmi) Hdmi = Platform::File::OpenForRead("/sys/class/switch/hdmi/state");
                if(Hdmi)
                {
                    sint32 Value = 0;
                    Platform::File::Read(Hdmi, (uint08*) &Value, sizeof(sint32));
                    const bool HasPhygicalMonitor = ((Value & 1) == 1);
                    Platform::File::Close(Hdmi);
                    return HasPhygicalMonitor;
                }*/
            #endif
            return true;
        }

        id_image_read Platform::Utility::GetScreenshotImage(const rect128& rect)
        {
            QPixmap& ScreenshotPixmap = *BOSS_STORAGE_SYS(QPixmap);
            ScreenshotPixmap = QGuiApplication::primaryScreen()->grabWindow(
                0, rect.l, rect.t, rect.r - rect.l, rect.b - rect.t);
            return (id_image_read) &ScreenshotPixmap;
        }

        id_bitmap Platform::Utility::ImageToBitmap(id_image_read image, bool vflip)
        {
            if(!image) return nullptr;
            QImage CurImage = ((QPixmap*) image)->toImage();
            if(!CurImage.constBits()) return nullptr;
            CurImage = CurImage.convertToFormat(QImage::Format::Format_ARGB32);
            id_bitmap Result = Bmp::CloneFromBits(CurImage.constBits(),
                CurImage.width(), CurImage.height(), CurImage.bitPlaneCount(), vflip);
            return Result;
        }

        void Platform::Utility::GetCursorPos(point64& pos)
        {
            const QPoint& CursorPos = QCursor::pos();
            pos.x = CursorPos.x();
            pos.y = CursorPos.y();
        }

        sint32 Platform::Utility::GetPixelScale()
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_window);
            if(!g_window) return 1;
            return g_window->devicePixelRatio();
        }

        float Platform::Utility::GetFontScaleRate(sint32 def_depth, sint32 def_dpi)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_window);
            if(!g_window) return 1;
            float depth_rate = def_depth / (float) g_window->depth();
            float dpi_rate = g_window->logicalDpiX() / (float) def_dpi;
            return depth_rate * dpi_rate;
        }

        chars Platform::Utility::GetOSName()
        {
            return PlatformImpl::Wrap::Utility_GetOSName();
        }

        void Platform::Utility::Threading(ThreadCB cb, payload data)
        {
            ThreadClass::Begin(cb, data);
        }

        void* Platform::Utility::ThreadingEx(ThreadExCB cb, payload data)
        {
            return ThreadClass::BeginEx(cb, data);
        }

        uint64 Platform::Utility::CurrentThreadID()
        {
            return (uint64) QThread::currentThreadId();
        }

        uint64 Platform::Utility::CurrentTimeMsec()
        {
            return EpochToWindow(QDateTime::currentMSecsSinceEpoch());
        }

        sint64 Platform::Utility::CurrentAvailableMemory(sint64* totalbytes)
        {
            return PlatformImpl::Wrap::Utility_CurrentAvailableMemory(totalbytes);
        }

        static sint32 gHotKeyCode = -1;
        #if BOSS_WINDOWS && defined(_MSC_VER)
            static HHOOK gHookForHotKey = NULL;
        #endif
        sint32 Platform::Utility::LastHotKey()
        {
            #if BOSS_WINDOWS && defined(_MSC_VER)
                if(!gHookForHotKey)
                    gHookForHotKey = SetWindowsHookEx(WH_KEYBOARD_LL,
                        [](int nCode, WPARAM wParam, LPARAM lParam)->LRESULT
                        {
                            if(nCode == HC_ACTION)
                            {
                                auto pmsg = (const KBDLLHOOKSTRUCT*) lParam;
                                if(pmsg->flags & LLKHF_UP)
                                    gHotKeyCode = (sint32) pmsg->vkCode;
                            }
                            return CallNextHookEx(gHookForHotKey, nCode, wParam, lParam);
                        }, NULL, 0);
            #endif
            sint32 Result = gHotKeyCode;
            gHotKeyCode = -1;
            return Result;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // CLOCK
        ////////////////////////////////////////////////////////////////////////////////
        void Platform::Clock::SetBaseTime(chars timestring)
        {
            ClockClass::SetBaseTime(timestring);
        }

        id_clock Platform::Clock::Create(sint32 year, sint32 month, sint32 day,
            sint32 hour, sint32 min, sint32 sec, sint64 nsec)
        {
            buffer NewClock = Buffer::Alloc<ClockClass>(BOSS_DBG 1);
            ((ClockClass*) NewClock)->SetClock(year, month, day, hour, min, sec, nsec);
            return (id_clock) NewClock;
        }

        id_clock Platform::Clock::CreateAsCurrent()
        {
            buffer NewClock = Buffer::Alloc<ClockClass>(BOSS_DBG 1);
            return (id_clock) NewClock;
        }

        id_clock Platform::Clock::CreateAsClone(id_clock_read clock)
        {
            BOSS_ASSERT("파라미터가 nullptr입니다", clock);
            buffer NewClock = Buffer::AllocNoConstructorOnce<ClockClass>(BOSS_DBG 1);
            BOSS_CONSTRUCTOR(NewClock, 0, ClockClass, *((const ClockClass*) clock));
            return (id_clock) NewClock;
        }

        void Platform::Clock::Release(id_clock clock)
        {
            Buffer::Free((buffer) clock);
        }

        sint64 Platform::Clock::GetPeriodNsec(id_clock_read from, id_clock_read to)
        {
            BOSS_ASSERT("파라미터가 nullptr입니다", from && to);
            return ((const ClockClass*) to)->GetLap() - ((const ClockClass*) from)->GetLap();
        }

        void Platform::Clock::AddNsec(id_clock dest, sint64 nsec)
        {
            BOSS_ASSERT("파라미터가 nullptr입니다", dest);
            ((ClockClass*) dest)->AddLap(nsec);
        }

        uint64 Platform::Clock::GetMsec(id_clock clock)
        {
            BOSS_ASSERT("파라미터가 nullptr입니다", clock);
            return ((const ClockClass*) clock)->GetTotalSec() * 1000 +
                (uint64) (((const ClockClass*) clock)->GetNSecInSec() / 1000000);
        }

        sint64 Platform::Clock::GetLocalMsecFromUTC()
        {
            return ClockClass::GetLocalTimeMSecFromUtc();
        }

        void Platform::Clock::GetDetail(id_clock clock, sint64* nsec,
            sint32* sec, sint32* min, sint32* hour, sint32* day, sint32* month, sint32* year)
        {
            BOSS_ASSERT("파라미터가 nullptr입니다", clock);
            const sint64 CurTotalSec = ((const ClockClass*) clock)->GetTotalSec();
            // JulianDay는 BC-4300년을 기점으로 계산된 일수
            const QDate CurDate = QDate::fromJulianDay(CurTotalSec / (60 * 60 * 24));
            if(nsec) *nsec = ((const ClockClass*) clock)->GetNSecInSec();
            if(sec) *sec = CurTotalSec % 60;
            if(min) *min = (CurTotalSec / 60) % 60;
            if(hour) *hour = (CurTotalSec / (60 * 60)) % 24;
            if(day) *day = CurDate.day();
            if(month) *month = CurDate.month();
            if(year) *year = CurDate.year();
        }

        ////////////////////////////////////////////////////////////////////////////////
        // OPTION
        ////////////////////////////////////////////////////////////////////////////////
        void Platform::Option::SetFlag(chars name, bool flag)
        {
            PlatformImpl::Wrap::Option_SetOptionFlag(name, flag);
        }

        bool Platform::Option::GetFlag(chars name)
        {
            return PlatformImpl::Wrap::Option_GetOptionFlag(name);
        }

        Strings Platform::Option::GetFlagNames()
        {
            return PlatformImpl::Wrap::Option_GetOptionFlagNames();
        }

        void Platform::Option::SetText(chars name, chars text)
        {
            PlatformImpl::Wrap::Option_SetOptionText(name, text);
        }

        chars Platform::Option::GetText(chars name)
        {
            return PlatformImpl::Wrap::Option_GetOptionText(name);
        }

        Strings Platform::Option::GetTextNames()
        {
            return PlatformImpl::Wrap::Option_GetOptionTextNames();
        }

        void Platform::Option::SetPayload(chars name, payload data)
        {
            PlatformImpl::Wrap::Option_SetOptionPayload(name, data);
        }

        payload Platform::Option::GetPayload(chars name)
        {
            return PlatformImpl::Wrap::Option_GetOptionPayload(name);
        }

        Strings Platform::Option::GetPayloadNames()
        {
            return PlatformImpl::Wrap::Option_GetOptionPayloadNames();
        }

        ////////////////////////////////////////////////////////////////////////////////
        // GRAPHICS
        ////////////////////////////////////////////////////////////////////////////////
        void Platform::Graphics::SetScissor(float x, float y, float w, float h)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", CanvasClass::get());
            const QMatrix& LastMatrix = CanvasClass::get()->painter().matrix();
            const float LastZoom = (float) LastMatrix.m11();
            CanvasClass::get()->painter().setClipRect(QRectF(
                QPointF(Math::Floor(x * LastZoom) / LastZoom, Math::Floor(y * LastZoom) / LastZoom),
                QPointF(Math::Ceil((x + w) * LastZoom) / LastZoom, Math::Ceil((y + h) * LastZoom) / LastZoom)));
        }

        void Platform::Graphics::SetColor(uint08 r, uint08 g, uint08 b, uint08 a)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", CanvasClass::get());
            CanvasClass::get()->SetColor(r, g, b, a);
        }

        void Platform::Graphics::SetMask(MaskRole role)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", CanvasClass::get());
            switch(role)
            {
            case MR_SrcOver: CanvasClass::get()->SetMask(QPainter::CompositionMode_SourceOver); break;
            case MR_DstOver: CanvasClass::get()->SetMask(QPainter::CompositionMode_DestinationOver); break;
            case MR_Clear: CanvasClass::get()->SetMask(QPainter::CompositionMode_Clear); break;
            case MR_Src: CanvasClass::get()->SetMask(QPainter::CompositionMode_Source); break;
            case MR_Dst: CanvasClass::get()->SetMask(QPainter::CompositionMode_Destination); break;
            case MR_SrcIn: CanvasClass::get()->SetMask(QPainter::CompositionMode_SourceIn); break;
            case MR_DstIn: CanvasClass::get()->SetMask(QPainter::CompositionMode_DestinationIn); break;
            case MR_SrcOut: CanvasClass::get()->SetMask(QPainter::CompositionMode_SourceOut); break;
            case MR_DstOut: CanvasClass::get()->SetMask(QPainter::CompositionMode_DestinationOut); break;
            case MR_SrcAtop: CanvasClass::get()->SetMask(QPainter::CompositionMode_SourceAtop); break;
            case MR_DstAtop: CanvasClass::get()->SetMask(QPainter::CompositionMode_DestinationAtop); break;
            case MR_Xor: CanvasClass::get()->SetMask(QPainter::CompositionMode_Xor); break;
            }
        }

        void Platform::Graphics::SetFont(chars name, float size)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", CanvasClass::get());
            #if BOSS_MAC_OSX
                size *= 1.2f;
            #elif BOSS_IPHONE
                size *= 1.2f;
            #elif BOSS_ANDROID
                static const sint32 PixelScale = Platform::Utility::GetPixelScale();
                size *= 0.3f * PixelScale;
            #endif
            CanvasClass::get()->painter().setFont(QFont(name, (sint32) size));
        }

        void Platform::Graphics::SetZoom(float zoom)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", CanvasClass::get());
            QMatrix NewMatrix(zoom, 0, 0, zoom, 0, 0);
            CanvasClass::get()->painter().setMatrix(NewMatrix);
            CanvasClass::get()->painter().setRenderHint(QPainter::SmoothPixmapTransform, zoom < 1);
        }

        void Platform::Graphics::EraseRect(float x, float y, float w, float h)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", CanvasClass::get());
            auto OldCompositionMode = CanvasClass::get()->painter().compositionMode();
            CanvasClass::get()->painter().setCompositionMode(QPainter::CompositionMode_Clear);
            CanvasClass::get()->painter().eraseRect(QRectF(x, y, w, h));
            CanvasClass::get()->painter().setCompositionMode(OldCompositionMode);
        }

        void Platform::Graphics::FillRect(float x, float y, float w, float h)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", CanvasClass::get());
            CanvasClass::get()->painter().setCompositionMode(CanvasClass::get()->mask());
            CanvasClass::get()->painter().fillRect(QRectF(x, y, w, h), CanvasClass::get()->color());
        }

        void Platform::Graphics::FillCircle(float x, float y, float w, float h)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", CanvasClass::get());
            CanvasClass::get()->painter().setPen(Qt::NoPen);
            CanvasClass::get()->painter().setBrush(QBrush(CanvasClass::get()->color()));
            CanvasClass::get()->painter().setCompositionMode(CanvasClass::get()->mask());
            CanvasClass::get()->painter().drawEllipse(QRectF(x, y, w, h));
        }

        void Platform::Graphics::FillPolygon(float x, float y, Points p)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", CanvasClass::get());
            const sint32 Count = p.Count();
            if(Count < 3) return;

            QPointF* NewPoint = new QPointF[Count];
            for(sint32 i = 0; i < Count; ++i)
            {
                NewPoint[i].setX(x + p[i].x);
                NewPoint[i].setY(y + p[i].y);
            }

            CanvasClass::get()->painter().setPen(Qt::NoPen);
            CanvasClass::get()->painter().setBrush(QBrush(CanvasClass::get()->color()));
            CanvasClass::get()->painter().setCompositionMode(CanvasClass::get()->mask());
            CanvasClass::get()->painter().drawPolygon(NewPoint, Count);
            delete[] NewPoint;
        }

        void Platform::Graphics::DrawRect(float x, float y, float w, float h, float thick)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", CanvasClass::get());
            QPen NewPen(QBrush(CanvasClass::get()->color()), thick);
            NewPen.setCapStyle(Qt::FlatCap);
            NewPen.setJoinStyle(Qt::MiterJoin);
            CanvasClass::get()->painter().setPen(NewPen);
            CanvasClass::get()->painter().setBrush(Qt::NoBrush);
            CanvasClass::get()->painter().setCompositionMode(CanvasClass::get()->mask());
            CanvasClass::get()->painter().drawRect(QRectF(x - thick / 2, y - thick / 2, w + thick, h + thick));
        }

        void Platform::Graphics::DrawLine(const Point& begin, const Point& end, float thick)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", CanvasClass::get());
            CanvasClass::get()->painter().setPen(QPen(QBrush(CanvasClass::get()->color()), thick));
            CanvasClass::get()->painter().setBrush(Qt::NoBrush);
            CanvasClass::get()->painter().setCompositionMode(CanvasClass::get()->mask());
            CanvasClass::get()->painter().drawLine(QPointF(begin.x, begin.y), QPointF(end.x, end.y));
        }

        void Platform::Graphics::DrawCircle(float x, float y, float w, float h, float thick)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", CanvasClass::get());
            CanvasClass::get()->painter().setPen(QPen(QBrush(CanvasClass::get()->color()), thick));
            CanvasClass::get()->painter().setBrush(Qt::NoBrush);
            CanvasClass::get()->painter().setCompositionMode(CanvasClass::get()->mask());
            CanvasClass::get()->painter().drawEllipse(QRectF(x, y, w, h));
        }

        void Platform::Graphics::DrawBezier(const Vector& begin, const Vector& end, float thick)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", CanvasClass::get());
            QPainterPath NewPath;
            NewPath.moveTo(begin.x, begin.y);
            NewPath.cubicTo(begin.x + begin.vx, begin.y + begin.vy,
                end.x + end.vx, end.y + end.vy, end.x, end.y);

            CanvasClass::get()->painter().setPen(QPen(QBrush(CanvasClass::get()->color()), thick));
            CanvasClass::get()->painter().setBrush(Qt::NoBrush);
            CanvasClass::get()->painter().setCompositionMode(CanvasClass::get()->mask());
            CanvasClass::get()->painter().drawPath(NewPath);
        }

        void Platform::Graphics::DrawPolyLine(float x, float y, Points p, float thick)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", CanvasClass::get());
            const sint32 Count = p.Count();
            if(Count < 2) return;

            QPointF* NewPoint = new QPointF[Count];
            for(sint32 i = 0; i < Count; ++i)
            {
                NewPoint[i].setX(x + p[i].x);
                NewPoint[i].setY(y + p[i].y);
            }

            CanvasClass::get()->painter().setPen(QPen(QBrush(CanvasClass::get()->color()), thick));
            CanvasClass::get()->painter().setBrush(Qt::NoBrush);
            CanvasClass::get()->painter().setCompositionMode(CanvasClass::get()->mask());
            CanvasClass::get()->painter().drawPolyline(NewPoint, Count);
            delete[] NewPoint;
        }

        void Platform::Graphics::DrawPolyBezier(float x, float y, Points p, float thick, bool showfirst, bool showlast)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", CanvasClass::get());
            const sint32 Count = p.Count();
            if(Count < 4) return;

            QPainterPath NewPath;
            if(showfirst)
            {
                NewPath.moveTo(x + p[0].x, y + p[0].y);
                NewPath.lineTo(x + p[1].x, y + p[1].y);
            }
            else NewPath.moveTo(x + p[1].x, y + p[1].y);

            for(sint32 i = 2; i < Count - 1; ++i)
            {
                const sint32 A = i - 2, B = i - 1, C = i, D = i + 1;
                const float Ctrl1X = x + p[B].x + ((A == 0)? (p[B].x - p[A].x) * 6 : (p[C].x - p[A].x) / 6);
                const float Ctrl1Y = y + p[B].y + ((A == 0)? (p[B].y - p[A].y) * 6 : (p[C].y - p[A].y) / 6);
                const float Ctrl2X = x + p[C].x + ((D == Count - 1)? (p[C].x - p[D].x) * 6 : (p[B].x - p[D].x) / 6);
                const float Ctrl2Y = y + p[C].y + ((D == Count - 1)? (p[C].y - p[D].y) * 6 : (p[B].y - p[D].y) / 6);
                NewPath.cubicTo(Ctrl1X, Ctrl1Y, Ctrl2X, Ctrl2Y, x + p[C].x, y + p[C].y);
            }

            if(showlast)
                NewPath.lineTo(x + p[-1].x, y + p[-1].y);

            CanvasClass::get()->painter().setPen(QPen(QBrush(CanvasClass::get()->color()), thick));
            CanvasClass::get()->painter().setBrush(Qt::NoBrush);
            CanvasClass::get()->painter().setCompositionMode(CanvasClass::get()->mask());
            CanvasClass::get()->painter().drawPath(NewPath);
        }

        id_image Platform::Graphics::CreateImage(id_bitmap_read bitmap, const Color& coloring, sint32 resizing_width, sint32 resizing_height)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data && g_window);

            const sint32 SrcWidth = Bmp::GetWidth(bitmap);
            const sint32 SrcHeight = Bmp::GetHeight(bitmap);
            const sint32 DstWidth = (resizing_width == -1)? SrcWidth : resizing_width;
            const sint32 DstHeight = (resizing_height == -1)? SrcHeight : resizing_height;
            QImage NewImage(DstWidth, DstHeight, QImage::Format_ARGB32);

            const bool NeedColoring = (coloring.rgba != Color::ColoringDefault);
            const bool NeedResizing = (DstWidth != SrcWidth || DstHeight != SrcHeight);
            Bmp::bitmappixel* DstBits = (Bmp::bitmappixel*) NewImage.bits();
            const Bmp::bitmappixel* SrcBits = (const Bmp::bitmappixel*) Bmp::GetBits(bitmap);
            if(NeedColoring || NeedResizing)
            {
                // 컬러링 테이블링
                static uint08 AlphaTable[256 * 256];
                static uint08 ColorTable[256 * 256];
                static bool NeedTabling = true;
                if(NeedColoring && NeedTabling)
                {
                    NeedTabling = false;
                    for(sint32 src = 0; src < 256; ++src)
                    for(sint32 key = 0; key < 256; ++key)
                    {
                        const sint32 i = key * 256 + src;
                        if(src < 0x80)
                        {
                            if(key < 0x80)
                            {
                                const sint32 key2 = 0x80 - key;
                                AlphaTable[i] = (uint08) (src * key / 0x80);
                                ColorTable[i] = (uint08) (src - key2 + key2 * (0x80 - src) / 0x80);
                            }
                            else
                            {
                                const sint32 src2 = src * 2;
                                AlphaTable[i] = (uint08) (src2 - (src2 - src) * (0xFF - key) / 0x7F);
                                ColorTable[i] = (uint08) (0xFF - (0xFF - src) * (0xFF - key) / 0x7F);
                            }
                        }
                        else
                        {
                            if(key < 0x80)
                            {
                                const uint08 value = (uint08) (src * key / 0x80);
                                AlphaTable[i] = value;
                                ColorTable[i] = value;
                            }
                            else
                            {
                                const sint32 key2 = key - 0x80;
                                const uint08 value = (uint08) (src + key2 - key2 * (src - 0x80) / 0x7F);
                                AlphaTable[i] = value;
                                ColorTable[i] = value;
                            }
                        }
                    }
                }

                // 리사이징이 있는 경우
                if(NeedResizing)
                {
                    const uint32 level = 16; // 2의 승수
                    // 스트레칭 테이블링
                    sint32s& sxpool = *BOSS_STORAGE_SYS(sint32s);
                    sint32* sxpool_ptr = sxpool.AtDumping(0, DstWidth * 4);
                    sint32* sxbegins = &sxpool_ptr[DstWidth * 0];
                    sint32* sxends = &sxpool_ptr[DstWidth * 1];
                    sint32* sxbegin_rates = &sxpool_ptr[DstWidth * 2];
                    sint32* sxend_rates = &sxpool_ptr[DstWidth * 3];
                    for(sint32 dx = 0; dx < DstWidth; ++dx)
                    {
                        sint32 SumA = 0, SumR = 0, SumG = 0, SumB = 0, SumAlphaRate = 0, SumColorRate = 0;
                        sxbegins[dx] = dx * SrcWidth / DstWidth;
                        sxends[dx] = ((dx + 1) * SrcWidth + DstWidth - 1) / DstWidth;
                        sxbegin_rates[dx] = (sxbegins[dx] + 1) * level - dx * level * SrcWidth / DstWidth;
                        sxend_rates[dx] = ((dx + 1) * level * SrcWidth + DstWidth - 1) / DstWidth - (sxends[dx] - 1) * level;
                    }
                    // 스트레칭
                    for(sint32 dy = 0; dy < DstHeight; ++dy)
                    {
                        Bmp::bitmappixel* CurDstBits = &DstBits[dy * DstWidth];
                        const sint32 sybegin = dy * SrcHeight / DstHeight;
                        const sint32 syend = ((dy + 1) * SrcHeight + DstHeight - 1) / DstHeight;
                        const sint32 sybegin_rate = (sybegin + 1) * level - dy * level * SrcHeight / DstHeight;
                        const sint32 syend_rate = ((dy + 1) * level * SrcHeight + DstHeight - 1) / DstHeight - (syend - 1) * level;
                        for(sint32 dx = 0; dx < DstWidth; ++dx)
                        {
                            // 과한 축소로 오버플로우가 발생하여 색상오류가 나면 아래 선언을 uint32에서 float로 바꾸면 됨
                            uint32 SumA = 0, SumR = 0, SumG = 0, SumB = 0, SumAlphaRate = 0, SumColorRate = 0;
                            const sint32 sxbegin = sxbegins[dx];
                            const sint32 sxend = sxends[dx];
                            const sint32 sxbegin_rate = sxbegin_rates[dx];
                            const sint32 sxend_rate = sxend_rates[dx];
                            for(sint32 sy = sybegin; sy < syend; ++sy)
                            {
                                const Bmp::bitmappixel* CurSrcBits = &SrcBits[(SrcHeight - 1 - sy) * SrcWidth];
                                const sint32 sy_rate = (sy == sybegin)? sybegin_rate : ((sy < syend - 1)? level : syend_rate);
                                for(sint32 sx = sxbegin; sx < sxend; ++sx)
                                {
                                    const Bmp::bitmappixel& CurSrcBit = CurSrcBits[sx];
                                    const sint32 sx_rate = (sx == sxbegin)? sxbegin_rate : ((sx < sxend - 1)? level : sxend_rate);
                                    const uint32 alpha_rate = (uint32) (sx_rate * sy_rate);
                                    if(CurSrcBit.a)
                                    {
                                        const uint32 color_rate = CurSrcBit.a * alpha_rate / (level * level); // 오버플로우 방지
                                        SumA += CurSrcBit.a * alpha_rate;
                                        SumR += CurSrcBit.r * color_rate;
                                        SumG += CurSrcBit.g * color_rate;
                                        SumB += CurSrcBit.b * color_rate;
                                        SumAlphaRate += alpha_rate;
                                        SumColorRate += color_rate;
                                    }
                                    else SumAlphaRate += alpha_rate;
                                }
                            }
                            if(0 < SumColorRate)
                            {
                                CurDstBits->a = (uint08) (SumA / SumAlphaRate);
                                CurDstBits->r = (uint08) (SumR / SumColorRate);
                                CurDstBits->g = (uint08) (SumG / SumColorRate);
                                CurDstBits->b = (uint08) (SumB / SumColorRate);
                            }
                            else CurDstBits->argb = 0x00000000;
                            CurDstBits++;
                        }
                    }
                    // 리사이징과 함께 컬러링도 있는 경우
                    if(NeedColoring)
                    {
                        // 컬러링
                        const uint08* CurTableA = &AlphaTable[coloring.a * 256];
                        const uint08* CurTableR = &ColorTable[coloring.r * 256];
                        const uint08* CurTableG = &ColorTable[coloring.g * 256];
                        const uint08* CurTableB = &ColorTable[coloring.b * 256];
                        for(sint32 y = 0; y < DstHeight; ++y)
                        {
                            Bmp::bitmappixel* CurDstBits = &DstBits[y * DstWidth];
                            for(sint32 x = 0; x < DstWidth; ++x)
                            {
                                CurDstBits->a = CurTableA[CurDstBits->a];
                                CurDstBits->r = CurTableR[CurDstBits->r];
                                CurDstBits->g = CurTableG[CurDstBits->g];
                                CurDstBits->b = CurTableB[CurDstBits->b];
                                CurDstBits++;
                            }
                        }
                    }
                }
                // 컬러링만 있는 경우
                else
                {
                    // 컬러링
                    const uint08* CurTableA = &AlphaTable[coloring.a * 256];
                    const uint08* CurTableR = &ColorTable[coloring.r * 256];
                    const uint08* CurTableG = &ColorTable[coloring.g * 256];
                    const uint08* CurTableB = &ColorTable[coloring.b * 256];
                    for(sint32 y = 0; y < DstHeight; ++y)
                    {
                        Bmp::bitmappixel* CurDstBits = &DstBits[y * DstWidth];
                        const Bmp::bitmappixel* CurSrcBits = &SrcBits[(SrcHeight - 1 - y) * SrcWidth];
                        for(sint32 x = 0; x < DstWidth; ++x)
                        {
                            CurDstBits->a = CurTableA[CurSrcBits->a];
                            CurDstBits->r = CurTableR[CurSrcBits->r];
                            CurDstBits->g = CurTableG[CurSrcBits->g];
                            CurDstBits->b = CurTableB[CurSrcBits->b];
                            CurDstBits++;
                            CurSrcBits++;
                        }
                    }
                }
            }
            // 컬러링도 없고 리사이징도 없는 경우
            else for(sint32 y = 0; y < DstHeight; ++y)
                Memory::Copy(&DstBits[y * DstWidth], &SrcBits[(SrcHeight - 1 - y) * SrcWidth],
                    sizeof(Bmp::bitmappixel) * SrcWidth);

            buffer NewPixmap = Buffer::Alloc<QPixmap>(BOSS_DBG 1);
            ((QPixmap*) NewPixmap)->convertFromImage(NewImage);
            return (id_image) NewPixmap;
        }

        sint32 Platform::Graphics::GetImageWidth(id_image_read image)
        {
            if(const QPixmap* CurPixmap = (const QPixmap*) image)
                return CurPixmap->width();
            return 0;
        }

        sint32 Platform::Graphics::GetImageHeight(id_image_read image)
        {
            if(const QPixmap* CurPixmap = (const QPixmap*) image)
                return CurPixmap->height();
            return 0;
        }

        void Platform::Graphics::RemoveImage(id_image image)
        {
            Buffer::Free((buffer) image);
        }

        void Platform::Graphics::DrawImage(id_image_read image, float ix, float iy, float iw, float ih, float x, float y, float w, float h)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", CanvasClass::get());
            BOSS_ASSERT("image파라미터가 nullptr입니다", image);

            CanvasClass::get()->painter().setCompositionMode(CanvasClass::get()->mask());
            if(w == iw && h == ih)
                CanvasClass::get()->painter().drawPixmap(QPoint((sint32) x, (sint32) y), *((const QPixmap*) image),
                    QRect((sint32) ix, (sint32) iy, (sint32) iw, (sint32) ih));
            else CanvasClass::get()->painter().drawPixmap(QRect((sint32) x, (sint32) y, (sint32) w, (sint32) h),
                *((const QPixmap*) image), QRect((sint32) ix, (sint32) iy, (sint32) iw, (sint32) ih));

            //if(w == iw && h == ih)
            //    CanvasClass::get()->painter().drawPixmap(QPointF(x, y), *((const QPixmap*) image), QRectF(ix, iy, iw, ih));
            //else CanvasClass::get()->painter().drawPixmap(QRectF(x, y, w, h), *((const QPixmap*) image), QRectF(ix, iy, iw, ih));
        }

        static Qt::Alignment _ExchangeAlignment(UIFontAlign align)
        {
            Qt::Alignment Result = Qt::AlignCenter;
            switch(align)
            {
            case UIFA_LeftTop: Result = (Qt::Alignment) (Qt::AlignLeft | Qt::AlignTop); break;
            case UIFA_CenterTop: Result = (Qt::Alignment) (Qt::AlignHCenter | Qt::AlignTop); break;
            case UIFA_RightTop: Result = (Qt::Alignment) (Qt::AlignRight | Qt::AlignTop); break;
            case UIFA_JustifyTop: Result = (Qt::Alignment) (Qt::AlignJustify | Qt::AlignTop); break;
            case UIFA_LeftMiddle: Result = (Qt::Alignment) (Qt::AlignLeft | Qt::AlignVCenter); break;
            case UIFA_CenterMiddle: Result = (Qt::Alignment) (Qt::AlignHCenter | Qt::AlignVCenter); break;
            case UIFA_RightMiddle: Result = (Qt::Alignment) (Qt::AlignRight | Qt::AlignVCenter); break;
            case UIFA_JustifyMiddle: Result = (Qt::Alignment) (Qt::AlignJustify | Qt::AlignVCenter); break;
            case UIFA_LeftAscent: Result = (Qt::Alignment) (Qt::AlignLeft | Qt::AlignBaseline); break;
            case UIFA_CenterAscent: Result = (Qt::Alignment) (Qt::AlignHCenter | Qt::AlignBaseline); break;
            case UIFA_RightAscent: Result = (Qt::Alignment) (Qt::AlignRight | Qt::AlignBaseline); break;
            case UIFA_JustifyAscent: Result = (Qt::Alignment) (Qt::AlignJustify | Qt::AlignBaseline); break;
            case UIFA_LeftBottom: Result = (Qt::Alignment) (Qt::AlignLeft | Qt::AlignBottom); break;
            case UIFA_CenterBottom: Result = (Qt::Alignment) (Qt::AlignHCenter | Qt::AlignBottom); break;
            case UIFA_RightBottom: Result = (Qt::Alignment) (Qt::AlignRight | Qt::AlignBottom); break;
            case UIFA_JustifyBottom: Result = (Qt::Alignment) (Qt::AlignJustify | Qt::AlignBottom); break;
            }
            return Result;
        }

        static Qt::TextElideMode _ExchangeTextElideMode(UIFontElide elide)
        {
            Qt::TextElideMode Result = Qt::ElideNone;
            switch(elide)
            {
            case UIFE_None: Result = Qt::ElideNone; break;
            case UIFE_Left: Result = Qt::ElideLeft; break;
            case UIFE_Center: Result = Qt::ElideMiddle; break;
            case UIFE_Right: Result = Qt::ElideRight; break;
            }
            return Result;
        }

        bool Platform::Graphics::DrawString(float x, float y, float w, float h, chars string, UIFontAlign align, UIFontElide elide)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", CanvasClass::get());
            CanvasClass::get()->painter().setPen(CanvasClass::get()->color());
            CanvasClass::get()->painter().setBrush(Qt::NoBrush);
            CanvasClass::get()->painter().setCompositionMode(CanvasClass::get()->mask());

            const QString Text = QString::fromUtf8(string);
            if(elide != UIFE_None)
            {
                const QString ElidedText = CanvasClass::get()->painter().fontMetrics().elidedText(Text, _ExchangeTextElideMode(elide), w);
                if(ElidedText != Text)
                {
                    CanvasClass::get()->painter().drawText(QRectF(x, y, w, h), ElidedText, QTextOption(_ExchangeAlignment(align)));
                    return true;
                }
            }
            CanvasClass::get()->painter().drawText(QRectF(x, y, w, h), Text, QTextOption(_ExchangeAlignment(align)));
            return false;
        }

        bool Platform::Graphics::DrawStringW(float x, float y, float w, float h, wchars string, UIFontAlign align, UIFontElide elide)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", CanvasClass::get());
            CanvasClass::get()->painter().setPen(CanvasClass::get()->color());
            CanvasClass::get()->painter().setBrush(Qt::NoBrush);
            CanvasClass::get()->painter().setCompositionMode(CanvasClass::get()->mask());

            const QString Text = QString::fromWCharArray(string);
            if(elide != UIFE_None)
            {
                const QString ElidedText = CanvasClass::get()->painter().fontMetrics().elidedText(Text, _ExchangeTextElideMode(elide), w);
                if(ElidedText != Text)
                {
                    CanvasClass::get()->painter().drawText(QRectF(x, y, w, h), ElidedText, QTextOption(_ExchangeAlignment(align)));
                    return true;
                }
            }
            CanvasClass::get()->painter().drawText(QRectF(x, y, w, h), Text, QTextOption(_ExchangeAlignment(align)));
            return false;
        }

        sint32 Platform::Graphics::GetStringWidth(chars string)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", CanvasClass::get());
            return CanvasClass::get()->painter().fontMetrics().width(QString::fromUtf8(string));
        }

        sint32 Platform::Graphics::GetStringWidthW(wchars string)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", CanvasClass::get());
            return CanvasClass::get()->painter().fontMetrics().width(QString::fromWCharArray(string));
        }

        sint32 Platform::Graphics::GetStringHeight()
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", CanvasClass::get());
            return CanvasClass::get()->painter().fontMetrics().height();
        }

        sint32 Platform::Graphics::GetStringAscent()
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", CanvasClass::get());
            return CanvasClass::get()->painter().fontMetrics().ascent();
        }

        void Platform::Graphics::BeginGL()
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", CanvasClass::get());
            SurfaceClass::LockForGL();
            CanvasClass::get()->painter().beginNativePainting();
        }

        void Platform::Graphics::EndGL()
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", CanvasClass::get());
            CanvasClass::get()->painter().endNativePainting();
            SurfaceClass::UnlockForGL();
        }

        id_surface Platform::Graphics::CreateSurface(sint32 width, sint32 height)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", g_data);
            if(!g_data->getGLWidget()) return nullptr;
            
            QOpenGLFramebufferObjectFormat SurfaceFormat;
            SurfaceFormat.setSamples(4);
            SurfaceFormat.setMipmap(false);
            SurfaceFormat.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
            SurfaceFormat.setTextureTarget(GL_TEXTURE_2D);
            #if BOSS_IPHONE | BOSS_ANDROID
                SurfaceFormat.setInternalTextureFormat(GL_RGBA8);
            #else
                SurfaceFormat.setInternalTextureFormat(GL_RGBA32F_ARB);
            #endif

            buffer NewSurface = Buffer::AllocNoConstructorOnce<SurfaceClass>(BOSS_DBG 1);
            BOSS_CONSTRUCTOR(NewSurface, 0, SurfaceClass, width, height, &SurfaceFormat);
            return (id_surface) NewSurface;
        }

        uint32 Platform::Graphics::GetSurfaceId(id_surface_read surface)
        {
            if(!surface) return 0;
            return ((const SurfaceClass*) surface)->id();
        }

        sint32 Platform::Graphics::GetSurfaceWidth(id_surface_read surface)
        {
            if(!surface) return 0;
            return ((const SurfaceClass*) surface)->width();
        }

        sint32 Platform::Graphics::GetSurfaceHeight(id_surface_read surface)
        {
            if(!surface) return 0;
            return ((const SurfaceClass*) surface)->height();
        }

        void Platform::Graphics::RemoveSurface(id_surface surface)
        {
            Buffer::Free((buffer) surface);
        }

        void Platform::Graphics::BindSurface(id_surface surface)
        {
            if(surface)
                ((SurfaceClass*) surface)->BindGraphics();
        }

        void Platform::Graphics::UnbindSurface(id_surface surface)
        {
            if(surface)
                ((SurfaceClass*) surface)->UnbindGraphics();
        }

        void Platform::Graphics::DrawSurface(id_surface_read surface, float sx, float sy, float sw, float sh, float x, float y, float w, float h)
        {
            BOSS_ASSERT("호출시점이 적절하지 않습니다", CanvasClass::get());
            if(!surface) return;

            auto OldOpacity = CanvasClass::get()->painter().opacity();
            CanvasClass::get()->painter().setOpacity(Math::Min(128, CanvasClass::get()->color().alpha()) / 128.0f);
            CanvasClass::get()->painter().setCompositionMode(CanvasClass::get()->mask());
            if(w == sw && h == sh)
                CanvasClass::get()->painter().drawImage(QPoint((sint32) x, (sint32) y),
                    ((const SurfaceClass*) surface)->mLastImage, QRect((sint32) sx, (sint32) sy, (sint32) sw, (sint32) sh));
            else CanvasClass::get()->painter().drawImage(QRect((sint32) x, (sint32) y, (sint32) w, (sint32) h),
                ((const SurfaceClass*) surface)->mLastImage, QRect((sint32) sx, (sint32) sy, (sint32) sw, (sint32) sh));
            CanvasClass::get()->painter().setOpacity(OldOpacity);
        }

        id_image_read Platform::Graphics::GetImageFromSurface(id_surface_read surface)
        {
            QPixmap& SurfacePixmap = *BOSS_STORAGE_SYS(QPixmap);
            if(!surface) return nullptr;

            SurfacePixmap = QPixmap::fromImage(((const SurfaceClass*) surface)->mLastImage);
            return (id_image_read) &SurfacePixmap;
        }

        id_bitmap_read Platform::Graphics::GetBitmapFromSurface(id_surface_read surface, bool vflip)
        {
            Image& SurfaceImage = *BOSS_STORAGE_SYS(Image);
            if(!surface) return nullptr;

            QImage CurImage = ((const SurfaceClass*) surface)->mLastImage.convertToFormat(QImage::Format::Format_ARGB32);
            SurfaceImage.LoadBitmapFromBits(CurImage.constBits(), CurImage.width(), CurImage.height(),
                CurImage.bitPlaneCount(), vflip);
            return SurfaceImage.GetBitmap();
        }

        ////////////////////////////////////////////////////////////////////////////////
        // FILE
        ////////////////////////////////////////////////////////////////////////////////
        bool Platform::File::Exist(chars filename)
        {
            QFileInfo CurInfo(QString::fromUtf8(filename).replace('\\', '/'));
            return CurInfo.exists() && CurInfo.isFile();
        }

        bool Platform::File::ExistForDir(chars dirname)
        {
            QFileInfo CurInfo(QString::fromUtf8(dirname).replace('\\', '/'));
            return CurInfo.exists() && CurInfo.isDir();
        }

        id_file_read Platform::File::OpenForRead(chars filename)
        {
            QFile* NewFile = new QFile(QString::fromUtf8(filename).replace('\\', '/'));
            if(!NewFile->open(QFileDevice::ReadOnly))
            {
                BOSS_TRACE("OpenForRead(%s) - The file is nonexistent", filename);
                delete NewFile;
                return nullptr;
            }
            BOSS_TRACE("OpenForRead(%s)", filename);
            return (id_file_read) NewFile;
        }

        static void _CreateMiddleDir(chars itemname)
        {
            String Dirname;
            for(chars iChar = itemname; *iChar; ++iChar)
            {
                const char OneChar = *iChar;
                if(OneChar == '/' || OneChar == '\\')
                {
                    const bool Result = QDir().mkdir(QString::fromUtf8(Dirname));
                    BOSS_TRACE("_CreateMiddleDir(%s)%s", (chars) Dirname, Result? "" : " - Failed");
                    Dirname += '/';
                }
                else Dirname += OneChar;
            }
        }

        id_file Platform::File::OpenForWrite(chars filename, bool autocreatedir)
        {
            QFile* NewFile = new QFile(QString::fromUtf8(filename).replace('\\', '/'));
            if(!NewFile->open(QFileDevice::WriteOnly))
            {
                delete NewFile;
                if(autocreatedir)
                {
                    _CreateMiddleDir(filename);
                    return OpenForWrite(filename, false);
                }
                else
                {
                    BOSS_TRACE("OpenForWrite(%s) - The folder is nonexistent", filename);
                    return nullptr;
                }
            }
            BOSS_TRACE("OpenForWrite(%s)", filename);
            return (id_file) NewFile;
        }

        void Platform::File::Close(id_file_read file)
        {
            delete (QFile*) file;
        }

        const sint32 Platform::File::Size(id_file_read file)
        {
            BOSS_ASSERT("해당 파일이 없습니다", file);
            return ((QFile*) file)->size();
        }

        const sint32 Platform::File::Read(id_file_read file, uint08* data, const sint32 size)
        {
            BOSS_ASSERT("해당 파일이 없습니다", file);
            return ((QFile*) file)->read((char*) data, size);
        }

        const sint32 Platform::File::ReadLine(id_file_read file, char* text, const sint32 size)
        {
            BOSS_ASSERT("해당 파일이 없습니다", file);
            return ((QFile*) file)->readLine(text, size);
        }

        const sint32 Platform::File::Write(id_file file, bytes data, const sint32 size)
        {
            BOSS_ASSERT("해당 파일이 없습니다", file);
            return ((QFile*) file)->write((chars) data, size);
        }

        void Platform::File::Seek(id_file_read file, const sint32 focus)
        {
            BOSS_ASSERT("해당 파일이 없습니다", file);
            ((QFile*) file)->seek(focus);
        }

        const sint32 Platform::File::Focus(id_file_read file)
        {
            BOSS_ASSERT("해당 파일이 없습니다", file);
            return (sint32) ((QFile*) file)->pos();
        }

        sint32 Platform::File::Search(chars dirname, SearchCB cb, payload data, bool needfullpath)
        {
            const String PathUTF8 = PlatformImpl::Core::NormalPath(dirname);
            QString PathQ = QString::fromUtf8(PathUTF8).replace('\\', '/');
            const QString& Tail = PathQ.right(2);
            if(!Tail.compare("/*")) PathQ = PathQ.left(PathQ.length() - 2);
            sint32 FindPos = PathQ.lastIndexOf("assets:");
            if(0 <= FindPos) PathQ = PathQ.mid(FindPos);

            QDir TargetDir;
            bool Exists = false;
            if(FindPos < 0)
            {
                TargetDir = PathQ;
                Exists = TargetDir.exists();
            }
            else
            {
                TargetDir = String(RootForAssets()).Sub(1) + PathQ.mid(7);
                Exists = TargetDir.exists();
                if(!Exists)
                {
                    TargetDir = String(RootForAssetsRem()).Sub(1) + PathQ.mid(7);
                    Exists = TargetDir.exists();
                }
            }
            if(!Exists)
            {
                BOSS_TRACE("Search(%s) - The TargetDir is nonexistent", (chars) PathUTF8);
                return -1;
            }

            const QStringList& List = TargetDir.entryList(QDir::Files | QDir::Dirs | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot);
            BOSS_TRACE("Search(%s) - %d", (chars) PathUTF8, List.size());

            if(cb) for(sint32 i = 0, iend = List.size(); i < iend; ++i)
            {
                if(needfullpath)
                    cb(String::Format("%s/%s", PathQ.toUtf8().constData(),
                        List.at(i).toUtf8().constData()), data);
                else cb(List.at(i).toUtf8().constData(), data);
                BOSS_TRACE("Search() - Result: %s", List.at(i).toUtf8().constData());
            }
            return List.size();
        }

        sint32 Platform::File::GetAttributes(wchars itemname, uint64* size, uint64* ctime, uint64* atime, uint64* mtime)
        {
            const WString ItemnameUTF16 = PlatformImpl::Core::NormalPathW(itemname);
            const String ItemnameUTF8 = String::FromWChars(ItemnameUTF16);
            const QString ItemnameQ = QString::fromUtf8(ItemnameUTF8).replace('\\', '/');

            QFileInfo CurInfo(ItemnameQ);
            if(!CurInfo.exists()) return -1; // INVALID_FILE_ATTRIBUTES

            sint32 Result = 0;
            if(!CurInfo.isWritable()) Result |= 0x1; // FILE_ATTRIBUTE_READONLY
            if(CurInfo.isHidden()) Result |= 0x2; // FILE_ATTRIBUTE_HIDDEN
            if(CurInfo.isDir()) Result |= 0x10; // FILE_ATTRIBUTE_DIRECTORY
            if(CurInfo.isSymLink()) Result |= 0x400; // FILE_ATTRIBUTE_REPARSE_POINT

            if(size) *size = (uint64) CurInfo.size();
            if(ctime) *ctime = 10 * 1000 * EpochToWindow(CurInfo.created().toMSecsSinceEpoch());
            if(atime) *atime = 10 * 1000 * EpochToWindow(CurInfo.lastRead().toMSecsSinceEpoch());
            if(mtime) *mtime = 10 * 1000 * EpochToWindow(CurInfo.lastModified().toMSecsSinceEpoch());
            return Result;
        }

        WString Platform::File::GetFullPath(wchars itemname)
        {
            const String ItemnameUTF8 = String::FromWChars(PlatformImpl::Core::NormalPathW(itemname, false));
            const QString ItemnameQ = QString::fromUtf8(ItemnameUTF8).replace('\\', '/');

            if((('A' <= ItemnameUTF8[0] && ItemnameUTF8[0] <= 'Z') ||
                ('a' <= ItemnameUTF8[0] && ItemnameUTF8[0] <= 'z')) && ItemnameUTF8[1] == ':')
                return WString::FromChars(ItemnameQ.toUtf8().constData());

            QFileInfo CurInfo(QString::fromUtf8(BOSS::Platform::File::RootForAssetsRem()) + ItemnameQ);
            String AbsoluteName = CurInfo.absoluteFilePath().toUtf8().constData();
            if(AbsoluteName[-2] == '/' || AbsoluteName[-2] == '\\')
                AbsoluteName.Sub(1);

            #if BOSS_WINDOWS
                return WString::FromChars(AbsoluteName);
            #elif BOSS_MAC_OSX || BOSS_IPHONE
                return WString::FromChars("Q:" + AbsoluteName);
            #elif BOSS_ANDROID
                return WString::FromChars("Q:" + AbsoluteName);
            #endif
        }

        WString Platform::File::GetDirName(wchars itemname, wchar_t badslash, wchar_t goodslash)
        {
            return PlatformImpl::Wrap::File_GetDirName(itemname, badslash, goodslash);
        }

        WString Platform::File::GetShortName(wchars itemname)
        {
            return PlatformImpl::Wrap::File_GetShortName(itemname);
        }

        sint32 Platform::File::GetDriveCode()
        {
            return PlatformImpl::Wrap::File_GetDriveCode();
        }

        bool Platform::File::CanAccess(wchars itemname)
        {
            const String ItemnameUTF8 = String::FromWChars(PlatformImpl::Core::NormalPathW(itemname));
            QFileInfo CurInfo(QString::fromUtf8(ItemnameUTF8).replace('\\', '/'));

            const bool Result = CurInfo.exists();
            BOSS_TRACE("CanAccess(%s)%s", (chars) ItemnameUTF8, Result? "" : " - Failed");
            return Result;
        }

        bool Platform::File::CanWritable(wchars itemname)
        {
            const String ItemnameUTF8 = String::FromWChars(PlatformImpl::Core::NormalPathW(itemname));
            QFileInfo CurInfo(QString::fromUtf8(ItemnameUTF8).replace('\\', '/'));

            const bool Result = (CurInfo.exists() && CurInfo.isWritable());
            BOSS_TRACE("CanWritable(%s)%s", (chars) ItemnameUTF8, Result? "" : " - Failed");
            return Result;
        }

        static void _RemoveMiddleDir(chars itemname)
        {
            String Itemname, LastDirname;
            for(chars iChar = itemname; *iChar; ++iChar)
            {
                const char OneChar = *iChar;
                if(OneChar == '/' || OneChar == '\\')
                {
                    LastDirname = Itemname;
                    Itemname += '/';
                }
                else Itemname += OneChar;
            }
            if(0 < LastDirname.Length())
            {
                const bool Result = QDir().rmdir(QString::fromUtf8(LastDirname));
                BOSS_TRACE("_RemoveMiddleDir(%s)%s", (chars) LastDirname, Result? "" : " - Failed");
                if(Result) _RemoveMiddleDir(LastDirname);
            }
        }

        bool Platform::File::Remove(wchars itemname, bool autoremovedir)
        {
            const String ItemnameUTF8 = String::FromWChars(PlatformImpl::Core::NormalPathW(itemname));
            const QString ItemnameQ = QString::fromUtf8(ItemnameUTF8).replace('\\', '/');

            const bool Result = QFile::remove(ItemnameQ);
            if(Result && autoremovedir)
                _RemoveMiddleDir(ItemnameQ.toUtf8().constData());
            BOSS_TRACE("Remove(%s)%s", (chars) ItemnameUTF8, Result? "" : " - Failed");
            return Result;
        }

        bool Platform::File::Rename(wchars existing_itemname, wchars new_itemname)
        {
            const String ExistingItemnameUTF8 = String::FromWChars(PlatformImpl::Core::NormalPathW(existing_itemname));
            const String NewItemnameUTF8 = String::FromWChars(PlatformImpl::Core::NormalPathW(new_itemname));

            const bool Result = QFile::rename(
                QString::fromUtf8(ExistingItemnameUTF8).replace('\\', '/'),
                QString::fromUtf8(NewItemnameUTF8).replace('\\', '/'));
            BOSS_TRACE("Rename(%s -> %s)%s", (chars) ExistingItemnameUTF8, (chars) NewItemnameUTF8, Result? "" : " - Failed");
            return Result;
        }

        bool Platform::File::Tempname(char* format, sint32 length)
        {
            return PlatformImpl::Wrap::File_Tempname(format, length);
        }

        bool Platform::File::CreateDir(wchars dirname, bool autocreatedir)
        {
            const String DirnameUTF8 = String::FromWChars(PlatformImpl::Core::NormalPathW(dirname));
            const QString DirnameQ = QString::fromUtf8(DirnameUTF8).replace('\\', '/');

            if(QDir(DirnameQ).exists())
            {
                BOSS_TRACE("CreateDir(%s) - Skipped", (chars) DirnameUTF8);
                return true;
            }

            if(!QDir().mkdir(DirnameQ))
            {
                if(autocreatedir)
                {
                    _CreateMiddleDir(DirnameQ.toUtf8().constData());
                    return CreateDir(dirname, false);
                }
                else
                {
                    BOSS_TRACE("CreateDir(%s) - Failed", (chars) DirnameUTF8);
                    return nullptr;
                }
            }
            BOSS_TRACE("CreateDir(%s)", (chars) DirnameUTF8);
            return true;
        }

        bool Platform::File::RemoveDir(wchars dirname, bool autoremovedir)
        {
            const String DirnameUTF8 = String::FromWChars(PlatformImpl::Core::NormalPathW(dirname));
            const QString DirnameQ = QString::fromUtf8(DirnameUTF8).replace('\\', '/');

            if(!QDir(DirnameQ).exists())
            {
                if(autoremovedir)
                    _RemoveMiddleDir(DirnameQ.toUtf8().constData());
                BOSS_TRACE("RemoveDir(%s) - Skipped", (chars) DirnameUTF8);
                return true;
            }

            const bool Result = QDir().rmdir(DirnameQ);
            if(Result && autoremovedir)
                _RemoveMiddleDir(DirnameQ.toUtf8().constData());
            BOSS_TRACE("RemoveDir(%s)%s", (chars) DirnameUTF8, Result? "" : " - Failed");
            return Result;
        }

        extern chars _private_GetFileName(boss_file file);
        extern sint64 _private_GetFileOffset(boss_file file);
        extern void _private_SetFileRetain(boss_file file);

        class FDFile
        {
            BOSS_DECLARE_NONCOPYABLE_CLASS(FDFile)
        public:
            FDFile()
            {
                mID = -1;
                mFile = nullptr;
            }
            ~FDFile()
            {
                boss_fclose(mFile);
            }
        public:
            void SetID(sint32 id)
            {
                mID = id;
            }
            sint32 SetFile(boss_file file, bool retain)
            {
                mFile = file;
                if(retain)
                    _private_SetFileRetain(mFile);
                return mID;
            }
            boss_file GetRetainedFile()
            {
                _private_SetFileRetain(mFile);
                return mFile;
            }
        public:
            inline boss_file file() {return mFile;}
        private:
            sint32 mID;
            boss_file mFile;
        };

        class FDFilePool
        {
        public:
            FDFilePool() {mMutex = Mutex::Open();}
            ~FDFilePool() {Mutex::Close(mMutex);}
        private:
            static sint32 MakeID() {static sint32 _ = -1; return ++_;}
        public:
            FDFile& New()
            {
                Mutex::Lock(mMutex);
                const sint32 NewID = MakeID();
                FDFile& NewFile = mMap[NewID];
                NewFile.SetID(NewID);
                Mutex::Unlock(mMutex);
                return NewFile;
            }
            FDFile* Get(sint32 fd)
            {
                Mutex::Lock(mMutex);
                FDFile* CurFile = mMap.Access(fd);
                Mutex::Unlock(mMutex);
                return CurFile;
            }
            void Delete(sint32 fd)
            {
                Mutex::Lock(mMutex);
                mMap.Remove(fd);
                Mutex::Unlock(mMutex);
            }
        private:
            id_mutex mMutex;
            Map<FDFile> mMap;
        } gFilePool;

        sint32 Platform::File::FDOpen(wchars filename, bool writable, bool append, bool exclusive, bool truncate)
        {
            const String FileName = String::FromWChars(filename);
            if(exclusive && File::Exist(FileName))
                return -1;

            chars Mode = nullptr;
            if(writable)
            {
                if(append && !truncate)
                    Mode = "ab";
                else if(!truncate)
                    Mode = "r+b";
                else Mode = "wb";
            }
            else
            {
                if(truncate)
                    Mode = "w+b";
                else Mode = "rb";
            }

            FDFile& NewFile = gFilePool.New();
            return NewFile.SetFile(boss_fopen(FileName, Mode), false);
        }

        bool Platform::File::FDClose(sint32 fd)
        {
            FDFile* OldFile = gFilePool.Get(fd);
            if(OldFile)
            {
                gFilePool.Delete(fd);
                return true;
            }
            return false;
        }

        sint32 Platform::File::FDFromFile(boss_file file)
        {
            FDFile& NewFile = gFilePool.New();
            return NewFile.SetFile(file, true);
        }

        boss_file Platform::File::FDToFile(sint32 fd)
        {
            FDFile* CurFile = gFilePool.Get(fd);
            if(CurFile)
                return CurFile->GetRetainedFile();
            return nullptr;
        }

        sint64 Platform::File::FDRead(sint32 fd, void* data, sint64 size)
        {
            FDFile* CurFile = gFilePool.Get(fd);
            if(CurFile)
                return boss_fread(data, 1, size, CurFile->file());
            return -1;
        }

        sint64 Platform::File::FDWrite(sint32 fd, const void* data, sint64 size)
        {
            FDFile* CurFile = gFilePool.Get(fd);
            if(CurFile)
                return boss_fwrite(data, 1, size, CurFile->file());
            return -1;
        }

        sint64 Platform::File::FDSeek(sint32 fd, sint64 offset, sint32 origin)
        {
            FDFile* CurFile = gFilePool.Get(fd);
            if(CurFile)
            {
                const sint64 Result = boss_fseek(CurFile->file(), offset, origin);
                if(Result == 0)
                    return _private_GetFileOffset(CurFile->file());
            }
            return -1;
        }

        bool Platform::File::FDResize(sint32 fd, sint64 size)
        {
            BOSS_ASSERT("Further development is needed.", false);
            return false;
        }

        void* Platform::File::FDMap(boss_file file, sint64 offset, sint64 size, bool readonly)
        {
            BOSS_ASSERT("Further development is needed.", false);
            return nullptr;
        }

        bool Platform::File::FDUnmap(const void* map)
        {
            BOSS_ASSERT("Further development is needed.", false);
            return false;
        }

        uint32 Platform::File::FDAttributes(sint32 fd, uint64* size, uint64* ctime, uint64* atime, uint64* mtime)
        {
            FDFile* CurFile = gFilePool.Get(fd);
            if(CurFile)
            {
                QFileInfo CurInfo(_private_GetFileName(CurFile->file()));

                uint32 Result = 0;
                if(!CurInfo.isWritable()) Result |= 0x1; // FILE_ATTRIBUTE_READONLY
                if(CurInfo.isHidden()) Result |= 0x2; // FILE_ATTRIBUTE_HIDDEN
                if(CurInfo.isDir()) Result |= 0x10; // FILE_ATTRIBUTE_DIRECTORY
                if(CurInfo.isSymLink()) Result |= 0x400; // FILE_ATTRIBUTE_REPARSE_POINT

                if(size) *size = (uint64) CurInfo.size();
                if(ctime) *ctime = 10 * 1000 * EpochToWindow(CurInfo.created().toMSecsSinceEpoch());
                if(atime) *atime = 10 * 1000 * EpochToWindow(CurInfo.lastRead().toMSecsSinceEpoch());
                if(mtime) *mtime = 10 * 1000 * EpochToWindow(CurInfo.lastModified().toMSecsSinceEpoch());
                return Result;
            }
            return -1;
        }

        void Platform::File::ResetAssetsRoot(chars dirname)
        {
            PlatformImpl::Wrap::File_ResetAssetsRoot(dirname);
            BOSS_TRACE("Platform::File::ResetAssetsRoot() ==> \"%s\"", (chars) PlatformImpl::Core::g_AssetsRoot);
        }

        void Platform::File::ResetAssetsRemRoot(chars dirname)
        {
            PlatformImpl::Wrap::File_ResetAssetsRemRoot(dirname);
            BOSS_TRACE("Platform::File::ResetAssetsRemRoot() ==> \"%s\"", (chars) PlatformImpl::Core::g_AssetsRemRoot);
        }

        const String& Platform::File::RootForAssets()
        {
            if(0 < PlatformImpl::Core::g_AssetsRoot.Length())
                return PlatformImpl::Core::g_AssetsRoot;

            #if BOSS_WINDOWS || BOSS_LINUX
                PlatformImpl::Core::g_AssetsRoot = "../assets/";
                const String AssetsPathA = String::Format("%s/..", QCoreApplication::applicationDirPath().replace('\\', '/').toUtf8().constData());
                if(Platform::File::ExistForDir(AssetsPathA + "/assets"))
                    PlatformImpl::Core::g_AssetsRoot = AssetsPathA + "/assets/";
                else
                {
                    const String AssetsPathB = String::Format("%s/../../..", QCoreApplication::applicationDirPath().replace('\\', '/').toUtf8().constData());
                    if(Platform::File::ExistForDir(AssetsPathB + "/assets"))
                        PlatformImpl::Core::g_AssetsRoot = AssetsPathB + "/assets/";
                }
            #elif BOSS_MAC_OSX
                PlatformImpl::Core::g_AssetsRoot = String::Format("%s/../../assets/", QCoreApplication::applicationDirPath().replace('\\', '/').toUtf8().constData());
            #elif BOSS_IPHONE
                PlatformImpl::Core::g_AssetsRoot = String::Format("%s/assets/", QCoreApplication::applicationDirPath().replace('\\', '/').toUtf8().constData());
            #elif BOSS_ANDROID
                PlatformImpl::Core::g_AssetsRoot = "assets:/";
            #else
                PlatformImpl::Core::g_AssetsRoot = "../assets/";
            #endif
            BOSS_TRACE("Platform::File::RootForAssets() ==> \"%s\"", (chars) PlatformImpl::Core::g_AssetsRoot);
            return PlatformImpl::Core::g_AssetsRoot;
        }

        const String& Platform::File::RootForAssetsRem()
        {
            if(0 < PlatformImpl::Core::g_AssetsRemRoot.Length())
                return PlatformImpl::Core::g_AssetsRemRoot;

            #if BOSS_WINDOWS || BOSS_LINUX
                QString RootQ = "..";
                const String AssetsPathA = String::Format("%s/..", QCoreApplication::applicationDirPath().replace('\\', '/').toUtf8().constData());
                if(Platform::File::ExistForDir(AssetsPathA + "/assets"))
                    RootQ = AssetsPathA;
                else
                {
                    const String AssetsPathB = String::Format("%s/../../..", QCoreApplication::applicationDirPath().replace('\\', '/').toUtf8().constData());
                    if(Platform::File::ExistForDir(AssetsPathB + "/assets"))
                        RootQ = AssetsPathB;
                }
            #elif BOSS_MAC_OSX
                QString RootQ = QCoreApplication::applicationDirPath().replace('\\', '/');
                RootQ += "/../../../..";
            #else
                QString RootQ = QStandardPaths::standardLocations(QStandardPaths::DataLocation).value(0).replace('\\', '/');
                if(!QFileInfo(RootQ).exists()) QDir().mkdir(RootQ);
            #endif

            RootQ += "/assets-rem";
            if(!QFileInfo(RootQ).exists()) QDir().mkdir(RootQ);
            PlatformImpl::Core::g_AssetsRemRoot = (RootQ + "/").toUtf8().constData();
            BOSS_TRACE("Platform::File::RootForAssetsRem() ==> \"%s\"", (chars) PlatformImpl::Core::g_AssetsRemRoot);
            return PlatformImpl::Core::g_AssetsRemRoot;
        }

        const String& Platform::File::RootForData()
        {
            static String Result;
            if(0 < Result.Length())
                return Result;

            QString RootQ = QStandardPaths::standardLocations(QStandardPaths::DataLocation).value(0);
            #if BOSS_ANDROID
                String Root = getenv("SECONDARY_STORAGE");
                Root.Replace(':', '\0');
                if(File::ExistForDir(Root))
                    RootQ = (chars) Root;
            #endif
            if(!QFileInfo(RootQ).exists()) QDir().mkdir(RootQ);
            Result = (RootQ + "/").toUtf8().constData();
            BOSS_TRACE("Platform::File::RootForData() ==> \"%s\"", (chars) Result);
            return Result;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // SOUND
        ////////////////////////////////////////////////////////////////////////////////
        id_sound Platform::Sound::OpenForFile(chars filename, bool loop, sint32 fade_msec)
        {
            BOSS_ASSERT("해당 사운드파일이 존재하지 않습니다", Platform::File::Exist(filename));
            SoundClass* NewSound = new SoundClass(filename, loop);
            return (id_sound) NewSound;
        }

        id_sound Platform::Sound::OpenForStream(sint32 channel, sint32 sample_rate, sint32 sample_size)
        {
            SoundClass* NewSound = new SoundClass(channel, sample_rate, sample_size);
            return (id_sound) NewSound;
        }

        void Platform::Sound::Close(id_sound sound)
        {
            SoundClass* OldSound = (SoundClass*) sound;
            delete OldSound;
        }

        void Platform::Sound::SetVolume(float volume, sint32 fade_msec)
        {
        }

        void Platform::Sound::Play(id_sound sound, float volume_rate)
        {
            SoundClass* CurSound = (SoundClass*) sound;
            if(!CurSound) return;
            CurSound->Play(volume_rate);
        }

        void Platform::Sound::Stop(id_sound sound)
        {
            SoundClass* CurSound = (SoundClass*) sound;
            if(!CurSound) return;
            CurSound->Stop();
        }

        bool Platform::Sound::NowPlaying(id_sound sound)
        {
            SoundClass* CurSound = (SoundClass*) sound;
            if(CurSound)
                return CurSound->NowPlaying();
            return false;
        }

        sint32 Platform::Sound::AddStreamForPlay(id_sound sound, bytes raw, sint32 size, sint32 timeout)
        {
            SoundClass* CurSound = (SoundClass*) sound;
            if(!CurSound) return false;
            return CurSound->AddStreamForPlay(raw, size, timeout);
        }

        void Platform::Sound::StopAll()
        {
        }

        void Platform::Sound::PauseAll()
        {
        }

        void Platform::Sound::ResumeAll()
        {
        }

        ////////////////////////////////////////////////////////////////////////////////
        // SOCKET
        ////////////////////////////////////////////////////////////////////////////////
        class SocketBox
        {
        public:
            SocketBox()
            {
                m_socket = nullptr;
                m_udp = false;
                m_udpip.clear();
                m_udpport = 0;
            }
            ~SocketBox() {delete m_socket;}

        public:
            bool CheckState(chars name)
            {
                if(!m_udp)
                {
                    if(!m_socket->isValid())
                    {
                        BOSS_TRACE("%s(-1) - Socket is broken", name);
                        return false;
                    }
                    if(m_socket->state() == QAbstractSocket::UnconnectedState)
                    {
                        BOSS_TRACE("%s(-1) - Socket is unconnected", name);
                        return false;
                    }
                }
                return true;
            }

        public:
            QAbstractSocket* m_socket;
            bool m_udp;
            QHostAddress m_udpip;
            quint16 m_udpport;

        private:
            class STClass
            {
            public:
                STClass()
                {
                    static uint64 LastThreadID = 0;
                    m_lastId = (++LastThreadID) << 32;
                }
                ~STClass() {}
            public:
                Map<SocketBox> m_map;
                ublock m_lastId;
            };
            static inline STClass& ST() {return *BOSS_STORAGE_SYS(STClass);}

        public:
            static void Create(id_socket& result, bool udp)
            {
                STClass& CurClass = ST();
                SocketBox* Ptr = &CurClass.m_map[++CurClass.m_lastId];
                result = (id_socket) AnyTypeToPtr(CurClass.m_lastId);
                Ptr->m_socket = (udp)? (QAbstractSocket*) new QUdpSocket() : (QAbstractSocket*) new QTcpSocket();
                Ptr->m_udp = udp;
            }
            static SocketBox* Access(id_socket id)
            {
                BOSS_ASSERT("타 스레드에서 생성된 소켓입니다", (ST().m_lastId >> 32) == (((ublock) id) >> 32));
                return ST().m_map.Access(PtrToUint64(id));
            }
            static void Remove(id_socket id)
            {
                ST().m_map.Remove(PtrToUint64(id));
            }
        };

        class Hostent
        {
        public:
            Hostent() :
                h_addrtype(2), // AF_INET
                h_length(4) // IPv4
            {
                h_name = nullptr;
                h_aliases = nullptr;
                h_addr_list = nullptr;
            }

            ~Hostent()
            {
                Clear();
            }

        public:
            void Clear()
            {
                delete[] h_name;
                if(h_aliases)
                for(chars* ptr_aliases = h_aliases; *ptr_aliases; ++ptr_aliases)
                    delete[] *ptr_aliases;
                delete[] h_aliases;
                if(h_addr_list)
                for(bytes* ptr_addr_list = h_addr_list; *ptr_addr_list; ++ptr_addr_list)
                    delete[] *ptr_addr_list;
                delete[] h_addr_list;
                h_name = nullptr;
                h_aliases = nullptr;
                h_addr_list = nullptr;
            }

        public:
            chars h_name;
            chars* h_aliases;
            const sint16 h_addrtype;
            const sint16 h_length;
            bytes* h_addr_list;
        };

        class Servent
        {
        public:
            Servent()
            {
                s_name = nullptr;
                s_aliases = nullptr;
                s_port = 0;
                s_proto = nullptr;
            }

            ~Servent()
            {
                Clear();
            }

        public:
            void Clear()
            {
                delete[] s_name;
                for(chars* ptr_aliases = s_aliases; ptr_aliases; ++ptr_aliases)
                    delete[] *ptr_aliases;
                delete[] s_aliases;
                delete[] s_proto;
                s_name = nullptr;
                s_aliases = nullptr;
                s_port = 0;
                s_proto = nullptr;
            }

        public:
            chars s_name;
            chars* s_aliases;
            sint16 s_port;
            chars s_proto;
        };

        id_socket Platform::Socket::OpenForTcp()
        {
            id_socket Result = nullptr;
            SocketBox::Create(Result, false);
            return Result;
        }

        id_socket Platform::Socket::OpenForUdp()
        {
            id_socket Result = nullptr;
            SocketBox::Create(Result, true);
            return Result;
        }

        bool Platform::Socket::Close(id_socket socket, sint32 timeout)
        {
            SocketBox* CurSocketBox = SocketBox::Access(socket);
            if(!CurSocketBox) return false;

            bool Result = (CurSocketBox->m_socket->error() == QAbstractSocket::UnknownSocketError);
            if(CurSocketBox->m_socket->state() == QAbstractSocket::ConnectedState)
            {
                CurSocketBox->m_socket->disconnectFromHost();
                Result &= (CurSocketBox->m_socket->state() == QAbstractSocket::UnconnectedState ||
                    CurSocketBox->m_socket->waitForDisconnected(timeout));
            }
            SocketBox::Remove(socket);
            return Result;
        }

        bool Platform::Socket::Connect(id_socket socket, chars domain, uint16 port, sint32 timeout)
        {
            SocketBox* CurSocketBox = SocketBox::Access(socket);
            if(!CurSocketBox) return false;

            if(CurSocketBox->m_udp)
            {
                // GetHostByName를 사용하지 않고 빠르게 IP구하기
                sint32 FastIP[4] = {0, 0, 0, 0};
                sint32 FastIPFocus = 0;
                for(chars DomainFocus = domain; *DomainFocus != '\0'; ++DomainFocus)
                {
                    if('0' <= *DomainFocus && *DomainFocus <= '9')
                    {
                        FastIP[FastIPFocus] = FastIP[FastIPFocus] * 10;
                        FastIP[FastIPFocus] += *DomainFocus - '0';
                        if(FastIP[FastIPFocus] <= 255)
                            continue;
                    }
                    else if(*DomainFocus == '.' && ++FastIPFocus < 4)
                        continue;
                    FastIPFocus = -1;
                    break;
                }

                if(FastIPFocus == 3)
                {
                    quint32 ip4Address =
                        ((FastIP[0] & 0xFF) << 24) |
                        ((FastIP[1] & 0xFF) << 16) |
                        ((FastIP[2] & 0xFF) <<  8) |
                        ((FastIP[3] & 0xFF) <<  0);
                    CurSocketBox->m_udpip.setAddress(ip4Address);
                }
                else
                {
                    Hostent* CurHostent = (Hostent*) GetHostByName(domain);
                    quint32 ip4Address = *((quint32*) CurHostent->h_addr_list[0]);
                    CurSocketBox->m_udpip.setAddress(ip4Address);
                }
                CurSocketBox->m_udpport = port;
                BOSS_TRACE("Connect-UDP(%s:%d)", domain, (sint32) port);
                return true;
            }

            CurSocketBox->m_socket->connectToHost(QString::fromUtf8(domain), port);
            bool Result = CurSocketBox->m_socket->waitForConnected(timeout);
            BOSS_TRACE("Connect-TCP(%s:%d)%s", domain, (sint32) port, Result? "" : " - Failed");
            return Result;
        }

        bool Platform::Socket::Disconnect(id_socket socket, sint32 timeout)
        {
            SocketBox* CurSocketBox = SocketBox::Access(socket);
            if(!CurSocketBox) return false;

            if(CurSocketBox->m_udp)
            {
                CurSocketBox->m_udpip.clear();
                CurSocketBox->m_udpport = 0;
                BOSS_TRACE("Disconnect-UDP()");
                return true;
            }

            CurSocketBox->m_socket->abort();
            bool Result = (CurSocketBox->m_socket->state() == QAbstractSocket::UnconnectedState ||
                CurSocketBox->m_socket->waitForDisconnected(timeout));
            BOSS_TRACE("Disconnect-TCP()%s", Result? "" : " - Failed");
            return Result;
        }

        bool Platform::Socket::BindForUdp(id_socket socket, uint16 port, sint32 timeout)
        {
            SocketBox* CurSocketBox = SocketBox::Access(socket);
            if(!CurSocketBox) return false;

            if(CurSocketBox->m_udp)
            {
                auto UdpSocket = (QUdpSocket*) CurSocketBox->m_socket;
                bool Result = UdpSocket->bind(port);
                BOSS_TRACE("BindForUdp-UDP()%s", Result? "" : " - Failed");
                return Result;
            }
            return false;
        }

        sint32 Platform::Socket::RecvAvailable(id_socket socket)
        {
            SocketBox* CurSocketBox = SocketBox::Access(socket);
            if(!CurSocketBox || !CurSocketBox->CheckState("RecvAvailable"))
                return -1;

            if(CurSocketBox->m_udp)
            {
                auto UdpSocket = (QUdpSocket*) CurSocketBox->m_socket;
                return (UdpSocket->hasPendingDatagrams())? UdpSocket->pendingDatagramSize() : 0;
            }
            return CurSocketBox->m_socket->bytesAvailable();
        }

        sint32 Platform::Socket::Recv(id_socket socket, uint08* data, sint32 size, sint32 timeout, ip4address* ip_udp, uint16* port_udp)
        {
            SocketBox* CurSocketBox = SocketBox::Access(socket);
            if(!CurSocketBox || !CurSocketBox->CheckState("Recv"))
                return -1;

            sint32 Result = -1;
            if(CurSocketBox->m_udp)
            {
                auto UdpSocket = (QUdpSocket*) CurSocketBox->m_socket;
                if(ip_udp || port_udp)
                {
                    QHostAddress GetIP;
                    quint16 GetPort = 0;
                    Result = UdpSocket->readDatagram((char*) data, size, &GetIP, &GetPort);
                    if(ip_udp)
                    {
                        auto IPv4Address = GetIP.toIPv4Address();
                        ip_udp->ip[0] = (IPv4Address >> 24) & 0xFF;
                        ip_udp->ip[1] = (IPv4Address >> 16) & 0xFF;
                        ip_udp->ip[2] = (IPv4Address >>  8) & 0xFF;
                        ip_udp->ip[3] = (IPv4Address >>  0) & 0xFF;
                    }
                    if(port_udp) *port_udp = GetPort;
                }
                else Result = UdpSocket->readDatagram((char*) data, size);
                BOSS_TRACE("Recv-UDP(%d)%s", Result, (0 <= Result)? "" : " - Failed");
            }
            else
            {
                if(CurSocketBox->m_socket->bytesAvailable() == 0 &&
                    !CurSocketBox->m_socket->waitForReadyRead(timeout))
                {
                    BOSS_TRACE("Recv-TCP(-10035) - WSAEWOULDBLOCK");
                    return -10035; // WSAEWOULDBLOCK
                }
                Result = CurSocketBox->m_socket->read((char*) data, size);
                BOSS_TRACE("Recv-TCP(%d)%s", Result, (0 <= Result)? "" : " - Failed");
            }
            return Result;
        }

        sint32 Platform::Socket::Send(id_socket socket, bytes data, sint32 size, sint32 timeout)
        {
            SocketBox* CurSocketBox = SocketBox::Access(socket);
            if(!CurSocketBox || !CurSocketBox->CheckState("Send"))
                return -1;

            if(CurSocketBox->m_udp)
            {
                auto UdpSocket = (QUdpSocket*) CurSocketBox->m_socket;
                if(UdpSocket->writeDatagram((chars) data, size, CurSocketBox->m_udpip, CurSocketBox->m_udpport) < size)
                {
                    BOSS_TRACE("Send-UDP(-1) - Failed");
                    return -1;
                }
                BOSS_TRACE("Send-UDP(%d)", size);
            }
            else
            {
                if(CurSocketBox->m_socket->write((chars) data, size) < size ||
                    !CurSocketBox->m_socket->waitForBytesWritten(timeout))
                {
                    BOSS_TRACE("Send-TCP(-1) - Failed");
                    return -1;
                }
                BOSS_TRACE("Send-TCP(%d)", size);
            }
            return size;
        }

        void* Platform::Socket::GetHostByName(chars name)
        {
            const QHostInfo& HostInfo = QHostInfo::fromName(name);
            const sint32 HostAddressCount = HostInfo.addresses().size();
            if(HostAddressCount == 0) return nullptr;

            Hostent& LastHostent = *BOSS_STORAGE_SYS(Hostent);
            LastHostent.Clear();

            // h_name
            String HostName = HostInfo.hostName().toUtf8().constData();
            char* NewName = new char[HostName.Length() + 1];
            Memory::Copy(NewName, (chars) HostName, HostName.Length() + 1);
            LastHostent.h_name = NewName;

            // h_addr_list
            LastHostent.h_addr_list = new bytes[HostAddressCount + 1];
            LastHostent.h_addr_list[HostAddressCount] = nullptr;
            for(sint32 i = 0; i < HostAddressCount; ++i)
            {
                const QHostAddress& CurAddress = HostInfo.addresses().at(i);
                const uint32 IPv4 = CurAddress.toIPv4Address();
                uint08* NewIPv4 = new uint08[4];
                Memory::Copy(NewIPv4, &IPv4, 4);
                LastHostent.h_addr_list[i] = NewIPv4;
            }
            return &LastHostent;
        }

        void* Platform::Socket::GetServByName(chars name, chars proto)
        {
            Servent& LastServent = *BOSS_STORAGE_SYS(Servent);
            LastServent.Clear();

            if(!LastServent.s_name)
            {
                // s_name
                String ServName = name;
                char* NewName = new char[ServName.Length() + 1];
                Memory::Copy(NewName, (chars) ServName, ServName.Length() + 1);
                LastServent.s_name = NewName;

                // s_port
                LastServent.s_port = (sint16) Parser(ServName).ReadInt();
            }
            return &LastServent;
        }

        ip4address Platform::Socket::GetLocalAddress()
        {
            ip4address Result = {};
            foreach(const QHostAddress& CurAddress, QNetworkInterface::allAddresses())
            {
                if(CurAddress.protocol() == QAbstractSocket::IPv4Protocol && CurAddress != QHostAddress(QHostAddress::LocalHost))
                {
                    auto IPv4Address = CurAddress.toIPv4Address();
                    Result.ip[0] = (IPv4Address >> 24) & 0xFF;
                    Result.ip[1] = (IPv4Address >> 16) & 0xFF;
                    Result.ip[2] = (IPv4Address >>  8) & 0xFF;
                    Result.ip[3] = (IPv4Address >>  0) & 0xFF;
                }
            }
            return Result;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // SERVER
        ////////////////////////////////////////////////////////////////////////////////
        id_server Platform::Server::Create(bool sizefield)
        {
            TCPAgent* NewAgent = (TCPAgent*) Buffer::AllocNoConstructorOnce<TCPAgent>(BOSS_DBG 1);
            BOSS_CONSTRUCTOR(NewAgent, 0, TCPAgent, sizefield);
            return (id_server) NewAgent;
        }

        void Platform::Server::Release(id_server server)
        {
            Buffer::Free((buffer) server);
        }

        bool Platform::Server::Listen(id_server server, uint16 port)
        {
            if(TCPAgent* CurAgent = (TCPAgent*) server)
            {
                if(CurAgent->isListening()) return true;
                return CurAgent->listen(QHostAddress::Any, port);
            }
            return false;
        }

        bool Platform::Server::TryNextPacket(id_server server)
        {
            if(TCPAgent* CurAgent = (TCPAgent*) server)
                return CurAgent->TryPacket();
            return false;
        }

        packettype Platform::Server::GetPacketType(id_server server)
        {
            if(TCPAgent* CurAgent = (TCPAgent*) server)
            {
                TCPPacket* FocusedPacket = CurAgent->GetFocusedPacket();
                return FocusedPacket->Type;
            }
            return packettype_null;
        }

        sint32 Platform::Server::GetPacketPeerID(id_server server)
        {
            if(TCPAgent* CurAgent = (TCPAgent*) server)
            {
                TCPPacket* FocusedPacket = CurAgent->GetFocusedPacket();
                return FocusedPacket->PeerID;
            }
            return -1;
        }

        bytes Platform::Server::GetPacketBuffer(id_server server, sint32* getsize)
        {
            if(TCPAgent* CurAgent = (TCPAgent*) server)
            {
                TCPPacket* FocusedPacket = CurAgent->GetFocusedPacket();
                if(getsize) *getsize = Buffer::CountOf(FocusedPacket->Buffer);
                return (bytes) FocusedPacket->Buffer;
            }
            return nullptr;
        }

        bool Platform::Server::SendToPeer(id_server server, sint32 peerid, const void* buffer, sint32 buffersize)
        {
            if(TCPAgent* CurAgent = (TCPAgent*) server)
                return CurAgent->SendPacket(peerid, buffer, buffersize);
            return false;
        }

        bool Platform::Server::KickPeer(id_server server, sint32 peerid)
        {
            if(TCPAgent* CurAgent = (TCPAgent*) server)
                return CurAgent->KickPeer(peerid);
            return false;
        }

        bool Platform::Server::GetPeerInfo(id_server server, sint32 peerid, ip4address* ip4, ip6address* ip6, uint16* port)
        {
            if(TCPAgent* CurAgent = (TCPAgent*) server)
                return CurAgent->GetPeerAddress(peerid, ip4, ip6, port);
            return false;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // WEB
        ////////////////////////////////////////////////////////////////////////////////
        h_web Platform::Web::Create(chars url, sint32 width, sint32 height, bool clearcookies, EventCB cb, payload data)
        {
            WebPrivate* NewWeb = (WebPrivate*) Buffer::Alloc<WebPrivate>(BOSS_DBG 1);
            if(clearcookies) NewWeb->ClearCookies();
            if(cb) NewWeb->SetCallback(cb, data);
            NewWeb->Resize(width, height);
            NewWeb->Reload(url);

            h_web NewWebHandle = h_web::create_by_buf(BOSS_DBG (buffer) NewWeb);
            NewWeb->attachHandle(NewWebHandle);
            return NewWebHandle;
        }

        void Platform::Web::Release(h_web web)
        {
            web.set_buf(nullptr);
        }

        void Platform::Web::Reload(h_web web, chars url)
        {
            if(WebPrivate* CurWeb = (WebPrivate*) web.get())
                CurWeb->Reload(url);
        }

        bool Platform::Web::Resize(h_web web, sint32 width, sint32 height)
        {
            if(WebPrivate* CurWeb = (WebPrivate*) web.get())
                return CurWeb->Resize(width, height);
			return false;
        }

        void Platform::Web::SendTouchEvent(h_web web, TouchType type, sint32 x, sint32 y)
        {
            if(WebPrivate* CurWeb = (WebPrivate*) web.get())
                CurWeb->SendTouchEvent(type, x, y);
        }

        void Platform::Web::SendKeyEvent(h_web web, sint32 code, chars text, bool pressed)
        {
            if(WebPrivate* CurWeb = (WebPrivate*) web.get())
                CurWeb->SendKeyEvent(code, text, pressed);
        }

        id_image_read Platform::Web::GetScreenshotImage(h_web web)
        {
            if(WebPrivate* CurWeb = (WebPrivate*) web.get())
            {
                QPixmap& ScreenshotPixmap = *BOSS_STORAGE_SYS(QPixmap);
                ScreenshotPixmap = CurWeb->GetPixmap();
                return (id_image_read) &ScreenshotPixmap;
            }
            return nullptr;
        }

        id_bitmap_read Platform::Web::GetScreenshotBitmap(h_web web, bool vflip)
        {
            if(WebPrivate* CurWeb = (WebPrivate*) web.get())
            {
                Image& ScreenshotImage = *BOSS_STORAGE_SYS(Image);
                const QImage& CurImage = CurWeb->GetImage();
                ScreenshotImage.LoadBitmapFromBits(CurImage.constBits(), CurImage.width(), CurImage.height(),
                    CurImage.bitPlaneCount(), vflip);
                return ScreenshotImage.GetBitmap();
            }
            return nullptr;
        }

        h_web_native Platform::Web::CreateNative(chars url, bool clearcookies, EventCB cb, payload data)
        {
            return PlatformImpl::Wrap::Web_CreateNative(url, clearcookies, cb, data);
        }

        void Platform::Web::ReleaseNative(h_web_native web_native)
        {
            PlatformImpl::Wrap::Web_ReleaseNative(web_native);
        }

        void Platform::Web::ReloadNative(h_web_native web_native, chars url)
        {
            PlatformImpl::Wrap::Web_ReloadNative(web_native, url);
        }

        ////////////////////////////////////////////////////////////////////////////////
        // PURCHASE
        ////////////////////////////////////////////////////////////////////////////////
        id_purchase Platform::Purchase::Open(chars name, PurchaseType type)
        {
            auto NewPurchase = new PurchasePrivate();
            if(!NewPurchase->Register(name, type))
            {
                delete NewPurchase;
                return nullptr;
            }
            return (id_purchase) NewPurchase;
        }

        void Platform::Purchase::Close(id_purchase purchase)
        {
            if(!purchase) return;
            auto OldPurchase = (PurchasePrivate*) purchase;
            delete OldPurchase;
        }

        bool Platform::Purchase::IsPurchased(id_purchase purchase)
        {
            if(!purchase) return false;
            auto CurPurchase = (PurchasePrivate*) purchase;
            ///////////////////////////////////
            return false;
        }

        bool Platform::Purchase::Purchasing(id_purchase purchase, PurchaseCB cb)
        {
            if(!purchase) return false;
            auto CurPurchase = (PurchasePrivate*) purchase;
            return CurPurchase->Purchase(cb);
        }

        ////////////////////////////////////////////////////////////////////////////////
        // BLUETOOTH
        ////////////////////////////////////////////////////////////////////////////////
        Strings Platform::Bluetooth::GetAllUuids(chars service_uuid, sint32 timeout, String* spec)
        {
            BOSS_ASSERT("Further development is needed.", false);
            return Strings();
        }

        id_bluetooth Platform::Bluetooth::Open(chars uuid)
        {
            BOSS_ASSERT("Further development is needed.", false);
            return nullptr;
        }

        void Platform::Bluetooth::Close(id_bluetooth bluetooth)
        {
            BOSS_ASSERT("Further development is needed.", false);
        }

        bool Platform::Bluetooth::Connected(id_bluetooth bluetooth)
        {
            BOSS_ASSERT("Further development is needed.", false);
            return false;
        }

        sint32 Platform::Bluetooth::ReadAvailable(id_bluetooth bluetooth)
        {
            BOSS_ASSERT("Further development is needed.", false);
            return 0;
        }

        sint32 Platform::Bluetooth::Read(id_bluetooth bluetooth, uint08* data, const sint32 size)
        {
            BOSS_ASSERT("Further development is needed.", false);
            return 0;
        }

        void Platform::Bluetooth::Write(id_bluetooth bluetooth, const uint08* data, const sint32 size)
        {
            BOSS_ASSERT("Further development is needed.", false);
        }

        chars Platform::Bluetooth::EventFlush(id_bluetooth bluetooth)
        {
            BOSS_ASSERT("Further development is needed.", false);
            return nullptr;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // SERIAL
        ////////////////////////////////////////////////////////////////////////////////
        Strings Platform::Serial::GetAllNames(String* spec)
        {
            return SerialClass::GetList(spec);
        }

        id_serial Platform::Serial::Open(chars name, SerialDecodeCB dec, SerialEncodeCB enc)
        {
            SerialClass* NewSerial = new SerialClass(name, dec, enc);
            if(NewSerial->IsValid())
                return (id_serial) NewSerial;
            delete NewSerial;
            return nullptr;
        }

        void Platform::Serial::Close(id_serial serial)
        {
            delete (SerialClass*) serial;
        }

        bool Platform::Serial::Connected(id_serial serial)
        {
            if(!serial) return false;
            SerialClass* CurSerial = (SerialClass*) serial;
            return CurSerial->Connected();
        }

        bool Platform::Serial::ReadReady(id_serial serial, sint32* gettype)
        {
            if(!serial) return false;
            SerialClass* CurSerial = (SerialClass*) serial;
            return CurSerial->ReadReady(gettype);
        }

        sint32 Platform::Serial::ReadAvailable(id_serial serial)
        {
            if(!serial) return 0;
            SerialClass* CurSerial = (SerialClass*) serial;
            return CurSerial->ReadAvailable();
        }

        sint32 Platform::Serial::Read(id_serial serial, chars format, ...)
        {
            if(!serial) return 0;
            SerialClass* CurSerial = (SerialClass*) serial;
            va_list Args;
            va_start(Args, format);
            const sint32 Result = CurSerial->Read(format, Args);
            va_end(Args);
            return Result;
        }

        sint32 Platform::Serial::Write(id_serial serial, chars format, ...)
        {
            if(!serial) return 0;
            SerialClass* CurSerial = (SerialClass*) serial;
            va_list Args;
            va_start(Args, format);
            const sint32 Result = CurSerial->Write(format, Args);
            va_end(Args);
            return Result;
        }

        void Platform::Serial::WriteFlush(id_serial serial, sint32 type)
        {
            if(!serial) return;
            SerialClass* CurSerial = (SerialClass*) serial;
            CurSerial->WriteFlush(type);
        }

        ////////////////////////////////////////////////////////////////////////////////
        // CAMERA
        ////////////////////////////////////////////////////////////////////////////////
        Strings Platform::Camera::GetAllNames(String* spec)
        {
            return CameraClass::GetList(spec);
        }

        id_camera Platform::Camera::Open(chars name, sint32 width, sint32 height)
        {
            CameraClass* NewCamera = new CameraClass(name, width, height);
            if(NewCamera->IsValid())
                return (id_camera) NewCamera;
            delete NewCamera;
            return nullptr;
        }

        void Platform::Camera::Close(id_camera camera)
        {
            if(camera && ((CameraClass*) camera)->SubRef())
                delete (CameraClass*) camera;
        }

        id_camera Platform::Camera::Clone(id_camera camera)
        {
            if(!camera) return nullptr;
            return (id_camera) ((CameraClass*) camera)->AddRef();
        }

        void Platform::Camera::Capture(id_camera camera, bool preview, bool needstop)
        {
            if(!camera) return;
            CameraClass* CurCamera = (CameraClass*) camera;
            CurCamera->Capture(preview, needstop);
        }

        id_image_read Platform::Camera::LastCapturedImage(id_camera camera, sint32 maxwidth, sint32 maxheight)
        {
            if(!camera) return nullptr;
            CameraClass* CurCamera = (CameraClass*) camera;
            return CurCamera->LastCapturedImage(maxwidth, maxheight);
        }

        id_bitmap_read Platform::Camera::LastCapturedBitmap(id_camera camera, bool vflip)
        {
            if(!camera) return nullptr;
            CameraClass* CurCamera = (CameraClass*) camera;
            return CurCamera->LastCapturedBitmap(vflip);
        }

        size64 Platform::Camera::LastCapturedSize(id_camera camera)
        {
            if(!camera) return {0, 0};
            CameraClass* CurCamera = (CameraClass*) camera;
            return CurCamera->LastCapturedSize();
        }

        uint64 Platform::Camera::LastCapturedTimeMS(id_camera camera)
        {
            if(!camera) return 0;
            CameraClass* CurCamera = (CameraClass*) camera;
            return CurCamera->LastCapturedTimeMS();
        }

        sint32 Platform::Camera::TotalPictureShotCount(id_camera camera)
        {
            if(!camera) return 0;
            CameraClass* CurCamera = (CameraClass*) camera;
            return CurCamera->TotalPictureShotCount();
        }

        sint32 Platform::Camera::TotalPreviewShotCount(id_camera camera)
        {
            if(!camera) return 0;
            CameraClass* CurCamera = (CameraClass*) camera;
            return CurCamera->TotalPreviewShotCount();
        }

        ////////////////////////////////////////////////////////////////////////////////
        // MICROPHONE
        ////////////////////////////////////////////////////////////////////////////////
        Strings Platform::Microphone::GetAllNames(String* spec)
        {
            return MicrophoneClass::GetList(spec);
        }

        id_microphone Platform::Microphone::Open(chars name, sint32 maxcount)
        {
            MicrophoneClass* NewMicrophone = new MicrophoneClass(name, maxcount);
            if(NewMicrophone->IsValid())
                return (id_microphone) NewMicrophone;
            delete NewMicrophone;
            return nullptr;
        }

        void Platform::Microphone::Close(id_microphone microphone)
        {
            delete (MicrophoneClass*) microphone;
        }

        sint32 Platform::Microphone::GetBitRate(id_microphone microphone)
        {
            if(!microphone) return 0;
            MicrophoneClass* CurMicrophone = (MicrophoneClass*) microphone;
            return CurMicrophone->GetAudioSettings().bitRate();
        }

        sint32 Platform::Microphone::GetChannel(id_microphone microphone)
        {
            if(!microphone) return 0;
            MicrophoneClass* CurMicrophone = (MicrophoneClass*) microphone;
            return CurMicrophone->GetAudioSettings().channelCount();
        }

        sint32 Platform::Microphone::GetSampleRate(id_microphone microphone)
        {
            if(!microphone) return 0;
            MicrophoneClass* CurMicrophone = (MicrophoneClass*) microphone;
            return CurMicrophone->GetAudioSettings().sampleRate();
        }

        bool Platform::Microphone::TryNextSound(id_microphone microphone)
        {
            if(!microphone) return false;
            MicrophoneClass* CurMicrophone = (MicrophoneClass*) microphone;
            return CurMicrophone->TryLastData();
        }

        bytes Platform::Microphone::GetSoundData(id_microphone microphone, sint32* length, uint64* timems)
        {
            if(!microphone) return nullptr;
            MicrophoneClass* CurMicrophone = (MicrophoneClass*) microphone;
            const auto& LastData = CurMicrophone->GetLastData(timems);
            if(length) *length = LastData.Count();
            return (0 < LastData.Count())? &LastData[0] : nullptr;
        }
    }

#endif
