package com.boss2d;
import android.app.Activity;
import android.content.Context;
import android.os.Build;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.CookieManager;
import android.webkit.CookieSyncManager;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.LinearLayout;
import android.widget.Toast;

import java.util.ArrayList;

public class BossWebView
{
    public static int Create(Activity activity, String url, boolean clearcache)
    {
        Log.d("TAG", "### BossWebView: Create()");
        final int NewID = sActivities.size();
        final Activity SavedActivity = activity;
        final String SavedUrl = url;
        final boolean SavedClearCache = clearcache;

        activity.runOnUiThread(new Runnable()
        {
            public void run()
            {
                BossWebView NewView = new BossWebView(NewID, SavedActivity, SavedUrl, SavedClearCache);
                sActivities.add(NewView);
                LinearLayout.LayoutParams LLP = new LinearLayout.LayoutParams(
                        LinearLayout.LayoutParams.FILL_PARENT, LinearLayout.LayoutParams.FILL_PARENT);
                SavedActivity.addContentView(NewView.view, LLP);
            }
        });
        return NewID;
    }

    public static void Release(int id)
    {
        Log.d("TAG", "### BossWebView: Release()");
        BossWebView OldView = sActivities.get(id);
        if(OldView.view.getParent() != null && OldView.view instanceof ViewGroup)
            ((ViewGroup) OldView.view.getParent()).removeView(OldView.view);
        sActivities.set(id, null);
    }

    private static native void OnEvent(int id, String type, String text);
    private static ArrayList<BossWebView> sActivities = new ArrayList<>();

    public BossWebView(int id, Context context, String url, boolean clearcache)
    {
        final int MyID = id;
        view = new WebView(context);
        if(clearcache)
        {
            view.clearCache(true);
            view.clearHistory();
            if(Build.VERSION_CODES.LOLLIPOP_MR1 <= Build.VERSION.SDK_INT)
            {
                CookieManager.getInstance().removeAllCookies(null);
                CookieManager.getInstance().flush();
            }
            else
            {
                CookieSyncManager syncManager = CookieSyncManager.createInstance(context);
                syncManager.startSync();
                CookieManager.getInstance().removeAllCookie();
                CookieManager.getInstance().removeSessionCookie();
                syncManager.stopSync();
                syncManager.sync();
            }
            CookieSyncManager.createInstance(context);
            CookieManager cookieManager = CookieManager.getInstance();
            cookieManager.removeAllCookie();
        }

        view.loadUrl(url);
        view.setWebViewClient(new WebViewClient()
        {
            public boolean shouldOverrideUrlLoading(WebView view, String url)
            {
                OnEvent(MyID, "UrlChanged", url);
                view.loadUrl(url);
                return false;
            }
        });
    }

    private WebView view = null;
}
