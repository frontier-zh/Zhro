package com.example.zhro;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.example.zhro.async.AsyncActivity;
import com.example.zhro.browser.BrowserActivity;
import com.example.zhro.dialog.DialogActivity;
import com.example.zhro.http.HttpActivity;
import com.example.zhro.login.LoginMainActivity;
import com.example.zhro.login.LoginPageActivity;
import com.example.zhro.sync.ThreadActivity;
import com.example.zhro.wifi.MyWiFiActivity;
import com.example.zhro.xml.XmlActivity;

import android.os.Bundle;
import android.app.Activity;
import android.app.ListActivity;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.ViewGroup;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends ListActivity {

	private static final String TAG = "ZHRO";
	private ListView listView;
	private List<Map<String, Object>> mData;
	private int imgs[] = { R.drawable.a, R.drawable.b, R.drawable.c,
			R.drawable.d, R.drawable.e, R.drawable.f, R.drawable.g,
			R.drawable.h, R.drawable.i, R.drawable.j, R.drawable.k,
			R.drawable.l };
	private String titles[] = { "登陆L", "网络HTTP", "解析XML", "解析JSON", "浏览器",
			"Dialog", "Wi-Fi P2P", "Handler_Thread", "AsyncTask异步", "项目9",
			"项目10", "项目11" };
	private String infos[] = { "登陆练习:数据库验证,信息preference保存", "网络访问:请求http,结果展示",
			"解析XML:请求数据解析显示", "解析JSON:请求数据解析展示", "webview模拟浏览器", "Dialog使用",
			"Wi-Fi P2P 系统广播接收者 判断状态", "UI主线程之外线程操作及通信",
			"AsynTask 替代handler-thread-looper-message机制", "实例demo9",
			"实例demo10", "实例demo11" };

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		SimpleAdapter adapter = new SimpleAdapter(this, getData(),
				R.layout.global_list, new String[] { "title", "info", "img" },
				new int[] { R.id.title, R.id.info, R.id.img });
		setListAdapter(adapter);
		listView = (ListView) getListView();
		listView.setBackgroundResource(R.drawable.bg_default);
		listView.setDividerHeight(3);
	}

	@Override
	protected void onListItemClick(ListView l, View v, int position, long id) {
		super.onListItemClick(l, v, position, id);

		Intent i = new Intent();
		switch (position) {
		case 0:
			Toast.makeText(MainActivity.this,
					String.valueOf(position) + "登陆demo", 500).show();
			i.setClass(MainActivity.this, LoginPageActivity.class);
			startActivity(i);
			break;
		case 1:
			Toast.makeText(MainActivity.this,
					String.valueOf(position) + "请求http", 500).show();
			i.setClass(MainActivity.this, HttpActivity.class);
			startActivity(i);
			break;
		case 2:
			Toast.makeText(MainActivity.this,
					String.valueOf(position) + "解析Xml", 500).show();
			i.setClass(MainActivity.this, XmlActivity.class);
			startActivity(i);
			break;
		case 3:
			Toast.makeText(MainActivity.this,
					String.valueOf(position) + "暂时未添加", 500).show();
			break;
		case 4:
			Toast.makeText(MainActivity.this,
					String.valueOf(position) + "webView模拟浏览器", 500).show();
			i.setClass(MainActivity.this, BrowserActivity.class);
			startActivity(i);
			break;
		case 5:
			Toast.makeText(MainActivity.this,
					String.valueOf(position) + "Dialog对话框", 500).show();
			i.setClass(MainActivity.this, DialogActivity.class);
			startActivity(i);
			break;
		case 6:
			Toast.makeText(MainActivity.this,
					String.valueOf(position) + "Wi-Fi P2P", 500).show();
			i.setClass(MainActivity.this, MyWiFiActivity.class);
			startActivity(i);
			break;
		case 7:
			Toast.makeText(MainActivity.this,
					String.valueOf(position) + "Handler-Thread", 500).show();
			i.setClass(MainActivity.this, ThreadActivity.class);
			startActivity(i);
			break;
		case 8:
			Toast.makeText(
					MainActivity.this,
					String.valueOf(position)
							+ "布局内嵌布局时layout_height 不能设置 fill_parent/match_parent 否则其下边的部件无法显示",
					2000).show();
			i.setClass(MainActivity.this, AsyncActivity.class);
			startActivity(i);
			break;
		case 9:
			Toast.makeText(MainActivity.this,
					String.valueOf(position) + "暂时未添加", 500).show();
			break;
		case 10:
			Toast.makeText(MainActivity.this,
					String.valueOf(position) + "暂时未添加", 500).show();
			break;
		case 11:
			Toast.makeText(MainActivity.this,
					String.valueOf(position) + "暂时未添加", 500).show();
			break;
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	private List<Map<String, Object>> getData() {
		List<Map<String, Object>> data = new ArrayList<Map<String, Object>>();
		Map<String, Object> map = null;
		for (int m = 0; m < imgs.length; m++) {
			map = new HashMap<String, Object>();
			map.put("img", imgs[m]);
			map.put("title", titles[m]);
			map.put("info", infos[m]);
			data.add(map);
		}
		Log.i(TAG,
				"MainActivity---getData  size:" + String.valueOf(data.size()));
		return data;

	}

}