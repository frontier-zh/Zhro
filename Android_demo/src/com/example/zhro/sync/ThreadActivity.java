package com.example.zhro.sync;
import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;


public class ThreadActivity extends Activity {
	
	protected static final String TAG = "ZHRO";
	protected static final int FLAG1 = 0x101;
	
	MyView	myview = null;
	
	public  Handler handler = new Handler(){
		@Override
		public void handleMessage(Message msg) {
			switch(msg.what){
			case FLAG1:
					Log.i(TAG, "ThreadActivity---handleMessage");
					myview.setTxt((String)msg.obj);
					myview.invalidate();
				break;
			}
			super.handleMessage(msg);
		}
	};
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		this.myview = new MyView(this);
		setContentView(this.myview);
		new Thread(new MyThread()).start();
	}
	
	class  MyThread  implements Runnable{

		@Override
		public void run() {
			int i = 0;
			while( i < 50 ){
				Message  msg = new Message();
				msg.what = ThreadActivity.this.FLAG1;
				msg.obj = "辅助线程传来消息";
				Log.i(TAG, "ThreadActivity---sendMessage");
				ThreadActivity.this.handler.sendMessage(msg);
				try {
					Thread.sleep(200);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
					Thread.currentThread().interrupt();
				}
				i++;
			}
		}
		
	}
}
