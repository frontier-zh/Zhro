package com.example.zhro.browser;

import com.example.zhro.R;

import android.app.Activity;
import android.os.Bundle;
import android.view.KeyEvent;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;

public class BrowserActivity extends Activity {


	private	WebView	webv;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		
		super.onCreate(savedInstanceState);
		final Activity activity = this;
		setContentView(R.layout.webmain);
		init();
	}

	private void init() {
		webv= (WebView) findViewById(R.id.webview);
		webv.loadUrl("http://www.baidu.com");
		WebSettings webs = webv.getSettings();
		webs.setJavaScriptEnabled(true);
		webv.setWebViewClient(new WebViewClient(){

			@Override
			public boolean shouldOverrideUrlLoading(WebView view, String url) {
				// TODO Auto-generated method stub
				view.loadUrl(url);
				return true;
			}		
		});		
		webv.setWebChromeClient(new WebChromeClient(){

			@Override
			public void onProgressChanged(WebView view, int progress) {
				BrowserActivity.this.setTitle(String.valueOf(progress) + "%");
				//BrowserActivity.this.setProgress(progress*100);
				if( progress == 100 ){
					BrowserActivity.this.setTitle("Load OK!");
				}
			}
		});
	}
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub
		
		if( keyCode == KeyEvent.KEYCODE_BACK && webv.canGoBack() ){
			webv.goBack();
			return true;
		}
		return super.onKeyDown(keyCode, event);
	}

}
