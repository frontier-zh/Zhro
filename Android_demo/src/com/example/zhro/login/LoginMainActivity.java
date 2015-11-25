package com.example.zhro.login;

import com.example.zhro.R;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class LoginMainActivity extends Activity {

	private static final String TAG = "ZHRO";
	private static final String FILE = "login";
	private String account;
	private String password;
	private TextView textview;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.loginmain);
		Log.i(TAG, "SeccondActivity---onCreate");
		textview = (TextView)findViewById(R.id.second_tv1);
		Intent i = this.getIntent();
		account = i.getStringExtra("account");
		password = i.getStringExtra("password");
		
		textview.setText("»¶Ó­  "+ account.toString() + "µÇÂ½ÏµÍ³");
	}

	@Override
	protected void onStart() {
		// TODO Auto-generated method stub
		super.onStart();
		Log.i(TAG, "SeccondActivity---onStart");
		SharedPreferences pref = this.getSharedPreferences(FILE, MODE_APPEND);
		Editor edit = pref.edit();
		edit.putString("account", account.toString());
		edit.putString("password", password.toString());
		edit.commit();
	}

	@Override
	protected void onRestart() {
		// TODO Auto-generated method stub
		super.onRestart();
		Log.i(TAG, "SeccondActivity---onRestart");
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		Log.i(TAG, "SeccondActivity---onResume");
	}

	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
		Log.i(TAG, "SeccondActivity---onPause");
	}

	@Override
	protected void onStop() {
		// TODO Auto-generated method stub
		super.onStop();
		Log.i(TAG, "SeccondActivity---onStop");
	}

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		Log.i(TAG, "SeccondActivity---onDestroy");
	}

}
