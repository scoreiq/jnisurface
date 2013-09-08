package com.example.surfacetest;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class MySurface extends SurfaceView implements SurfaceHolder.Callback {

	DrawThread drawThread;
	
	public MySurface(Context context) {
		super(context);
		Log.d("SURF","Constructor");
		getHolder().addCallback(this);
	}
	
	@Override
	public void draw(Canvas canvas)
	{
		Log.d("SURF","draw");
		Paint paint = new Paint();
		paint.setColor(Color.GREEN);
		canvas.drawRect(0, 0, 200, 200, paint);
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width,
			int height) {
		Log.d("SURF","surfaceChanged");
		
	}

	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		Log.d("SURF","surfaceCreated");
		drawThread = new DrawThread(getHolder(), getResources());
		drawThread.setRunning(true);
		drawThread.start();
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		Log.d("SURF","surfaceDestroyed");
		boolean retry = true;
        // завершаем работу потока
        drawThread.setRunning(false);
        while (retry) {
            try {
                drawThread.join();
                retry = false;
            } catch (InterruptedException e) {
                // если не получилось, то будем пытаться еще и еще
            }
        }
	}
}
