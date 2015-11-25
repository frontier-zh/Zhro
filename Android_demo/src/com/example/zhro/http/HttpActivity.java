package com.example.zhro.http;

import org.apache.http.Header;

import com.loopj.android.http.TextHttpResponseHandler;
import com.example.zhro.R;
import com.example.zhro.login.LoginPageActivity;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.SpinnerAdapter;
import android.widget.TextView;
import android.widget.Toast;

public class HttpActivity extends Activity {

	private static final String TAG = "ZHRO";
	private EditText urlString;
	private Spinner spinner;
	private EditText strResult;
	private Button reqButton;
	private SpinnerAdapter adapter;
	private String	 strEncode;

	@Override
	protected void onCreate(Bundle savedInstanceState) {

		super.onCreate(savedInstanceState);
		setContentView(R.layout.netmain);
		urlString = (EditText) findViewById(R.id.editText1);
		spinner = (Spinner) findViewById(R.id.spinner1);
		ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(
				this, R.array.array_encode,
				android.R.layout.simple_spinner_item);
		adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		spinner.setAdapter(adapter);
		spinner.setOnItemSelectedListener(listener1);
		strResult = (EditText) findViewById(R.id.editText3);
		reqButton = (Button) findViewById(R.id.button1);
		reqButton.setOnClickListener(listener);
	}

	private OnClickListener listener = new OnClickListener() {

		@Override
		public void onClick(View v) {
			if (v.getId() == R.id.button1) {
				if (!urlString.getText().toString().isEmpty()
						&& urlString.getText().toString().startsWith("http")) {
					HttpNetwork.get(urlString.getText().toString(), null,
							new TextHttpResponseHandler() {

								@Override
								public void onSuccess(int statusCode,
										Header[] headers, String responseBody) {
									super.onSuccess(statusCode, headers,
											responseBody);
									strResult.setText(responseBody);
									for (int i = 0; i < headers.length; i++) {
										Log.i(TAG, headers[i].getName() + ":"
												+ headers[i].getValue());
									}
								}
							});
				} else {
					Toast.makeText(HttpActivity.this, "UrlÎ´¿Õ/¸ñÊ½´íÎó" + strEncode, 1000)
							.show();
					
				}
			}
		}
	};
	
	private  OnItemSelectedListener listener1 = new OnItemSelectedListener() {

		@Override
		public void onItemSelected(AdapterView<?> parent, View view,
				int position, long id) {
			TextView  tv = (TextView)view;
			Toast.makeText(getApplicationContext(), tv.getText(), Toast.LENGTH_SHORT).show();
			strEncode = tv.getText().toString();
		}

		@Override
		public void onNothingSelected(AdapterView<?> parent) {
			// TODO Auto-generated method stub
			
		}
	};

}
