package com.example.zhro.login;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteDatabase.CursorFactory;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;

public class DBHelper extends SQLiteOpenHelper {

	private static final String TAG = "ZHRO";
	
	public DBHelper(Context context, String name, CursorFactory factory,
			int version) {
		super(context, name, factory, version);
		Log.i(TAG, "SQLiteOpenHelper---DBHelper");
	}

	@Override
	public void onCreate(SQLiteDatabase db) {
		Log.i(TAG, "SQLiteOpenHelper---onCreate");
		db.execSQL("create table if not exists  verfied (" +
				"account varchar(10) not null primary key, password varchar(20), alais varchar(10), " +
				"name varchar(20),phone varchar(12),addr varchar(50));");
		db.execSQL("insert into  verfied (account, password)" +
				"values('712457','zbj198956');");

	}

	@Override
	public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
		// TODO Auto-generated method stub
		Log.i(TAG, "SQLiteOpenHelper---onUpgrade");

	}

}
