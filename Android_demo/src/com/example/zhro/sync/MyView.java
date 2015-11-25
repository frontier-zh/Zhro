package com.example.zhro.sync;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.view.View;

public class MyView extends View {
	float	x = 40;
	String	txt;
	public MyView(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
	}
	

	@Override
	protected void onDraw(Canvas canvas) {
		// TODO Auto-generated method stub
		x += 5;
		super.onDraw(canvas);
		Paint  paint = new Paint();
		paint.setAntiAlias(true);
		paint.setColor(Color.DKGRAY);
		
		canvas.drawCircle(x, 40, 40, paint);
		canvas.drawText(txt, 40, x + 60, paint);
	}
	
	public String getTxt() {
		return txt;
	}


	public void setTxt(String obj) {
		// TODO Auto-generated method stub
		this.txt = obj;
	}
}
