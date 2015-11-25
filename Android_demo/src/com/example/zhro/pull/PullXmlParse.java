package com.example.zhro.pull;

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.ProtocolException;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;
import org.xmlpull.v1.XmlPullParserFactory;

import android.util.Log;

public class PullXmlParse {

	private static final String TAG = "ZHRO";
	
	public static List<Map<String, String>>  pullParsexml(InputStream in){
		
		String		 pName = "";
		List<Map<String,String>> data = new ArrayList<Map<String,String>>();
		Map<String, String>  map = null;
		
		try {
			Log.i(TAG, "PullXmlParse---pullParse");
			XmlPullParserFactory  factory = XmlPullParserFactory.newInstance();
			XmlPullParser parse = factory.newPullParser();
			parse.setInput(in, "UTF-8");
			int  eventType = parse.getEventType();
			while( XmlPullParser.END_DOCUMENT != eventType) {
				String	nodeName = parse.getName();
				switch (eventType) {
				case XmlPullParser.START_DOCUMENT:
					break;
				case XmlPullParser.START_TAG:
					if( "province".equals(nodeName) ){
						pName = parse.getAttributeValue(0);
						map = new HashMap<String, String>();
						map.put("province", pName);
						map.put("city", "");
						map.put("code", "");
						data.add(map);
						Log.i(TAG, pName + ":" + map.get("city").toString() + ":" + map.get("code").toString());
					}else if( "city".equals(nodeName) ){
						map = new HashMap<String, String>();
						map.put("province", pName);
						map.put("city", parse.getAttributeValue(1));
						map.put("code", parse.getAttributeValue(0));
						data.add(map);
						Log.i(TAG, pName + ":" + map.get("city").toString() + ":" + map.get("code").toString());
					}
					
					break;
				case XmlPullParser.TEXT:
					break;
				case XmlPullParser.END_TAG:
					break;
				case XmlPullParser.END_DOCUMENT:
					break;
				}
				eventType = parse.next();
			}
		} catch (XmlPullParserException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		return data;
			
	}
}
