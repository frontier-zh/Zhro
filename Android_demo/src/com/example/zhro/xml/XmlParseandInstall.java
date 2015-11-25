package com.example.zhro.xml;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import android.util.Log;

public class XmlParseandInstall {
	private static final String TAG = "ZHRO";
	public static List<Map<String, String>> parseXml(String in) {
		
		List<Map<String,String>> data = new ArrayList<Map<String,String>>();
		Map<String, String>  map = null;
		
		DocumentBuilderFactory	factory = DocumentBuilderFactory.newInstance();
		try {
			DocumentBuilder	builder = factory.newDocumentBuilder();
			InputStream 	is = new ByteArrayInputStream(in.getBytes());
			Document	doc = builder.parse(is);
			Element		root = doc.getDocumentElement();
			NodeList	provinces = root.getElementsByTagName("province");
			for( int i = 0; i < provinces.getLength(); i++ ){
				Node	province = provinces.item(i);
				String	pName = province.getAttributes().getNamedItem("name").getNodeValue();
				map = new HashMap<String, String>();
				map.put("province", pName);
				map.put("city", "");
				map.put("code", "");
				data.add(map);
				NodeList	citys = province.getChildNodes();  //此 NodeList中，结尾符/开始符/标签 三者都算 Node
				for( int j = 0; j < citys.getLength(); j++ ){
					map = new HashMap<String, String>();
					Node  	city = citys.item(j);
					if( city.hasAttributes() ){
						String	cName = city.getAttributes().getNamedItem("name").getNodeValue();
						String	cCode = city.getAttributes().getNamedItem("jm").getNodeValue();
						map.put("province", pName);
						map.put("city", cName);
						map.put("code", cCode);
						Log.i(TAG, pName + ":" + cName + ":" + cCode);
						data.add(map);
						map = null;
					}
				}
			}
		} catch (ParserConfigurationException e) {
			e.printStackTrace();
		} catch (SAXException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		return data;
	}

}
