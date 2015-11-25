package com.example.zhro.async;

import java.io.ByteArrayOutputStream;

import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import com.example.zhro.R;

import android.app.Activity;
import android.content.Context;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.SimpleAdapter;
import android.widget.TextView;
import android.widget.AdapterView.OnItemClickListener;

public class AsyncActivity extends Activity {

	private static final String TAG = "ZHRO";
	private TextView	prompt;
	private EditText keyword;
	private Button search;
	private Button cancel;
	private ListView jsonlist;
	private ProgressBar progress;
	private MyTask task;
	private ZhroAdapter  adapter;
	//private SimpleAdapter adapter;
	List<Map<String,String>> data;
	Map<String, String>  map;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Log.i(TAG, "AsyncActivity---onCreate");
		setContentView(R.layout.asyncmain);
		prompt  = (TextView) findViewById(R.id.prompt);
		keyword = (EditText) findViewById(R.id.keyword);
		search = (Button) findViewById(R.id.search);
		cancel = (Button) findViewById(R.id.cancel);
		jsonlist = (ListView) findViewById(R.id.jsonlist);  //此处注意, 布局内嵌布局时layout_height 不能设置 fill_parent/match_parent 否则其下边的部件无法显示
		jsonlist.setEmptyView(findViewById(android.R.id.empty));
		progress = (ProgressBar) findViewById(R.id.progress);
		search.setOnClickListener(listener);
		cancel.setOnClickListener(listener);
		Log.i(TAG, "AsyncActivity---onCreate " + jsonlist.toString());
		jsonlist.setEmptyView(findViewById(android.R.id.empty));
		jsonlist.setOnItemClickListener(new OnItemClickListener(){

			@Override
			public void onItemClick(AdapterView<?> parent, View view,
					int position, long id) {
				
			}
		});
	}

	private OnClickListener listener = new OnClickListener() {

		@Override
		public void onClick(View v) {
			if (!keyword.getText().toString().isEmpty()
					&& !keyword.getText().toString().equals(""))
				switch (v.getId()) {
				case R.id.search:
					String urlstring = "";
					String url = "http://api.map.baidu.com/place/v2/search?ak=KtWGDFOIGstDmsZwQ4bFsdyI&output=json&query=%sld&page_size=20&page_num=0&scope=1&region=全国";
					urlstring = url.replace("%sld", keyword.getText().toString());
					task = new MyTask();
					task.execute(urlstring);
					search.setEnabled(false);
					break;
				case R.id.cancel:
					task.cancel(true);
					search.setEnabled(true);
					break;
				default:
					break;
				}

		}
	};

	/**
	 * 异步类
	 */
	private class MyTask extends AsyncTask<String, Integer, String> {

		@Override
		protected void onPreExecute() {
			Log.i(TAG, "MyTask---onPreExecute");
			super.onPreExecute();
			prompt.setText("loading...");
		}
		//结果返回
		@Override
		protected void onPostExecute(String result) {
			Log.i(TAG, "MyTask---onPostExecute");
			super.onPostExecute(result);
			search.setEnabled(true);
			
			data = new ArrayList<Map<String,String>>();
			
			try {
				JSONArray  jsonObjs = new JSONObject(result).getJSONArray("results");
				for(int i = 0; i < jsonObjs.length() ; i++){ 
					Map<String, String>  map = new HashMap<String, String>();
	                JSONObject jsonObj = (JSONObject)jsonObjs.get(i);   
	                
	                String name = jsonObj.getString("name");  
	                map.put("name", name);
	                String address = jsonObj.getString("address");
	                map.put("address", address);
	                JSONObject  subjsonObj = jsonObj.getJSONObject("location");
	                if( subjsonObj != null ){
		                String lng = subjsonObj.getString("lng");  
		                map.put("lng", lng);
		                String lat = subjsonObj.getString("lat");
		                map.put("lat", lat);
	                }
	                data.add(map);  
	            }   
			} catch (JSONException e) {
				e.printStackTrace();
			}
			
			Log.i(TAG, "MyTask---onPostExecute List Size: " + String.valueOf(data.size()));
			adapter = new ZhroAdapter(AsyncActivity.this, data, R.layout.async_item);
			jsonlist.setAdapter(adapter);
			
		}

		//更新progress
		@Override
		protected void onProgressUpdate(Integer... values) {
			Log.i(TAG, "MyTask---onProgressUpdate");
			super.onProgressUpdate(values);
			progress.setProgress(values[0]);
			prompt.setText("loading... "+ String.valueOf(values[0]) + " %");
		}

		@Override
		protected void onCancelled() {
			Log.i(TAG, "MyTask---onCancelled");
			super.onCancelled();
		}

		//后台执行任务 
		@Override
		protected String doInBackground(String... params) {
			Log.i(TAG, "MyTask---doInBackground");
			
            try {  
                HttpClient client = new DefaultHttpClient();  
                HttpGet get = new HttpGet(params[0]);  
                HttpResponse response = client.execute(get);  
                if (response.getStatusLine().getStatusCode() == HttpStatus.SC_OK) {  
                    HttpEntity entity = response.getEntity();  
                    InputStream is = entity.getContent();  
                    long total = entity.getContentLength();  
                    ByteArrayOutputStream baos = new ByteArrayOutputStream();  
                    byte[] buf = new byte[1024];  
                    int count = 0;  
                    int length = -1;  
                    while ((length = is.read(buf)) != -1) {  
                        baos.write(buf, 0, length);  
                        count += length;  
                        //调用publishProgress公布进度,最后onProgressUpdate方法将被执行  
                        publishProgress((int) ((count / (float) total) * 100));  
                        //为了演示进度,休眠500毫秒  
                        //Thread.sleep(100);  
                    }  
                    return new String(baos.toByteArray());  
                }  
            } catch (Exception e) {  
                Log.e(TAG, "MyTask---doInBackground" + e.getMessage());  
            }  
			return null;
		}

	}
	
	/**
	 * 适配器
	 */
	static class ViewHolder   
    {  
        public TextView name = null;  
        public TextView address = null;  
        public TextView lng = null;  
        public TextView lat = null;  
    }
	
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
			// TODO Auto-generated method stub
			return data.size();
		}

		@Override
		public Object getItem(int position) {
			// TODO Auto-generated method stub
			return null;
		}

		@Override
		public long getItemId(int position) {
			// TODO Auto-generated method stub
			return 0;
		}

		@Override
		public View getView(int position, View convertView, ViewGroup parent) {
			
			map = data.get(position);
			ViewHolder  vholder = new ViewHolder();
			
			convertView = inflater.inflate(resource, null);
			vholder.name = (TextView)convertView.findViewById(R.id.async_name);
			vholder.address = (TextView)convertView.findViewById(R.id.async_addr);
			vholder.lng = (TextView)convertView.findViewById(R.id.async_lng);
			vholder.lat = (TextView)convertView.findViewById(R.id.async_lat);

			Log.i(TAG, map.get("name") + ":" + String.valueOf(position) + ":" +  map.get("lat") + ":" + map.get("lng"));

			vholder.name.setText(map.get("name"));
			vholder.address.setText(map.get("address"));
			vholder.lng.setText(map.get("lng"));
			vholder.lat.setText(map.get("lat"));
		
			return convertView;
		}
		
	}


}
