package com.example.test;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;
import java.util.List;
import java.util.Map;

import com.example.zhro.pull.PullXmlParse;
import com.example.zhro.xml.XmlParseandInstall;

import android.test.AndroidTestCase;
import android.util.Log;

public class TestXmlParase extends AndroidTestCase {

	private static final String TAG = "ZHRO";

	public void testparseXml() {
		
		InputStream is = null;
		try {
			is = new FileInputStream("/data/data/com.example.zhro/files/citylist.xml");
			List<Map<String,String>> data = PullXmlParse.pullParsexml(is);
			Log.i(TAG, "TestXmlParase---Size " + String.valueOf(data.size()));
			
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		
	}
}
