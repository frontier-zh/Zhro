package com.example.zhro.http;

import org.apache.http.Header;

import android.content.Context;

import com.loopj.android.http.AsyncHttpClient;
import com.loopj.android.http.AsyncHttpResponseHandler;
import com.loopj.android.http.RequestParams;


public class HttpNetwork {

	private  static AsyncHttpClient client = new AsyncHttpClient();
	
	public   static void get(String url, RequestParams params, AsyncHttpResponseHandler responseHandler){
		client.get(url, params, responseHandler);
	}
	
	public   static void get(Context context, String url, Header[] headers, RequestParams params, AsyncHttpResponseHandler responseHandler){
		client.get(context, url, headers, params, responseHandler);
	}
	
	public   static void post(String url, RequestParams params, AsyncHttpResponseHandler responseHandler){
		client.post(url, params, responseHandler);
	}
	
	public	 static void post(Context context, String url, Header[] headers, RequestParams params, String contentType, AsyncHttpResponseHandler responseHandler){
		client.post(context, url, headers, params, contentType, responseHandler);
	}
}
