package com.example.zhro.login;

import com.example.zhro.R;

import android.os.Bundle;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.Toast;

public class LoginPageActivity extends Activity {

	private EditText usr = null;
	private EditText pwd;
	private ImageButton	listbtn;

	private CheckBox savepwd;
	private CheckBox visable;
	private CheckBox vibrate;
	private CheckBox gropmsg;
	private CheckBox quietly;
	private Boolean isSavepwd = true;
	private Boolean isVisable = false; // 是否隐身
	private Boolean isVibrate = false; // 是否震动
	private Boolean isGropmsg = false; // 接收组消息
	private Boolean isQuietly = false; // 是否静音
	private Button login;
	private DBHelper	dbhelper;
	
	private static final String FILE = "login";
	private static final String TAG = "ZHRO";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.loginpage);
		dbhelper = new DBHelper(this, "zhro.db", null, 1);
		savepwd = (CheckBox) findViewById(R.id.login_cb_savepwd);
		visable = (CheckBox) findViewById(R.id.login_cb_visible);
		vibrate = (CheckBox) findViewById(R.id.login_cb_openvibra);
		gropmsg = (CheckBox) findViewById(R.id.login_cb_receivegroupmsg);
		quietly = (CheckBox) findViewById(R.id.login_cb_quite);

		login = (Button) findViewById(R.id.loginbtn);
		login.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				usr = (EditText) findViewById(R.id.login_edit_account);
				pwd = (EditText) findViewById(R.id.login_edit_password);
				if (v.getId() == R.id.loginbtn) {
					

					if (!usr.getText().toString().equals("") && !pwd.getText().toString().equals("")) {
						
						if( onVerfiedQuery(dbhelper) ){
							Toast.makeText(LoginPageActivity.this, "账户验证成功", 1000).show();
							Intent i = new Intent(LoginPageActivity.this,
									LoginMainActivity.class);
							i.putExtra("account", usr.getText().toString());
							i.putExtra("password", pwd.getText().toString());
							startActivity(i);
						}else{
							Toast.makeText(LoginPageActivity.this, "账户验证失败", 1000).show();
						}
					}
				}
			}
		});
		
		listbtn = (ImageButton)findViewById(R.id.imgbtn1);
		listbtn.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				usr = (EditText) findViewById(R.id.login_edit_account);
				pwd = (EditText) findViewById(R.id.login_edit_password);
				
				Toast.makeText(LoginPageActivity.this, "获取历史账号", 1000).show();
				SharedPreferences pref = LoginPageActivity.this.getSharedPreferences(FILE, MODE_PRIVATE);
				usr.setText(pref.getString("account", "frontier"));
				pwd.setText(pref.getString("password", "232722"));
			}
		});
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	public boolean onVerfiedQuery(DBHelper dbhelper){
		
		SQLiteDatabase db = dbhelper.getReadableDatabase();
		Cursor cursor = db.query("verfied", new String[] {"account","password"}, "account = ?", new String []{usr.getText().toString()}, null, null, null);
		if( cursor.moveToNext() ){
			String account = cursor.getString(0);
			String password = cursor.getString(1);
			Log.i(TAG, "MainActivity---onVerfiedQuery: 账户：" + account +"; 密码" + password);
			if( usr.getText().toString().equals(account) && pwd.getText().toString().equals(password)){
				return true;
			}else{
				return false;
			}
		}else{
			return false;
		}
	}

}
