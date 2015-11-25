package com.example.zhro.dialog;

import com.example.zhro.R;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Toast;

public class DialogActivity extends Activity {

	private  final	int	DIALOG_1 = 1;
	private  final	int	DIALOG_2 = 2;
	private  final	int	DIALOG_3 = 3;
	private  final	int	DIALOG_4 = 4;
	private  final	int	DIALOG_5 = 5;
	private  final	int	DIALOG_6 = 6;
	private  AlertDialog	alert = null;
	private  Button		btn1;
	private  Button		btn2;
	private  Button		btn3;
	private  Button		btn4;
	private  String[]	array;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.dialogmain);
		array = getResources().getStringArray(R.array.array_encode);
		btn1 = (Button) findViewById(R.id.btn_dialog1);
		btn1.setOnClickListener(listener);
		btn2 = (Button) findViewById(R.id.btn_dialog2);
		btn2.setOnClickListener(listener);
		btn3 = (Button) findViewById(R.id.btn_dialog3);
		btn3.setOnClickListener(listener);
		btn4 = (Button) findViewById(R.id.btn_dialog4);
		btn4.setOnClickListener(listener);
	}
	private  OnClickListener listener = new OnClickListener() {
		
		@Override
		public void onClick(View v) {
			// TODO Auto-generated method stub
			switch (v.getId()) {
			case R.id.btn_dialog1:
					showDialog(DIALOG_1);
				break;
			case R.id.btn_dialog2:
					showDialog(DIALOG_2);
				break;
			case R.id.btn_dialog3:
					showDialog(DIALOG_3);
			break;
			case R.id.btn_dialog4:
					showDialog(DIALOG_4);
			break;
			default:
				break;
			}
			
		}
	};
	
	@Override
	protected Dialog onCreateDialog(int id) {
		// TODO Auto-generated method stub
		AlertDialog.Builder bulider = new AlertDialog.Builder(this);
		switch (id) {
		case DIALOG_1:
				//bulider.setTitle("标题");
				bulider.setMessage("Zhro内容");
				bulider.setPositiveButton("确定", new DialogInterface.OnClickListener() {
						
						@Override
						public void onClick(DialogInterface dialog, int which) {
							DialogActivity.this.finish();
						}
					});
				bulider.setNegativeButton("取消", new DialogInterface.OnClickListener() {
							
						@Override
						public void onClick(DialogInterface dialog, int which) {
							dialog.cancel();
						}
					});
				return bulider.create();
		case DIALOG_2:
				bulider.setTitle("颜色选取");
				bulider.setItems(R.array.array_encode, new DialogInterface.OnClickListener() {
					
					@Override
					public void onClick(DialogInterface dialog, int which) {
						Toast.makeText(getApplicationContext(), array[which], Toast.LENGTH_SHORT).show();
					}
				});
				return bulider.create();
		case DIALOG_3:
				bulider.setTitle("颜色选取【单选】");
				bulider.setSingleChoiceItems(R.array.array_encode, 0, new DialogInterface.OnClickListener() {
					
					@Override
					public void onClick(DialogInterface dialog, int which) {
						Toast.makeText(getApplicationContext(), array[which], Toast.LENGTH_SHORT).show();
						if( alert != null ){
							alert.dismiss();
						}
					}
				});
				alert = bulider.create();
				return alert;
				
		case DIALOG_4:
				bulider.setTitle("颜色选取【复选】");
				bulider.setMultiChoiceItems(R.array.array_encode, null, new DialogInterface.OnMultiChoiceClickListener() {
					
					@Override
					public void onClick(DialogInterface dialog, int which, boolean isChecked) {
						Toast.makeText(getApplicationContext(), array[which], Toast.LENGTH_SHORT).show();						
					}
				});
				return bulider.create();
		default:
			break;
		}
		return null;

	}
	@Override
	protected void onPrepareDialog(int id, Dialog dialog) {
		// TODO Auto-generated method stub
		super.onPrepareDialog(id, dialog);
	}
}
