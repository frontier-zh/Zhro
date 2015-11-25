package com.example.zhro.xml;

import java.io.FileDescriptor;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.http.Header;

import com.example.zhro.R;
import com.example.zhro.http.HttpNetwork;
import com.example.zhro.pull.PullXmlParse;
import com.loopj.android.http.TextHttpResponseHandler;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.TextView;
import android.widget.Toast;

public class XmlActivity extends Activity {

	private static final String TAG = "ZHRO";
	private final String FILENAME = "citylist.xml";
	private ListView listView;
	private Button 	 clickBtn;
	private SimpleAdapter simpleAdapter;
	private List<Map<String, String>> mData = new ArrayList<Map<String,String>>();

	@Override
	protected void onCreate(Bundle savedInstanceState) {

		super.onCreate(savedInstanceState);
		Log.i(TAG, "XmlActivity---onCreate");
		setContentView(R.layout.xmlmain);
		listView = (ListView) findViewById(R.id.xml_lv);
		clickBtn = (Button) findViewById(R.id.xml_cbtn);
		clickBtn.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				Toast.makeText(XmlActivity.this, "点击获取", 200).show();
				httpGet();
			}
		});

//		simpleAdapter = new SimpleAdapter(this, mData,
//				R.layout.xml_item, new String[] { "province", "city", "code" },
//				new int[] { R.id.xml_item_province, R.id.xml_item_city, R.id.xml_item_code });
//		listView.setAdapter(simpleAdapter);
//		if( mData.size() == 0 ){
			listView.setEmptyView(findViewById(android.R.id.empty));
//		}
		listView.setOnItemClickListener(new OnItemClickListener() {
			@Override
			public void onItemClick(AdapterView<?> parent, View view,
					int position, long id) {
				Toast.makeText(XmlActivity.this, "第 " + String.valueOf(position) + " 个被点击.", 200).show();
			}
		});
	}
	
	private	void httpGet(){
		String url = "http://qd.cityhouse.cn/webservice/citylist.html";
		HttpNetwork.get(url, null, new TextHttpResponseHandler(){

			@Override
			public void onSuccess(int statusCode, Header[] headers,
					String responseBody) {
				super.onSuccess(statusCode, headers, responseBody);
				Log.i(TAG, "XmlActivity---httpGet");
				XmlActivity.this.parseXml(responseBody);
				XmlActivity.this.saveFile(responseBody);
				
			}
			
		});
	}
	
	private void saveFile(String in) {
		// TODO Auto-generated method stub
		FileOutputStream fos;
		try {
			fos = openFileOutput(FILENAME, Context.MODE_PRIVATE);
			FileDescriptor fd = fos.getFD();
			fos.write(in.getBytes());
			fos.flush();
			fos.close();
			Log.i(TAG, "XmlActivity---saveFile Path: " + fd.getClass().getName());
		} catch (Exception e) {
			e.printStackTrace();
		}

	}

	private void parseXml(String in){
		List<Map<String,String>> data = new ArrayList<Map<String,String>>();
		Map<String, String>  title = new HashMap<String, String>();
		title.put("province", "省份");
		title.put("city", "城市");
		title.put("code", "简码");
		try {
			Log.i(TAG, "XmlActivity---parseXml0");
			data = XmlParseandInstall.parseXml(in);
			//data = PullXmlParse.pullParsexml();
			mData.add(title);
			mData.addAll(data);
		} catch (Exception e) {
			e.printStackTrace();
		}
		Log.i(TAG, "XmlActivity---parseXml " + String.valueOf(mData.size()));
		
		simpleAdapter = new SimpleAdapter(this, mData,
				R.layout.xml_item, new String[] { "province", "city", "code" },
				new int[] { R.id.xml_item_province, R.id.xml_item_city, R.id.xml_item_code });
		ZhroAdapter  adapter = new ZhroAdapter(this, mData, R.layout.xml_item);
		listView.setAdapter(adapter);

	}
	static class ViewHolder   
    {  
        public TextView tv_prov = null;  
        public TextView tv_city = null;  
        public TextView tv_code = null;  
    }
	
	@SuppressLint("ResourceAsColor")
	class  ZhroAdapter  extends BaseAdapter{

        private List<Map<String, String>> data;
        private int resource;
        private LayoutInflater inflater;
        private Map<String, String> 	map;
        
		public ZhroAdapter(Context context,
                List<Map<String, String>> data, int resource) {
            this.data = data;
            this.resource = resource;
            inflater = (LayoutInflater) getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		}

		@Override
		public int getCount() {
			return data.size();
		}

		@Override
		public Object getItem(int position) {
			return null;
		}

		@Override
		public long getItemId(int position) {
			return 0;
		}

		@Override
		public View getView(int position, View convertView, ViewGroup parent) {
			
			map = data.get(position);
			ViewHolder  vholder = new ViewHolder();
			
			convertView = inflater.inflate(resource, null);
			vholder.tv_prov = (TextView)convertView.findViewById(R.id.xml_item_province);
			vholder.tv_city = (TextView)convertView.findViewById(R.id.xml_item_city);
			vholder.tv_code = (TextView)convertView.findViewById(R.id.xml_item_code);
			if( !map.get("province").isEmpty() && map.get("city").isEmpty() && map.get("code").isEmpty() ){
				vholder.tv_prov.setBackgroundColor(R.color.backblue);
				vholder.tv_prov.setTextColor(R.color.white);
				vholder.tv_prov.setTextSize(18);
				vholder.tv_prov.setGravity(Gravity.LEFT);
				Log.i(TAG, map.get("province") + ":" + String.valueOf(position) + ":" +  map.get("city") + ":" + map.get("code"));
			}
			vholder.tv_prov.setText(map.get("province"));
			vholder.tv_city.setText(map.get("city"));
			vholder.tv_code.setText(map.get("code"));
		
			return convertView;
		}
	}
}
